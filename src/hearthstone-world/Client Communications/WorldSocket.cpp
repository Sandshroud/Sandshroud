/***
 * Demonstrike Core
 */
// Class WorldSocket - Main network code functions, handles
// reading/writing of all packets.

#include "StdAfx.h"

/* echo send/received packets to console */
//#define ECHO_PACKET_LOG_TO_CONSOLE 1

extern bool bServerShutdown;

#pragma pack(push, 1)

struct ClientPktHeader
{
    uint16 size;
    uint32 cmd;
};

struct ServerPktHeader
{
    ServerPktHeader(uint32 size, uint16 cmd) : size(size)
    {
        uint8 headerIndex = 0;
        if (isLargePacket())
            header[headerIndex++] = 0x80 | (0xFF & (size >> 16));
        header[headerIndex++] = 0xFF &(size >> 8);
        header[headerIndex++] = 0xFF & size;

        header[headerIndex++] = 0xFF & cmd;
        header[headerIndex++] = 0xFF & (cmd >> 8);
    }

    uint8 getHeaderLength() { return 2 + (isLargePacket() ? 3 : 2); }
    bool isLargePacket() { return size > 0x7FFF; }

    uint32 size;
    uint8 header[5];
};

#pragma pack(PRAGMA_POP)

WorldSocket::WorldSocket(SOCKET fd, const sockaddr_in * peer) : TcpSocket(fd, WORLDSOCKET_RECVBUF_SIZE, WORLDSOCKET_SENDBUF_SIZE, false, peer)
{
    Authed = false;
    mOpcode = mRemaining = mUnaltered = 0;
    _latency = 0;
    mSession = NULL;
    mSeed = RandomUInt();
    addonPacket = NULL;
    mQueued = false;
    mRequestID = 0;
    m_nagleEanbled = false;
    m_fullAccountName = NULL;
}

WorldSocket::~WorldSocket()
{
    WorldPacket * pck;
    queueLock.Acquire();
    while((pck = _queue.Pop()))
        delete pck;
    queueLock.Release();

    if(addonPacket)
        delete addonPacket;
    addonPacket = NULL;

    if(mSession)
    {
        mSession->SetSocket(NULL);
        mSession=NULL;
    }

    if( m_fullAccountName != NULL )
    {
        delete m_fullAccountName;
        m_fullAccountName = NULL;
    }
}

void WorldSocket::OnDisconnect()
{
    if(mSession)
    {
        mSession->SetSocket(0);
        mSession = NULL;
    }

    if(mRequestID != 0)
    {
        sLogonCommHandler.UnauthedSocketClose(mRequestID);
        mRequestID = 0;
    }

    if(mQueued)
    {
        sWorld.RemoveQueuedSocket(this);    // Remove from queued sockets.
        mQueued = false;
    }

    // clear buffer
    queueLock.Acquire();
    WorldPacket *pck;
    while((pck = _queue.Pop()))
        delete pck;
    queueLock.Release();
}

void WorldSocket::OutPacket(uint16 opcode, size_t len, const void* data, bool InWorld)
{
    OUTPACKET_RESULT res;
    if( (len + 10) > WORLDSOCKET_SENDBUF_SIZE )
    {
        printf("WARNING: Tried to send a packet of %u bytes (which is too large) to a socket. Opcode was: %u (0x%03X)\n", uint(len), uint(opcode), uint(opcode));
        return;
    }

    res = _OutPacket(opcode, len, data, InWorld);
    if(res == OUTPACKET_RESULT_SUCCESS)
        return;
    if(res == OUTPACKET_RESULT_PACKET_ERROR)
    { // Track packets that cause packet errors

    }
    else if(res == OUTPACKET_RESULT_NO_ROOM_IN_BUFFER)
    {
        /* queue the packet */
        queueLock.Acquire();
        WorldPacket *pck = new WorldPacket(opcode, len);
        pck->SetOpcode(opcode);
        if(len)
            pck->append((const uint8*)data, len);
        _queue.Push(pck);
        queueLock.Release();
    }
}

void WorldSocket::UpdateQueuedPackets()
{
    queueLock.Acquire();
    if(!_queue.HasItems())
    {
        queueLock.Release();
        return;
    }

    WorldPacket * pck;
    while((pck = _queue.front()))
    {
        /* try to push out as many as you can */
        switch(_OutPacket(pck->GetOpcode(), pck->size(), pck->size() ? pck->contents() : NULL))
        {
        case OUTPACKET_RESULT_SUCCESS:
            {
                delete pck;
                _queue.pop_front();
            }break;

        case OUTPACKET_RESULT_NO_ROOM_IN_BUFFER:
            {
                /* still connected */
                queueLock.Release();
                return;
            }break;

        default:
            {
                /* kill everything in the buffer */
                while((pck = _queue.Pop()))
                    delete pck;
                queueLock.Release();
                return;
            }break;
        }
    }
    queueLock.Release();
}

OUTPACKET_RESULT WorldSocket::_OutPacket(uint16 opcode, size_t len, const void* data, bool InWorld)
{
    bool rv;
    if(!IsConnected())
        return OUTPACKET_RESULT_NOT_CONNECTED;
    uint16 newOpcode = sOpcodeMgr.ConvertOpcodeForOutput(opcode);
    if(newOpcode == MSG_NULL_ACTION)
    {
        printf("Unset packet %u rejected\n", opcode);
        return OUTPACKET_RESULT_PACKET_ERROR;
    }
    if( GetWriteBuffer()->GetSpace() < (len+4) )
        return OUTPACKET_RESULT_NO_ROOM_IN_BUFFER;
/*  FILE *codeLog = NULL;
    fopen_s(&codeLog, "OpcodeLog.txt", "a+b");
    if(codeLog)
    {
        fprintf(codeLog, "Sending packet %s(%03u) with size %u\r\n", sOpcodeMgr.GetOpcodeName(opcode), opcode, len);
        fclose(codeLog);
    }*/

    LockWriteBuffer();
    // Encrypt the packet
    // First, create the header.
    ServerPktHeader Header(len + 2, newOpcode);
    _crypt.EncryptSend((uint8*)&Header.header, Header.getHeaderLength());

    // Pass the header to our send buffer
    rv = WriteButHold((const uint8*)&Header.header, Header.getHeaderLength());

    // Pass the rest of the packet to our send buffer (if there is any)
    if(len > 0 && rv)
        rv = Write((const uint8*)data, (uint32)len);
    else if(rv)
        rv = ForceSend();

    UnlockWriteBuffer();
    if(len > 0 && rv && !bServerShutdown)
        sWorld.NetworkStressOut += float(float(len+4)/1024);
    return rv ? OUTPACKET_RESULT_SUCCESS : OUTPACKET_RESULT_SOCKET_ERROR;
}

void WorldSocket::OnConnect()
{
    sWorld.mAcceptedConnections++;
    _latency = getMSTime();

    WorldPacket data (SMSG_AUTH_CHALLENGE, 25);
    data.append(sWorld.authSeed1.AsByteArray(), 16);
    data << uint8(1);
    data << mSeed;
    data.append(sWorld.authSeed2.AsByteArray(), 16);
    SendPacket(&data);
}

void WorldSocket::_HandleAuthSession(WorldPacket* recvPacket)
{
    uint32 addonSize;
    std::string account;
    _latency = getMSTime() - _latency;

    try
    {
        *recvPacket >> hashDigest[14] >> hashDigest[7] >> hashDigest[16];
        *recvPacket >> hashDigest[9] >> hashDigest[4] >> hashDigest[5] >> hashDigest[15];
        recvPacket->read_skip<uint32>();
        *recvPacket >> hashDigest[18];
        recvPacket->read_skip<uint64>();
        recvPacket->read_skip<uint32>();
        *recvPacket >> hashDigest[13];
        recvPacket->read_skip<uint8>();
        *recvPacket >> hashDigest[10] >> hashDigest[6];
        *recvPacket >> mClientSeed;
        recvPacket->read_skip<uint32>();
        *recvPacket >> hashDigest[19] >> hashDigest[11] >> hashDigest[17];
        *recvPacket >> hashDigest[8] >> hashDigest[12] >> hashDigest[0];
        *recvPacket >> mClientBuild;
        *recvPacket >> hashDigest[3];
        recvPacket->read_skip<uint8>();
        recvPacket->read_skip<uint32>();
        *recvPacket >> hashDigest[1] >> hashDigest[2];

        *recvPacket >> addonSize;
        if(addonSize)
        {
            uint8 *addonBytes = new uint8[addonSize];
            recvPacket->read(addonBytes, addonSize);
            addonPacket = new WorldPacket(CMSG_AUTH_SESSION, addonSize);
            addonPacket->append(addonBytes, addonSize);
            delete addonBytes;
        }

        *recvPacket >> account;
    }
    catch(ByteBufferException &)
    {
        sLog.outDebug("Incomplete copy of AUTH_SESSION Received.");
        return;
    }

    if(mClientBuild != CL_BUILD_SUPPORT)
    {
        OutPacket(SMSG_AUTH_RESPONSE, 1, "\x14");
        return;
    }

    // Send out a request for this account.
    mRequestID = sLogonCommHandler.ClientConnected(account, this);
    if(mRequestID == 0xFFFFFFFF)
    {
        Disconnect();
        return;
    }

    // shitty hash !
    m_fullAccountName = new string( account );
}

void WorldSocket::InformationRetreiveCallback(WorldPacket & recvData, uint32 requestid)
{
    uint32 error;
    recvData >> error;
    if(error != 0 || requestid != mRequestID)
    {
        // something happened wrong @ the logon server
        OutPacket(SMSG_AUTH_RESPONSE, 1, "\x0D");
        return;
    }

    // Extract account information from the packet.
    string AccountName, GMFlags;
    const string * ForcedPermissions;
    uint32 AccountID;
    uint8 AccountFlags;
    string lang = "enUS";

    recvData >> AccountID >> AccountName >> GMFlags >> AccountFlags;
    ForcedPermissions = sLogonCommHandler.GetForcedPermissions(AccountName);
    if( ForcedPermissions != NULL )
        GMFlags.assign(ForcedPermissions->c_str());

    sLog.Debug( "WorldSocket","Received information packet from logon: `%s` ID %u (request %u)", AccountName.c_str(), AccountID, mRequestID);

    mRequestID = 0;
    uint8 K[40];
    recvData.read(K, 40);
    _crypt.Init(K);

    BigNumber BNK;
    BNK.SetBinary(K, 40);

    recvData.read((uint8*)lang.data(), 4);

    //checking if player is already connected
    //disconnect current player and login this one(blizzlike)

    WorldSession *session = sWorld.FindSession( AccountID );
    if( session != NULL )
    {
        if(session->_player != NULL && session->_player->GetMapMgr() == NULL)
        {
            sLog.Debug("WorldSocket","_player found without m_mapmgr during logon, trying to remove him [player %s, map %d, instance %d].", session->_player->GetName(), session->_player->GetMapId(), session->_player->GetInstanceID() );
            if(objmgr.GetPlayer(session->_player->GetLowGUID()))
                objmgr.RemovePlayer(session->_player);
            session->LogoutPlayer(false);
        }
        // AUTH_FAILED = 0x0D
        session->Disconnect();

        // clear the logout timer so he times out straight away
        session->SetLogoutTimer(1);

        // we must send authentication failed here.
        // the stupid newb can relog his client.
        // otherwise accounts dupe up and disasters happen.
        OutPacket(SMSG_AUTH_RESPONSE, 1, "\x15");
        return;
    }

    Sha1Hash sha;

    uint32 t = 0;
    if( m_fullAccountName == NULL )             // should never happen !
        sha.UpdateData(AccountName);
    else
    {
        sha.UpdateData(*m_fullAccountName);

        // this is unused now. we may as well free up the memory.
        delete m_fullAccountName;
        m_fullAccountName = NULL;
    }

    sha.UpdateData((uint8 *)&t, 4);
    sha.UpdateData((uint8 *)&mClientSeed, 4);
    sha.UpdateData((uint8 *)&mSeed, 4);
    sha.UpdateBigNumbers(&BNK, NULL);
    sha.Finalize();

    if (memcmp(sha.GetDigest(), hashDigest, 20))
    {
        // AUTH_UNKNOWN_ACCOUNT = 21
        OutPacket(SMSG_AUTH_RESPONSE, 1, "\x15");
        return;
    }

    // Allocate session
    WorldSession * pSession = (mSession = new WorldSession(AccountID, AccountName, this));
    ASSERT(mSession);
    pSession->deleteMutex.Acquire();

    // Set session properties
    pSession->permissioncount = 0;//just to make sure it's 0
    pSession->SetClientBuild(mClientBuild);
    pSession->LoadSecurity(GMFlags);
    pSession->SetAccountFlags(AccountFlags);
    pSession->m_lastPing = (uint32)UNIXTIME;
    recvData >> pSession->m_muted;

    pSession->LoadTutorials();
    pSession->LoadAccountData();

    sLog.Debug("Auth", "%s from %s:%u [%ums]", AccountName.c_str(), GetIP(), GetPort(), _latency);

    // Check for queue.
    if( (sWorld.GetSessionCount() < sWorld.GetPlayerLimit()) || pSession->HasGMPermissions() )
        Authenticate();
    else
    {
        // Queued, sucker.
        uint32 Position = sWorld.AddQueuedSocket(this);
        mQueued = true;
        sLog.Debug("Queue", "%s added to queue in position %u", AccountName.c_str(), Position);

        // Send packet so we know what we're doing
        SendAuthResponse(AUTH_WAIT_QUEUE, true, Position);
    }

    pSession->deleteMutex.Release();
}

void WorldSocket::Authenticate()
{
    mQueued = false;
    WorldSession *pSession = mSession;
    if(!pSession)
    {
        sLog.Debug( "WorldSocket","Lost Session");
        return;
    }

    SendAuthResponse(AUTH_OK, false);

    /*WorldPacket hotFix(true ? SMSG_HOTFIX_NOTIFY : SMSG_HOTFIX_NOTIFY_BLOP, 100); // Blop or not, client will accept the info
    hotFix << uint32(0); // count
    hotFix << uint32(true ? 0x919BE54E : 0x50238EC2); // This can be either, the client will ask for both if no current db2 info is found
    uint32 count = 0;
    for(SULF_MAP<uint32, uint8>::iterator itr = ItemPrototypeStorage.HotfixBegin(); itr != ItemPrototypeStorage.HotfixEnd(); itr++)
    {
        hotFix << uint32(((itr->second & 0x02) ? 0x50238EC2 : 0x919BE54E));
        hotFix << uint32(UNIXTIME);
        hotFix << itr->first;
        count++;
    }
    hotFix.put<uint32>(0, count);
    SendPacket(&hotFix);*/

    WorldPacket data(SMSG_CLIENTCACHE_VERSION, 4);
    data << uint32(13623);
    SendPacket(&data);

    data.Initialize(SMSG_TUTORIAL_FLAGS, 4 * 8);
    for (uint32 i = 0; i < 8; ++i)
        data << uint32(mSession->GetTutorialFlag(i));
    SendPacket(&data);

    if(addonPacket)
    {
        sAddonMgr.SendAddonInfoPacket(addonPacket, pSession);
        delete addonPacket;
        addonPacket = NULL;
    }

    pSession->_latency = _latency;
    sWorld.AddSession(pSession);
    sWorld.AddGlobalSession(pSession);
}

void WorldSocket::SendAuthResponse(uint8 code, bool holdsPosition, uint32 position)
{
    WorldPacket data(SMSG_AUTH_RESPONSE, 15);
    data << uint8(code) << uint32(0) << uint8(0) << uint32(0);
    if(mSession)
    {
        if(mSession->HasFlag(ACCOUNT_FLAG_XPACK_03))
            data << uint8(3)<< uint8(3);
        else if(mSession->HasFlag(ACCOUNT_FLAG_XPACK_02))
            data << uint8(2)<< uint8(2);
        else if(mSession->HasFlag(ACCOUNT_FLAG_XPACK_01))
            data << uint8(1)<< uint8(1);
        else
            data << uint8(0)<< uint8(0);
    }
    else data << uint8(0)<< uint8(0);
    if(holdsPosition)
        data << position << uint8(0);
    SendPacket(&data);
}

void WorldSocket::_HandlePing(WorldPacket* recvPacket)
{
    uint32 ping;

    *recvPacket >> _latency;
    *recvPacket >> ping;

    if(mSession)
    {
        mSession->_latency = _latency;
        mSession->m_lastPing = (uint32)UNIXTIME;

        // reset the move time diff calculator, don't worry it will be re-calculated next movement packet.
        mSession->m_clientTimeDelay = 0;
    }

    OutPacket(SMSG_PONG, 4, &ping);

#ifdef WIN32
    // Dynamically change nagle buffering status based on latency.
    if(_latency >= 250)
    {
        if(!m_nagleEanbled)
        {
            u_long arg = 0;
            setsockopt(GetFd(), 0x6, 0x1, (const char*)&arg, sizeof(arg));
            m_nagleEanbled = true;
        }
    }
    else
    {
        if(m_nagleEanbled)
        {
            u_long arg = 1;
            setsockopt(GetFd(), 0x6, 0x1, (const char*)&arg, sizeof(arg));
            m_nagleEanbled = false;
        }
    }
#endif
}

void WorldSocket::OnRecvData()
{
    for(;;)
    {
        // Check for the header if we don't have any bytes to wait for.
        if(mRemaining == 0)
        {
            if(GetReadBuffer()->GetSize() < 6)
            {
                // No header in the packet, let's wait.
                return;
            }

            // Copy from packet buffer into header local var
            ClientPktHeader Header;
            Read(&Header, 6);

            // Decrypt the header
            _crypt.DecryptRecv((uint8*)&Header, sizeof (ClientPktHeader));
            mRemaining = ntohs(Header.size) - 4;
            mUnaltered = Header.cmd;
            mOpcode = sOpcodeMgr.ConvertOpcodeForInput(Header.cmd);
        }

        if(mRemaining > 0)
        {
            if( GetReadBuffer()->GetSize() < mRemaining )
            {
                // We have a fragmented packet. Wait for the complete one before proceeding.
                return;
            }
        }

        WorldPacket *Packet = new WorldPacket(mOpcode, mRemaining);
        if(mRemaining > 0)
        {
            Packet->resize(mRemaining);
            Read((uint8*)Packet->contents(), mRemaining);

            if(!bServerShutdown)
                sWorld.NetworkStressIn += float(float(mRemaining+6)/1024);
        }
        mRemaining = mOpcode = 0;

        // Check for packets that we handle
        switch(Packet->GetOpcode())
        {
        case CMSG_PING:
            {
                _HandlePing(Packet);
                delete Packet;
            }break;
        case CMSG_AUTH_SESSION:
            {
                _HandleAuthSession(Packet);
            }break;
        case MSG_NULL_ACTION:
            { // We need to log opcodes that are non existent
                //mUnaltered
                printf("Received unhandled packet %u(%u)\n", mUnaltered, Packet->GetOpcode());
                delete Packet;
                Packet = NULL;
            }break;
        default:
            {
                if(mSession)
                    mSession->QueuePacket(Packet);
                else
                {
                    delete Packet;
                    Packet = NULL;
                }
            }break;
        }
        mUnaltered = 0;
    }
}
