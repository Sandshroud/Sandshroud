/***
 * Demonstrike Core
 * Citric
 */

#pragma once

#pragma pack(push,1)

struct AchievementEntry
{
    uint32 ID;                      // 0
    uint32 factionFlag;             // 1 -1=all, 0=horde, 1=alliance
    uint32 mapID;                   // 2 -1=none
    uint32 Previous_achievement;    // 3 Lots of 0's
    char* name;                     // 4
    char* description;              // 5
    uint32 categoryId;              // 6 Category
    uint32 points;                  // 7 reward points
//  uint32 OrderInCategory;         // 8
    uint32 flags;                   // 9
//  uint32 icon;                    // 10 - SpellIcon.dbc
//  char* RewardTitle;              // 11 - Rewarded Title

//  Used for counting criteria.
//  Example: http://www.wowhead.com/achievement=1872
    uint32 count;                   // 12
    uint32 refAchievement;          // 13

    uint32 AssociatedCriteria[32];  // Custom stuff
    uint32 AssociatedCriteriaCount;
};

struct AchievementCriteriaEntry
{
    uint32 ID;                      // 0
    uint32 referredAchievement;     // 1
    uint32 requiredType;            // 2
    union
    {
        // ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE = 0
        // TODO: also used for player deaths..
        struct
        {
            uint32 creatureID;                          // 3
            uint32 creatureCount;                       // 4
        } kill_creature;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_BG = 1
        // TODO: there are further criterias instead just winning
        struct
        {
            uint32 bgMapID;                         // 3
            uint32 winCount;                            // 4
        } win_bg;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL = 5
        struct
        {
            uint32 unused;                              // 3
            uint32 level;                               // 4
        } reach_level;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL = 7
        struct
        {
            uint32 skillID;                         // 3
            uint32 skillLevel;                          // 4
        } reach_skill_level;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT = 8
        struct
        {
            uint32 linkedAchievement;                   // 3
        } complete_achievement;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT = 9
        struct
        {
            uint32 unused;                              // 3
            uint32 totalQuestCount;                 // 4
        } complete_quest_count;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST_DAILY = 10
        struct
        {
            uint32 unused;                              // 3
            uint32 numberOfDays;                        // 4
        } complete_daily_quest_daily;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE = 11
        struct
        {
            uint32 zoneID;                              // 3
            uint32 questCount;                          // 4
        } complete_quests_in_zone;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST = 14
        struct
        {
            uint32 unused;                              // 3
            uint32 questCount;                          // 4
        } complete_daily_quest;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND= 15
        struct
        {
            uint32 mapID;                               // 3
        } complete_battleground;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP= 16
        struct
        {
            uint32 mapID;                               // 3
        } death_at_map;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_RAID = 19
        struct
        {
            uint32 groupSize;                           // 3 can be 5, 10 or 25
        } complete_raid;

        // ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE = 20
        struct
        {
            uint32 creatureEntry;                       // 3
        } killed_by_creature;

        // ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING = 24
        struct
        {
            uint32 unused;                              // 3
            uint32 fallHeight;                          // 4
        } fall_without_dying;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST = 27
        struct
        {
            uint32 questID;                         // 3
            uint32 questCount;                          // 4
        } complete_quest;

        // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET = 28
        // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2= 69
        struct
        {
            uint32 spellID;                         // 3
            uint32 spellCount;                          // 4
        } be_spell_target;

        // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL= 29
        struct
        {
            uint32 spellID;                         // 3
            uint32 castCount;                           // 4
        } cast_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA = 31
        struct
        {
            uint32 areaID;                              // 3 Reference to AreaTable.dbc
            uint32 killCount;                           // 4
        } honorable_kill_at_area;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA = 32
        struct
        {
            uint32 mapID;                               // 3 Reference to Map.dbc
        } win_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA = 33
        struct
        {
            uint32 mapID;                               // 3 Reference to Map.dbc
        } play_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL = 34
        struct
        {
            uint32 spellID;                         // 3 Reference to Map.dbc
        } learn_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM = 36
        struct
        {
            uint32 itemID;                              // 3
            uint32 itemCount;                           // 4
        } own_item;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA = 37
        struct
        {
            uint32 unused;                              // 3
            uint32 count;                               // 4
            uint32 flag;                                // 5 4=in a row
        } win_rated_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING = 38
        struct
        {
            uint32 teamtype;                            // 3 {2,3,5}
        } highest_team_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_TEAM_RATING = 39
        struct
        {
            uint32 teamtype;                            // 3 {2,3,5}
            uint32 teamrating;                          // 4
        } reach_team_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL = 40
        struct
        {
            uint32 skillID;                         // 3
            uint32 skillLevel;                          // 4 apprentice=1, journeyman=2, expert=3, artisan=4, master=5, grand master=6
        } learn_skill_level;

        // ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM = 41
        struct
        {
            uint32 itemID;                              // 3
            uint32 itemCount;                           // 4
        } use_item;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM = 42
        struct
        {
            uint32 itemID;                              // 3
            uint32 itemCount;                           // 4
        } loot_item;

        // ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA = 43
        struct
        {
            // TODO: This rank is _NOT_ the index from AreaTable.dbc
            uint32 areaReference;                       // 3
        } explore_area;

        // ACHIEVEMENT_CRITERIA_TYPE_OWN_RANK= 44
        struct
        {
            // TODO: This rank is _NOT_ the index from CharTitles.dbc
            uint32 rank;                                // 3
        } own_rank;

        // ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT= 45
        struct
        {
            uint32 unused;                              // 3
            uint32 numberOfSlots;                       // 4
        } buy_bank_slot;

        // ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION= 46
        struct
        {
            uint32 factionID;                           // 3
            uint32 reputationAmount;                    // 4 Total reputation amount, so 42000 = exalted
        } gain_reputation;

        // ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION= 47
        struct
        {
            uint32 unused;                              // 3
            uint32 numberOfExaltedFactions;         // 4
        } gain_exalted_reputation;

        // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM = 49
        // TODO: where is the required itemlevel stored?
        struct
        {
            uint32 itemSlot;                            // 3
        } equip_epic_item;

        // ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT= 50
        struct
        {
            uint32 rollValue;                           // 3
            uint32 count;                               // 4
        } roll_need_on_loot;

        // ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS = 52
        struct
        {
            uint32 classID;                         // 3
            uint32 count;                               // 4
        } hk_class;

        // ACHIEVEMENT_CRITERIA_TYPE_HK_RACE = 53
        struct
        {
            uint32 raceID;                              // 3
            uint32 count;                               // 4
        } hk_race;

        // ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE = 54
        // TODO: where is the information about the target stored?
        struct
        {
            uint32 emoteID;                         // 3
        } do_emote;

        // ACHIEVEMENT_CRITERIA_TYPE_HEALING_DONE = 55
        struct
        {
            uint32 unused;                              // 3
            uint32 count;                               // 4
            uint32 flag;                                // 5 =3 for battleground healing
            uint32 mapid;                               // 6
        } healing_done;

        // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM = 57
        struct
        {
            uint32 itemID;                              // 3
        } equip_item;


        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY = 67
        struct
        {
            uint32 unused;                              // 3
            uint32 goldInCopper;                        // 4
        } loot_money;

        // ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT = 68
        struct
        {
            uint32 goEntry;                         // 3
            uint32 useCount;                            // 4
        } use_gameobject;

        // ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL= 70
        // TODO: are those special criteria stored in the dbc or do we have to add another sql table?
        struct
        {
            uint32 unused;                              // 3
            uint32 killCount;                           // 4
        } special_pvp_kill;

        // ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT = 72
        struct
        {
            uint32 goEntry;                         // 3
            uint32 lootCount;                           // 4
        } fish_in_gameobject;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS= 75
        struct
        {
            uint32 skillLine;                           // 3
            uint32 spellCount;                          // 4
        } learn_skilline_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL = 76
        struct
        {
            uint32 unused;                              // 3
            uint32 duelCount;                           // 4
        } win_duel;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_POWER = 96
        struct
        {
            uint32 powerType;                           // 3 mana=0, 1=rage, 3=energy, 6=runic power
        } highest_power;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_STAT = 97
        struct
        {
            uint32 statType;                            // 3 4=spirit, 3=int, 2=stamina, 1=agi, 0=strength
        } highest_stat;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_SPELLPOWER = 98
        struct
        {
            uint32 spellSchool;                     // 3
        } highest_spellpower;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_RATING = 100
        struct
        {
            uint32 ratingType;                          // 3
        } highest_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE = 109
        struct
        {
            uint32 lootType;                            // 3 3=fishing, 2=pickpocket, 4=disentchant
            uint32 lootTypeCount;                       // 4
        } loot_type;

        // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2 = 110
        struct
        {
            uint32 skillLine;                           // 3
            uint32 spellCount;                          // 4
        } cast_spell2;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE= 112
        struct
        {
            uint32 skillLine;                           // 3
            uint32 spellCount;                          // 4
        } learn_skill_line;

        // ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL = 113
        struct
        {
            uint32 unused;                              // 3
            uint32 killCount;                           // 4
        } honorable_kill;

        struct
        {
            uint32 field3;                              // 3 main requirement
            uint32 field4;                              // 4 main requirement count
            uint32 additionalRequirement1_type;         // 5 additional requirement 1 type
            uint32 additionalRequirement1_value;        // 6 additional requirement 1 value
            uint32 additionalRequirement2_type;         // 7 additional requirement 2 type
            uint32 additionalRequirement2_value;        // 8 additional requirement 1 value
        } raw;
    };

    char* name;
    uint32 completionFlag;
    uint32 timedData[3];
    uint32 showOrder;
    //uint32 unk[2];
    uint32 moreRequirement[3];
    uint32 moreRequirementValue[3];
};

#pragma pack(pop)
