/***
 * Demonstrike Core
 */

#include "StdAfx.h"

createFileSingleton(FactionSystem);

FactionSystem::FactionSystem()
{

}

FactionSystem::~FactionSystem()
{

}

bool FactionSystem::AC_GetAttackableStatus(Player *plr, Unit *target)
{
    return (intisAttackable(plr, target, false) == 0);
}

int FactionSystem::GetFactionsInteractStatus(Unit *unitA, Unit* unitB)
{
    if(unitA == NULL || unitB == NULL)
        return FI_STATUS_NONE;
    FactionTemplateDBC *factionA = unitA->m_faction, *factionB = unitB->m_faction;
    FactionDBC *factiondbcA = unitA->m_factionDBC, *factiondbcB = unitB->m_factionDBC;
    if(factionA == NULL || factionB == NULL || factiondbcA == NULL || factiondbcB == NULL)
        return FI_STATUS_NONE;
    if(factionA == factionB || factiondbcA == factiondbcB)
        return FI_STATUS_FRIENDLY; // Same faction, we can skip the rest of the checks
    if(factionA->ID == 35 || factionB->ID == 35)
        return FI_STATUS_FRIENDLY; // 35 is forced friendly to all

    // Check hostile masks
    if(factionA->HostileMask & factionB->FactionMask)
        return FI_STATUS_HOSTILE;
    if(factionB->HostileMask & factionA->FactionMask)
        return FI_STATUS_HOSTILE;

    // check friend/enemy list
    for(uint32 i = 0; i < 4; i++)
    {
        if(factionA->EnemyFactions[i] && factionA->EnemyFactions[i] == factionB->Faction)
            return FI_STATUS_HOSTILE;
        if(factionB->EnemyFactions[i] && factionB->EnemyFactions[i] == factionA->Faction)
            return FI_STATUS_HOSTILE;
    }

    // Reputation System Checks
    if(unitA->IsPlayer() && !unitB->IsPlayer() && unitB->m_factionDBC)
    {
        if(unitB->m_factionDBC->RepListId >= 0)
            if(TO_PLAYER(unitA)->IsHostileBasedOnReputation( unitB->m_factionDBC ))
                return FI_STATUS_HOSTILE;

        if(unitB->m_factionDBC->RepListId == -1 && unitB->m_faction->HostileMask == 0 && unitB->m_faction->FriendlyMask == 0)
            return FI_STATUS_HOSTILE;
    }
    else if(unitB->IsPlayer() && !unitA->IsPlayer() && unitA->m_factionDBC)
    {
        if(unitA->m_factionDBC->RepListId >= 0)
            if(TO_PLAYER(unitB)->IsHostileBasedOnReputation( unitA->m_factionDBC ))
                return FI_STATUS_HOSTILE;

        if(unitA->m_factionDBC->RepListId == -1 && unitA->m_faction->HostileMask == 0 && unitA->m_faction->FriendlyMask == 0)
            return FI_STATUS_HOSTILE;
    }

    return FI_STATUS_FRIENDLY;
}

int FactionSystem::GetAreaInteractionStatus(Unit *unitA, Unit *unitB)
{
    // Do not let units attack each other in sanctuary
    if(unitA->HasAreaFlag(OBJECT_AREA_FLAG_INSANCTUARY) || unitB->HasAreaFlag(OBJECT_AREA_FLAG_INSANCTUARY))
    {
        bool allowedCombat = true;
        if(unitA->IsPlayer() && unitB->IsPlayer())
            allowedCombat = false;
        else if((unitA->IsPet() || unitA->IsSummon()) && unitB->IsPlayer())
            allowedCombat = false;
        else if((unitB->IsPet() || unitB->IsSummon()) && unitA->IsPlayer())
            allowedCombat = false;
        if(!allowedCombat)
            return FI_STATUS_NONE;
    }
    return FI_STATUS_HOSTILE;
}

/// Where we check if we object A can attack object B. This is used in many feature's
/// Including the spell class, the player class, and the AI interface class.
int FactionSystem::intisAttackable(Object* objA, Object* objB, bool CheckStealth)// A can attack B?
{
    // can't attack self.. this causes problems with buffs if we don't have it :p
    if( !objA || !objB || objA == objB )
        return FI_STATUS_NONE;

    if( !objA->IsInWorld() )
        return FI_STATUS_NONE;

    // can't attack corpses neither...
    if( objA->GetTypeId() == TYPEID_CORPSE || objB->GetTypeId() == TYPEID_CORPSE )
        return FI_STATUS_NONE;

    // Dead people can't attack anything.
    if( (objA->IsUnit() && !TO_UNIT(objA)->isAlive()) || (objB->IsUnit() && !TO_UNIT(objB)->isAlive()) )
        return FI_STATUS_NONE;

    // Checks for untouchable, unattackable
    if( objA->IsUnit() && (objA->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9) ||
        objA->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI) || objA->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE)))
        return FI_STATUS_NONE;

    if( objB->IsUnit() && (objB->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9) ||
        objB->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI) || objB->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE)))
        return FI_STATUS_NONE;

    if(!objA->PhasedCanInteract(objB))
        return FI_STATUS_NONE;

    // we cannot attack sheathed units. Maybe checked in other places too ?
    // !! warning, this presumes that objA is attacking ObjB
    if( CheckStealth && objB->IsUnit() && TO_UNIT(objB)->InStealth() )
        if(objA->CalcDistance(objB) > 5.0f)
            return FI_STATUS_NONE;

    // Get players (or owners of pets/totems)
    Player* player_objA = GetPlayerFromObject(objA);
    Player* player_objB = GetPlayerFromObject(objB);
    if(objA->IsUnit() && objB->IsVehicle())
        if(TO_VEHICLE(objB)->GetPassengerSlot(TO_UNIT(objA)) != -1)
            return FI_STATUS_NONE;
    else if(objB->IsUnit() && objA->IsVehicle())
        if(TO_VEHICLE(objA)->GetPassengerSlot(TO_UNIT(objB)) != -1)
            return FI_STATUS_NONE;

    // Always kill critters
    if(!player_objB && objB->IsCreature() && TO_CREATURE(objB)->GetCreatureType() == CRITTER)
        if(player_objA)
            return FI_STATUS_HOSTILE;

    // Disable GM attacking.
    if(player_objA && player_objB && player_objA->bGMTagOn)
        return FI_STATUS_FRIENDLY;

    // Disable GM attacking.
    if(player_objA && !player_objB && player_objA->bGMTagOn)
        return FI_STATUS_FRIENDLY;

    // Don't allow players to attack GMs
    if(player_objA && player_objB && player_objB->bGMTagOn)
        return FI_STATUS_FRIENDLY;

    // Creatures cannot attack a GM with tag on.
    if(!player_objA && player_objB && player_objB->bGMTagOn)
        return FI_STATUS_FRIENDLY;

    if(objA->IsCreature() && isTargetDummy(objA->GetEntry()))
        return FI_STATUS_NONE; // Bwahahaha

    if( player_objA && player_objB )
    {
        if(player_objA->DuelingWith == player_objB && player_objA->GetDuelState() == DUEL_STATE_STARTED )
            return FI_STATUS_HOSTILE;
    }
    else if(player_objA)
    {
        if(objB->IsPet() && TO_PET(objB)->GetOwner()->DuelingWith == player_objA)
            return FI_STATUS_HOSTILE;
        if(objB->IsSummon())
        {
            Object* summoner = TO_SUMMON(objB)->GetSummonOwner();
            if(summoner && summoner->IsPlayer())
            {
                if(TO_PLAYER(summoner)->DuelingWith == player_objA)
                    return FI_STATUS_HOSTILE;
            }
        }
    }
    else if(player_objB)
    {
        if(objA->IsPet() && TO_PET(objA)->GetOwner()->DuelingWith == player_objB)
            return FI_STATUS_HOSTILE;
        if(objA->IsSummon())
        {
            Object* summoner = TO_SUMMON(objA)->GetSummonOwner();
            if(summoner && summoner->IsPlayer())
            {
                if(TO_PLAYER(summoner)->DuelingWith == player_objB)
                    return FI_STATUS_HOSTILE;
            }
        }
    }

    // Do not let units attack each other in sanctuary
    if(objA->HasAreaFlag(OBJECT_AREA_FLAG_INSANCTUARY) || objB->HasAreaFlag(OBJECT_AREA_FLAG_INSANCTUARY) )
    {
        bool allowedCombat = true;
        if(objA->IsPlayer() && objB->IsPlayer())
            allowedCombat = false;
        else if(objA->IsPet() || objB->IsPet())
            allowedCombat = false;
        else if(objA->IsSummon() || objB->IsSummon())
            allowedCombat = false;
        else if(objA->m_faction == NULL || objB->m_faction == NULL || objA->m_faction == objB->m_faction)
            allowedCombat = false;
        else if(objA->m_faction->ID == 35 || objB->m_faction->ID == 35)
            allowedCombat = false;
        if(!allowedCombat)
            return FI_STATUS_NONE;
    }

    if(objA->IsCreature())
    {
        if(objA->IsPet())
        {
            if(player_objB && !player_objB->IsPvPFlagged())
                return FI_STATUS_NONE;

            if(player_objB)
            {
                if(TO_PET(objA)->GetOwner())
                {
                    if(!TO_PET(objA)->GetOwner()->IsPvPFlagged())
                        return FI_STATUS_NONE;
                    // the target is PvP, its okay.
                }
                else
                    return FI_STATUS_NONE;
            }
        }
        else if(objA->IsSummon())
        {
            if(player_objB && !player_objB->IsPvPFlagged())
                return FI_STATUS_NONE;

            if(player_objB)
            {
                Object* summonownerA = TO_SUMMON(objA)->GetSummonOwner();
                if(summonownerA && summonownerA->IsPlayer())
                {
                    if(!TO_UNIT(summonownerA)->IsPvPFlagged())
                        return FI_STATUS_NONE;
                    // the target is PvP, its okay.
                }
                else
                    return FI_STATUS_NONE;
            }
        }
    }

    if( player_objA && player_objB )
    {
        if(player_objA->IsPvPFlagged() && !player_objB->IsPvPFlagged() && player_objA->DuelingWith != player_objB)
            return FI_STATUS_NONE;
        if(!player_objA->IsPvPFlagged() && !player_objB->IsPvPFlagged() && player_objA->DuelingWith != player_objB)
            return FI_STATUS_NONE;
        if(player_objA->IsFFAPvPFlagged() && player_objB->IsFFAPvPFlagged())
        {
            if( player_objA->GetGroup() && player_objA->GetGroup() == player_objB->GetGroup() )
                return FI_STATUS_NONE;

            if( player_objA == player_objB ) // Totems...
                return FI_STATUS_NONE;

            return FI_STATUS_HOSTILE;       // can hurt each other in FFA pvp
        }

        //Handle BG's
        if( player_objA->m_bg != NULL)
        {
            //Handle Arenas
            if( player_objA->GetTeam() != player_objB->GetTeam() )
                return FI_STATUS_HOSTILE;
        }

        // same faction can't kill each other.
        if(player_objA->m_faction == player_objB->m_faction)
            return FI_STATUS_NONE;

        return FI_STATUS_HOSTILE; // Skip the rest of this, it's all faction shit.
    }

    return GetFactionsInteractStatus(TO_UNIT(objA), TO_UNIT(objB));
}

bool FactionSystem::CanEitherUnitAttack(Unit* objA, Unit* objB, bool CheckStealth)// A can attack B?
{
    // can't attack self.. this causes problems with buffs if we don't have it :p
    if( !objA || !objB || objA == objB )
        return false;

    if( !objA->IsInWorld() || !objB->IsInWorld() )
        return false;

    // can't attack corpses neither...
    if( objA->GetTypeId() == TYPEID_CORPSE || objB->GetTypeId() == TYPEID_CORPSE )
        return false;

    // Dead people can't attack anything.
    if( (objA->IsUnit() && !TO_UNIT(objA)->isAlive()) || (objB->IsUnit() && !TO_UNIT(objB)->isAlive()) )
        return false;

    // Checks for untouchable, unattackable
    if( objA->IsUnit() && (objA->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9) ||
        objA->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI) || objA->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE)))
        return false;

    if( objB->IsUnit() && (objB->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9) ||
        objB->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI) || objB->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE)))
        return false;

    if(!objA->PhasedCanInteract(objB) || !objB->PhasedCanInteract(objA))
        return false;

    // we cannot attack sheathed units. Maybe checked in other places too ?
    if(CheckStealth)
    {
        if( objB->IsUnit() && TO_UNIT(objB)->InStealth() )
            if(objA->CalcDistance(objB) > 5.0f)
                return false;
        if( objA->IsUnit() && TO_UNIT(objA)->InStealth() )
            if(objB->CalcDistance(objA) > 5.0f)
                return false;
    }

    // Get players (or owners of pets/totems)
    Player* player_objA = GetPlayerFromObject(objA);
    Player* player_objB = GetPlayerFromObject(objB);
    if(objA->IsUnit() && objB->IsVehicle())
        if(TO_VEHICLE(objB)->GetPassengerSlot(TO_UNIT(objA)) != -1)
            return false;
    if(objB->IsUnit() && objA->IsVehicle())
        if(TO_VEHICLE(objA)->GetPassengerSlot(TO_UNIT(objB)) != -1)
            return false;

    // Disable GM attacking.
    if(player_objA && player_objB && player_objA->bGMTagOn)
        return false;

    // Disable GM attacking.
    if(player_objA && !player_objB && player_objA->bGMTagOn)
        return false;

    // Don't allow players to attack GMs
    if(player_objA && player_objB && player_objB->bGMTagOn)
        return false;

    // Creatures cannot attack a GM with tag on.
    if(!player_objA && player_objB && player_objB->bGMTagOn)
        return false;

    if(objA->IsCreature() && isTargetDummy(objA->GetEntry()))
        return false; // Bwahahaha

    if( player_objA && player_objB )
    {
        if(player_objA->DuelingWith == player_objB && player_objA->GetDuelState() == DUEL_STATE_STARTED )
            return true;
    }
    else if(player_objA)
    {
        if(objB->IsPet() && TO_PET(objB)->GetOwner()->DuelingWith == player_objA)
            return true;
        if(objB->IsSummon())
        {
            Object* summoner = TO_SUMMON(objB)->GetSummonOwner();
            if(summoner && summoner->IsPlayer())
            {
                if(TO_PLAYER(summoner)->DuelingWith == player_objA)
                    return true;
            }
        }
    }
    else if(player_objB)
    {
        if(objA->IsPet() && TO_PET(objA)->GetOwner()->DuelingWith == player_objB)
            return true;
        if(objA->IsSummon())
        {
            Object* summoner = TO_SUMMON(objA)->GetSummonOwner();
            if(summoner && summoner->IsPlayer())
            {
                if(TO_PLAYER(summoner)->DuelingWith == player_objB)
                    return true;
            }
        }
    }
    else if(player_objA == NULL && player_objB == NULL) // Ignore players, we have critters in sanctuaries
    {
        // Do not let units attack each other in sanctuary
        // We know they aren't dueling
        if(objA->HasAreaFlag(OBJECT_AREA_FLAG_INSANCTUARY) || objB->HasAreaFlag(OBJECT_AREA_FLAG_INSANCTUARY))
        {
            if(objA->m_faction && objB->m_faction && objA->m_faction != objB->m_faction)
            {
                if(objA->m_faction->FactionMask == FACTION_MASK_MONSTER && objB->m_faction->FactionMask != FACTION_MASK_MONSTER)
                    return true;
                if(objB->m_faction->FactionMask == FACTION_MASK_MONSTER && objA->m_faction->FactionMask != FACTION_MASK_MONSTER)
                    return true;
            }
            return false;
        }
    }

    if(objA->IsCreature())
    {
        if(objA->IsPet())
        {
            if(player_objB && !player_objB->IsPvPFlagged())
                return false;

            if(player_objB)
            {
                if(TO_PET(objA)->GetOwner())
                {
                    if(!TO_PET(objA)->GetOwner()->IsPvPFlagged())
                        return false;
                    // the target is PvP, its okay.
                }
                else
                    return false;
            }
        }
        else if(objA->IsSummon())
        {
            if(player_objB && !player_objB->IsPvPFlagged())
                return false;

            if(player_objB)
            {
                Object* summonownerA = TO_SUMMON(objA)->GetSummonOwner();
                if(summonownerA && summonownerA->IsPlayer())
                {
                    if(!TO_UNIT(summonownerA)->IsPvPFlagged())
                        return false;
                    // the target is PvP, its okay.
                }
                else
                    return false;
            }
        }
    }

    if( player_objA && player_objB )
    {
        if(objA->HasAreaFlag(OBJECT_AREA_FLAG_INSANCTUARY) || objB->HasAreaFlag(OBJECT_AREA_FLAG_INSANCTUARY))
            return false;

        if(player_objA->IsPvPFlagged() && !player_objB->IsPvPFlagged() && player_objA->DuelingWith != player_objB)
            return false;
        if(!player_objA->IsPvPFlagged() && !player_objB->IsPvPFlagged() && player_objA->DuelingWith != player_objB)
            return false;

        if(player_objA->IsFFAPvPFlagged() && player_objB->IsFFAPvPFlagged())
        {
            if( player_objA->GetGroup() && player_objA->GetGroup() == player_objB->GetGroup() )
                return false;

            if( player_objA == player_objB ) // Totems...
                return false;

            return true;        // can hurt each other in FFA pvp
        }

        //Handle BG's
        if( player_objA->m_bg != NULL)
        {
            //Handle Arenas
            if( player_objA->GetTeam() != player_objB->GetTeam() )
                return true;
        }

        // same faction can't kill each other.
        if(player_objA->m_faction == player_objB->m_faction)
            return false;

        return true; // Skip the rest of this, it's all faction shit.
    }

    return (GetFactionsInteractStatus(TO_UNIT(objA), TO_UNIT(objB)) == FI_STATUS_HOSTILE);
}

bool FactionSystem::isAttackable(Object* objA, Object* objB, bool CheckStealth)// A can attack B?
{
    return (intisAttackable(objA, objB, CheckStealth) == FI_STATUS_HOSTILE);
}

bool FactionSystem::isHostile(Object* objA, Object* objB)// B is hostile for A?
{
    return isAttackable(objA, objB, false);
}

Player* FactionSystem::GetPlayerFromObject(Object* obj)
{
    Player* player_obj = NULLPLR;

    if( obj->IsPlayer() )
    {
        player_obj =  TO_PLAYER( obj );
    }
    else if( obj->IsPet() )
    {
        Pet* pet_obj = TO_PET(obj);
        if( pet_obj )
            player_obj =  pet_obj->GetPetOwner();
    }
    else if(obj->IsSummon()) // If it's not a player nor a pet, it can still be a totem.
        player_obj =  TO_PLAYER(TO_SUMMON(obj)->GetSummonOwner());

    return player_obj;
}

bool FactionSystem::isCombatSupport(Object* objA, Object* objB)// B combat supports A?
{
    if( !objA || !objB )
        return false;

    // can't support corpses...
    if( objA->GetTypeId() == TYPEID_CORPSE || objB->GetTypeId() == TYPEID_CORPSE )
        return false;

    // We do need all factiondata for this
    if( objB->m_factionDBC == NULL || objA->m_factionDBC == NULL || objB->m_faction == NULL || objA->m_faction == NULL )
        return false;

    if(!objA->PhasedCanInteract(objB))
        return false;

    if(objB->IsCreature() && isTargetDummy(objB->GetEntry()))
        return false;

    bool combatSupport = false;
    uint32 fSupport = objB->m_faction->FriendlyMask;
    uint32 myFaction = objA->m_faction->FactionMask;
    if(fSupport & myFaction)
        combatSupport = true;

    // check friend/enemy list
    for(uint32 i = 0; i < 4; i++)
    {
        if(objB->m_faction->FriendlyFactions[i] && objB->m_faction->FriendlyFactions[i] == objA->m_faction->Faction)
        {
            combatSupport = true;
            break;
        }
        if(objB->m_faction->EnemyFactions[i] && objB->m_faction->EnemyFactions[i] == objA->m_faction->Faction)
        {
            combatSupport = false;
            break;
        }
    }
    return combatSupport;
}

bool FactionSystem::isAlliance(Object* objA)// A is alliance?
{
    if(!objA || objA->m_factionDBC == NULL || objA->m_faction == NULL)
        return true;

    //Get stormwind faction frm dbc (11/72)
    FactionTemplateDBC * m_sw_faction = dbcFactionTemplate.LookupEntry(11);
    FactionDBC * m_sw_factionDBC = dbcFaction.LookupEntry(72);

    if(m_sw_faction == objA->m_faction || m_sw_factionDBC == objA->m_factionDBC)
        return true;

    //Is StormWind hostile to ObjectA?
    uint32 faction = m_sw_faction->Faction;
    uint32 hostilemask = objA->m_faction->HostileMask;

    if(faction & hostilemask)
        return false;

    // check friend/enemy list
    for(uint32 i = 0; i < 4; i++)
    {
        if(objA->m_faction->EnemyFactions[i] == faction)
            return false;
    }

    //Is ObjectA hostile to StormWind?
    faction = objA->m_faction->Faction;
    hostilemask = m_sw_faction->HostileMask;

    if(faction & hostilemask)
        return false;

    // check friend/enemy list
    for(uint32 i = 0; i < 4; i++)
    {
        if(objA->m_faction->EnemyFactions[i] == faction)
            return false;
    }

    //We're not hostile towards SW, so we are allied
    return true;
}
