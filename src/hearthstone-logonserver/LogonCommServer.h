/***
 * Demonstrike Core
 */

#ifndef __LOGON_COMM_SERVER_H
#define __LOGON_COMM_SERVER_H

#include <RC4Engine.h>
#include "WorldPacket.h"

class LogonCommServerSocket : public TcpSocket
{
	uint32 remaining;
	uint16 opcode;
	uint32 seed;
	RC4Engine sendCrypto;
	RC4Engine recvCrypto;
public:
	uint32 authenticated;
	bool use_crypto;

	LogonCommServerSocket(SOCKET fd, const sockaddr_in * peer);
	~LogonCommServerSocket();

	void OnRecvData();
	void OnDisconnect();
	void OnConnect();
	void SendPacket(WorldPacket * data);
	void HandlePacket(WorldPacket & recvData);

	void HandleRegister(WorldPacket & recvData);
	void HandlePong(WorldPacket & recvData);
	void HandleSessionRequest(WorldPacket & recvData);
	void HandleSQLExecute(WorldPacket & recvData);
	void HandleReloadAccounts(WorldPacket & recvData);
	void HandleAuthChallenge(WorldPacket & recvData);
	void HandleMappingReply(WorldPacket & recvData);
	void HandleUpdateMapping(WorldPacket & recvData);
	void HandleTestConsoleLogin(WorldPacket & recvData);
	void HandleServerPong(WorldPacket &recvData);
	void HandleDatabaseModify(WorldPacket& recvData);
	void HandlePopulationRespond(WorldPacket& recvData);

	void SendPing();
	void RefreshRealmsPop();
	void SendRPing();

	uint32 latency;
	uint32 last_ping;
	uint32 last_pong;
	bool removed;
	set<uint32> server_ids;
};

typedef void (LogonCommServerSocket::*logonpacket_handler)(WorldPacket&);

#endif
