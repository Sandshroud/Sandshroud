/*
 * Lacrimi Scripts Copyright 2010 - 2011
 *
 * ############################################################
 * # ##            #       ####### ####### ##    #    #    ## #
 * # ##           ###      ##      ##   ## ##   ###  ###   ## #
 * # ##          ## ##     ##      ##   ## ##   ###  ###   ## #
 * # ##         #######    ##      ####### ##  ## #### ##  ## #
 * # ##        ##     ##   ##      #####   ##  ## #### ##  ## #
 * # ##       ##       ##  ##      ##  ##  ## ##   ##   ## ## #
 * # ####### ##         ## ####### ##   ## ## ##   ##   ## ## #
 * # :::::::.::.........::.:::::::.::...::.::.::...::...::.:: #
 * ############################################################
 *
 */

#include "LacrimiStdAfx.h"

class OrgrimmarGuard : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player* plr, bool AutoSend)
    {
        GossipMenu* Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, plr);
        Menu->AddItem(0, "The bank"                 , 1);
        Menu->AddItem(0, "The wind rider master"    , 2);
        Menu->AddItem(0, "The guild master"         , 3);
        Menu->AddItem(0, "The inn"                  , 4);
        Menu->AddItem(0, "The mailbox"              , 5);
        Menu->AddItem(0, "The auction house"        , 6);
        Menu->AddItem(0, "The zeppelin master"      , 7);
        Menu->AddItem(0, "The weapon master"        , 8);
        Menu->AddItem(0, "The stable master"        , 9);
        Menu->AddItem(0, "The officers' lounge"     , 10);
        Menu->AddItem(0, "The battlemaster"         , 11);
        Menu->AddItem(0, "A class trainer"          , 12);
        Menu->AddItem(0, "A profession trainer"     , 13);
        Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
    {
        GossipMenu* Menu;
        switch(IntId)
        {
        case 0: 
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers
            /////
        case 1:     // The bank
            Plr->Gossip_SendPOI(1631.51f, -4375.33f, 7, 6, 0, "Bank of Orgrimmar");
            SendEmptyMenu(2554);
            break;

        case 2:     // The wind rider master
            Plr->Gossip_SendPOI(1676.6f, -4332.72f, 7, 6, 0, "The Sky Tower");
            SendEmptyMenu(2555);
            break;

        case 3:     // The guild master
            Plr->Gossip_SendPOI(1576.93f, -4294.75f, 7, 6, 0, "Horde Embassy");
            SendEmptyMenu(2556);
            break;

        case 4:     // The inn
            Plr->Gossip_SendPOI(1644.51f, -4447.27f, 7, 6, 0, "Orgrimmar Inn");
            SendEmptyMenu(2557);
            break;

        case 5:     // The mailbox
            Plr->Gossip_SendPOI(1622.53f, -4388.79f, 7, 6, 0, "Orgrimmar Mailbox");
            SendEmptyMenu(2558);
            break;

        case 6:     // The auction house
            Plr->Gossip_SendPOI(1679.21f, -4450.1f, 7, 6, 0, "Orgrimmar Auction House");
            SendEmptyMenu(3075);
            break;

        case 7:     // The zeppelin master
            Plr->Gossip_SendPOI(1337.36f, -4632.7f, 7, 6, 0, "Orgrimmar Zeppelin Tower");
            SendEmptyMenu(3173);
            break;

        case 8:     // The weapon master
            Plr->Gossip_SendPOI(2092.56f, -4823.95f, 7, 6, 0, "Sayoc & Hanashi");
            SendEmptyMenu(4519);
            break;

        case 9:     // The stable master
            Plr->Gossip_SendPOI(2133.12f, -4663.93f, 7, 6, 0, "Xon'cha");
            SendEmptyMenu(5974);
            break;

        case 10:    // The officers' lounge
            Plr->Gossip_SendPOI(1633.56f, -4249.37f, 7, 6, 0, "Hall of Legends");
            SendEmptyMenu(7046);
            break;

        case 11:    // The battlemaster
            Plr->Gossip_SendPOI(1990.41f, -4794.15f, 7, 6, 0, "Battlemasters Orgrimmar");
            SendEmptyMenu(7521);
            break;

        case 12:    // A class trainer
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2599, Plr);
                Menu->AddItem(0, "Hunter"       , 14);
                Menu->AddItem(0, "Mage"         , 15);
                Menu->AddItem(0, "Priest"       , 16);
                Menu->AddItem(0, "Shaman"       , 17);
                Menu->AddItem(0, "Rogue"        , 18);
                Menu->AddItem(0, "Warlock"      , 19);
                Menu->AddItem(0, "Warrior"      , 20);
                Menu->AddItem(0, "Paladin"      , 21);
                Menu->SendTo(Plr);
            }break;

        case 13:    // A profession trainer
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2594, Plr);
                Menu->AddItem(0, "Alchemy"           , 22);
                Menu->AddItem(0, "Blacksmithing"     , 23);
                Menu->AddItem(0, "Cooking"           , 24);
                Menu->AddItem(0, "Enchanting"        , 25);
                Menu->AddItem(0, "Engineering"       , 26);
                Menu->AddItem(0, "First Aid"         , 27);
                Menu->AddItem(0, "Fishing"           , 28);
                Menu->AddItem(0, "Herbalism"         , 29);
                Menu->AddItem(0, "Leatherworking"    , 30);
                Menu->AddItem(0, "Mining"            , 31);
                Menu->AddItem(0, "Skinning"          , 32);
                Menu->AddItem(0, "Tailoring"         , 33);
                Menu->SendTo(Plr);
            }break;

            ////////////////
            // Class trainer submenu
            ////////
        case 14: //Hunter
            Plr->Gossip_SendPOI(2114.84f, -4625.31f, 7, 6, 0, "Orgrimmar Hunter's Hall");
            SendQuickMenu(2559);
            break;

        case 15: //Mage
            Plr->Gossip_SendPOI(1451.26f, -4223.33f, 7, 6, 0, "Darkbriar Lodge");
            SendQuickMenu(2560);
            break;

        case 16: //Priest
            Plr->Gossip_SendPOI(1442.21f, -4183.24f, 7, 6, 0, "Spirit Lodge");
            SendQuickMenu(2561);
            break;

        case 17: //Shaman
            Plr->Gossip_SendPOI(1925.34f, -4181.89f, 7, 6, 0, "Thrall's Fortress");
            SendQuickMenu(2562);
            break;

        case 18: //Rogue
            Plr->Gossip_SendPOI(1773.39f, -4278.97f, 7, 6, 0, "Shadowswift Brotherhood");
            SendQuickMenu(2563);
            break;

        case 19: //Warlock
            Plr->Gossip_SendPOI(1849.57f, -4359.68f, 7, 6, 0, "Darkfire Enclave");
            SendQuickMenu(2564);
            break;

        case 20: //Warrior
            Plr->Gossip_SendPOI(1983.92f, -4794.2f, 7, 6, 0, "Hall of the Brave");
            SendQuickMenu(2565);
            break;

        case 21: //Paladin
            Plr->Gossip_SendPOI(1937.53f, -4141.0f, 7, 6, 0, "Thrall's Fortress");
            SendQuickMenu(2566);
            break;

        case 22: //Alchemy
            Plr->Gossip_SendPOI(1955.17f, -4475.79f, 7, 6, 0, "Yelmak's Alchemy and Potions");
            SendQuickMenu(2497);
            break;

        case 23: //Blacksmithing
            Plr->Gossip_SendPOI(2054.34f, -4831.85f, 7, 6, 0, "The Burning Anvil");
            SendQuickMenu(2499);
            break;

        case 24: //Cooking
            Plr->Gossip_SendPOI(1780.96f, -4481.31f, 7, 6, 0, "Borstan's Firepit");
            SendQuickMenu(2500);
            break;

        case 25: //Enchanting
            Plr->Gossip_SendPOI(1917.5f, -4434.95f, 7, 6, 0, "Godan's Runeworks");
            SendQuickMenu(2501);
            break;

        case 26: //Engineering
            Plr->Gossip_SendPOI(2038.45f, -4744.75f, 7, 6, 0, "Nogg's Machine Shop");
            SendQuickMenu(2653);
            break;

        case 27: //First Aid
            Plr->Gossip_SendPOI(1485.21f, -4160.91f, 7, 6, 0, "Survival of the Fittest");
            SendQuickMenu(2502);
            break;

        case 28: //Fishing
            Plr->Gossip_SendPOI(1994.15f, -4655.7f, 7, 6, 0, "Lumak's Fishing");
            SendQuickMenu(2503);
            break;

        case 29: //Herbalism
            Plr->Gossip_SendPOI(1898.61f, -4454.93f, 7, 6, 0, "Jandi's Arboretum");
            SendQuickMenu(2504);
            break;

        case 30: //Leatherworking
            Plr->Gossip_SendPOI(1852.82f, -4562.31f, 7, 6, 0, "Kodohide Leatherworkers");
            SendQuickMenu(2513);
            break;

        case 31: //Mining
            Plr->Gossip_SendPOI(2029.79f, -4704, 7, 6, 0, "Red Canyon Mining");
            SendQuickMenu(2515);
            break;

        case 32: //Skinning
            Plr->Gossip_SendPOI(1852.82f, -4562.31f, 7, 6, 0, "Kodohide Leatherworkers");
            SendQuickMenu(2516);
            break;

        case 33: //Tailoring
            Plr->Gossip_SendPOI(1802.66f, -4560.66f, 7, 6, 0, "Magar's Cloth Goods");
            SendQuickMenu(2518);
            break;
        }
    }
};

class RazorHillGuard : public GossipScript
{
    public:
        void GossipHello(Object* pObject, Player* plr, bool AutoSend)
        {
            GossipMenu* Menu;
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4037, plr);
            Menu->AddItem(0, "The bank",              1);
            Menu->AddItem(0, "The wind rider master", 2);
            Menu->AddItem(0, "The inn",               3);
            Menu->AddItem(0, "The stable master",     4);
            Menu->AddItem(0, "A class trainer",       5);
            Menu->AddItem(0, "A profession trainer",  6);

            Menu->SendTo(plr);
        }

        void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
        {
            GossipMenu* Menu;
            switch(IntId)
            {
                case 0:     // Return to start
                    GossipHello(pObject, Plr, true);
                    break;

                    //////////////////////
                    // Main menu handlers
                    /////
                case 1:     // The bank
                    SendEmptyMenu(4032);
                    break;

                case 2:     // The wind rider master
                    SendEmptyMenu(4033);
                    break;

                case 3:     // The inn
                    Plr->Gossip_SendPOI(338.7f, -4688.87f, 7, 6, 0, "Razor Hill Inn");
                    SendEmptyMenu(4034);
                    break;

                case 4:     // The stable master
                    Plr->Gossip_SendPOI(330.31f, -4710.66f, 7, 6, 0, "Shoja'my");
                    SendEmptyMenu(5973);
                    break;

                case 5:     // A class trainer
                    {
                        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4035, Plr);
                        Menu->AddItem(0, "Hunter"       , 7);
                        Menu->AddItem(0, "Mage"         , 8);
                        Menu->AddItem(0, "Priest"       , 9);
                        Menu->AddItem(0, "Rogue"       , 10);
                        Menu->AddItem(0, "Shaman"      , 11);
                        Menu->AddItem(0, "Warlock"     , 12);
                        Menu->AddItem(0, "Warrior"     , 13);
                        Menu->SendTo(Plr);
                    }
                    break;

                case 6:     // A profession trainer
                    {
                        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3541, Plr);
                        Menu->AddItem(0, "Alchemy"           , 14);
                        Menu->AddItem(0, "Blacksmithing"     , 15);
                        Menu->AddItem(0, "Cooking"           , 16);
                        Menu->AddItem(0, "Enchanting"        , 17);
                        Menu->AddItem(0, "Engineering"       , 18);
                        Menu->AddItem(0, "First Aid"         , 19);
                        Menu->AddItem(0, "Fishing"           , 20);
                        Menu->AddItem(0, "Herbalism"         , 21);
                        Menu->AddItem(0, "Leatherworking"    , 22);
                        Menu->AddItem(0, "Mining"            , 23);
                        Menu->AddItem(0, "Skinning"          , 24);
                        Menu->AddItem(0, "Tailoring"         , 25);
                        Menu->SendTo(Plr);
                    }
                    break;

                    ////////////////
                    // Class trainer submenu
                    ////////
                case 7: //Hunter
                    {
                        Plr->Gossip_SendPOI(276, -4706.72f, 7, 6, 0, "Thotar");
                        SendQuickMenu(4013);
                    }
                    break;

                case 8: //Mage
                    {
                        Plr->Gossip_SendPOI(-839.33f, -4935.6f, 7, 6, 0, "Un'Thuwa");
                        SendQuickMenu(4014);
                    }
                    break;

                case 9: //Priest
                    {
                        Plr->Gossip_SendPOI(296.22f, -4828.1f, 7, 6, 0, "Tai'jin");
                        SendQuickMenu(4015);
                    }
                    break;

                case 10: //Rogue
                    {
                        Plr->Gossip_SendPOI(265.76f, -4709, 7, 6, 0, "Kaplak");
                        SendQuickMenu(4016);
                    }
                    break;

                case 11: //Shaman
                    {
                        Plr->Gossip_SendPOI(307.79f, -4836.97f, 7, 6, 0, "Swart");
                        SendQuickMenu(4017);
                    }
                    break;

                case 12: //Warlock
                    {
                        Plr->Gossip_SendPOI(355.88f, -4836.45f, 7, 6, 0, "Dhugru Gorelust");
                        SendQuickMenu(4018);
                    }
                    break;

                case 13: //Warrior
                    {
                        Plr->Gossip_SendPOI(312.3f, -4824.66f, 7, 6, 0, "Tarshaw Jaggedscar");
                        SendQuickMenu(4019);
                    }
                    break;

                case 14: //Alchemy
                    {
                        Plr->Gossip_SendPOI(-800.25f, -4894.33f, 7, 6, 0, "Miao'zan");
                        SendQuickMenu(4020);
                    }
                    break;

                case 15: //Blacksmithing
                    {
                        Plr->Gossip_SendPOI(373.24f, -4716.45f, 7, 6, 0, "Dwukk");
                        SendQuickMenu(4021);
                    }
                    break;

                case 16: //Cooking
                    {
                        SendQuickMenu(4022);
                    }
                    break;

                case 17: //Enchanting
                    {
                        SendQuickMenu(4023);
                    }
                    break;

                case 18: //Engineering
                    {
                        Plr->Gossip_SendPOI(368.95f, -4723.95f, 7, 6, 0, "Mukdrak");
                        SendQuickMenu(4024);
                    }
                    break;

                case 19: //First Aid
                    {
                        Plr->Gossip_SendPOI(327.17f, -4825.62f, 7, 6, 0, "Rawrk");
                        SendQuickMenu(4025);
                    }
                    break;

                case 20: //Fishing
                    {
                        Plr->Gossip_SendPOI(-1065.48f, -4777.43f, 7, 6, 0, "Lau'Tiki");
                        SendQuickMenu(4026);
                    }
                    break;

                case 21: //Herbalism
                    {
                        Plr->Gossip_SendPOI(-836.25f, -4896.89f, 7, 6, 0, "Mishiki");
                        SendQuickMenu(4027);
                    }
                    break;

                case 22: //Leatherworking
                    {
                        SendQuickMenu(4028);
                    }
                    break;

                case 23: //Mining
                    {
                        Plr->Gossip_SendPOI(366.94f, -4705, 7, 6, 0, "Krunn");
                        SendQuickMenu(4029);
                    }
                    break;

                case 24: //Skinning
                    {
                        Plr->Gossip_SendPOI(-2252.94f, -291.32f, 7, 6, 0, "Yonn Deepcut");
                        SendQuickMenu(4030);
                    }
                    break;

                case 25: //Tailoring
                    {
                        SendQuickMenu(4031);
                    }
                    break;
            }
        }
};

void Lacrimi::SetupOrgrimmar()
{
    RegisterCtrGossipScript(3296,    OrgrimmarGuard);    // Orgrimmar Grunt
    RegisterCtrGossipScript(5953,    RazorHillGuard);    // Razor Hill Grunt
}
