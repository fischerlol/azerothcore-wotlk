#include <ctime>
#include <chrono>
#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "WinzigWorld.h"
#include "Group.h"

class WinzigPlayer : public PlayerScript
{
public:
    WinzigPlayer() : PlayerScript("WinzigPlayer") { }

    void OnLogin(Player *player) override
    {
        if (sConfigMgr->GetOption<bool>("Winzig.Enable", false))
            ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00Winzig |rmodule.");

        uint32 guid = player->GetGUID().GetCounter();
        QueryResult result = CharacterDatabase.Query("SELECT `logout_time` FROM `characters` WHERE guid = '{}'", guid);
        std::chrono::system_clock::time_point logout_time_point(std::chrono::seconds(result->Fetch()->Get<uint32>()));
        time_t logout_time = std::chrono::system_clock::to_time_t(logout_time_point);
        time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        time_t reset = getReset();

        if (std::difftime(reset, logout_time) > 0 && std::difftime(now, reset) >= 0)
            sendDailyReward(player);
    }

    void OnLevelChanged(Player *player, uint8 /*oldlevel*/) override
    {
        player->AddItem(WinzigWorld::CustomCurrency, WinzigWorld::LevelReward);

        if (player->GetsRecruitAFriendBonus(true)) {
            if (Group* group = player->GetGroup()) {
                for (GroupReference* itr = group->GetFirstMember(); itr != nullptr; itr = itr->next()) {
                    Player* target = itr->GetSource();

                    if (!target)
                        continue;

                    if (target->GetSession()->GetAccountId() == player->GetSession()->GetRecruiterId())
                        target->AddItem(WinzigWorld::CustomCurrency, WinzigWorld::LevelReward);
                }
            }
        }
    }

    void OnCreatureKill(Player *killer, Creature *killed) override
    {
        if (killed && killed->IsDungeonBoss())
            giveCurrency(killer);
    }

    void OnCreatureKilledByPet(Player* owner, Creature* killed) override
    {
        if (killed && killed->IsDungeonBoss())
            giveCurrency(owner);
    }

private:
    void giveCurrency(Player *killer)
    {
        Map *map = killer->GetMap();
        if (map->GetEntry()->IsDungeon()) {
            Map::PlayerList const &players = map->GetPlayers();

            if (!players.IsEmpty()) {
                for (Map::PlayerList::const_iterator iter = players.begin(); iter != players.end(); ++iter) {
                    if (Player *player = iter->GetSource()) {
                        player->AddItem(WinzigWorld::CustomCurrency, WinzigWorld::KillReward);
                    }
                }
            }
        }
    }

    void sendDailyReward(Player *player)
    {
        ChatHandler(player->GetSession()).PSendSysMessage("Daily login reward!");
        player->AddItem(WinzigWorld::CustomCurrency, WinzigWorld::DailyReward);
    }

    time_t getReset()
    {
        time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        tm local = *localtime(&now);
        local.tm_sec = 0;
        local.tm_min = 0;
        local.tm_hour = 0;
        time_t reset = mktime(&local);
        return reset;
    }
};

void AddWinzigPlayerScripts()
{
    new WinzigPlayer();
}