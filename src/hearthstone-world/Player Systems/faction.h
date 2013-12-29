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

namespace FactionSystem
{
    // 0 - friendly, 1 - hostile
    SERVER_DECL int GetFactionsInteractStatus(Unit *unitA, Unit *unitB);

    // System checks
    SERVER_DECL bool isHostile(Object* objA, Object* objB);
    SERVER_DECL bool isAttackable(Object* objA, Object* objB, bool CheckStealth = true);
    SERVER_DECL bool isCombatSupport(Object* objA, Object* objB); // B combat supports A?;
    SERVER_DECL bool isAlliance(Object* objA); // A is alliance?
    SERVER_DECL bool CanEitherUnitAttack(Object* objA, Object* objB, bool CheckStealth = true);

    HEARTHSTONE_INLINE bool isFriendly(Object* objA, Object* objB)// B is friendly to A if its not hostile
    {
        return !FactionSystem::isHostile(objA, objB);
    }

    HEARTHSTONE_INLINE bool isSameFaction(Object* objA, Object* objB)
    {
        // shouldn't be necessary but still
        if( objA->m_faction == NULL || objB->m_faction == NULL )
            return false;

        return (objB->m_faction->Faction == objA->m_faction->Faction);
    }

    Player* GetPlayerFromObject(Object* obj);

    SERVER_DECL int intisAttackable(Object* objA, Object* objB, bool CheckStealth);
};
