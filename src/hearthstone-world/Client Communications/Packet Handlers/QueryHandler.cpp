/***
 * Demonstrike Core
 */

#include "StdAfx.h"

//////////////////////////////////////////////////////////////
/// This function handles CMSG_NAME_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleNameQueryOpcode( WorldPacket & recv_data )
{
    CHECK_PACKET_SIZE(recv_data, 8);
    uint64 guid;
    recv_data >> guid;

    PlayerInfo *pn = objmgr.GetPlayerInfo( (uint32)guid );
    if(pn == NULL)
        return;

    // We query our own name on player create so check to send MOTD
    if(!_player->sentMOTD)
    {
        _player->sendMOTD();
        _player->sentMOTD = true;
    }

    sLog.Debug("WorldSession","Received CMSG_NAME_QUERY for: %s", pn->name );
    WorldPacket data(SMSG_NAME_QUERY_RESPONSE, 10000);
    FastGUIDPack(data, guid);
    data << uint8(0);
    data << pn->name;
    data << uint8(0);
    data << uint8(pn->race);
    data << uint8(pn->gender);
    data << uint8(pn->_class);
    data << uint8(0);
    SendPacket( &data );
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_QUERY_TIME:
//////////////////////////////////////////////////////////////
void WorldSession::HandleQueryTimeOpcode( WorldPacket & recv_data )
{
    WorldPacket data(SMSG_QUERY_TIME_RESPONSE, 8);
    data << uint64(UNIXTIME);
    SendPacket(&data);
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_CREATURE_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleCreatureQueryOpcode( WorldPacket & recv_data )
{
    CHECK_PACKET_SIZE(recv_data, 12);
    uint32 entry;
    uint64 guid;

    recv_data >> entry;
    recv_data >> guid;

    WorldPacket data(SMSG_CREATURE_QUERY_RESPONSE, 150);
    if(entry == 300000)
    {
        data << (uint32)entry;
        data << "WayPoint";
        data << uint8(0) << uint8(0) << uint8(0);
        data << "Level is WayPoint ID";
        for(uint32 i = 0; i < 10; i++)
            data << uint32(0);
        data << float(0.0f);
        data << float(0.0f);
        data << uint8(0);
        for(uint32 i = 0; i < 7; i++)
            data << uint32(0);
        SendPacket( &data );
        return;
    }

    CreatureInfo* ci = CreatureNameStorage.LookupEntry(entry);
    if(ci == NULL)
        return;

    sLog.Debug("WORLD","HandleCreatureQueryOpcode CMSG_CREATURE_QUERY '%s'", ci->Name);
    data << entry;
    data << ci->Name << uint8(0) << uint8(0) << uint8(0);
    data << ci->SubName;
    data << ci->info_str; //!!! this is a string in 2.3.0 Example: stormwind guard has : "Direction"
    data << ci->Flags1;
    data << ci->Type;
    data << ci->Family;
    data << ci->Rank;
    data << ci->Unk[0];
    data << ci->Unk[1];
    data << ci->Male_DisplayID;
    data << ci->Female_DisplayID;
    data << ci->Male_DisplayID2;
    data << ci->Female_DisplayID2;
    data << ci->modHealth;
    data << ci->modMana;
    data << ci->Leader;
    ObjectQuestLoot* objQuestLoot = lootmgr.GetCreatureQuestLoot(entry);
    for(uint32 i = 0; i < 6; i++)
        data << uint32(objQuestLoot ? objQuestLoot->QuestLoot[i] : 0); // QuestItems
    data << uint32(0);  // CreatureMovementInfo.dbc
    data << uint32(0);
    SendPacket( &data );
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_GAMEOBJECT_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleGameObjectQueryOpcode( WorldPacket & recv_data )
{
    CHECK_PACKET_SIZE(recv_data, 12);
    WorldPacket data(SMSG_GAMEOBJECT_QUERY_RESPONSE, 150);

    uint32 entryID;
    uint64 guid;
    recv_data >> entryID;
    recv_data >> guid;

    sLog.Debug("WORLD","HandleGameObjectQueryOpcode CMSG_GAMEOBJECT_QUERY '%u'", entryID);

    GameObjectInfo* goinfo = GameObjectNameStorage.LookupEntry(entryID);
    if(goinfo == NULL)
        return;

    data << entryID;
    data << goinfo->Type;
    data << goinfo->DisplayID;
    data << goinfo->Name << uint8(0) << uint8(0) << uint8(0);
    data << goinfo->Icon;
    data << goinfo->CastBarText;
    data << uint8(0);
    for(uint32 d = 0; d < 24; d++)
        data << goinfo->RawData.ListedData[d];
    data << float(1.f);
    ObjectQuestLoot* objQuestLoot = lootmgr.GetGameObjectQuestLoot(entryID);
    for(uint32 i = 0; i < 6; i++)
        data << uint32(objQuestLoot ? objQuestLoot->QuestLoot[i] : 0); // QuestItems
    data << uint32(0);
    SendPacket( &data );
}

void BuildCorpseInfo(WorldPacket* data, Corpse* corpse)
{
    MapInfo *pMapinfo = WorldMapInfoStorage.LookupEntry(corpse->GetMapId());
    if(pMapinfo == NULL || (pMapinfo->type == INSTANCE_NULL || pMapinfo->type == INSTANCE_PVP))
    {
        *data << uint8(0x01); //show ?
        *data << corpse->GetMapId(); // mapid (that tombstones shown on)
        *data << corpse->GetPositionX();
        *data << corpse->GetPositionY();
        *data << corpse->GetPositionZ();
        *data << corpse->GetMapId();
    }
    else
    {
        *data << uint8(0x01); //show ?
        *data << pMapinfo->repopmapid; // mapid (that tombstones shown on)
        *data << pMapinfo->repopx;
        *data << pMapinfo->repopy;
        *data << pMapinfo->repopz;
        *data << corpse->GetMapId(); //instance mapid (needs to be same as mapid to be able to recover corpse)
    }
}

//////////////////////////////////////////////////////////////
/// This function handles MSG_CORPSE_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleCorpseQueryOpcode(WorldPacket &recv_data)
{
    sLog.outDebug("WORLD: Received MSG_CORPSE_QUERY");

    Corpse *pCorpse= objmgr.GetCorpseByOwner(GetPlayer()->GetLowGUID());
    if(pCorpse == NULL)
        return;

    WorldPacket data(MSG_CORPSE_QUERY, 21);
    BuildCorpseInfo(&data, pCorpse);
    SendPacket(&data);
}

void WorldSession::HandlePageTextQueryOpcode( WorldPacket & recv_data )
{
    CHECK_PACKET_SIZE(recv_data, 4);
    uint32 pageid = 0;
    uint64 itemguid;
    recv_data >> pageid;
    recv_data >> itemguid;

    WorldPacket data(SMSG_PAGE_TEXT_QUERY_RESPONSE, 300);
    while(pageid)
    {
        data.clear();
        ItemPage * page = ItemPageStorage.LookupEntry(pageid);
        if(page == NULL)
            return;

        char* text = page->text;
        if(text == NULL || *text == NULL)
            return;

        data << pageid;
        data << text;
        data << page->next_page;
        pageid = page->next_page;
        SendPacket(&data);
    }
}

void WorldSession::HandleInrangeQuestgiverQuery(WorldPacket & recv_data)
{
    CHECK_INWORLD_RETURN();

    WorldPacket data(SMSG_QUESTGIVER_STATUS_MULTIPLE, 1000);
    Object::InRangeSet::iterator itr;
    Creature* pCreature;
    uint32 count = 0;
    data << count;
    for( itr = _player->m_objectsInRange.begin(); itr != _player->m_objectsInRange.end(); itr++ )
    {
        pCreature = TO_CREATURE(*itr);
        if( pCreature->GetTypeId() != TYPEID_UNIT )
            continue;

        if( pCreature->isQuestGiver() )
        {
            data << pCreature->GetGUID();
            data << sQuestMgr.CalcStatus( pCreature, _player );
            ++count;
        }
    }

    data.put<uint32>(0, count);
    SendPacket(&data);
}