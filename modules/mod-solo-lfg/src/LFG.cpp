#include <random>
#include "Common.h"
#include "Chat.h"
#include "ScriptMgr.h"
#include "LFGMgr.h"
#include "Player.h"
#include "DBUpdater.h"
#include "Group.h"
#include "ObjectAccessor.h"
#include "Spell.h"
#include "MapMgr.h"

class LFGPlayer : public PlayerScript
{
public:
    LFGPlayer() : PlayerScript("LFGPlayer") {}

    bool CanJoinLfg(Player* player, uint8 /*roles*/, lfg::LfgDungeonSet& dungeons, const std::string& /*comment*/) override
    {
        if (isRaidBrowser(player) || isRandomDungeon(dungeons) || isLFGGroup(player))
            return true;

        uint32 dungeonId = randomDungeon(dungeons);
        summonGroup(player, dungeonId);
        return false;
    }

    void OnQueueRandomDungeon(Player* player, uint32& rDungeonId) override 
    {
        lfg::LfgDungeonSet const& dungeons = sLFGMgr->GetDungeonsByRandom(rDungeonId);
        uint32 minlevel = 80;
        uint32 maxlevel = 1;

        for (lfg::LfgDungeonSet::const_iterator it = dungeons.begin(); it != dungeons.end(); it++)
        {
            lfg::LFGDungeonData const* dungeon = sLFGMgr->GetLFGDungeon(*it);
            
            if (minlevel > dungeon->minlevel)
                minlevel = dungeon->minlevel;

            if (maxlevel < dungeon->maxlevel)
                maxlevel = dungeon->maxlevel;
        }

        announceRandomDungeon(player, minlevel, maxlevel);
    }

private:
    bool isRaidBrowser(Player* player)
    {
        return sLFGMgr->GetState(player->GetGUID()) == lfg::LFG_STATE_RAIDBROWSER;
    }

    bool isRandomDungeon(lfg::LfgDungeonSet& dungeons)
    {
        for (lfg::LfgDungeonSet::const_iterator it = dungeons.begin(); it != dungeons.end(); it++)
            if (sLFGMgr->GetDungeonType(*it) == lfg::LFG_TYPE_RANDOM)
                return true;

        return false;
    }

    bool isLFGGroup(Player* player)
    {
        Group* group = player->GetGroup();

        if (!group)
            return false;
        
        return group->isLFGGroup();
    }

    uint32 randomDungeon(lfg::LfgDungeonSet& dungeons)
    {
        std::random_device seed;
        std::mt19937 engine(seed());
        std::uniform_int_distribution<int> choose(0, dungeons.size() - 1);
        lfg::LfgDungeonSet::const_iterator it = dungeons.begin();
        int index = choose(engine);

        for (int i = 0; i < index; i++)
            it++;
        
        return *it;
    }

    bool summonGroup(Player* player, uint32 id)
    {
        lfg::LFGDungeonData const* dungeon = sLFGMgr->GetLFGDungeon(id);

        if (!dungeon)
        {
            player->GetSession()->SendLfgTeleportError(uint8(lfg::LFG_TELEPORTERROR_INVALID_LOCATION));
            return false;
        }

        lfg::LfgTeleportError error = canTeleport(player);
        if (error == lfg::LFG_TELEPORTERROR_OK)
            error = summonGroupToDungeon(player, dungeon);

        if (error != lfg::LFG_TELEPORTERROR_OK)
        {
            player->GetSession()->SendLfgTeleportError(uint8(error));
            return false;
        }

        announceSpecificDungeon(player, dungeon);

        return true;
    }

    void getLeaderAndCount(Player* player, Player** leader, uint32* count)
    {
        *leader = player;
        *count = 1;
        Group* group = player->GetGroup();

        if (group)
        {
            *count = group->GetMembersCount();

            for (GroupReference* itr = group->GetFirstMember(); itr != nullptr; itr = itr->next())
                if (Player* member = itr->GetSource())
                    if (group->GetLeaderGUID() == member->GetGUID())
                        *leader = member;
        }
    }

    void announceRandomDungeon(Player* player, uint32 minlevel, uint32 maxlevel)
    {
        uint32 count;
        Player* leader;

        getLeaderAndCount(player, &leader, &count);
        announceRandomDungeonToSession(player->GetSession(), minlevel, maxlevel, count, leader->GetPlayerName());

        const SessionMap& sessions = sWorld->GetAllSessions();
        for (SessionMap::const_iterator itr = sessions.begin(); itr != sessions.end(); ++itr)
        {
            if (!itr->second || !itr->second->GetPlayer() || !itr->second->GetPlayer()->IsInWorld())
                continue;
            
            if (player == itr->second->GetPlayer())
                continue;
            
            announceRandomDungeonToSession(itr->second, minlevel, maxlevel, count, leader->GetPlayerName());
        }
    }

    void announceRandomDungeonToSession(WorldSession* session, uint32 minlevel, uint32 maxlevel, uint32 count, std::string leader)
    {
        if (count == 1)
            ChatHandler(session).PSendSysMessage(
                "|cff00CC00[LFG Queue Announcer]:|r Random Dungeon -- [%u-%u][%u/5] Join %s now!",
                minlevel, maxlevel, count, leader.c_str()
            );
        else if (count < 5)
            ChatHandler(session).PSendSysMessage(
                "|cff00CC00[LFG Queue Announcer]:|r Random Dungeon -- [%u-%u][%u/5] Ask %s to open it up!",
                minlevel, maxlevel, count, leader.c_str()
            );
    }

    void announceSpecificDungeon(Player* player, lfg::LFGDungeonData const* dungeon)
    {
        uint32 count;
        Player* leader;

        getLeaderAndCount(player, &leader, &count);

        announceSpecificDungeonToSession(player->GetSession(), dungeon->name, dungeon->minlevel, dungeon->maxlevel, count, leader->GetPlayerName());

        const SessionMap& sessions = sWorld->GetAllSessions();
        for (SessionMap::const_iterator itr = sessions.begin(); itr != sessions.end(); ++itr)
        {
            if (!itr->second || !itr->second->GetPlayer() || !itr->second->GetPlayer()->IsInWorld())
                continue;
            
            if (player == itr->second->GetPlayer())
                continue;

            announceSpecificDungeonToSession(itr->second, dungeon->name, dungeon->minlevel, dungeon->maxlevel, count, leader->GetPlayerName());
        }
    }

    void announceSpecificDungeonToSession(WorldSession* session, std::string name, uint32 minlevel, uint32 maxlevel, uint32 count, std::string leader)
    {
        if (count < 5)
            ChatHandler(session).PSendSysMessage(
                "|cff00CC00[LFG Queue Announcer]:|r %s -- [%u-%u][%u/5] Ask %s for an invite!",
                name.c_str(), minlevel, maxlevel, count, leader.c_str()
            );
        else
            ChatHandler(session).PSendSysMessage(
                "|cff00CC00[LFG Queue Announcer]:|r %s -- [%u-%u][%u/5]",
                name.c_str(), minlevel, maxlevel, count, leader.c_str()
            );
    }

    lfg::LfgTeleportError canTeleport(Player* player)
    {
        lfg::LfgTeleportError error = lfg::LFG_TELEPORTERROR_OK;

        if (!player->IsAlive())
        {
            error = lfg::LFG_TELEPORTERROR_PLAYER_DEAD;
        }
        else if (player->IsFalling() || player->HasUnitState(UNIT_STATE_JUMPING))
        {
            error = lfg::LFG_TELEPORTERROR_FALLING;
        }
        else if (player->IsMirrorTimerActive(FATIGUE_TIMER))
        {
            error = lfg::LFG_TELEPORTERROR_FATIGUE;
        }
        else if (player->GetVehicle())
        {
            error = lfg::LFG_TELEPORTERROR_IN_VEHICLE;
        }
        else if (player->GetCharmGUID() || player->IsInCombat())
        {
            error = lfg::LFG_TELEPORTERROR_COMBAT;
        }

        return error;
    }

    lfg::LfgTeleportError summonGroupToDungeon(Player* leader, lfg::LFGDungeonData const* dungeon)
    {
        uint32 mapid = dungeon->map;
        float x = dungeon->x;
        float y = dungeon->y;
        float z = dungeon->z;
        float orientation = dungeon->o;
        uint32 zoneid = sMapMgr->GetZoneId(leader->GetPhaseMask(), mapid, x, y, z);

        if (!leader->GetMap()->IsDungeon() || leader->GetEntryPoint().GetMapId() == MAPID_INVALID)
            leader->SetEntryPoint();

        lfg::LfgTeleportError error = lfg::LFG_TELEPORTERROR_OK;

        if (leader->GetMapId() != mapid)
            if (!leader->TeleportTo(mapid, x, y, z, orientation, 0, nullptr, mapid == leader->GetMapId()))
                error = lfg::LFG_TELEPORTERROR_INVALID_LOCATION;

        Group* group = leader->GetGroup();

        if (!group)
            return error;

        ObjectGuid leaderid = group->GetLeaderGUID();

        for (GroupReference* itr = group->GetFirstMember(); itr != nullptr; itr = itr->next())
        {
            if (Player* player = itr->GetSource())
            {
                if (player->GetGUID() == leaderid || player->GetMapId() == mapid)
                    continue;

                if (!player->GetMap()->IsDungeon() || player->GetEntryPoint().GetMapId() == MAPID_INVALID)
                    player->SetEntryPoint();

                player->SetSummonPoint(mapid, x, y, z);

                WorldPacket data(SMSG_SUMMON_REQUEST, 8 + 4 + 4);
                data << leaderid;
                data << zoneid;
                data << uint32(MAX_PLAYER_SUMMON_DELAY * IN_MILLISECONDS);
                player->GetSession()->SendPacket(&data);
            }
        }

        return lfg::LFG_TELEPORTERROR_OK;
    }
};

class LFGGroup : public GroupScript
{
public:
    LFGGroup() : GroupScript("LFGGroup") {}

    void OnCreate(Group* group, Player* leader)
    {
        if (group->isLFGGroup())
            ChatHandler(leader->GetSession()).SendSysMessage("Group created!");
    }

    void OnAddMember(Group* group, ObjectGuid guid)
    {
        if (group->isLFGGroup())
        {
            Player* player = ObjectAccessor::FindConnectedPlayer(guid);
            ChatHandler(player->GetSession()).SendSysMessage("Member added!");
        }
    }
};

constexpr uint32 SPELL_HEARTHSTONE = 8690;

class item_hearthstone : public ItemScript
{
public:
    item_hearthstone() : ItemScript("item_hearthstone") {}

    bool OnUse(Player* player, Item* /*hearthstone*/, SpellCastTargets const& /*targets*/) override
    {
        bool isDungeon = player->GetMap()->IsDungeon();
        bool isEntryPointValid = player->GetEntryPoint().GetMapId() != MAPID_INVALID;

        if (!isDungeon || !isEntryPointValid)
        {
            player->CastSpell(player, SPELL_HEARTHSTONE, false);
            return true;
        }

        return player->TeleportToEntryPoint();
    }
};

class LFGDatabase : public DatabaseScript
{
public:
    LFGDatabase() : DatabaseScript("LFGDatabase") {}

    std::string path = "/modules/mod-lfg/sql/";
    void OnAfterDatabasesLoaded(uint32 updateFlags) override
    {
        if (DBUpdater<LoginDatabaseConnection>::IsEnabled(updateFlags))
        {
            std::vector<std::string> directories;
            directories.push_back(path + "auth");
            DBUpdater<LoginDatabaseConnection>::Update(LoginDatabase, &directories);
        }

        if (DBUpdater<CharacterDatabaseConnection>::IsEnabled(updateFlags))
        {
            std::vector<std::string> directories;
            directories.push_back(path + "characters");
            DBUpdater<CharacterDatabaseConnection>::Update(CharacterDatabase, &directories);
        }

        if (DBUpdater<WorldDatabaseConnection>::IsEnabled(updateFlags))
        {
            std::vector<std::string> directories;
            directories.push_back(path + "world");
            DBUpdater<WorldDatabaseConnection>::Update(WorldDatabase, &directories);
        }
    }
};

void AddLFGScripts()
{
    new LFGPlayer();
    new LFGGroup();
    new item_hearthstone();
    new LFGDatabase();
}