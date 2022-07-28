#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "AchievementMgr.h"
#include "DBUpdater.h"

class RetroportingDatabase : public DatabaseScript
{
public:
    RetroportingDatabase() : DatabaseScript("RetroportingDatabase") {}

    std::string path = "/modules/mod-retroporting/sql/";
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

void AddRetroportingScripts()
{
    new RetroportingDatabase();
}
