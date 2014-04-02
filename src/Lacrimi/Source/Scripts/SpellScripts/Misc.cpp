
#include "LacrimiStdAfx.h"

////////////////////////
//// Spell Scripts  ////
////////////////////////
bool Cannibalize(uint32 i, Spell* pSpell)
{
    if(pSpell->p_caster != NULL)
    {
        pSpell->p_caster->CastSpell(pSpell->p_caster, 20578, true);
        pSpell->p_caster->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_CANNIBALIZE);
    }
    return true;
}

bool CalloftheAshbringer(uint32 i, Spell* pSpell)
{
    Unit* unitTarget = pSpell->GetUnitTarget();
    if(pSpell->p_caster != NULL && unitTarget != NULL)
    {
        uint32 ashcall = RandomUInt(12);

        WorldPacket data(SMSG_PLAY_OBJECT_SOUND, 12), *crap;
        std::stringstream Reply;
        switch(ashcall)
        {
        case 1:
            {
                data << uint32(8906) << unitTarget->GetGUID();
                pSpell->p_caster->SendMessageToSet(&data, true);
                Reply << "I... was... pure... once.";
                crap = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.str().c_str(), pSpell->p_caster->GetGUID(), 0);
            }break;
        case 2:
            {
                data << uint32(8907) << unitTarget->GetGUID();
                pSpell->p_caster->SendMessageToSet(&data, true);
                Reply << "Fought... for... righteousness.";
                crap = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.str().c_str(), pSpell->p_caster->GetGUID(), 0);
            }break;
        case 3:
            {
                data << uint32(8908) << unitTarget->GetGUID();
                pSpell->p_caster->SendMessageToSet(&data, true);
                Reply << "I... was... once... called... Ashbringer.";
                crap = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.str().c_str(), pSpell->p_caster->GetGUID(), 0);
            }break;
        case 4:
            {
                data << uint32(8920) << unitTarget->GetGUID();
                pSpell->p_caster->SendMessageToSet(&data, true);
                Reply << "Betrayed... by... my... order.";
                crap = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.str().c_str(), pSpell->p_caster->GetGUID(), 0);
            }break;
        case 5:
            {
                data << uint32(8921) << unitTarget->GetGUID();
                pSpell->p_caster->SendMessageToSet(&data, true);
                Reply << "Destroyed... by... Kel'Thuzad.";
                crap = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.str().c_str(), pSpell->p_caster->GetGUID(), 0);
            }break;
        case 6:
            {
                data << uint32(8922) << unitTarget->GetGUID();
                pSpell->p_caster->SendMessageToSet(&data, true);
                Reply << "Made... to serve.";
                crap = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.str().c_str(), pSpell->p_caster->GetGUID(), 0);
            }break;
        case 7:
            {
                data << uint32(8923) << unitTarget->GetGUID();
                pSpell->p_caster->SendMessageToSet(&data, true);
                Reply << "My... son... watched... me... die.";
                crap = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.str().c_str(), pSpell->p_caster->GetGUID(), 0);
            }break;
        case 8:
            {
                data << uint32(8924) << unitTarget->GetGUID();
                pSpell->p_caster->SendMessageToSet(&data, true);
                Reply << "Crusades... fed his rage.";
                crap = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.str().c_str(), pSpell->p_caster->GetGUID(), 0);
            }break;
        case 9:
            {
                data << uint32(8925) << unitTarget->GetGUID();
                pSpell->p_caster->SendMessageToSet(&data, true);
                Reply << "Truth... is... unknown... to him.";
                crap = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.str().c_str(), pSpell->p_caster->GetGUID(), 0);
            }break;
        case 10:
            {
                data << uint32(8926) << unitTarget->GetGUID();
                pSpell->p_caster->SendMessageToSet(&data, true);
                Reply << "Scarlet... Crusade... is pure... no longer.";
                crap = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.str().c_str(), pSpell->p_caster->GetGUID(), 0);
            }break;
        case 11:
            {
                data << uint32(8927) << unitTarget->GetGUID();
                pSpell->p_caster->SendMessageToSet(&data, true);
                Reply << "Balnazzar's... crusade... corrupted... my son.";
                crap = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.str().c_str(), pSpell->p_caster->GetGUID(), 0);
            }break;
        case 12:
            {
                data << uint32(8928) << unitTarget->GetGUID();
                pSpell->p_caster->SendMessageToSet(&data, true);
                Reply << "Kill... them... all!";
                crap = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.str().c_str(), pSpell->p_caster->GetGUID(), 0);
            }break;
        }
        pSpell->p_caster->GetSession()->SendPacket(crap);
    }
    return true;
}

bool CleansingVialDND(uint32 i, Spell* pSpell)
{
    if(pSpell->p_caster != NULL)
    {
        QuestLogEntry *en = pSpell->p_caster->GetQuestLogForEntry(9427);
        if(en != NULL)
        {
            en->Quest_Status = QUEST_STATUS__COMPLETE;
            en->SendQuestComplete();
            en->UpdatePlayerFields();
        }
    }
    return true;
}

bool ForceCastPortalEffectSunwellIsle(uint32 i, Spell* pSpell)
{
    if(pSpell->u_caster != NULL)
    {
        if(pSpell->p_caster != NULL && pSpell->p_caster->getLevel() < 70)
        {
            sChatHandler.RedSystemMessage(pSpell->p_caster->GetSession(),"You must be level 70 to use this!");
            return true;
        }

        pSpell->u_caster->CastSpell(pSpell->GetUnitTarget(), pSpell->damage, true);
    }
    return true;
}

////////////////////////
//// Damage Scripts ////
////////////////////////
void DistanceDamage(uint32 i, Spell* pSpell, uint32 effect)
{
    if( effect == SPELL_EFFECT_SCHOOL_DAMAGE )
    {
        if( pSpell->u_caster != NULL)
        {
            float dist = pSpell->u_caster->CalcDistance( TO_OBJECT( pSpell->GetUnitTarget() ) );
            if( dist <= 20.0f && dist >= 0.0f )
                pSpell->damage = float2int32( -450 * dist + pSpell->damage );
        }
    }
}

void CataclysmicBolt(uint32 i, Spell* pSpell, uint32 effect)
{
    if( effect == SPELL_EFFECT_SCHOOL_DAMAGE )
    {
        pSpell->damage = pSpell->GetUnitTarget()->GetMaxHealth() / 2;
    }
}

void Thundercrash(uint32 i, Spell* pSpell, uint32 effect)
{
    if( effect == SPELL_EFFECT_SCHOOL_DAMAGE )
    {
        pSpell->damage = pSpell->GetUnitTarget()->GetHealth() / 2;
        if(pSpell->damage < 200)
            pSpell->damage = 200;
    }
}

void FatalAttraction(uint32 i, Spell* pSpell, uint32 effect)
{
    if( effect == SPELL_EFFECT_SCHOOL_DAMAGE )
    {
        if(pSpell->GetUnitTarget()->HasAura(43690))// Saber Lash
            pSpell->damage = 0;
    }
}

void TympanicTantrum(uint32 i, Spell* pSpell, uint32 effect)
{
    if( effect == SPELL_EFFECT_SCHOOL_DAMAGE )
    {
        pSpell->damage = pSpell->GetUnitTarget()->GetMaxHealth() / 10;
    }
}

void ChimeraShotSerpant(uint32 i, Spell* pSpell, uint32 effect)
{
    if( effect == SPELL_EFFECT_SCHOOL_DAMAGE )
    {
        pSpell->damage += pSpell->forced_basepoints[0];
    }
}

/////////////////////////
/// Proclimit Scripts ///
/////////////////////////
bool Todo2(Unit *target, uint32 &uSpellId, int32 &damage, SpellCastTargets &targets, ProcTriggerSpell *triggered, ProcDataHolder *dataHolder);

void Lacrimi::SetupMiscSpells()
{
    ////////////////////////
    //// Spell Scripts  ////
    ////////////////////////
    RegisterSpellScriptEffect(20577, Cannibalize);

    RegisterSpellScriptEffect(28414, CalloftheAshbringer);

    RegisterSpellScriptEffect(29297, CleansingVialDND);

    RegisterSpellScriptEffect(44876, ForceCastPortalEffectSunwellIsle);

    ////////////////////////
    //// Damage Scripts ////
    ////////////////////////
    RegisterSpellEffectModifier(33654, DistanceDamage);
    RegisterSpellEffectModifier(33671, DistanceDamage);
    RegisterSpellEffectModifier(50810, DistanceDamage);
    RegisterSpellEffectModifier(50811, DistanceDamage);
    RegisterSpellEffectModifier(61547, DistanceDamage);
    RegisterSpellEffectModifier(61546, DistanceDamage);

    RegisterSpellEffectModifier(38441, CataclysmicBolt);

    RegisterSpellEffectModifier(25599, Thundercrash);

    RegisterSpellEffectModifier(41001, FatalAttraction);

    RegisterSpellEffectModifier(62775, TympanicTantrum);

    RegisterSpellEffectModifier(53353, ChimeraShotSerpant);

    /////////////////////////
    /// Proclimit Scripts ///
    /////////////////////////
    RegisterSpellScriptedProclimit(0, Todo2);
}


void Todo()
{
    switch(0)
    {
    /*************************
        Non-Class spells
        - ToDo
     *************************/
    case 6668:// Red Firework
        {
            // Shoots a firework into the air that bursts into a thousand red stars
        }break;
    case 8344:// Universal Remote
        {
            //FIXME:Allows control of a mechanical target for a short time.  It may not always work and may just root the machine or make it very very angry.  Gnomish engineering at its finest.
        }break;
    case 9976:// Polly Eats the E.C.A.C.
        {
            //FIXME: Don't know what this does
        }break;
    case 10137:// Fizzule's Whistle
        {
            //FIXME:Blow on the whistle to let Fizzule know you're an ally
            //This item comes after a finish of quest at venture co.
            //You must whistle this every time you reach there to make Fizzule
            //ally to you.
        }break;
    case 11540:// Blue Firework
        {
            //Shoots a firework into the air that bursts into a thousand blue stars
        }break;
    case 11541:// Green Firework
        {
            //Shoots a firework into the air that bursts into a thousand green stars
        }break;
    case 11542:// Red Streaks Firework
        {
            //Shoots a firework into the air that bursts into a thousand red streaks
        }break;
    case 11543:// Red, White and Blue Firework
        {
            //Shoots a firework into the air that bursts into red, white and blue stars
        }break;
    case 11544:// Yellow Rose Firework
        {
            //Shoots a firework into the air that bursts in a yellow pattern
        }break;
    case 12151:// Summon Atal'ai Skeleton
        {
            //8324  Atal'ai Skeleton

            //FIXME:Add here remove in time event
        }break;
    case 13535:// Tame Beast
        {

        }break;
    case 13006:// Shrink Ray
        {
            //FIXME:Schematic is learned from the gnomish engineering trainer. The gnomish/gobblin engineering decision is made when you are lvl40+ and your engineering is 200+. Interestingly, however, when this item fails to shrink the target, it can do a variety of things, such as...
            //-Make you bigger (attack power +250)
            //-Make you smaller (attack power -250)
            //-Make them bigger (same effect as above)
            //-Make your entire party bigger
            //-Make your entire party smaller
            //-Make every attacking enemy bigger
            //-Make ever attacking enemy smaller
            //Works to your advantage for the most part (about 70% of the time), but don't use in high-pressure situations, unless you're going to die if you don't. Could tip the scales the wrong way.
            //Search for spells of this


            //13004 - grow <- this one
            //13010 - shrink <-this one
            //
        }break;
    case 13180:// Gnomish Mind Control Cap
        {
            // FIXME:Take control of humanoid target by chance(it can be player)
        }break;
    case 13278:// Gnomish Death Ray
        {
            // FIXME:The devices charges over time using your life force and then directs a burst of energy at your opponent
            //Drops life
        }break;
    case 13280:// Gnomish Death Ray
        {
            //FIXME: Drop life
        }break;
    case 17816:// Sharp Dresser
        {
            //Impress others with your fashion sense
        }break;
    case 21343:// Snowball
        {
        }break;
    case 23645:// Hourglass Sand
        {
            //Indeed used at the Chromo fight in BWL. Chromo has a stunning debuff, uncleansable, unless you have hourglass sand. This debuff will stun you every 4 seconds, for 4 seconds. It is resisted a lot though. Mage's and other casters usually have to do this fight with the debuff on, healers, tanks and hunters will get some to cure themselves from the debuff
        }break;
    case 24325:// Pagle's Point Cast - Create Mudskunk Lure
        {
            //FIXME:Load with 5 Zulian Mudskunks, and then cast from Pagle's Point in Zul'Gurub
        }
    case 24392:// Frosty Zap
        {
            //FIXME:Your Frostbolt spells have a 6% chance to restore 50 mana when cast.
            //damage == 50
        }break;
    case 25822:// Firecrackers
        {
            //FIXME:Find firecrackers
        }break;
    case 26373:// Lunar Invititation
        {
            //FIXME: Teleports the caster from within Greater Moonlight
        }break;
    case 26889:// Give Friendship Bracelet
        {
            //Give to a Heartbroken player to cheer them up
            //laugh emote
        }break;
    case 27662:// Throw Cupid's Dart
        {
            //FIXME:Shoot a player, and Kwee Q. Peddlefeet will find them! (Only works on players with no current critter pets.)
        }break;
    case 28806:// Toss Fuel on Bonfire
        {
            //FIXME:Dont know what this dummy does
        }break;
    case 7669:// Bethor's Potion
        {
            // related to Hex of Ravenclaw,
            // its a dispell spell.
            //FIXME:Dont know whats the usage of this dummy
        }break;
    case 8283:// Snufflenose Command
        {
            //FIXME:Quest Blueleaf Tubers
            //For use on a Snufflenose Gopher
        }break;
    case 8913:// Sacred Cleansing
        {
            //FIXME:Removes the protective enchantments around Morbent Fel
            //Quest Morbent Fel
        }break;
    case 9962://Capture Treant
        {
            //Quest Treant Muisek
        }break;
    case 10113:// Flare Gun's flare
        {
            //FIXME:Quest Deep Cover
            //1543 may need to cast this
            //2 flares and the /salute
        }break;
    case 10617:// Release Rageclaw
        {
            //Quest Druid of the Claw
            //Use on the fallen body of Rageclaw
        }break;
    case 11402:// Shay's Bell
        {
            //FIXME:Quest Wandering Shay
            //Ring to call Shay back to you
        }break;
    case 11610:// Gammerita Turtle Camera
        {
            //Quest The Super Snapper FX
        }break;
    case 11886:// Capture Wildkin
        {
            //Quest Testing the Vessel
            //Shrink and Capture a Fallen Wildkin
        }break;
    case 11887:// Capture Hippogryph
        {
            //FIXME:Same with 11888
            //Quest Hippogryph Muisek
        }break;
    case 11888:// Capture Faerie Dragon
        {
            //FIXME:Check Faerie Dragon Muisek is killed or not if its killed update quest
            //And allow create of fearie Dragon which is effect 1
            //Quest: Faerie Dragon Muisek
        }break;
    case 11889:// Capture Mountain Giant
        {
            //FIXME:Same with 11888
            //Quest: Mountain Giant Muisek
        }break;
    case 12189:// Summon Echeyakee
        {
            //3475  Echeyakee

            //FIXME:Quest Echeyakee
        }break;
    case 12283:// Xiggs Signal Flare
        {
            //Quest Signal for Pickup
            //To be used at the makeshift helipad in Azshara. It will summon Pilot Xiggs Fuselighter to pick up the tablet rubbings
        }break;
    case 12938:// Fel Curse
        {
            //FIXME:Makes near target killable(servants of Razelikh the Defiler)
        }break;
    case 14247:// Blazerunner Dispel
        {
            //FIXME:Quest Aquementas and some more
        }break;
    case 14250:// Capture Grark
        {
            //Quest Precarious Predicament
        }break;
    case 14813:// Rocknot's Ale
        {
            //you throw the mug
            //and the guy gets pissed well everyone gets pissed and he crushes the door so you can get past
            //maybe after like 30 seconds so you can get past.  but lke I said I have never done it myself
            //so i am not 100% sure what happens.
        }break;
    case 15991://Revive Ringo
        {
            //Quest A Little Help From My Friends
            //Revive Ringo with water
        }break;
    case 15998:// Capture Worg Pup
        {
            //FIXME:Ends Kibler's Exotic Pets  (Dungeon) quest
        }break;
    case 16031:// Releasing Corrupt Ooze
        {
            //FIXME:Released ooze moves to master ooze and "Merged Ooze Sample"
            //occurs after some time.This item helps to finish quest
        }break;
    case 16378:// Temperature Reading
        {
            //FIXME:Quest Finding the Source
            //Take a reading of the temperature at a hot spot.
        }break;
    case 17166:// Release Umi's Yeti
        {
            //Quest Are We There, Yeti?
            //Select Umi's friend and click to release the Mechanical Yeti
        }break;
    case 17271:// Test Fetid Skull
        {
            //FIXME:Marauders of Darrowshire
            //Wave over a Fetid skull to test its resonance
        }break;
    case 18153:// Kodo Kombobulator
        {
            //FIXME:Kodo Roundup Quest
            //Kodo Kombobulator on any Ancient, Aged, or Dying Kodo to lure the Kodo to follow (one at a time)
        }break;
    case 19250:// Placing Smokey's Explosives
        {
            //This is something related to quest i think
        }break;
    case 20804:// Triage
        {
            //Quest Triage
            //Use on Injured, Badly Injured, and Critically Injured Soldiers
        }break;
    case 21050:// Melodious Rapture
        {
            //Quest Deeprun Rat Roundup
        }break;
    case 21332:// Aspect of Neptulon
        {
            //FIXME:Used on plagued water elementals in Eastern Plaguelands
            //Quest:Poisoned Water
        }break;
    case 21960:// Manifest Spirit
        {
            //FIXME:Forces the spirits of the first centaur Kahns to manifest in the physical world
            //thats a quest
            //its for maraudon i think
            //u use that on the spirit mobs
            //to release them
        }break;
    case 23359:// Transmogrify!
        {
            //Quest Zapped Giants
            //Zap a Feralas giant into a more manageable form
        }break;
    case 27184:// Summon Mor Grayhoof
        {
            //16044 Mor Grayhoof Trigger
            //16080 Mor Grayhoof

            //Related to quests The Left Piece of Lord Valthalak's Amulet  (Dungeon)
            //and The Right Piece of Lord Valthalak's Amulet  (Dungeon)
        }break;
    case 27190:// Summon Isalien
        {
            //16045 Isalien Trigger
            //16097 Isalien

            //Related to quests The Left Piece of Lord Valthalak's Amulet  (Dungeon)
            //and The Right Piece of Lord Valthalak's Amulet  (Dungeon)
        }break;
    case 27191:// Summon the remains of Jarien and Sothos
        {
            /*
            16046   Jarien and Sothos Trigger
            16101   Jarien
            16103   Spirit of Jarien

            16102   Sothos
            16104   Spirit of Sothos
            */

            //Related to quests The Left Piece of Lord Valthalak's Amulet  (Dungeon)
            //and The Right Piece of Lord Valthalak's Amulet  (Dungeon)
        }break;
    case 27201:// Summon the spirit of Kormok
        {
            /*16047 Kormok Trigger
            16118   Kormok
            */
            //Related to quests The Left Piece of Lord Valthalak's Amulet  (Dungeon)
            //and The Right Piece of Lord Valthalak's Amulet  (Dungeon)
        }break;
    case 27202:// Summon Lord Valthalak
        {
            /*
            16042   Lord Valthalak
            16048   Lord Valthalak Trigger
            16073   Spirit of Lord Valthalak

            */
            //Related to quests The Left Piece of Lord Valthalak's Amulet  (Dungeon)
            //and The Right Piece of Lord Valthalak's Amulet  (Dungeon)
        }break;
    case 27203:// Summon the spirits of the dead at haunted locations
        {
            //Related to quests The Left Piece of Lord Valthalak's Amulet  (Dungeon)
            //and The Right Piece of Lord Valthalak's Amulet  (Dungeon)
        }break;
    case 27517:// Use this banner at the Arena in Blackrock Depths to challenge Theldren
        {
            //This is used to make Theldren spawn at the place where it used
            //I couldnt find theldrin, and his men in creature names database
            //Someone has to write this and this is related to The Challange quest
            /*By moving to the center grate, you trigger the arena event.
            A random group of mobs (spiders, worms, bats, raptors) spawns,
            and you have to kill them. After the last one dies, and a small
            break, a boss mob spawns. Successfully completing this event
            turns the arena spectators from red to yellow*/
        }break;
    case 60103: { }break;
    }
}

bool Todo2(Unit *target, uint32 &uSpellId, int32 &damage, SpellCastTargets &targets, ProcTriggerSpell *triggered, ProcDataHolder *dataHolder)
{
    SpellEntry *casting = dataHolder->GetCastingSpell(), *applying = dbcSpell.LookupEntry(triggered->origId), *spe = dbcSpell.LookupEntry(uSpellId);
    if(casting == NULL || spe == NULL)
        return false;

    uint32 talentlevel = 0;
    switch( triggered->origId )
    {
        //mace specialization
        case 12284: {talentlevel = 1;}break;
        case 12701: {talentlevel = 2;}break;
        case 12702: {talentlevel = 3;}break;
        case 12703: {talentlevel = 4;}break;
        case 12704: {talentlevel = 5;}break;

        //Unbridled Wrath
        case 12999: {talentlevel = 1;}break;
        case 13000: {talentlevel = 2;}break;
        case 13001: {talentlevel = 3;}break;
        case 13002: {talentlevel = 4;}break;
    }

    switch(uSpellId)
    {
        //warlock - Nightfall
    case 17941:
        {
            //only trigger effect for specified spells
            if( casting->NameHash != SPELL_HASH_CORRUPTION && //Corruption
                casting->NameHash != SPELL_HASH_DRAIN_LIFE )//Drain Life
                return false;
        }break;

    case 32386://Shadow Embrace
    case 32388:
    case 32389:
    case 32390:
    case 32391:
        {
            if( casting->NameHash != SPELL_HASH_SHADOW_BOLT &&
                casting->NameHash != SPELL_HASH_HAUNT )
                return false;
        }break;

        //warlock - Aftermath
    case 18118:
        {
            //only trigger effect for specified spells
            SkillLineSpell* skillability = objmgr.GetSpellSkill(casting->Id);
            if( !skillability )
                return false;

            if( skillability->skilline != SKILL_DESTRUCTION )
                return false;
        }break;

        //warlock - Soul Leech
    case 30294:
        {
            if(applying == NULL)
                return false;
            if( dataHolder->GetDamage() == -1 )
                return false;

            damage = (applying->EffectBasePoints[0]+1) * dataHolder->GetDamage() / 100;
        }break;

        //warlock - pyroclasm
    case 18093:
        {
            //only trigger effect for specified spells
            if( casting->NameHash != SPELL_HASH_RAIN_OF_FIRE && //Rain of Fire
                casting->NameHash != SPELL_HASH_HELLFIRE_EFFECT && //Hellfire
                casting->NameHash != SPELL_HASH_SOUL_FIRE ) //Soul Fire
                return false;
        }break;

        //mage - Improved Scorch
    case 22959:
        {
            //only trigger effect for specified spells
            if( casting->NameHash != SPELL_HASH_SCORCH ) //Scorch
                return false;
        }break;

        //mage - Combustion
    case 28682:
        {
            //only trigger effect for specified spells
            if( !( casting->c_is_flags & SPELL_FLAG_IS_DAMAGING)
                || casting->School != SCHOOL_FIRE )
                return false;

            if( dataHolder->GetFlag2() & PROC_ON_SPELL_CRIT_HIT )
            {
                triggered->procCharges++;
                if( triggered->procCharges >= 3 ) //whatch that number cause it depends on original stack count !
                {
                    target->m_AuraInterface.RemoveAllAurasByNameHash( SPELL_HASH_COMBUSTION, false );
                }return false;
            }
        }break;

        //priest - Shadow Weaving
    case 15258:
        {
            //we need damaging spells for this, so we suppose all shadow spells casted on target are dmging spells = Wrong
            if( casting->School != SCHOOL_SHADOW || !( casting->c_is_flags & SPELL_FLAG_IS_DAMAGING ) )
                return false;
        }break;

        //priest - Inspiration
    case 15359:
    case 14893:
    case 15357:
        {
            if(!( casting->c_is_flags & SPELL_FLAG_IS_HEALING ) )
                return false;
        }break;

        //Druid Living Seed
    case 48504:
        {
            if ( casting->NameHash != SPELL_HASH_SWIFTMEND &&
                casting->NameHash != SPELL_HASH_REGROWTH &&
                casting->NameHash != SPELL_HASH_HEALING_TOUCH &&
                casting->NameHash != SPELL_HASH_NOURISH )
                return false;
        }break;

        //shaman - Healing Way
    case 29203:
        {
            //only trigger effect for specified spells
            if( casting->NameHash != SPELL_HASH_HEALING_WAVE ) //healing wave
                return false;
        }break;

    case 16180:
    case 16196:
    case 16198: // Improved Water Shield
        {
            // Proc for Lesser Healing Wave is 60% of base one
            if((casting->NameHash == SPELL_HASH_LESSER_HEALING_WAVE && Rand(40)))
                return false;

            Aura* shield = target->m_AuraInterface.FindPositiveAuraByNameHash(SPELL_HASH_WATER_SHIELD);
            if(!shield)
                return false;

            uSpellId = shield->m_spellProto->EffectTriggerSpell[0];
            shield->ModProcCharges(-1);
        }break;

    case 51525:
    case 51526:
    case 51527: // Static Shock
        {
            Aura* shield = target->m_AuraInterface.FindPositiveAuraByNameHash(SPELL_HASH_LIGHTNING_SHIELD);
            if(shield == NULL)
                return false;

            uSpellId = shield->m_spellProto->EffectTriggerSpell[0];
            shield->ModProcCharges(-1);
        }break;

        //shaman - Elemental Devastation
    case 29177:
    case 29178:
    case 30165:
        {
            //only trigger effect for specified spells
            if( !(casting->c_is_flags & SPELL_FLAG_IS_DAMAGING)) //healing wave
                return false;
        }break;

        //shaman - windfury weapon
    case 25504:
        {
            if(!target->IsPlayer() || dataHolder->GetWeaponDamageType() < 1 || dataHolder->GetWeaponDamageType() > 2)
                return false;

            Item* mh = TO_PLAYER(target)->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND + dataHolder->GetWeaponDamageType() -1);
            if(mh == NULL)
                return false;

            uint32 apBonus = 46; // use rank 1 bonus as default
            EnchantmentInstance * ei = mh->GetEnchantment(1);
            if(ei == NULL)
                return false;

            EnchantEntry * e = ei->Enchantment;
            if(e == NULL)
                return false;

            switch(e->Id)
            {
            case 284: apBonus = 119; break;
            case 525: apBonus = 249; break;
            case 1669: apBonus = 333; break;
            case 2636: apBonus = 445; break;
            case 3785: apBonus = 835; break;
            case 3786: apBonus = 1090; break;
            case 3787: apBonus = 1250; break;
            }

            float mhs = float( mh->GetProto()->Delay );
            // Calculate extra AP bonus damage
            uint32 extra_dmg=float2int32(mhs * apBonus / 14000.0f);
            if(dataHolder->GetWeaponDamageType() == 2) // offhand gets 50% bonus
                extra_dmg /= 2;

            target->Strike( dataHolder->GetVictim(), dataHolder->GetWeaponDamageType()-1, spe, extra_dmg, 0, 0, false, false );
            target->Strike( dataHolder->GetVictim(), dataHolder->GetWeaponDamageType()-1, spe, extra_dmg, 0, 0, false, false );
            uSpellId = 33010; // WF animation
        }break;

        // Ancestral Fortitude
    case 16237:
    case 16177:
    case 16236:
        {
            if(!( casting->c_is_flags & SPELL_FLAG_IS_HEALING ))
                return false;
        }break;

        // Flametongue Weapon
    case 8026:
    case 8028:
    case 8029:
    case 10445:
    case 16343:
    case 16344:
    case 25488:
    case 58786:
    case 58787:
    case 58788:
        {
            if(!target->IsPlayer() || dataHolder->GetWeaponDamageType() < 1 || dataHolder->GetWeaponDamageType() > 2)
                return false;

            uSpellId = 10444;    // Flametongue Weapon proc
            Item* mh = TO_PLAYER(target)->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND + dataHolder->GetWeaponDamageType() - 1 );

            if( mh == NULL)
                return false;

            float mhs = float( mh->GetProto()->Delay );
            damage = FL2UINT( mhs * 0.001f * (spe->EffectBasePoints[0] + 1)/88 );
        }break;

        //rogue - Ruthlessness
    case 14157:
        {
            //we need a finishing move for this
            if(!(casting->c_is_flags & SPELL_FLAG_IS_FINISHING_MOVE) || dataHolder->GetVictim() == TO_UNIT(target))
                return false;

            if(target->IsPlayer())
            {
                TO_PLAYER(target)->m_spellcomboPoints = 0;
                //TO_PLAYER(target)->UpdateComboPoints();
            }
        }break;

        //rogue - Relentless Strikes
    case 14181:
        {
            if(applying == NULL)
                return false;
            if(!target->IsPlayer() || TO_UNIT(target) != dataHolder->GetVictim())  // to prevent it proccing 2 times
                return false;//this should not ocur unless we made a fuckup somewhere

            if( !(casting->c_is_flags & SPELL_FLAG_IS_FINISHING_MOVE) )
                return false;

            int32 procChance = applying->EffectPointsPerComboPoint[0] * TO_PLAYER(target)->m_comboPoints;
            if(!Rand(procChance))
                return false;
        }break;

        //rogue - Find Weakness
    case 31234:
    case 31235:
    case 31236:
    case 31237:
    case 31238:
        {
            if(!(casting->c_is_flags & SPELL_FLAG_IS_FINISHING_MOVE))
                return false;
        }break;

        //rogue - Initiative
    case 13977:
        {
            //we need a Ambush, Garrote, or Cheap Shot
            if( casting->NameHash != SPELL_HASH_CHEAP_SHOT && //Cheap Shot
                casting->NameHash != SPELL_HASH_AMBUSH && //Ambush
                casting->NameHash != SPELL_HASH_GARROTE )  //Garrote
                return false;
        }break;

        //Priest - blackout
    case 15269:
        {
            if( casting->School != SCHOOL_SHADOW || !( casting->c_is_flags & SPELL_FLAG_IS_DAMAGING ) )
                return false;
        }break;

        //warrior - improved berserker rage
    case 23690:
    case 23691:
        {
            if(casting->NameHash != SPELL_HASH_BERSERKER_RAGE )
                return false;
        }break;

        //mage - Arcane Concentration
    case 12536:
        {
            if( !( casting->c_is_flags & SPELL_FLAG_IS_DAMAGING ) )
                return false;
        }break;

        //mage - Improved Blizzard
    case 12484:
    case 12485:
    case 12486:
        {
            if( casting->NameHash != SPELL_HASH_BLIZZARD || dataHolder->GetVictim() == TO_UNIT(target) ) //Blizzard
                return false;
        }break;

        //mage - Master of Elements
    case 29077:
        {
            if(applying == NULL)
                return false;
            damage = float2int32(target->GetSpellBaseCost(casting) * ((applying->EffectBasePoints[0] + 1) / 100));
        }break;

        // Burnout
    case 44450:
        {
            if(applying == NULL)
                return false;
            int32 addcost = float2int32(target->GetSpellBaseCost(casting) * ((applying->EffectBasePoints[1] + 1) / 100));
            if( (target->GetUInt32Value(UNIT_FIELD_POWER1) - addcost) < 0 )
                target->SetUInt32Value(UNIT_FIELD_POWER1, 0);
            else
                target->ModUnsigned32Value(UNIT_FIELD_POWER1, -addcost);
        }break;

        //Hunter - Thrill of the Hunt
    case 34720:
        {
            damage = float2int32(target->GetSpellBaseCost(casting) * (40.0f / 100.0f));
        }break;

        // Improved Steady Shot
    case 53220:
        {
            if( casting->NameHash != SPELL_HASH_STEADY_SHOT )
                return false;
        }break;

        //rogue - improved sprint
    case 30918:
        {
            if( casting->NameHash != SPELL_HASH_SPRINT || dataHolder->GetVictim() != TO_UNIT(target) ) //sprint
                return false;
        }break;

        //paladin - Improved Lay on Hands
    case 20233:
    case 20236:
        {
            if( casting->NameHash != SPELL_HASH_LAY_ON_HANDS )
                return false;
        }break;

        //paladin - Spiritual Attunement
    case 31786:
        {
            if(applying == NULL)
                return false;
            //trigger only on heal spell cast by NOT us
            if( !( casting->c_is_flags & SPELL_FLAG_IS_HEALING ) || TO_UNIT(target) == dataHolder->GetVictim() )
                return false;

            damage = (casting->EffectBasePoints[IsHealingSpell(casting)-1] + 1) * (applying->EffectBasePoints[0] + 1 ) / 100;
        }break;

        //paladin - Eye for an Eye
    case 25997:
        {
            if(applying == NULL)
                return false;
            if( dataHolder->GetVictim() == TO_UNIT(target) )
                return false; //not self casted crits

            if(!(casting->c_is_flags & SPELL_FLAG_IS_DAMAGING))
                return false;

            damage = ( dataHolder->GetDamage() *  (applying->EffectBasePoints[0] + 1 )) / 100 ; //only half dmg
            int32 half_health = target->GetUInt32Value(UNIT_FIELD_HEALTH) >> 1;
            if( damage > half_health )
                damage = half_health;
        }break;

        //paladin  - Sacred Cleansing
    case 53659:
        {
            if(casting->NameHash != SPELL_HASH_CLEANSE )
                return false;
        }break;

        //paladin - Infusion of Light
    case 53672:
    case 54149:
        {
            if( casting->NameHash != SPELL_HASH_HOLY_SHOCK )
                return false;
        }break;

        //paladin - art of war
    case 53489:
    case 59578:
        {
            if( casting->NameHash != SPELL_HASH_CRUSADER_STRIKE &&
                casting->NameHash != SPELL_HASH_DIVINE_STORM &&
                !(casting->buffType & SPELL_TYPE_JUDGEMENT) )
                return false;
        }break;

    case 61840:
        {
            if(applying == NULL)
                return false;
            if(!target->IsPlayer())
                return false;
            if( casting->NameHash != SPELL_HASH_CRUSADER_STRIKE &&
                casting->NameHash != SPELL_HASH_DIVINE_STORM &&
                casting->buffType != SPELL_TYPE_JUDGEMENT )
                return false;

            damage = float2int32(float(dataHolder->GetDamage())*0.0375f);
        }break;

        // Paladin - Sheat of Light
    case 54203:
        {
            if(applying == NULL)
                return false;
            if(!(casting->c_is_flags & SPELL_FLAG_IS_HEALING))
                return false;
            damage = dataHolder->GetDamage() * (applying->EffectBasePoints[1] + 1) / 400;
        }break;

        //Energized
    case 43751:
        {
            if( casting->NameHash != SPELL_HASH_LIGHTNING_BOLT )
                return false;
        }break;

        //Spell Haste Trinket
    case 33370:
        {
            if( !( casting->c_is_flags & SPELL_FLAG_IS_DAMAGING ) )
                return false;
        }break;

        // Fathom-Brooch of the Tidewalker proc
    case 37243:
        {
            if( casting->School != SCHOOL_NATURE )
                return false;
        }break;

        //shaman - Lightning Overload
    case 39805:
        {
            //trigger on lightning and chain lightning. Spell should be identical , well maybe next time :P
            if(dataHolder->GetVictim() && (casting->NameHash == SPELL_HASH_LIGHTNING_BOLT || casting->NameHash == SPELL_HASH_CHAIN_LIGHTNING))
            {
                uSpellId = casting->Id;
                damage = (casting->EffectBasePoints[0] + 1) / 2; //only half dmg
                targets.m_unitTarget = dataHolder->GetVictim()->GetGUID();
            }
            else
                return false;
        }break;

        //item - Band of the Eternal Sage
    case 35084:
        {
            //requires offensive spell. ! might not cover all spells
            if( !( casting->c_is_flags & SPELL_FLAG_IS_DAMAGING ) )
                return false;
        }break;

    case 44544: //Fingers of Frost
    case 57761: //Brain Freeze
        {
            if( casting->NameHash != SPELL_HASH_FROSTBOLT &&
                casting->NameHash != SPELL_HASH_CONE_OF_COLD &&
                casting->NameHash != SPELL_HASH_FROSTFIRE_BOLT )
                return false;
        }break;

        // druid - Celestial Focus
    case 16922:
        {
            if( casting->NameHash != SPELL_HASH_STARFIRE )
                return false;
        }break;

    case 37565: //setbonus
        {
            if (casting->NameHash != SPELL_HASH_FLASH_HEAL)
                return false;
        }break;

        //SETBONUSES
    case 37379:
        {
            if (casting->School != SCHOOL_SHADOW || !(casting->c_is_flags & SPELL_FLAG_IS_DAMAGING))
                return false;
        }break;

    case 37378:
        {
            if (casting->School != SCHOOL_FIRE || !(casting->c_is_flags & SPELL_FLAG_IS_DAMAGING))
                return false;
        }break;

    case 39950:
        {
            if (!(casting->c_is_flags & SPELL_FLAG_IS_HEALING))
                return false;
        }break;

    case 37234:
    case 37214:
    case 37601:
        {
            if (!(casting->c_is_flags & SPELL_FLAG_IS_DAMAGING))
                return false;
        }break;

    case 37237:
        {
            if (casting->NameHash != SPELL_HASH_LIGHTNING_BOLT)
                return false;
        }break;

    case 37193:
        {
            if (casting->NameHash != SPELL_HASH_HOLY_SHIELD)
                return false;
        }break;

    case 37196:
    case 43838:
        {
            if( casting->buffIndexType != SPELL_TYPE_INDEX_JUDGEMENT )
                return false;
        }break;

    case 43837:
        {
            if( casting->NameHash != SPELL_HASH_FLASH_OF_LIGHT &&
                casting->NameHash != SPELL_HASH_HOLY_LIGHT )
                return false;
        }break;

    case 37529:
        {
            if( casting->NameHash != SPELL_HASH_OVERPOWER )
                return false;
        }break;

    case 37517:
        {
            if( casting->Id == 37517 || casting->NameHash != SPELL_HASH_REVENGE )
                return false;
        }break;

        //SETBONUSES END
        //Pendulum of Telluric Currents
    case 60483:
        {
            if(!( casting->c_is_flags & SPELL_FLAG_IS_DAMAGING ))
                return false;
        }break;

        //http://www.wowhead.com/?item=32493 Ashtongue Talisman of Shadows
    case 40480:
        {
            if(casting->NameHash != SPELL_HASH_CORRUPTION )
                return false;
        }break;

        //http://www.wowhead.com/?item=32488 Ashtongue Talisman of Insight
    case 40483:
        {
            if( !( casting->c_is_flags & SPELL_FLAG_IS_DAMAGING ) )
                return false;
        }break;

        //http://www.wowhead.com/?item=32487 Ashtongue Talisman of Swiftness
    case 40487:
        {
            if(casting->NameHash != SPELL_HASH_STEADY_SHOT)
                return false;
        }break;

        //http://www.wowhead.com/?item=32485 Ashtongue Talisman of Valor
    case 40459:
        {
            if((casting->NameHash != SPELL_HASH_MORTAL_STRIKE
                || casting->NameHash != SPELL_HASH_BLOODTHIRST
                || casting->NameHash != SPELL_HASH_SHIELD_SLAM))
                return false;
        }break;

        //item - Band of the Eternal Restorer
    case 35087:
        {
            if(!(casting->c_is_flags & SPELL_FLAG_IS_HEALING)) //requires healing spell.
                return false;
        }break;

        //http://www.wowhead.com/?item=32486 Ashtongue Talisman of Equilibrium
    case 40452: //Mangle has a 40% chance to grant 140 Strength for 8 sec
        {
            if(casting->NameHash != SPELL_HASH_MANGLE__BEAR_
                || casting->NameHash != SPELL_HASH_MANGLE__CAT_)
                return false;
        }break;

    case 40445: //Starfire has a 25% chance to grant up to 150 spell damage for 8 sec
        {
            if(casting->NameHash != SPELL_HASH_STARFIRE)
                return false;
        }break;

    case 40446: //Rejuvenation has a 25% chance to grant up to 210 healing for 8 sec
        {
            if(casting->NameHash != SPELL_HASH_REJUVENATION)
                return false;
        }break;

        //http://www.wowhead.com/?item=32490 Ashtongue Talisman of Acumen
    case 40441: //Each time your Shadow Word: Pain deals damage, it has a 10% chance to grant you 220 spell damage for 10 sec
        {
            if(casting->NameHash != SPELL_HASH_SHADOW_WORD__PAIN)
                return false;
        }break;

        //http://www.wowhead.com/?item=32490 Ashtongue Talisman of Acumen
    case 40440: //Each time your Renew heals, it has a 10% chance to grant you 220 healing for 5 sec
        {
            if(casting->NameHash != SPELL_HASH_RENEW)
                return false;
        }break;

        //http://www.wowhead.com/?item=32492 Ashtongue Talisman of Lethality
    case 40461:
        {
            //we need a finishing move for this
            if(!(casting->c_is_flags & SPELL_FLAG_IS_FINISHING_MOVE) || dataHolder->GetVictim() == TO_UNIT(target))
                return false;
        }break;

    case 37445: //using a mana gem grants you 225 spell damage for 15 sec
        {
            if (casting->NameHash != SPELL_HASH_REPLENISH_MANA)
                return false;
        }break;

    case 38395:
        {
            if( casting->NameHash != SPELL_HASH_IMMOLATE &&
                casting->NameHash != SPELL_HASH_CORRUPTION)
                return false;
        }break;

    case 48108: // [Mage] Hot Streak
        {
            if( casting->NameHash != SPELL_HASH_FIREBALL &&
                casting->NameHash != SPELL_HASH_FIRE_BLAST &&
                casting->NameHash != SPELL_HASH_SCORCH &&
                casting->NameHash != SPELL_HASH_LIVING_BOMB &&
                casting->NameHash != SPELL_HASH_FROSTFIRE_BOLT )
                return false;

            target->m_hotStreakCount++;
            if (target->m_hotStreakCount >= 2)
                target->m_hotStreakCount = 0;
            else
                return false;
        }break;

    case 52179: // Astral Shift
        {
            if( !spe ||
                !(Spell::HasMechanic(casting, MECHANIC_STUNNED) ||
                Spell::HasMechanic(casting, MECHANIC_FLEEING) ||
                Spell::HasMechanic(casting, MECHANIC_SILENCED)) )
                return false;

            SpellDuration *sd = dbcSpellDuration.LookupEntry(casting->DurationIndex);
            if(!sd)
                return false;

            int32 dur = GetDBCDuration(sd);
            if(dur > 10000)
                dur = 10000;

            Aura* aura = new Aura(spe, dur, dataHolder->GetVictim(), TO_UNIT(target));
            aura->AddMod(SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, -30, 127, 0);
            target->AddAura(aura);
            return false;
        }break;

    case 51693: // Waylay
        {
            if(casting->NameHash != SPELL_HASH_AMBUSH)
                return false;
        }break;

    case 60503: // Taste for Blood
        {
            if(casting->NameHash != SPELL_HASH_REND)
                return false;
        }break;

    case 46989: //improved blink
    case 47000:
        {
            if (casting->NameHash != SPELL_HASH_BLINK )
                return false;
        }break;

    case 52752: // Ancestral Awakening
        {
            if(applying == NULL)
                return false;
            if((casting->NameHash != SPELL_HASH_HEALING_WAVE &&
                casting->NameHash != SPELL_HASH_LESSER_HEALING_WAVE &&
                casting->NameHash != SPELL_HASH_RIPTIDE) ||
                !target->IsPlayer())
                return false;

            targets.m_unitTarget = Spell::FindLowestHealthRaidMember(TO_PLAYER(target), 1600); // within 40 yards
            if(!targets.m_unitTarget)   // shouldn't happen
                return false;

            damage = dataHolder->GetDamage() * (applying->EffectBasePoints[0] + 1) / 100;
        }break;

    case 54748: //Burning Determination
        {
            if(!(Spell::HasMechanic(casting, MECHANIC_SILENCED)
                || Spell::HasMechanic(casting, MECHANIC_INTERRUPTED)))
                return false;
        }break;

    case 54741: //Firestarter
        {
            if( casting->NameHash != SPELL_HASH_BLAST_WAVE &&
                casting->NameHash != SPELL_HASH_DRAGON_S_BREATH )
                return false;
        }break;

    case 12494: //Frostbite
        {
            //me thinks its correct
            if( !( casting->SpellGroupType[0] & 0x100220 ) || dataHolder->GetVictim() == TO_UNIT(target) ) //frost
                return false;
        }break;

    case 53390: //Tidal Waves
        {
            if( casting->NameHash != SPELL_HASH_CHAIN_HEAL &&
                casting->NameHash != SPELL_HASH_RIPTIDE )
                return false;
        }break;

        //Earthliving
    case 51945:
    case 51990:
    case 51997:
    case 51998:
    case 51999:
    case 52000:
        {
            if( !(casting->c_is_flags & SPELL_FLAG_IS_HEALING) )
                return false;
        }break;

        //Borrowed Time
    case 59887:
    case 59888:
    case 59889:
    case 59890:
    case 59891:
        {
            if( casting->NameHash != SPELL_HASH_POWER_WORD__SHIELD )
                return false;
        }break;

    case 60229: // Darkmoon Card: Greatness
        {
            if(!(casting->c_is_flags & (SPELL_FLAG_IS_DAMAGING | SPELL_FLAG_IS_HEALING) ))
                return false;
        }break;

    case 49694://Improved Spirit Tap
    case 59000:
        {
            if( casting->NameHash != SPELL_HASH_MIND_BLAST &&
                casting->NameHash != SPELL_HASH_SHADOW_WORD__DEATH )
                return false;
        }break;

    case 33196://Misery
    case 33197:
    case 33198:
        {
            if( casting->NameHash != SPELL_HASH_MIND_FLAY &&
                casting->NameHash != SPELL_HASH_VAMPIRIC_TOUCH &&
                casting->NameHash != SPELL_HASH_SHADOW_WORD__PAIN )
                return false;
        }break;

    case 47383://molten core
        {
            if( !( casting->c_is_flags & SPELL_FLAG_IS_DAMAGING ) )
                return false;

            if( casting->School != SCHOOL_SHADOW )
                return false;
        }break;

    case 53655://judgements of the pure
    case 53656:
    case 53657:
    case 54152:
    case 54153:
        {
            if( casting->buffIndexType != SPELL_TYPE_INDEX_JUDGEMENT )
                return false;
        }break;

    case 60803://Glyph of Remove Curse
        {
            if( casting->NameHash != SPELL_HASH_REMOVE_CURSE || dataHolder->GetVictim() == target )
                return false;
        }break;

    case 16246: // Elemental focus
        {
            if( !( casting->c_is_flags & SPELL_FLAG_IS_DAMAGING ) )
                return false;

            if( casting->School != SCHOOL_FIRE &&
                casting->School != SCHOOL_NATURE &&
                casting->School != SCHOOL_FROST )
                return false;
        }break;

    case 55021:
    case 18469:
        {
            if( casting->NameHash != SPELL_HASH_COUNTERSPELL || dataHolder->GetVictim() == target )
                return false;
        }break;

    case 60433:
    case 60432:
    case 60431: // Earth and Moon
        {
            if( casting->NameHash != SPELL_HASH_WRATH &&
                casting->NameHash != SPELL_HASH_STARFIRE )
                return false;
        }break;

        // Bloodsurge
    case 46916:
        {
            if( casting->NameHash != SPELL_HASH_HEROIC_STRIKE &&
                casting->NameHash != SPELL_HASH_BLOODTHIRST &&
                casting->NameHash != SPELL_HASH_WHIRLWIND )
                return false;
        }break;

    case 58362: // Glyph of heroic strike
        {
            if( casting->NameHash != SPELL_HASH_HEROIC_STRIKE )
                return false;
        }break;

    case 43738://primal instinct
        {
            if( casting->NameHash != SPELL_HASH_MANGLE__CAT_ &&
                casting->NameHash != SPELL_HASH_MANGLE__BEAR_ )
                return false;
        }break;

    case 44401: // Missile Barrage
        {
            if( casting->NameHash != SPELL_HASH_ARCANE_BLAST &&
                casting->NameHash != SPELL_HASH_ARCANE_BARRAGE &&
                casting->NameHash != SPELL_HASH_FIREBALL &&
                casting->NameHash != SPELL_HASH_FROSTBOLT &&
                casting->NameHash != SPELL_HASH_FROSTFIRE_BOLT )
                return false;
        }break;

    case 16886:
        {
            if( !(casting->c_is_flags & SPELL_FLAG_IS_HEALING) )
                return false;
        }break;

    case 47948:
        {
            if( casting->NameHash != SPELL_HASH_MIND_FLAY )
                return false;
        }break;

        // Illumination
    case 20272:
        {
            if( casting->NameHash != SPELL_HASH_HOLY_LIGHT &&
                casting->NameHash != SPELL_HASH_FLASH_OF_LIGHT &&
                casting->NameHash != SPELL_HASH_HOLY_SHOCK )
                return false;

            damage = float2int32(target->GetSpellBaseCost(casting) * 0.3f);
        }break;

        // Omen of Clarity
    case 16870:
        {
            if(!(casting->c_is_flags & SPELL_FLAG_IS_HEALING ||
                casting->c_is_flags & SPELL_FLAG_IS_DAMAGING) )
                return false;
        }break;

        // Gag order
    case 18498:
        {
            if( casting->NameHash != SPELL_HASH_SHIELD_BASH && casting->NameHash != SPELL_HASH_HEROIC_THROW )
                return false;
        }break;

        // Replenishment
    case 57669:
        {
            if( casting->NameHash != SPELL_HASH_EXPLOSIVE_SHOT
                && casting->NameHash != SPELL_HASH_ARCANE_SHOT
                && casting->NameHash != SPELL_HASH_STEADY_SHOT )
                return false;

            if( target->IsPlayer() )
            {
                Player* caster = TO_PLAYER( target );
                if( caster->GetGroup() )
                {
                    uint32 TargetCount = 0;
                    caster->GetGroup()->Lock();
                    for(uint32 x = 0; x < caster->GetGroup()->GetSubGroupCount(); ++x)
                    {
                        if( TargetCount == 10 )
                            break;

                        for(GroupMembersSet::iterator itr = caster->GetGroup()->GetSubGroup( x )->GetGroupMembersBegin(); itr != caster->GetGroup()->GetSubGroup( x )->GetGroupMembersEnd(); itr++)
                        {
                            if((*itr)->m_loggedInPlayer && TargetCount <= 10)
                            {
                                Player* p_target = (*itr)->m_loggedInPlayer;
                                if( p_target->GetPowerType() != POWER_TYPE_MANA )
                                    return false;

                                SpellEntry* Replinishment = dbcSpell.LookupEntry( 57669 );
                                Spell* pSpell = NULLSPELL;
                                pSpell = (new Spell(TO_PLAYER( target ), Replinishment, true, NULLAURA));
                                pSpell->forced_basepoints[0] = float2int32(p_target->GetUInt32Value(UNIT_FIELD_BASE_MANA) * 0.0025f);
                                SpellCastTargets tgt;
                                tgt.m_unitTarget = p_target->GetGUID();
                                pSpell->prepare(&tgt);
                                TargetCount++;
                            }
                        }
                    }
                    caster->GetGroup()->Unlock();
                    return false;
                }
            }
        }break;

        // Blade Barrier
    case 51789:
        {
            if( !target->IsPlayer() )
                return false;

            SpellRuneCostEntry * sr = dbcSpellRuneCost.LookupEntry( casting->RuneCostID );
            if( !sr || sr->bloodRuneCost == 0 ) // not costing blood.
                return false;
            if( TO_PLAYER(target)->GetRune(0) == RUNE_TYPE_BLOOD || TO_PLAYER(target)->GetRune(1) == RUNE_TYPE_BLOOD )
                return false; // oh snap, still have blood runes, this doesn't count.
        }break;
    case 55666: // Desecration Rank 1
    case 55667: // Desecration Rank 2
        {
            if(casting->NameHash != SPELL_HASH_PLAGUE_STRIKE && casting->NameHash != SPELL_HASH_SCOURGE_STRIKE)
                return false;
        }break;

    case 64745:
        {
            if(casting->NameHash != SPELL_HASH_ANTI_MAGIC_SHELL)
                return false;
        }break;

    case 64936:
        {
            if(casting->NameHash != SPELL_HASH_SHIELD_BLOCK)
                return false;
        }break;
    case 60675:
    case 60685:
    case 60686:
    case 60687:
    case 60688:
    case 60690:
        {
            if(casting->NameHash != SPELL_HASH_PLAGUE_STRIKE)
                return false;
        }break;

    case 50040:
    case 50041:
    case 50043:
        {
            if(casting->NameHash != SPELL_HASH_FROST_FEVER)
                return false;
        }break;
    }

    switch(uSpellId) // Independent of Casting spell
    {
        case 25997:
            {
                if(casting == NULL)
                    return false;
            }break;

        case 54180:
        case 31930:
            {
                if(casting == NULL)
                    return false;
            }break;

    case 31616: // Nature's Guardian
        {
            if(applying == NULL)
                return false;

            if(target->GetHealthPct() > 30)
                return false;

            //heal value depends on the rank of parent spell
            //maybe we should use CalculateEffect(uint32 i) to gain SM benefits
            int32 value = 0;
            int32 basePoints = applying->EffectBasePoints[0]+1;//+(getLevel()*basePointsPerLevel);
            int32 randomPoints = applying->EffectDieSides[0];
            if(randomPoints <= 1)
                value = basePoints;
            else
                value = basePoints + RandomUInt(randomPoints);
            damage = target->GetUInt32Value(UNIT_FIELD_MAXHEALTH) * value / 100;
        }break;

    case 37309:
        {
            if( !target->IsPlayer() )
                return false;

            if( TO_PLAYER(target)->GetShapeShift() != FORM_BEAR ||
                TO_PLAYER(target)->GetShapeShift() != FORM_DIREBEAR )
                return false;
        }break;

    case 37310:
        {
            if( !target->IsPlayer() || TO_PLAYER(target)->GetShapeShift() != FORM_CAT )
                return false;
        }break;

    case 5530:
        {
            //warrior/rogue mace specialization can trigger only when using maces
            Item* it;
            if( TO_PLAYER(target)->GetItemInterface() )
            {
                it = TO_PLAYER(target)->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
                if( it != NULL && it->GetProto() )
                {
                    uint32 reqskill = Item::GetSkillByProto( it->GetProto()->Class, it->GetProto()->SubClass );
                    if( reqskill != SKILL_MACES && reqskill != SKILL_2H_MACES )
                        return false;
                }
                else
                    return false; //no weapon no joy
            }
            else
                return false; //no weapon no joy

            //let's recalc chance to cast since we have a full 100 all time on this one
            //how lame to get talentpointlevel for this spell :(
            //float chance=it->GetProto()->Delay*100*talentlevel/60000;
        }break;

    case 16459:
    case 4350:
        {
            //sword specialization
            if( TO_PLAYER(target)->GetItemInterface())
            {
                Item* it = TO_PLAYER(target)->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
                if( it != NULL && it->GetProto() )
                {
                    uint32 reqskill = Item::GetSkillByProto( it->GetProto()->Class, it->GetProto()->SubClass );
                    if( reqskill != SKILL_SWORDS && reqskill != SKILL_2H_SWORDS )
                        return false;
                }
                else
                    return false; //no weapon no joy
            }
            else
                return false; //no weapon no joy
        }break;

    case 12721:
        {
            //deep wound requires a melee weapon
            Item* it;
            if( TO_PLAYER(target)->GetItemInterface())
            {
                it = TO_PLAYER(target)->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
                if( it != NULL && it->GetProto() )
                {
                    //class 2 means weapons ;)
                    if( it->GetProto()->Class != ITEM_CLASS_WEAPON )
                        return false;
                }
                else 
                    return false; //no weapon no joy
            }
            else 
                return false; //no weapon no joy
        }break;

        //warrior - Unbridled Wrath
    case 12964:
        {
            //let's recalc chance to cast since we have a full 100 all time on this one
            Item* it;
            if( TO_PLAYER(target)->GetItemInterface() )
            {
                it = TO_PLAYER(target)->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
                if( !( it != NULL && it->GetProto() ) )
                    return false; //no weapon no joy
            }
            else
                return false; //no weapon no joy

            //float chance=float(it->GetProto()->Delay)*float(talentlevel)/600.0f;
            float chance = float( it->GetProto()->Delay ) * float(talentlevel ) / 300.0f; //zack this had a very low proc rate. Kinda liek a waisted talent
            uint32 myroll = RandomUInt( 100 );
            if( myroll > chance )
                return false;
        }break;

        //priest - Blessed Recovery
    case 27813:
    case 27817:
    case 27818:
        {
            if(!target->IsPlayer() || !dataHolder->GetDamage())
                return false;

            int32 val = applying->EffectBasePoints[0] + 1;
            Spell* spell(new Spell(TO_UNIT(target), spe ,true, NULLAURA));
            spell->forced_basepoints[0] = (val*dataHolder->GetDamage())/300; //per tick
            SpellCastTargets targets;
            targets.m_unitTarget = target->GetGUID();
            spell->prepare(&targets);
            return false;
        }break;

        //priest - Reflective Shield
    case 33619:
        {
            if(applying == NULL)
                return false;

            //requires Power Word: Shield active
            uint32 power_word_id = target->m_AuraInterface.GetAuraSpellIDWithNameHash( SPELL_HASH_POWER_WORD__SHIELD );
            if( !power_word_id )
                return false;//this should not ocur unless we made a fuckup somewhere

            //make a direct strike then exit rest of handler
            int tdmg = dataHolder->GetAbsorb() * ( applying->EffectBasePoints[0] + 1 ) / 100;
            //somehow we should make this not caused any threat (tobedone)
            target->SpellNonMeleeDamageLog( dataHolder->GetVictim(), power_word_id, tdmg, false, true );
            return false;
        }break;

        //rogue - combat potency
    case 35542:
    case 35545:
    case 35546:
    case 35547:
    case 35548:
        {
            if( !target->IsPlayer() || !dataHolder->GetDamage() || dataHolder->GetWeaponDamageType() != 2 )
                return false;
        }break;

        // Pallas
    case 43747:
    case 34260:
        {
            if( !target->HasDummyAura(SPELL_HASH_SEAL_OF_COMMAND) )
                return false;
        }break;

    case 48391: // Owlkin Frenzy
        {
            if( !target->IsPlayer() )
                return false;

            if( TO_PLAYER( target )->GetShapeShift() != FORM_MOONKIN )
                return false;
        }break;

    case 59653:
        {
            float shdmg = 0.0f;
            if( target->HasDummyAura(SPELL_HASH_DAMAGE_SHIELD) )
                target->GetDummyAura(SPELL_HASH_DAMAGE_SHIELD)->Id == 58874 ? shdmg = 0.2f : shdmg = 0.1f;

            damage = float2int32(target->GetUInt32Value(PLAYER_SHIELD_BLOCK) * shdmg);
        }break;

        // Aspect of the Viper
    case 34075:
        {
            if( !target->IsPlayer() )
                return false;

            Player* caster = TO_PLAYER(target);
            Item* it = caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
            if( !it )
                it = caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
            if( it )
            {
                float mod = it->GetProto()->Delay / 1000.0f;
                damage = float2int32( 0.01f * mod * caster->GetUInt32Value(UNIT_FIELD_BASE_MANA) );
            }
            else
                damage = 1;
        }break;

    case 50475:
        {
            if( dataHolder->GetDamage() ) // heal him :)
            {
                int32 toheal = (int32)(dataHolder->GetDamage() * 0.04);
                target->Heal( TO_UNIT(target), 50475, toheal );
            }
        }break;

    case 60064: // Sundial of the Exiled
        {
            if(!dataHolder->GetVictim() || !sFactionSystem.isAttackable(target, dataHolder->GetVictim(), false))   // harmful spells
                return false;
        }break;

        // Spells that are takedamage but less than 35.
    case 45057: // Trinket: Evasive Maneuvers
    case 45058: // Commendation of Kael'thas
    case 44440: // Fiery Payback 1
    case 44441: // Fiery Payback 2
    case 71634: // Corpse Tongue
    case 71640: // Corpse Tongue H
        {
            if(target->GetHealthPct() > 35)
                return false;
        }break;

    case 63685:// Frozen Power - Freeze
        {
            uint32 minDistance = spe->EffectBasePoints[0] + 1;
            if(!dataHolder->GetVictim() || target->GetDistanceSq(dataHolder->GetVictim()) < minDistance * minDistance)
                return false;   // victim not far enough
        }break;

    case 63375: // Improved Stormstrike
        {
            damage = (spe->EffectBasePoints[0] + 1) * target->GetUInt32Value(UNIT_FIELD_BASE_MANA) / 100;
        }break;

    case 14189: //Seal Fate
        {
            return false; // Fuck this shit.
        }break;

    case 71484:
    case 71485:
    case 71486:
    case 71487:
    case 71491:
    case 71492:
    case 71556:
    case 71558:
    case 71559:
    case 71560:
    case 71561:
        {
            bool hasaura = false;
            uint32 auras[11] = { 71484, 71485, 71486, 71487, 71491, 71492, 71556, 71558, 71559, 71560, 71561 };
            for(int i = 0; i < 11; i++)
            {
                if(target->HasAura(auras[i]))
                {
                    hasaura = true;
                    break;
                }
            }
            if(hasaura)
                return false;
        }break;
    }

    if( casting && uSpellId == 54370 )
    {
        uint32 NewSpellID = 0;
        switch( casting->School )
        {
        case SCHOOL_HOLY: NewSpellID = 54370; break;
        case SCHOOL_FIRE: NewSpellID = 54371; break;
        case SCHOOL_NATURE: NewSpellID = 54375; break;
        case SCHOOL_FROST: NewSpellID = 54372; break;
        case SCHOOL_SHADOW: NewSpellID = 54374; break;
        case SCHOOL_ARCANE: NewSpellID = 54373; break;
        }

        if( NewSpellID )
            uSpellId = NewSpellID;
    }

    return true;
}
