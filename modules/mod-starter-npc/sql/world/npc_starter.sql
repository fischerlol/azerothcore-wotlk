set @base_gossip_hello = 62000;
set @base_gossip_select = 63000;
set @base_text = 70000;

-- add creature script to each starting creature
UPDATE `creature_template` SET `ScriptName` = 'npc_starter' WHERE `entry` = 823;   -- Deputy Willem (Human)
UPDATE `creature_template` SET `ScriptName` = 'npc_starter' WHERE `entry` = 2079;  -- Conservator Ilthalaine (Night Elf)
UPDATE `creature_template` SET `ScriptName` = 'npc_starter' WHERE `entry` = 16475; -- Megelon (Draenei)
UPDATE `creature_template` SET `ScriptName` = 'npc_starter' WHERE `entry` = 658;   -- Sten Stoutarm (Dwarf/Gnome)
UPDATE `creature_template` SET `ScriptName` = 'npc_starter' WHERE `entry` = 15278; -- Magistrix Erona (Blood Elf)
UPDATE `creature_template` SET `ScriptName` = 'npc_starter' WHERE `entry` = 1568;  -- Undertaker Mordo (Undead)
UPDATE `creature_template` SET `ScriptName` = 'npc_starter' WHERE `entry` = 10176; -- Kaltunk (Orc/Troll)
UPDATE `creature_template` SET `ScriptName` = 'npc_starter' WHERE `entry` = 2980;  -- Grull Hawkwind (Tauren)

-- add the gossip hello menu options to the database
DELETE FROM `gossip_menu_option` WHERE `MenuID` = @base_gossip_hello;
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcFlag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(@base_gossip_hello, 1, 0, 'Tell me about hardcore...', 0, 0, 0, @base_gossip_hello, 0, 0, 0, '', 0, 0);

DELETE FROM `gossip_menu_option` WHERE `MenuID` = @base_gossip_hello+1;
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcFlag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(@base_gossip_hello+1, 1, 0, 'Tell me about level 15 boost...', 0, 0, 0, @base_gossip_hello+1, 0, 0, 0, '', 0, 0);

-- add the gossip select diaglogue to the database
DELETE FROM `gossip_menu_option` WHERE `MenuID` = @base_gossip_select;
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcFlag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(@base_gossip_select, 1, 0, 'I would like to opt-in to harcore mode', 0, 0, 0, 0, 0, 0, 0, 'Are you sure?', 0, 0);

DELETE FROM `gossip_menu_option` WHERE `MenuID` = @base_gossip_select+1;
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcFlag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(@base_gossip_select+1, 1, 0, 'I would like to use the level 15 boost', 0, 0, 0, @base_gossip_select+1, 0, 0, 0, 'Are you sure?', 0, 0);

DELETE FROM `gossip_menu_option` WHERE `MenuID` = @base_gossip_select+2;
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcFlag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(@base_gossip_select+2, 1, 0, 'Back...', 0, 0, 0, 0, 0, 0, 0, '', 0, 0);

-- add custom npc text
DELETE FROM `npc_text` WHERE `ID` = @base_text;
INSERT INTO `npc_text` (`ID`, `text0_0`) VALUES (@base_text, 'Note: Wobblin’ Goblin is in no way responsible for your Hardcore character. If you choose to create and play a Hardcore character, you do so at your own risk. Wobbling Goblin is not responsible for the death and loss of your hardcore characters for any reason including Internet lag, bugs, Acts of God, your little sister, or any other reason whatsoever. Consult the End User License Agreement for more details. Wobbling Goblin will not, and does not have the capability to restore any deceased Hardcore characters. Don\'t even ask. La-la-la-la-la, we can\'t hear you...');

DELETE FROM `npc_text` WHERE `ID` = @base_text+1;
INSERT INTO `npc_text` (`ID`, `text0_0`) VALUES (@base_text+1, 'Info text that boost person will see.');