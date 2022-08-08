#include "Chat.h"
#include "Config.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "DataMap.h"
#include "Group.h"
#include "DBUpdater.h"

#define SPELL_AURA_HARDCORE     3758285

#define GOSSIP_OPTION_HARDCORE   1
#define GOSSIP_OPTION_BOOST     100
#define GOSSIP_BACK             999

#define NPC_TEXT_HARDCORE       70000
#define NPC_TEXT_BOOST          70001

#define GOSSIP_HELLO_HARDCORE   62000
#define GOSSIP_HELLO_BOOST      62001

#define GOSSIP_SELECT_HARDCORE  63000
#define GOSSIP_SELECT_BOOST     63001
#define GOSSIP_SELECT_BACK      63002

#define ITEM_CONTAINER          90000
#define ITEM_ARMOR              91000
#define ITEM_WEAPON             92000

struct ClassSpells
{
    int RaceId;
    int ClassId;
    int SpellId;
    int RequiredLevel;
    int RequiredSpellId;
    int RequiresQuest;
};

std::vector<ClassSpells> lsClassSpells;

enum SpellType
{
    CLASS = 0,
};

int warrior_proficiency[] =
{
    196,
    197,
    198,
    199,
    200,
    201,
    202,
    227,
    1180,
    5011,
    15590,
    3127, //parry
};
    

class npc_starter : public CreatureScript
{
public:
    npc_starter() : CreatureScript("npc_starter") {}

    void LearnProficiency(Player* player, int proficiency[])
    {

        for (int i = 0; i < proficiency[i]; ++i)
        {
            if (!player->HasSpell(proficiency[i]))
                player->learnSpell(proficiency[i]);
        }
    }

    void LoadClassSpells()
    {
        QueryResult result = WorldDatabase.Query("SELECT `race_id`, `class_id`, `spell_id`, `required_level`, `required_spell_id`, `requires_quest` FROM `mod_learnspells` WHERE `type`={} ORDER BY `id` ASC", SpellType::CLASS);

        if (!result)
        {
            LOG_INFO("server.loading", ">> Loaded 0 class spells. DB table `mod_learnspells` has no spells of type 0.");
            return;
        }

        lsClassSpells.clear();

        int i = 0;
        do
        {
            Field* fields = result->Fetch();

            lsClassSpells.push_back(ClassSpells());
            lsClassSpells[i].RaceId = fields[0].Get<int32>();
            lsClassSpells[i].ClassId = fields[1].Get<int32>();
            lsClassSpells[i].SpellId = fields[2].Get<int32>();
            lsClassSpells[i].RequiredLevel = fields[3].Get<int32>();
            lsClassSpells[i].RequiredSpellId = fields[4].Get<int32>();
            lsClassSpells[i].RequiresQuest = fields[5].Get<int32>();

            i++;
        } while (result->NextRow());

        LOG_INFO("server.loading", ">> Loaded {} class spells", i);
    }

    void LearnSpellsForNewLevel(Player* player)
    {
        for (auto& classSpell : lsClassSpells)
        {
            if (classSpell.RequiresQuest == 1)
                continue;

            if (classSpell.RaceId == -1 || classSpell.RaceId == player->getRace())
                if (classSpell.ClassId == player->getClass())
                    if (player->getLevel() >= classSpell.RequiredLevel)
                        if (classSpell.RequiredSpellId == -1 || player->HasSpell(classSpell.RequiredSpellId))
                            if (!player->HasSpell(classSpell.SpellId))
                                player->learnSpell(classSpell.SpellId);
        }
    }

    void AddShamanTotems(Player* player)
    {
        uint32 totems[4][3] = {
            {5175, 2, 4}, // Earth Totem, TotemCategory 2, Level 4
            {5176, 4, 10}, // Fire Totem, TotemCategory 4, Level 10
            {5177, 5, 20}, // Water Totem, TotemCategory 5, Level 20
            {5178, 3, 30} // Air Totem, TotemCategory 3, Level 30
        };

        for (int i = 0; i <= 3; i++)
        {
            if (player->getLevel() >= totems[i][2])
                if (!player->HasItemTotemCategory(totems[i][1]))
                    player->AddItem(totems[i][0], 1);
        }
    }

    void LearnSpells(Player* player, int player_class[])
    {
        LoadClassSpells();
        LearnSpellsForNewLevel(player);
        LearnProficiency(player, player_class);
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        ClearGossipMenuFor(player);

        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        std::string query = "SELECT counter FROM character_achievement_progress WHERE criteria = 111 AND guid = " + std::to_string(player->GetGUID().GetCounter());
        QueryResult result = CharacterDatabase.Query(query);

        bool isLevelOne = player->getLevel() == 1;
        bool hasMoney = player->GetMoney() != 0;
        bool isHardcore = player->HasAura(SPELL_AURA_HARDCORE);

        if (sConfigMgr->GetOption<bool>("HardcoreGossip.Enable", true))
        {
            if (isLevelOne && !hasMoney && !isHardcore)
                AddGossipItemFor(player, GOSSIP_HELLO_HARDCORE, 1, GOSSIP_SENDER_MAIN, GOSSIP_OPTION_HARDCORE);
                
        }

        if (sConfigMgr->GetOption<bool>("PlayerBoost.Enable", true))
        {
            if (isLevelOne)
                AddGossipItemFor(player, GOSSIP_HELLO_BOOST, 1, GOSSIP_SENDER_MAIN, GOSSIP_OPTION_BOOST);  
        }

        switch (player->getRace())
        {
        case RACE_HUMAN:
            SendGossipMenuFor(player, 50016, creature->GetGUID());
            break;
        case RACE_NIGHTELF:
            SendGossipMenuFor(player, 4936, creature->GetGUID());
            break;
        case RACE_DWARF:
            SendGossipMenuFor(player, 4937, creature->GetGUID());
            break;
        case RACE_GNOME:
            SendGossipMenuFor(player, 4937, creature->GetGUID());
            break;
        case RACE_DRAENEI:
            SendGossipMenuFor(player, 8667, creature->GetGUID());
            break;
        case RACE_ORC:
            SendGossipMenuFor(player, 3583, creature->GetGUID());
            break;
        case RACE_TROLL:
            SendGossipMenuFor(player, 3583, creature->GetGUID());
            break;
        case RACE_TAUREN:
            SendGossipMenuFor(player, 4935, creature->GetGUID());
            break;
        case RACE_BLOODELF:
            SendGossipMenuFor(player, 16703, creature->GetGUID());
            break;
        case RACE_UNDEAD_PLAYER:
            SendGossipMenuFor(player, 938, creature->GetGUID());
            break;
        }

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        if (sender != GOSSIP_SENDER_MAIN)
            return false;

        if (action == GOSSIP_BACK)
        {
            OnGossipHello(player, creature);
        }
        else if (action == GOSSIP_OPTION_HARDCORE)
        {
            ClearGossipMenuFor(player);
            AddGossipItemFor(player, GOSSIP_SELECT_HARDCORE, 1, GOSSIP_SENDER_MAIN, GOSSIP_OPTION_HARDCORE + 1);
            AddGossipItemFor(player, GOSSIP_SELECT_BACK, 1, GOSSIP_SENDER_MAIN, GOSSIP_BACK);
            SendGossipMenuFor(player, NPC_TEXT_HARDCORE, creature->GetGUID());
        }
        else if (action == GOSSIP_OPTION_HARDCORE + 1)
        {
            player->CastSpell(player, SPELL_AURA_HARDCORE, true);
            CloseGossipMenuFor(player);
        }
        else if (action == GOSSIP_OPTION_BOOST)
        {
            ClearGossipMenuFor(player);
            AddGossipItemFor(player, GOSSIP_SELECT_BOOST, 1, GOSSIP_SENDER_MAIN, GOSSIP_OPTION_BOOST + 1);
            AddGossipItemFor(player, GOSSIP_SELECT_BACK, 1, GOSSIP_SENDER_MAIN, GOSSIP_BACK);
            SendGossipMenuFor(player, NPC_TEXT_BOOST, creature->GetGUID());
        }
        else if (action == GOSSIP_OPTION_BOOST + 1)
        {
            player->GiveLevel(15);

            switch (player->getClass())
            {
            case CLASS_WARRIOR:
                player->AddItem(ITEM_CONTAINER, 1);
                LearnSpells(player, warrior_proficiency);
                break;
            case CLASS_PALADIN:
                player->AddItem(ITEM_CONTAINER + 1, 1);
                LearnSpells(player, warrior_proficiency);
                break;
            case CLASS_HUNTER:
                player->AddItem(ITEM_CONTAINER + 2, 1);
                LearnSpells(player, warrior_proficiency);
                break;
            case CLASS_ROGUE:
                player->AddItem(ITEM_CONTAINER + 3, 1);
                LearnSpells(player, warrior_proficiency);
                break;
            case CLASS_PRIEST:
                player->AddItem(ITEM_CONTAINER + 4, 1);
                LearnSpells(player, warrior_proficiency);
                break;
            case CLASS_SHAMAN:
                player->AddItem(ITEM_CONTAINER + 5, 1);
                LearnSpells(player, warrior_proficiency);
                AddShamanTotems(player);
                break;
            case CLASS_MAGE:
                player->AddItem(ITEM_CONTAINER + 6, 1);
                LearnSpells(player, warrior_proficiency);
                break;
            case CLASS_WARLOCK:
                player->AddItem(ITEM_CONTAINER + 7, 1);
                LearnSpells(player, warrior_proficiency);
                break;
            case CLASS_DRUID:
                player->AddItem(ITEM_CONTAINER + 8, 1);
                LearnSpells(player, warrior_proficiency);
                break;
            }

            CloseGossipMenuFor(player);
        }

        return true;
    }
};

void AddStarterNpcGossipScripts()
{
    new npc_starter();
}

