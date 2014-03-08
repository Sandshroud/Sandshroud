/***
 * Demonstrike Core
 */

#pragma once

#include "../hearthstone-shared/Client/OpcodeList.h"

class OpcodeManager : public Singleton<OpcodeManager>
{
public:
    OpcodeManager();
    ~OpcodeManager();

    void SetOpcodeListData();

    uint32 ConvertOpcodeForInput(uint32 opcode);
    uint32 ConvertOpcodeForOutput(uint32 opcode);
    const char* GetOpcodeName(uint32 opcode);

private:
    void SetOpcodeData(uint32 opcode, char* name, uint32 newOpcode);

    std::map<uint32, uint32> outputMap, inputMap;
    std::map<uint32, char*> opcodeNames;
};

#define sOpcodeMgr OpcodeManager::getSingleton()

enum FriendsResult {
    FRIEND_DB_ERROR             = 0x00,
    FRIEND_LIST_FULL            = 0x01,
    FRIEND_ONLINE               = 0x02,
    FRIEND_OFFLINE              = 0x03,
    FRIEND_NOT_FOUND            = 0x04,
    FRIEND_REMOVED              = 0x05,
    FRIEND_ADDED_ONLINE         = 0x06,
    FRIEND_ADDED_OFFLINE        = 0x07,
    FRIEND_ALREADY              = 0x08,
    FRIEND_SELF                 = 0x09,
    FRIEND_ENEMY                = 0x0A,
    FRIEND_IGNORE_FULL          = 0x0B,
    FRIEND_IGNORE_SELF          = 0x0C,
    FRIEND_IGNORE_NOT_FOUND     = 0x0D,
    FRIEND_IGNORE_ALREADY       = 0x0E,
    FRIEND_IGNORE_ADDED         = 0x0F,
    FRIEND_IGNORE_REMOVED       = 0x10,
    FRIEND_IGNORE_AMBIGUOUS     = 0x11,
    FRIEND_MUTE_FULL            = 0x12,
    FRIEND_MUTE_SELF            = 0x13,
    FRIEND_MUTE_NOT_FOUND       = 0x14,
    FRIEND_MUTE_ALREADY         = 0x15,
    FRIEND_MUTE_ADDED           = 0x16,
    FRIEND_MUTE_REMOVED         = 0x17,
    FRIEND_MUTE_AMBIGUOUS       = 0x18,
    FRIEND_UNK7                 = 0x19,
    FRIEND_UNKNOWN              = 0x1A
};

enum NPCFlags {
/* 2.4.3 old format
//                                  Hex             Decimal         Bit  Comments
    UNIT_NPC_FLAG_NONE              = 0x00000000,   //0,            01
    UNIT_NPC_FLAG_GOSSIP            = 0x00000001,   //1,            02  Gossip/Talk (CMSG_GOSSIP_HELLO)
    UNIT_NPC_FLAG_QUESTGIVER        = 0x00000002,   //2,            03  Questgiver
    UNIT_NPC_FLAG_UNK1              = 0x00000004,   //4,            04
    UNIT_NPC_FLAG_UNK2              = 0x00000008,   //8,            05
    UNIT_NPC_FLAG_TRAINER_CLASS     = 0x00000020,   //32,           07
    UNIT_NPC_FLAG_TRAINER_PROF      = 0x00000040,   //64,           08
    UNIT_NPC_FLAG_VENDOR            = 0x00000080,   //128,          09  Vendor (CMSG_LIST_INVENTORY)
    UNIT_NPC_FLAG_VENDOR_AMMO       = 0x00000100,   //256,          10
    UNIT_NPC_FLAG_VENDOR_FOOD       = 0x00000200,   //512,          11
    UNIT_NPC_FLAG_VENDOR_POISON     = 0x00000400,   //1024,         12
    UNIT_NPC_FLAG_VENDOR_REAGENT    = 0x00000800,   //2048,         13
    UNIT_NPC_FLAG_ARMORER           = 0x00001000,   //4096,         14
    UNIT_NPC_FLAG_TAXIVENDOR        = 0x00002000,   //8192,         15  Taxi Vendor (CMSG_TAXIQUERYAVAILABLENODES)
    UNIT_NPC_FLAG_SPIRITHEALER      = 0x00004000,   //16384,        16  Spirithealer
    UNIT_NPC_FLAG_SPIRITGUIDE       = 0x00008000,   //32768,        17
    UNIT_NPC_FLAG_INNKEEPER         = 0x00010000,   //65536,        18  Innkeeper Asking for Bind Point(CMSG_BINDER_ACTIVATE)
    UNIT_NPC_FLAG_BANKER            = 0x00020000,   //131072,       19  Banker
    UNIT_NPC_FLAG_ARENACHARTER      = 0x00040000,   //262144,       20  GuildMasters also have UNIT_NPC_FLAG_TABARDCHANGER!!
    UNIT_NPC_FLAG_TABARDCHANGER     = 0x00080000,   //524288,       21  Tabard Design Changer Flag
    UNIT_NPC_FLAG_BATTLEFIELDPERSON = 0x00100000,   //1048576,      22  BattleMaster (CMSG_BATTLEMASTER_HELLO (0x02D7))
    UNIT_NPC_FLAG_AUCTIONEER        = 0x00200000,   //2097152,      23  Auctioneer (MSG_AUCTION_HELLO)
    UNIT_NPC_FLAG_STABLEMASTER      = 0x00400000,   //4194304,      24  (MSG_LIST_STABLED_PETS (0x026F))
    UNIT_NPC_FLAG_GUILD_BANK        = 0x00800000,   //8388608,      25
    UNIT_NPC_FLAG_SPELLCLICK        = 0x01000000,   //16777216,     26 */

//                                  Hex             Decimal         Bit  Comments
    UNIT_NPC_FLAG_NONE              = 0x00000000,   //0,            00
    UNIT_NPC_FLAG_GOSSIP            = 0x00000001,   //1,            01  Gossip/Talk (CMSG_GOSSIP_HELLO)
    UNIT_NPC_FLAG_QUESTGIVER        = 0x00000002,   //2,            02  Questgiver
//  UNIT_NPC_FLAG_UNK1              = 0x00000004,   //4,            03
//  UNIT_NPC_FLAG_UNK2              = 0x00000008,   //8,            04
    UNIT_NPC_FLAG_TRAINER           = 0x00000010,   //16,           05
//  UNIT_NPC_FLAG_UNK3              = 0x00000020,   //32,           06
//  UNIT_NPC_FLAG_UNK4              = 0x00000040,   //64,           06
    UNIT_NPC_FLAG_VENDOR            = 0x00000080,   //128,          07  Vendor (CMSG_LIST_INVENTORY)
//  UNIT_NPC_FLAG_UNK5              = 0x00000100,   //256,          08
//  UNIT_NPC_FLAG_UNK6              = 0x00000200,   //512,          09
//  UNIT_NPC_FLAG_UNK7              = 0x00000400,   //1024,         10
//  UNIT_NPC_FLAG_UNK8              = 0x00000800,   //2048,         11
    UNIT_NPC_FLAG_ARMORER           = 0x00001000,   //4096,         12
    UNIT_NPC_FLAG_TAXIVENDOR        = 0x00002000,   //8192,         13  Taxi Vendor (CMSG_TAXIQUERYAVAILABLENODES)
    UNIT_NPC_FLAG_TRAINER_PROF      = 0x00004000,   //16384,        14
    UNIT_NPC_FLAG_SPIRITHEALER      = 0x00008000,   //32768,        15  Spirithealer (CMSG_BINDER_ACTIVATE)
    UNIT_NPC_FLAG_INNKEEPER         = 0x00010000,   //65536,        16  Innkeeper Asking for Bind Point
    UNIT_NPC_FLAG_BANKER            = 0x00020000,   //131072,       17  Banker
    UNIT_NPC_FLAG_ARENACHARTER      = 0x00040000,   //262144,       18  GuildMasters also have UNIT_NPC_FLAG_TABARDCHANGER!!
    UNIT_NPC_FLAG_TABARDCHANGER     = 0x00080000,   //524288,       19  Tabard Design Changer Flag
    UNIT_NPC_FLAG_BATTLEFIELDPERSON = 0x00100000,   //1048576,      20  BattleMaster (CMSG_BATTLEMASTER_HELLO (0x02D7))
    UNIT_NPC_FLAG_AUCTIONEER        = 0x00200000,   //2097152,      21  Auctioneer (MSG_AUCTION_HELLO)
    UNIT_NPC_FLAG_STABLEMASTER      = 0x00400000,   //4194304,      22  (MSG_LIST_STABLED_PETS (0x026F))
    UNIT_NPC_FLAG_GUILD_BANK        = 0x00800000,   //8388608,      23
    UNIT_NPC_FLAG_SPELLCLICK        = 0x01000000,   //16777216,     24
    UNIT_NPC_FLAG_VEHICLE_MOUNT     = 0x02000000,   //33554432,     25
};

enum GossipIcons 
{
    GOSSIP_ICON_GOSSIP_NORMAL       = 0,
    GOSSIP_ICON_GOSSIP_VENDOR       = 1,
    GOSSIP_ICON_GOSSIP_FLIGHT       = 2,
    GOSSIP_ICON_GOSSIP_TRAINER      = 3,
    GOSSIP_ICON_GOSSIP_ENGINEER1    = 4,
    GOSSIP_ICON_GOSSIP_ENGINEER2    = 5,
    GOSSIP_ICON_GOSSIP_AUCTION      = 6,
    GOSSIP_ICON_GOSSIP_EXTRA        = 7,
    GOSSIP_ICON_GOSSIP_TABARD       = 8,
    GOSSIP_ICON_GOSSIP_ARENA        = 9,
    GOSSIP_ICON_GOSSIP_COIN         = 10,
    GOSSIP_ICON_GOSSIP_NORMAL_11    = 11,
    GOSSIP_ICON_GOSSIP_NORMAL_12    = 12,
    GOSSIP_ICON_GOSSIP_NORMAL_13    = 13,
    GOSSIP_ICON_DONT_USE            = 14, // Reported to crash client?
    GOSSIP_ICON_DONT_USE2           = 15, // Reported to crash client?
    GOSSIP_ICON_GOSSIP_NORMAL_16    = 16,
    GOSSIP_ICON_GOSSIP_NORMAL_17    = 17,
    GOSSIP_ICON_GOSSIP_NORMAL_18    = 18,
    GOSSIP_ICON_GOSSIP_NORMAL_19    = 19,
    GOSSIP_ICON_GOSSIP_NORMAL_20    = 20,
};

enum Poi_Icon
{
    ICON_POI_BLANK              =   0,
    ICON_POI_GREY_AV_MINE       =   1,
    ICON_POI_RED_AV_MINE        =   2,
    ICON_POI_BLUE_AV_MINE       =   3,
    ICON_POI_BWTOMB             =   4,
    ICON_POI_SMALL_HOUSE        =   5,
    ICON_POI_GREYTOWER          =   6,
    ICON_POI_REDFLAG            =   7,
    ICON_POI_TOMBSTONE          =   8,
    ICON_POI_BWTOWER            =   9,
    ICON_POI_REDTOWER           =   10,
    ICON_POI_BLUETOWER          =   11,
    ICON_POI_RWTOWER            =   12,
    ICON_POI_REDTOMB            =   13,
    ICON_POI_RWTOMB             =   14,
    ICON_POI_BLUETOMB           =   15,
    ICON_POI_16                 =   16,
    ICON_POI_17                 =   17,
    ICON_POI_18                 =   18,
    ICON_POI_19                 =   19,
    ICON_POI_20                 =   20,
    ICON_POI_GREYLOGS           =   21,
    ICON_POI_BWLOGS             =   22,
    ICON_POI_BLUELOGS           =   23,
    ICON_POI_RWLOGS             =   24,
    ICON_POI_REDLOGS            =   25,
    ICON_POI_26                 =   26,
    ICON_POI_27                 =   27,
    ICON_POI_28                 =   28,
    ICON_POI_29                 =   29,
    ICON_POI_30                 =   30,
    ICON_POI_GREYHOUSE          =   31,
    ICON_POI_BWHOUSE            =   32,
    ICON_POI_BLUEHOUSE          =   33,
    ICON_POI_RWHOUSE            =   34,
    ICON_POI_REDHOUSE           =   35,
    ICON_POI_GREYHORSE          =   36,
    ICON_POI_BWHORSE            =   37,
    ICON_POI_BLUEHORSE          =   38,
    ICON_POI_RWHORSE            =   39,
    ICON_POI_REDHORSE           =   40
};

HEARTHSTONE_INLINE bool CheckItemFaction(uint32 ItemFaction, uint32 PlayerTeam)
{
    // Client sets alliance = 2 since 0 = NULL
    // So reverse our checks, this is fucking bullshit.
    if(ItemFaction == 0)
        return true;
    else if(ItemFaction == 1)
    {
        if(PlayerTeam == 1)
            return true;
    }
    else if(ItemFaction == 2)
    {
        if(PlayerTeam == 0)
            return true;
    }
    else return true; // Fucked up DB.

    return false;
};
