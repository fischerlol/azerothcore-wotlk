#include "Chat.h"
#include "Config.h"
#include "Language.h"
#include "MapMgr.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptMgrMacros.h"
#include "Unit.h"
#include "Formulas.h"
#include "Group.h"
#include "DBUpdater.h"
#include "ItemTemplate.h"
#include "Spell.h"
#include "Pet.h"
#include <algorithm>
#include <cstdlib>
#include <vector>

// DPS count as 1 offensive unit. Tanks and healers count as 1 defensive unit.

// 5 man: 1 tank, 3 dps, 1 healer = 3 offensive units and 2 defensive units.
const float Offence5M = 1 / 3.0f, Defence5M = 1 / 2.0f;

enum Spells
{
    SPELL_LUCIFRON_CURSE = 19703,
    SPELL_GEHENNAS_CURSE = 19716,
    SPELL_IGNITE_MANA = 19659,
    SPELL_SHAZZRAH_CURSE = 19713,
    SPELL_BURNING_ADRENALINE = 18173,
    SPELL_BROOD_AFFLICTION_BLACK = 23154,
    SPELL_BROOD_AFFLICTION_BLUE = 23153,
    SPELL_BROOD_AFFLICTION_BRONZE = 23170,
    SPELL_BROOD_AFFLICTION_GREEN = 23169,
    SPELL_BROOD_AFFLICTION_RED = 23155,
    SPELL_ONYXIA_SCALE_CLOAK = 22683
};

class PlayerSettingsCreatureInfo : public DataMap::Base
{
public:
    PlayerSettingsCreatureInfo() {}
    uint32 nplayers = 0;
    // This is used to detect creatures that update their entry.
    uint32 entry = 0;
    float HealthMultiplier = 1;
};

class PlayerSettingsMapInfo : public DataMap::Base
{
public:
    PlayerSettingsMapInfo() {}
    uint32 nplayers = 0;
    uint32 veto = 0;
    std::map<uint32, float> honor;
    std::map<uint32, bool> rewarded;
    std::map<uint32, uint32> rdf;
};

static bool enabled;
static float experienceMultiplier, goldMultiplier, honorMultiplier;

class PlayerSettingsWorldScript : public WorldScript
{
public:
    PlayerSettingsWorldScript() : WorldScript("PlayerSettingsWorldScript") {}

    void OnBeforeConfigLoad(bool /*reload*/) override
    {
        SetInitialWorldSettings();
    }

    void SetInitialWorldSettings()
    {
        enabled = sConfigMgr->GetOption<bool>("PlayerSettings.Enable", 1);
        experienceMultiplier = sConfigMgr->GetOption<float>("PlayerSettings.Experience", 0.1);
        goldMultiplier = sConfigMgr->GetOption<float>("PlayerSettings.Gold", 0.1);
        honorMultiplier = sConfigMgr->GetOption<float>("PlayerSettings.Honor", 0.1);
    }
};

class PlayerSettingsPlayerScript : public PlayerScript
{
public:
    PlayerSettingsPlayerScript() : PlayerScript("PlayerSettingsPlayer") {}

    void OnGiveXP(Player *player, uint32 &amount, Unit *victim) override
    {
        if (victim)
        {
            Map *map = player->GetMap();

            if (map->IsDungeon() && !map->IsBattlegroundOrArena())
            {
                uint32 maxPlayers = ((InstanceMap *)sMapMgr->FindMap(map->GetId(), map->GetInstanceId()))->GetMaxPlayers();
                PlayerSettingsMapInfo *mapInfo = map->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");
                uint32 nplayers = std::min(5u, std::max(mapInfo->nplayers, mapInfo->veto));

                amount = amount * 2 * mapInfo->nplayers / maxPlayers * (1 + experienceMultiplier * (nplayers - 1));
            }
        }
    }

    void OnMoneyChanged(Player *player, int32 &amount) override
    {
        Map *map = player->GetMap();

        if (map->IsDungeon() && amount > 0)
        {
            uint32 maxPlayers = ((InstanceMap *)sMapMgr->FindMap(map->GetId(), map->GetInstanceId()))->GetMaxPlayers();
            PlayerSettingsMapInfo *mapInfo = map->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");

            amount = amount * mapInfo->nplayers / maxPlayers * (1 + goldMultiplier * (mapInfo->nplayers - 1));
        }
    }

    void OnCreatureKill(Player *player, Creature *killed) override
    {
        if (!killed)
            return;

        if (killed->GetCreatureTemplate()->type_flags & CREATURE_TYPE_FLAG_BOSS_MOB)
            return;

        if (killed->IsDungeonBoss())
            rewardHonor(player, killed);
    }

    void OnCreatureKilledByPet(Player *owner, Creature *killed) override
    {
        if (!killed)
            return;

        if (killed->GetCreatureTemplate()->type_flags & CREATURE_TYPE_FLAG_BOSS_MOB)
            return;

        if (killed->IsDungeonBoss())
            rewardHonor(owner, killed);
    }

    void OnPlayerCompleteQuest(Player *player, Quest const *quest) override
    {
        uint32 guid = player->GetGUID().GetCounter();
        Map *map = player->GetMap();
        PlayerSettingsMapInfo *mapInfo = map->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");

        switch (quest->GetQuestId())
        {
        case 24881:
        case 24882:
        case 24883:
        case 24884:
        case 24885:
        case 24886:
        case 24888:
        case 24922:
        case 24790:
        case 24788:
            mapInfo->rdf[guid] = 2;
            return;
        case 24889:
        case 24890:
        case 24891:
        case 24892:
        case 24893:
        case 24894:
        case 24896:
        case 24923:
        case 24791:
        case 24789:
            mapInfo->rdf[guid] = 1;
            return;
        }
    }

    void OnPlayerLeaveCombat(Player* player) override
    {
        Aura* lucifron = player->GetAura(SPELL_LUCIFRON_CURSE);
        
        if (lucifron)
            player->RemoveAura(lucifron);

        Aura* gehennas = player->GetAura(SPELL_GEHENNAS_CURSE);

        if (gehennas)
            player->RemoveAura(gehennas);

        Aura* ignite = player->GetAura(SPELL_IGNITE_MANA);

        if (ignite)
            player->RemoveAura(ignite);

        Aura* shazzrah = player->GetAura(SPELL_SHAZZRAH_CURSE);

        if (shazzrah)
            player->RemoveAura(shazzrah);

        Aura* vaelastrasz = player->GetAura(SPELL_BURNING_ADRENALINE);

        if (vaelastrasz)
            player->RemoveAura(vaelastrasz);

        Aura* black = player->GetAura(SPELL_BROOD_AFFLICTION_BLACK);

        if (black)
            player->RemoveAura(black);
        
        Aura* blue = player->GetAura(SPELL_BROOD_AFFLICTION_BLUE);

        if (blue)
            player->RemoveAura(blue);

        Aura* bronze = player->GetAura(SPELL_BROOD_AFFLICTION_BRONZE);

        if (bronze)
            player->RemoveAura(bronze);
        
        Aura* green = player->GetAura(SPELL_BROOD_AFFLICTION_GREEN);

        if (green)
            player->RemoveAura(green);
        
        Aura* red = player->GetAura(SPELL_BROOD_AFFLICTION_RED);

        if (red)
            player->RemoveAura(red);
    }

    void OnLootItem(Player* player, Item* item, uint32 /*count*/, ObjectGuid /*lootguid*/) override
    {
        ItemTemplate const *proto = sObjectMgr->GetItemTemplate(item->GetEntry());

        if (!proto->InventoryType)
            return;

        uint32 ilvl = characterLevelToItemLevel(player->getLevel());
        CharacterDatabase.Execute("INSERT INTO item_level (player, item, ilvl) VALUES ({}, {}, {})", player->GetGUID().GetCounter(), proto->ItemId, ilvl);
    }

    void OnCreateItem(Player* player, Item* item, uint32 /*count*/) override
    {
        ItemTemplate const *proto = sObjectMgr->GetItemTemplate(item->GetEntry());

        if (!proto->InventoryType)
            return;

        uint32 ilvl = characterLevelToItemLevel(player->getLevel());
        CharacterDatabase.Execute("INSERT INTO item_level (player, item, ilvl) VALUES ({}, {}, {})", player->GetGUID().GetCounter(), proto->ItemId, ilvl);
    }

    void OnQuestRewardItem(Player* player, Item* item, uint32 /*count*/) override
    {
        ItemTemplate const *proto = sObjectMgr->GetItemTemplate(item->GetEntry());

        if (!proto->InventoryType)
            return;

        uint32 ilvl = characterLevelToItemLevel(player->getLevel());
        CharacterDatabase.Execute("INSERT INTO item_level (player, item, ilvl) VALUES ({}, {}, {})", player->GetGUID().GetCounter(), proto->ItemId, ilvl);
    }

    void OnCustomScalingStatValueBefore(Player* player, ItemTemplate const* proto, uint8 slot, bool apply, uint32& CustomScalingStatValue) override
    {
        uint32 ilvl = getItemLevel(player, proto);

        if (!ilvl || ilvl < proto->ItemLevel)
            return;

        float multiplier = getStatMultiplier(slot, ilvl, proto);
        uint32 armor = handleArmor(player, proto, slot, ilvl, multiplier, apply);
        _Damage damages[MAX_ITEM_PROTO_DAMAGES];
        handleWeaponDamage(player, proto, damages, slot, ilvl, apply);

        applyFeralAPBonus(player, proto, multiplier, apply);
        updatePlayerCache(player, proto, multiplier, armor, damages);
        CustomScalingStatValue = 10000 * multiplier;
    }

    void OnCustomScalingStatValue(Player* /*player*/, ItemTemplate const* /*proto*/, uint32& /*statType*/, int32& val, uint8 /*itemProtoStatNumber*/, uint32 ScalingStatValue, ScalingStatValuesEntry const* /*ssv*/) override
    {
        float multiplier = ScalingStatValue / 10000.0f;
        val *= multiplier;
    }

private:
    static float characterLevelToItemLevel(uint32 level)
    {
        if (level < 61)
            return 1 * level + 4.93;

        if (level < 71)
            return 3 * level + -95;

        return 4 * level + -133;
    }

    static float primaryBudget(uint32 ilvl)
    {
        return 1.33 + 0.469 * ilvl + 1.05E-03 * pow(ilvl, 2);
    }

    static float secondaryBudget(uint32 ilvl)
    {
        return 0.845 + 0.36 * ilvl + 7.23E-04 * pow(ilvl, 2);
    }

    static float tertiaryBudget(uint32 ilvl)
    {
        return 0.597 + 0.27 * ilvl + 5.42E-04 * pow(ilvl, 2);
    }

    static float trinketBudget(uint32 ilvl)
    {
        return 0.845 + 0.36 * ilvl + 7.23E-04 * pow(ilvl, 2);
    }

    static float weaponBudget(uint32 ilvl)
    {
        return 0.523 + 0.2 * ilvl + 4.56E-04 * pow(ilvl, 2);
    }

    static float rangedBudget(uint32 ilvl)
    {
        return 0.435 + 0.149 * ilvl + 3.16E-04 * pow(ilvl, 2);
    }

    static float weaponDPS1H(uint32 ilvl)
    {
        return 0.631 * ilvl + 0.74;
    }

    static float weaponDPS2H(uint32 ilvl)
    {
        return 0.819 * ilvl + 1.13;
    }

    static float spellcasterDPS1H(uint32 ilvl)
    {
        return 0.445 * ilvl + 7.08;
    }

    static float spellcasterDPS2H(uint32 ilvl)
    {
        return 0.577 * ilvl + 9.41;
    }
    
    static float rangedDPS(uint32 ilvl)
    {
        return 0.561 * ilvl + 3.02;
    }

    static float wandDPS(uint32 ilvl)
    {
        return 1.2 * ilvl + -7.4;
    }

    void applyFeralAPBonus(Player* player, ItemTemplate const* proto, float multiplier, bool apply)
    {
        if (player->getClass() == CLASS_DRUID)
        {
            int32 dpsMod = 0;
            int32 feralBonus = 0;

            feralBonus += proto->getFeralBonus(dpsMod) * multiplier;

            if (feralBonus)
                player->ApplyFeralAPBonus(feralBonus, apply);
        }

    }

    uint32 getItemLevel(Player* player, ItemTemplate const* proto)
    {
        std::string query = "SELECT ilvl FROM item_level WHERE player = "
            + std::to_string(player->GetGUID().GetCounter())
            + " AND item = " + std::to_string(proto->ItemId)
            + " ORDER BY ilvl DESC LIMIT 1";
        QueryResult result = CharacterDatabase.Query(query);

        if (result)
            return (*result)[0].Get<uint32>();

        return 0;
    }

    static float getStatMultiplier(uint32 slot, uint32 ilvl, ItemTemplate const* proto)
    {
        switch (slot)
        {
            // Primary budget
            case EQUIPMENT_SLOT_HEAD:
            case EQUIPMENT_SLOT_BODY:
            case EQUIPMENT_SLOT_CHEST:
            case EQUIPMENT_SLOT_LEGS:
                return primaryBudget(ilvl) / primaryBudget(proto->ItemLevel);
            // Secondary budget
            case EQUIPMENT_SLOT_SHOULDERS:
            case EQUIPMENT_SLOT_FEET:
            case EQUIPMENT_SLOT_HANDS:
            case EQUIPMENT_SLOT_WAIST:
            case EQUIPMENT_SLOT_TRINKET1:
            case EQUIPMENT_SLOT_TRINKET2:
                return secondaryBudget(ilvl) / secondaryBudget(proto->ItemLevel);
            // Tertiary budget
            case EQUIPMENT_SLOT_WRISTS:
            case EQUIPMENT_SLOT_FINGER1:
            case EQUIPMENT_SLOT_FINGER2:
            case EQUIPMENT_SLOT_BACK:
                return tertiaryBudget(ilvl) / tertiaryBudget(proto->ItemLevel);
            // Weapon budget
            case EQUIPMENT_SLOT_MAINHAND: // 2H and 1H
            case EQUIPMENT_SLOT_OFFHAND:
                switch (proto->SubClass)
                {
                    case ITEM_SUBCLASS_WEAPON_AXE:
                    case ITEM_SUBCLASS_WEAPON_MACE:
                    case ITEM_SUBCLASS_WEAPON_SWORD:
                    case ITEM_SUBCLASS_WEAPON_FIST:
                    case ITEM_SUBCLASS_WEAPON_DAGGER:
                        return weaponBudget(ilvl) / weaponBudget(proto->ItemLevel);
                    case ITEM_SUBCLASS_WEAPON_AXE2:
                    case ITEM_SUBCLASS_WEAPON_MACE2:
                    case ITEM_SUBCLASS_WEAPON_POLEARM:
                    case ITEM_SUBCLASS_WEAPON_SWORD2:
                    case ITEM_SUBCLASS_WEAPON_STAFF:
                        return primaryBudget(ilvl) / primaryBudget(proto->ItemLevel);
                    default:
                        return 1;
                }
            // Ranged (bows, guns, and crossbows) and tertiary budget (thrown and wands)
            case EQUIPMENT_SLOT_RANGED:
                switch (proto->SubClass)
                {
                    case ITEM_SUBCLASS_WEAPON_BOW:
                    case ITEM_SUBCLASS_WEAPON_GUN:
                    case ITEM_SUBCLASS_WEAPON_CROSSBOW:
                        return rangedBudget(ilvl) / rangedBudget(proto->ItemLevel);
                    case ITEM_SUBCLASS_WEAPON_THROWN:
                    case ITEM_SUBCLASS_WEAPON_WAND:
                        return tertiaryBudget(ilvl) / tertiaryBudget(proto->ItemLevel);
                    default:
                        return 1;
                }
            default:
                return 1;
        }
    }

    static uint32 handleArmor(Player* player, ItemTemplate const* proto, uint32 slot, uint32 ilvl, float multiplier, bool apply)
    {
        uint32 armor = proto->Armor;

        if (armor)
        {
            UnitModifierType modType = TOTAL_VALUE;

            if (proto->Class == ITEM_CLASS_ARMOR)
            {
                switch (proto->SubClass)
                {
                    case ITEM_SUBCLASS_ARMOR_CLOTH:
                    case ITEM_SUBCLASS_ARMOR_LEATHER:
                    case ITEM_SUBCLASS_ARMOR_MAIL:
                    case ITEM_SUBCLASS_ARMOR_PLATE:
                    // Have to find scaling values for shield armor.
                    case ITEM_SUBCLASS_ARMOR_SHIELD:
                        modType = BASE_VALUE;
                        break;
                }
            }

            switch (slot)
            {
                // Primary armor
                case EQUIPMENT_SLOT_HEAD:
                case EQUIPMENT_SLOT_BODY:
                case EQUIPMENT_SLOT_CHEST:
                case EQUIPMENT_SLOT_LEGS:
                    switch (proto->SubClass)
                    {
                        case ITEM_SUBCLASS_ARMOR_CLOTH:
                            armor = 1.47 * ilvl + 1.76;
                            break;
                        case ITEM_SUBCLASS_ARMOR_LEATHER:
                            armor = 2.64 * ilvl + 21.1;
                            break;
                        case ITEM_SUBCLASS_ARMOR_MAIL:
                            armor = 6.62 * ilvl + -61.2;
                            break;
                        case ITEM_SUBCLASS_ARMOR_PLATE:
                            armor = 11.7 * ilvl + -86.6;
                            break;
                    }

                    break;
                // Secondary armor
                case EQUIPMENT_SLOT_SHOULDERS:
                case EQUIPMENT_SLOT_FEET:
                case EQUIPMENT_SLOT_HANDS:
                case EQUIPMENT_SLOT_WAIST:
                case EQUIPMENT_SLOT_WRISTS:
                    switch (proto->SubClass)
                    {
                        case ITEM_SUBCLASS_ARMOR_CLOTH:
                            armor = 1.1 * ilvl + 1.32;
                            break;
                        case ITEM_SUBCLASS_ARMOR_LEATHER:
                            armor = 1.98 * ilvl + 15.8;
                            break;
                        case ITEM_SUBCLASS_ARMOR_MAIL:
                            armor = 4.97 * ilvl + -45.9;
                            break;
                        case ITEM_SUBCLASS_ARMOR_PLATE:
                            armor = 8.74 * ilvl + -64.9;
                            break;
                    }

                    break;
                // Tertiary budget
                case EQUIPMENT_SLOT_BACK:
                    armor = 0.735 * ilvl + 0.872;
                    break;
                default:
                    break;
            }

            player->HandleStatModifier(UNIT_MOD_ARMOR, modType, float(armor), apply);
        }

        if (proto->ArmorDamageModifier > 0)
            player->HandleStatModifier(UNIT_MOD_ARMOR, TOTAL_VALUE, float(proto->ArmorDamageModifier) * multiplier, apply);

        if (proto->Block)
            player->HandleBaseModValue(SHIELD_BLOCK_VALUE, FLAT_MOD, float(proto->Block) * multiplier, apply);

        if (proto->HolyRes)
            player->HandleStatModifier(UNIT_MOD_RESISTANCE_HOLY, BASE_VALUE, float(proto->HolyRes) * multiplier, apply);

        if (proto->FireRes)
            player->HandleStatModifier(UNIT_MOD_RESISTANCE_FIRE, BASE_VALUE, float(proto->FireRes) * multiplier, apply);

        if (proto->NatureRes)
            player->HandleStatModifier(UNIT_MOD_RESISTANCE_NATURE, BASE_VALUE, float(proto->NatureRes) * multiplier, apply);

        if (proto->FrostRes)
            player->HandleStatModifier(UNIT_MOD_RESISTANCE_FROST, BASE_VALUE, float(proto->FrostRes) * multiplier, apply);

        if (proto->ShadowRes)
            player->HandleStatModifier(UNIT_MOD_RESISTANCE_SHADOW, BASE_VALUE, float(proto->ShadowRes) * multiplier, apply);

        if (proto->ArcaneRes)
            player->HandleStatModifier(UNIT_MOD_RESISTANCE_ARCANE, BASE_VALUE, float(proto->ArcaneRes) * multiplier, apply);
        
        return armor;
    }

    void handleWeaponDamage(Player* player, ItemTemplate const* proto, _Damage* damages, uint32 slot, uint32 ilvl, bool apply)
    {
        WeaponAttackType attType = BASE_ATTACK;

        for (int i = 0; i < MAX_ITEM_PROTO_DAMAGES; i++)
            damages[i] = proto->Damage[i];


        if (slot == EQUIPMENT_SLOT_RANGED && isInventoryTypeRanged(proto))
            attType = RANGED_ATTACK;
        else if (slot == EQUIPMENT_SLOT_OFFHAND)
            attType = OFF_ATTACK;

        if (player->CanUseAttackType(attType))
        {
            float dps = 0.0f;
            float protoDps = 0.0f;

            switch (slot)
            {
                case EQUIPMENT_SLOT_MAINHAND: // 2H and 1H
                case EQUIPMENT_SLOT_OFFHAND:
                    switch (proto->SubClass)
                    {
                        case ITEM_SUBCLASS_WEAPON_AXE:
                        case ITEM_SUBCLASS_WEAPON_MACE:
                        case ITEM_SUBCLASS_WEAPON_SWORD:
                        case ITEM_SUBCLASS_WEAPON_FIST:
                        case ITEM_SUBCLASS_WEAPON_DAGGER:
                            if (proto->HasSpellPowerStat())
                            {
                                dps = spellcasterDPS1H(ilvl);
                                protoDps = spellcasterDPS1H(proto->ItemLevel);
                            }
                            else
                            {
                                dps = weaponDPS1H(ilvl);
                                protoDps = weaponDPS1H(proto->ItemLevel);
                            }

                            break;
                        case ITEM_SUBCLASS_WEAPON_AXE2:
                        case ITEM_SUBCLASS_WEAPON_MACE2:
                        case ITEM_SUBCLASS_WEAPON_POLEARM:
                        case ITEM_SUBCLASS_WEAPON_SWORD2:
                        case ITEM_SUBCLASS_WEAPON_STAFF:
                            if (proto->HasSpellPowerStat())
                            {
                                dps = spellcasterDPS2H(ilvl);
                                protoDps = spellcasterDPS2H(proto->ItemLevel);
                            }
                            else
                            {
                                dps = weaponDPS2H(ilvl);
                                protoDps = weaponDPS2H(proto->ItemLevel);
                            }

                            break;
                    }
                    break;
                // Ranged (bows, guns, and crossbows) and tertiary budget (thrown and wands)
                case EQUIPMENT_SLOT_RANGED:
                    switch (proto->SubClass)
                    {
                        case ITEM_SUBCLASS_WEAPON_BOW:
                        case ITEM_SUBCLASS_WEAPON_GUN:
                        case ITEM_SUBCLASS_WEAPON_CROSSBOW:
                        case ITEM_SUBCLASS_WEAPON_THROWN:
                            dps = rangedDPS(ilvl);
                            protoDps = rangedDPS(proto->ItemLevel);
                            break;
                        case ITEM_SUBCLASS_WEAPON_WAND:
                            dps = wandDPS(ilvl);
                            protoDps = wandDPS(proto->ItemLevel);
                            break;
                    }

                    break;
                default:
                    break;
            }

            float average = dps * proto->Delay / 1000.0f;
            float minDamage = 0.7f * average;
            float maxDamage = 1.3f * average;
            float damage = 0.0;

            if (minDamage > 0)
            {
                damage = apply ? minDamage : BASE_MINDAMAGE;
                player->SetBaseWeaponDamage(attType, MINDAMAGE, damage);
            }

            if (maxDamage  > 0)
            {
                damage = apply ? maxDamage : BASE_MAXDAMAGE;
                player->SetBaseWeaponDamage(attType, MAXDAMAGE, damage);
            }

            if (proto->Delay && !player->IsInFeralForm())
            {
                if (slot == EQUIPMENT_SLOT_RANGED)
                    player->SetAttackTime(RANGED_ATTACK, apply ? proto->Delay : BASE_ATTACK_TIME);
                else if (slot == EQUIPMENT_SLOT_MAINHAND)
                    player->SetAttackTime(BASE_ATTACK, apply ? proto->Delay : BASE_ATTACK_TIME);
                else if (slot == EQUIPMENT_SLOT_OFFHAND)
                    player->SetAttackTime(OFF_ATTACK, apply ? proto->Delay : BASE_ATTACK_TIME);
            }

            if (player->CanModifyStats() && (damage || proto->Delay) && !player->IsInFeralForm())
                player->UpdateDamagePhysical(attType);

            damages[0].DamageMin = minDamage;
            damages[0].DamageMax = maxDamage;
        }
    }

    static bool isInventoryTypeRanged(ItemTemplate const* proto)
    {
        return proto->InventoryType == INVTYPE_RANGED || proto->InventoryType == INVTYPE_THROWN || proto->InventoryType == INVTYPE_RANGEDRIGHT;
    }

    void updatePlayerCache(Player* player, ItemTemplate const* proto, float statMultiplier, uint32 armor, _Damage damages[MAX_ITEM_PROTO_DAMAGES])
    {
        std::string Name = proto->Name1;
        std::string Description = proto->Description;

        int loc_idx = player->GetSession()->GetSessionDbLocaleIndex();

        if (loc_idx >= 0)
        {
            if (ItemLocale const* il = sObjectMgr->GetItemLocale(proto->ItemId))
            {
                ObjectMgr::GetLocaleString(il->Name, loc_idx, Name);
                ObjectMgr::GetLocaleString(il->Description, loc_idx, Description);
            }
        }

        // guess size
        WorldPacket data(SMSG_ITEM_QUERY_SINGLE_RESPONSE, 600);

        data << proto->ItemId;
        data << proto->Class;
        data << proto->SubClass;
        data << proto->SoundOverrideSubclass;
        data << Name;
        data << uint8(0x00);                                //proto->Name2; // blizz not send name there, just uint8(0x00); <-- \0 = empty string = empty name...
        data << uint8(0x00);                                //proto->Name3; // blizz not send name there, just uint8(0x00);
        data << uint8(0x00);                                //proto->Name4; // blizz not send name there, just uint8(0x00);
        data << proto->DisplayInfoID;
        data << proto->Quality;
        data << proto->Flags;
        data << proto->Flags2;
        data << proto->BuyPrice;
        data << proto->SellPrice;
        data << proto->InventoryType;
        data << proto->AllowableClass;
        data << proto->AllowableRace;
        data << proto->ItemLevel;
        data << proto->RequiredLevel;
        data << proto->RequiredSkill;
        data << proto->RequiredSkillRank;
        data << proto->RequiredSpell;
        data << proto->RequiredHonorRank;
        data << proto->RequiredCityRank;
        data << proto->RequiredReputationFaction;
        data << proto->RequiredReputationRank;
        data << int32(proto->MaxCount);
        data << int32(proto->Stackable);
        data << proto->ContainerSlots;
        data << proto->StatsCount;                         // item stats count

        for (uint32 i = 0; i < proto->StatsCount; ++i)
        {
            data << proto->ItemStat[i].ItemStatType;
            data << uint32(proto->ItemStat[i].ItemStatValue * statMultiplier);
        }

        data << proto->ScalingStatDistribution;            // scaling stats distribution
        data << proto->ScalingStatValue;                   // some kind of flags used to determine stat values column

        for (int i = 0; i < MAX_ITEM_PROTO_DAMAGES; ++i)
        {
            data << damages[i].DamageMin;
            data << damages[i].DamageMax;
            data << damages[i].DamageType;
        }

        // resistances (7)
        data << armor;
        data << uint32(proto->HolyRes * statMultiplier);
        data << uint32(proto->FireRes * statMultiplier);
        data << uint32(proto->NatureRes * statMultiplier);
        data << uint32(proto->FrostRes * statMultiplier);
        data << uint32(proto->ShadowRes * statMultiplier);
        data << uint32(proto->ArcaneRes * statMultiplier);

        data << proto->Delay;
        data << proto->AmmoType;
        data << proto->RangedModRange;

        for (int s = 0; s < MAX_ITEM_PROTO_SPELLS; ++s)
        {
            // send DBC data for cooldowns in same way as it used in Spell::SendSpellCooldown
            // use `item_template` or if not set then only use spell cooldowns
            SpellInfo const* spell = sSpellMgr->GetSpellInfo(proto->Spells[s].SpellId);

            if (spell)
            {
                bool db_data = proto->Spells[s].SpellCooldown >= 0 || proto->Spells[s].SpellCategoryCooldown >= 0;

                data << proto->Spells[s].SpellId;
                data << proto->Spells[s].SpellTrigger;
                data << int32(proto->Spells[s].SpellCharges);

                if (db_data)
                {
                    data << uint32(proto->Spells[s].SpellCooldown);
                    data << uint32(proto->Spells[s].SpellCategory);
                    data << uint32(proto->Spells[s].SpellCategoryCooldown);
                }
                else
                {
                    data << uint32(spell->RecoveryTime);
                    data << uint32(spell->GetCategory());
                    data << uint32(spell->CategoryRecoveryTime);
                }
            }
            else
            {
                data << uint32(0);
                data << uint32(0);
                data << uint32(0);
                data << uint32(-1);
                data << uint32(0);
                data << uint32(-1);
            }
        }

        data << proto->Bonding;
        data << Description;
        data << proto->PageText;
        data << proto->LanguageID;
        data << proto->PageMaterial;
        data << proto->StartQuest;
        data << proto->LockID;
        data << int32(proto->Material);
        data << proto->Sheath;
        data << proto->RandomProperty;
        data << proto->RandomSuffix;
        data << proto->Block;
        data << proto->ItemSet;
        data << proto->MaxDurability;
        data << proto->Area;
        data << proto->Map;                                // Added in 1.12.x & 2.0.1 client branch
        data << proto->BagFamily;
        data << proto->TotemCategory;

        for (int s = 0; s < MAX_ITEM_PROTO_SOCKETS; ++s)
        {
            data << proto->Socket[s].Color;
            data << proto->Socket[s].Content;
        }

        data << proto->socketBonus;
        data << proto->GemProperties;
        data << proto->RequiredDisenchantSkill;
        data << proto->ArmorDamageModifier;
        data << proto->Duration;                           // added in 2.4.2.8209, duration (seconds)
        data << proto->ItemLimitCategory;                  // WotLK, ItemLimitCategory
        data << proto->HolidayId;                          // Holiday.dbc?

        player->GetSession()->SendPacket(&data);
    }

    void rewardHonor(Player *killer, Creature *killed)
    {
        Map *map = killer->GetMap();

        if (map->IsDungeon() && !map->IsBattlegroundOrArena())
        {
            Map::PlayerList const &players = map->GetPlayers();

            if (!players.IsEmpty())
            {
                for (Map::PlayerList::const_iterator iter = players.begin(); iter != players.end(); ++iter)
                {
                    if (Player *player = iter->GetSource())
                    {
                        int honor = -1;
                        float honor_f = (float)honor;
                        player->UpdateHonorFields();
                        uint8 playerLevel = player->getLevel();
                        uint8 gray = Acore::XP::GetGrayLevel(playerLevel);
                        uint8 bossLevel = killed->getLevel();
                        PlayerSettingsMapInfo *mapInfo = map->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");
                        uint32 nplayers = std::min(5u, std::max(mapInfo->nplayers, mapInfo->veto));

                        if (bossLevel > gray)
                        {
                            honor_f = ceil(Acore::Honor::hk_honor_at_level_f(playerLevel) * (bossLevel - gray) / (playerLevel - gray));
                            honor_f *= sWorld->getRate(RATE_HONOR);
                            honor_f *= 1 + honorMultiplier * (nplayers - 1);
                            honor = int32(honor_f);
                            player->ModifyHonorPoints(honor);
                            ChatHandler(player->GetSession()).PSendSysMessage("You have been awarded %i honor.", honor);
                            uint32 guid = player->GetGUID().GetCounter();
                            mapInfo->honor[guid] += honor;
                            rewardBonusHonor(player, mapInfo->honor[guid]);
                        }
                    }
                }
            }
        }
    }

    void rewardBonusHonor(Player *player, float honor_f)
    {
        uint32 guid = player->GetGUID().GetCounter();
        Map *map = player->GetMap();

        if (map->IsDungeon() && !map->IsBattlegroundOrArena())
        {
            PlayerSettingsMapInfo *mapInfo = map->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");

            if (mapInfo->rewarded[guid] || !mapInfo->rdf[guid])
                return;

            honor_f *= sWorld->getRate(RATE_HONOR);
            honor_f *= mapInfo->rdf[guid];
            player->UpdateHonorFields();
            int honor = int32(honor_f);
            player->ModifyHonorPoints(honor);
            ChatHandler(player->GetSession()).PSendSysMessage("You have been awarded %i bonus honor.", honor);
            mapInfo->rewarded[guid] = true;
        }
    }
};

class PlayerSettingsUnitScript : public UnitScript
{
public:
    PlayerSettingsUnitScript() : UnitScript("PlayerSettingsUnitScript", true) {}

    void ModifyPeriodicDamageAurasTick(Unit* target, Unit* attacker, uint32& damage) override
    {
        if (check(attacker, target))
            damage = modify(attacker, target, damage);
    }

    void ModifyMeleeDamage(Unit* target, Unit* attacker, uint32& damage) override
    {
        if (check(attacker, target))
            damage = modify(attacker, target, damage);
    }

    void ModifySpellDamageTaken(Unit* target, Unit* attacker, int32& damage) override
    {
        if (check(attacker, target))
            damage = modify(attacker, target, damage);
    }

    void ModifyPeriodicHealthAurasTick(Unit* target, Unit* healer, uint32& gain) override
    {
        if (check(healer, target))
            gain = modify(healer, target, gain);
    }

    void ModifyHealRecieved(Unit* target, Unit* healer, uint32& gain) override
    {
        if (check(healer, target))
            gain = modify(healer, target, gain);
    }

private:
    bool inDungeon(Unit *target, Unit *attacker)
    {
        return target->GetMap()->IsDungeon() && attacker->GetMap()->IsDungeon();
    }

    bool inBattleground(Unit *target, Unit *attacker)
    {
        return target->GetMap()->IsBattleground() && attacker->GetMap()->IsBattleground();
    }

    bool check(Unit* attacker, Unit* target)
    {
        if (!target || !target->GetMap())
            return false;

        if (!attacker || !attacker->GetMap())
            return false;

        if (!inDungeon(target, attacker) || inBattleground(target, attacker))
            return false;

        return true;
    }

    uint32 modify(Unit* attacker, Unit* target, uint32 amount)
    {
        PlayerSettingsMapInfo *mapInfo = target->GetMap()->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");
        InstanceMap *instanceMap = ((InstanceMap *)sMapMgr->FindMap(target->GetMapId(), target->GetInstanceId()));

        if (!instanceMap)
            return amount;

        uint32 nplayers = std::max(mapInfo->nplayers, mapInfo->veto);
        uint32 maxPlayers = instanceMap->GetMaxPlayers();

        float defence = Defence5M;

        if (maxPlayers > 5)
            defence = 1 / (2 + (maxPlayers / 5.0f));

        float multiplier = 1.0f;
        bool isAttackerPlayer = attacker->GetTypeId() == TYPEID_PLAYER;
        bool isAttackerPet = (attacker->IsHunterPet() || attacker->IsPet() || attacker->IsSummon()) && attacker->IsControlledByPlayer();
        bool isTargetPlayer = target->GetTypeId() == TYPEID_PLAYER;
        bool isTargetPet = (target->IsHunterPet() || target->IsPet() || target->IsSummon()) && target->IsControlledByPlayer();

        if (!isAttackerPlayer && !isAttackerPet)
            multiplier = defence + (1 - defence) / (maxPlayers - 1) * (nplayers - 1);

        if (!instanceMap->IsRaidOrHeroicDungeon())
        {
            if ((!isAttackerPlayer && !isAttackerPet) && (isTargetPlayer || isTargetPet))
                multiplier = multiplier * playerCurve(target->getLevel()) / playerCurve(attacker->getLevel());
            else if ((isAttackerPlayer || isAttackerPet) && (!isTargetPlayer && !isTargetPet))
                multiplier = multiplier * creatureCurve(target->getLevel()) / creatureCurve(attacker->getLevel());
        }

        return amount * multiplier;
    }

    static float playerCurve(uint32 level)
    {
        return 45.2 * exp(0.0647 * level);
    }

    static float creatureCurve(uint32 level)
    {
        return 71.7 + 5.98 * level + 0.738 * pow(level, 2);
    }
};

class PlayerSettingsAllMapScript : public AllMapScript
{
public:
    PlayerSettingsAllMapScript() : AllMapScript("PlayerSettingsAllMapScript") {}

    void OnPlayerEnterAll(Map *map, Player *player)
    {
        if (!enabled)
            return;

        if (player->IsGameMaster())
            return;

        PlayerSettingsMapInfo *mapInfo = map->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");
        mapInfo->nplayers = map->GetPlayersCountExceptGMs();

        if (mapInfo->nplayers == 0)
            mapInfo->nplayers = 1;

        if (mapInfo->veto == 0)
            mapInfo->veto = mapInfo->nplayers;

        if (map->GetEntry()->IsDungeon() && player)
        {
            Map::PlayerList const &players = map->GetPlayers();

            if (mapInfo->nplayers > 1)
                for (Map::PlayerList::const_iterator iter = players.begin(); iter != players.end(); ++iter)
                    if (Player *handle = iter->GetSource())
                        ChatHandler(handle->GetSession()).PSendSysMessage("%s has entered the instance. The minions of hell grow stronger.", player->GetName().c_str());
        }
    }

    void OnPlayerLeaveAll(Map *map, Player *player)
    {
        if (!enabled)
            return;

        if (player->IsGameMaster())
            return;

        PlayerSettingsMapInfo *mapInfo = map->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");

        if (map->GetEntry() && map->GetEntry()->IsDungeon())
        {
            bool check = false;
            Map::PlayerList const &players = map->GetPlayers();

            for (Map::PlayerList::const_iterator iter = players.begin(); iter != players.end(); ++iter)
                if (Player *player = iter->GetSource())
                    if (player->IsInCombat())
                        check = true;

            if (!check)
                mapInfo->nplayers = map->GetPlayersCountExceptGMs() - 1;
        }

        if (map->GetEntry()->IsDungeon())
        {
            Map::PlayerList const &players = map->GetPlayers();

            if (mapInfo->nplayers > 0)
                for (Map::PlayerList::const_iterator iter = players.begin(); iter != players.end(); ++iter)
                    if (Player *handle = iter->GetSource())
                        ChatHandler(handle->GetSession()).PSendSysMessage("%s has left the instance. The minions of hell grow weaker.", player->GetName().c_str());
        }
    }
};

class PlayerSettingsAllCreatureScript : public AllCreatureScript
{
public:
    PlayerSettingsAllCreatureScript() : AllCreatureScript("PlayerSettingsAllCreatureScript") {}

    void OnAllCreatureUpdate(Creature *creature, uint32 /*diff*/) override
    {
        if (!enabled)
            return;

        ModifyCreatureAttributes(creature);
    }

    void ModifyCreatureAttributes(Creature *creature)
    {
        if (!creature || !creature->GetMap())
            return;

        if (!creature->GetMap()->IsDungeon())
            return;

        if (((creature->IsHunterPet() || creature->IsPet() || creature->IsSummon()) && creature->IsControlledByPlayer()))
            return;

        PlayerSettingsMapInfo *mapInfo = creature->GetMap()->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");
        CreatureTemplate const *creatureTemplate = creature->GetCreatureTemplate();
        InstanceMap *instanceMap = ((InstanceMap *)sMapMgr->FindMap(creature->GetMapId(), creature->GetInstanceId()));
        PlayerSettingsCreatureInfo *creatureInfo = creature->CustomData.GetDefault<PlayerSettingsCreatureInfo>("PlayerSettingsCreatureInfo");

        if (!creature->IsAlive())
            return;

        creatureInfo->nplayers = std::max(mapInfo->nplayers, mapInfo->veto);

        if (!creatureInfo->nplayers)
            return;

        creatureInfo->entry = creature->GetEntry();

        CreatureBaseStats const *stats = sObjectMgr->GetCreatureBaseStats(creature->getLevel(), creatureTemplate->unit_class);
        uint32 baseHealth = stats->GenerateHealth(creatureTemplate);
        uint32 scaledHealth = 0;
        uint32 nplayers = creatureInfo->nplayers;
        uint32 maxPlayers = instanceMap->GetMaxPlayers();

        float offence = Offence5M;

        if (maxPlayers > 5)
            offence = 1 / (maxPlayers - (2 + (maxPlayers / 5.0f)));

        creatureInfo->HealthMultiplier = offence + (1 - offence) / (maxPlayers - 1) * (nplayers - 1);
        scaledHealth = round(((float)baseHealth * creatureInfo->HealthMultiplier) + 1.0f);

        uint32 previousHealth = creature->GetHealth();
        uint32 previousMaxHealth = creature->GetMaxHealth();

        creature->SetCreateHealth(scaledHealth);
        creature->SetMaxHealth(scaledHealth);
        creature->ResetPlayerDamageReq();
        creature->SetModifierValue(UNIT_MOD_HEALTH, BASE_VALUE, (float)scaledHealth);

        uint32 scaledCurrentHealth = previousHealth && previousMaxHealth ? float(scaledHealth) / float(previousMaxHealth) * float(previousHealth) : 0;

        creature->SetHealth(scaledCurrentHealth);
        creature->UpdateAllStats();
    }
};

class PlayerSettingsCommandScript : public CommandScript
{
public:
    PlayerSettingsCommandScript() : CommandScript("PlayerSettingsCommandScript") {}

    Acore::ChatCommands::ChatCommandTable GetCommands() const
    {
        static Acore::ChatCommands::ChatCommandTable commands =
        {
            {"players", HandlePlayersCommand, SEC_PLAYER, Acore::ChatCommands::Console::No},
            {"playersettings", HandlePlayerSettingsCommand, SEC_PLAYER, Acore::ChatCommands::Console::No}
        };

        return commands;
    }

    static bool HandlePlayersCommand(ChatHandler *handler, std::string args)
    {
        char *x = strtok((char *)args.c_str(), " ");
        Player *player = handler->getSelectedPlayerOrSelf();
        Map *map = player->GetMap();
        PlayerSettingsMapInfo *mapInfo = map->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");
        uint32 maxPlayers = ((InstanceMap *)sMapMgr->FindMap(map->GetId(), map->GetInstanceId()))->GetMaxPlayers();

        if (!map->IsDungeon())
        {
            handler->SendSysMessage("Only usable in dungeons.");
            return true;
        }

        Map::PlayerList const &players = map->GetPlayers();
        if (!players.IsEmpty())
        {
            for (Map::PlayerList::const_iterator iter = players.begin(); iter != players.end(); ++iter)
            {
                if (Player *player = iter->GetSource())
                {
                    if (player->IsInCombat())
                    {
                        handler->SendSysMessage("Only usable outside of combat.");
                        return true;
                    }
                }
            }
        }

        if (x)
        {
            long int n = strtol(x, nullptr, 10);

            if (n < 1)
                n = 1;
            else if (n > maxPlayers)
                n = maxPlayers;

            mapInfo->veto = (uint32)n;
        }

        if (!players.IsEmpty())
        {
            for (Map::PlayerList::const_iterator iter = players.begin(); iter != players.end(); ++iter)
            {
                if (Player *player = iter->GetSource())
                {
                    ChatHandler(player->GetSession()).PSendSysMessage("Players set to %i.", std::max(mapInfo->nplayers, mapInfo->veto));
                }
            }
        }

        return true;
    }

    static bool HandlePlayerSettingsCommand(ChatHandler *handler)
    {

        Player *player = handler->getSelectedPlayerOrSelf();
        Map *map = player->GetMap();

        if (!map)
            return false;

        if (!map->IsDungeon())
            return false;

        PlayerSettingsMapInfo *mapInfo = map->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");

        if (!mapInfo)
            return false;

        InstanceMap *instanceMap = ((InstanceMap *)sMapMgr->FindMap(player->GetMapId(), player->GetInstanceId()));

        if (!instanceMap)
            return false;

        uint32 maxPlayers = instanceMap->GetMaxPlayers();

        float offence = Offence5M;
        float defence = Defence5M;

        if (maxPlayers > 5)
        {
            float tanks = 2.0f;
            float healers = maxPlayers / 5.0f;
            float dps = maxPlayers - (tanks + healers);
            offence = 1 / dps;
            defence = 1 / (tanks + healers);
        }

        uint32 nplayers = std::max(mapInfo->nplayers, mapInfo->veto);

        handler->PSendSysMessage("Players set to %i.", nplayers);
        handler->PSendSysMessage("Health multiplier set to %.2f.", offence + (1 - offence) / (maxPlayers - 1) * (nplayers - 1));
        handler->PSendSysMessage("Damage multiplier set to %.2f.", defence + (1 - defence) / (maxPlayers - 1) * (nplayers - 1));

        nplayers = std::min(5u, std::max(mapInfo->nplayers, mapInfo->veto));

        handler->PSendSysMessage("Experience multiplier set to %.2f.", !nplayers ? 0 : (1 + experienceMultiplier * (nplayers - 1)));
        handler->PSendSysMessage("Gold multiplier set to %.2f.", !nplayers ? 0 : (1 + goldMultiplier * (nplayers - 1)));
        handler->PSendSysMessage("Honor multiplier set to %.2f.", !nplayers ? 0 : (1 + honorMultiplier * (nplayers - 1)));

        return true;
    }
};

class PlayerSettingsDatabase : public DatabaseScript
{
public:
    PlayerSettingsDatabase() : DatabaseScript("PlayerSettingsDatabase") {}

    std::string path = "/modules/mod-player-settings/sql/";
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


void AddPlayerSettingsScripts()
{
    new PlayerSettingsWorldScript();
    new PlayerSettingsPlayerScript();
    new PlayerSettingsUnitScript();
    new PlayerSettingsAllMapScript();
    new PlayerSettingsAllCreatureScript();
    new PlayerSettingsCommandScript();
    new PlayerSettingsDatabase();
}
