/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "Config.h"

void AddProgressionSystemScripts();
void AddBracket_0_Scripts();
void AddBracket_1_19_Scripts();
void AddBracket_20_29_Scripts();
void AddBracket_30_39_Scripts();
void AddBracket_40_49_Scripts();
void AddBracket_50_59_A_Scripts();
void AddBracket_50_59_B_Scripts();
void AddBracket_60_1_Scripts();
void AddBracket_60_1_A_Scripts();
void AddBracket_60_2_Scripts();
void AddBracket_60_2_A_Scripts();
void AddBracket_60_3_A_Scripts();

void Addmod_progression_systemScripts()
{
    AddProgressionSystemScripts();

    if (!sConfigMgr->GetOption<bool>("ProgressionSystem.LoadScripts", true))
        return;

    AddBracket_0_Scripts();
    AddBracket_1_19_Scripts();
    AddBracket_20_29_Scripts();
    AddBracket_30_39_Scripts();
    AddBracket_40_49_Scripts();
    AddBracket_50_59_A_Scripts();
    AddBracket_50_59_B_Scripts();
    AddBracket_60_1_Scripts();
    AddBracket_60_1_A_Scripts();
    AddBracket_60_2_Scripts();
    AddBracket_60_2_A_Scripts();
    AddBracket_60_3_A_Scripts();
}
