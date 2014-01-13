/***
 * Demonstrike Core
 */

#pragma once

enum FactionMasks
{
    FACTION_MASK_NONE       = 0,
    FACTION_MASK_PLAYER     = 1,
    FACTION_MASK_ALLIANCE   = 2,
    FACTION_MASK_HORDE      = 4,
    FACTION_MASK_MONSTER    = 8
};

enum FactionInteractionStatus
{
    FI_STATUS_NONE      = 0,
    FI_STATUS_FRIENDLY  = 1,
    FI_STATUS_HOSTILE   = 2
};

class SERVER_DECL FactionSystem : public Singleton<FactionSystem>
{
public:
    FactionSystem();
    ~FactionSystem();

    void LoadFactionInteractionData();

public:
    bool CanEitherUnitAttack(Unit* objA, Unit* objB, bool CheckStealth = true);

    bool AC_GetAttackableStatus(Player* plr, Unit *target);

//private:
    // 0 - friendly, 1 - hostile
    int GetAreaInteractionStatus(Unit *unitA, Unit *unitB);
    int GetFactionsInteractStatus(Unit *unitA, Unit *unitB);

    int intisAttackable(Object* objA, Object* objB, bool CheckStealth);

    // System checks
    bool isHostile(Object* objA, Object* objB);
    bool isAttackable(Object* objA, Object* objB, bool CheckStealth = true);
    bool isCombatSupport(Object* objA, Object* objB); // B combat supports A?;
    bool isAlliance(Object* objA); // A is alliance?
    Player* GetPlayerFromObject(Object* obj);

    HEARTHSTONE_INLINE bool isFriendly(Object *objA, Object *objB) { return !isHostile(objA, objB); }
    HEARTHSTONE_INLINE bool isSameFaction(Object* objA, Object* objB)
    {
        // shouldn't be necessary but still
        if( objA->m_faction == NULL || objB->m_faction == NULL )
            return false;

        return (objB->m_faction->Faction == objA->m_faction->Faction);
    }

};

#define sFactionSystem FactionSystem::getSingleton()
