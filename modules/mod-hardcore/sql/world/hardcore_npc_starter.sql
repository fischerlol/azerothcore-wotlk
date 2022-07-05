-- add creature script to each starting creature
UPDATE `creature_template` SET `ScriptName` = 'npc_starter' WHERE `entry` = 823;   -- Deputy Willem (Human)
UPDATE `creature_template` SET `ScriptName` = 'npc_starter' WHERE `entry` = 2079;  -- Conservator Ilthalaine (Night Elf)
UPDATE `creature_template` SET `ScriptName` = 'npc_starter' WHERE `entry` = 16475; -- Megelon (Draenei)
UPDATE `creature_template` SET `ScriptName` = 'npc_starter' WHERE `entry` = 658;   -- Sten Stoutarm (Dwarf/Gnome)
UPDATE `creature_template` SET `ScriptName` = 'npc_starter' WHERE `entry` = 15278; -- Magistrix Erona (Blood Elf)
UPDATE `creature_template` SET `ScriptName` = 'npc_starter' WHERE `entry` = 1568;  -- Undertaker Mordo (Undead)
UPDATE `creature_template` SET `ScriptName` = 'npc_starter' WHERE `entry` = 10176; -- Kaltunk (Orc/Troll)
UPDATE `creature_template` SET `ScriptName` = 'npc_starter' WHERE `entry` = 2980;  -- Grull Hawkwind (Tauren)

-- delete duplicates
DELETE FROM `gossip_menu_option` WHERE `MenuID` = 62000;
DELETE FROM `gossip_menu_option` WHERE `MenuID` = 62001;
DELETE FROM `gossip_menu_option` WHERE `MenuID` = 62002;
DELETE FROM `npc_text` WHERE `ID` = 60047;

-- add the gossip menu options to the database
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcFlag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(62000, 1, 0, 'Tell me about hardcore...', 0, 0, 0, 62001, 0, 0, 0, '', 0, 0);
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcFlag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(62001, 1, 0, 'I would like to opt-in to harcore mode', 0, 0, 0, 0, 0, 0, 0, 'Are you sure?', 0, 0);
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcFlag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(62002, 1, 0, 'Back...', 0, 0, 0, 0, 0, 0, 0, '', 0, 0);

-- add custom npc text
INSERT INTO `npc_text` (`ID`, `text0_0`) VALUES ('60047', 'Note: Wobblin’ Goblin is in no way responsible for your Hardcore character. If you choose to create and play a Hardcore character, you do so at your own risk. Wobbling Goblin is not responsible for the death and loss of your hardcore characters for any reason including Internet lag, bugs, Acts of God, your little sister, or any other reason whatsoever. Consult the End User License Agreement for more details. Wobbling Goblin will not, and does not have the capability to restore any deceased Hardcore characters. Don\'t even ask. La-la-la-la-la, we can\'t hear you...');