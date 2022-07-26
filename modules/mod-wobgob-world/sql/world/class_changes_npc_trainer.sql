-- Dark Command
REPLACE INTO `npc_trainer` VALUES (200019, 56222, 6500, 0, 0, 60, 0);

-- Call Stabled Pet
REPLACE INTO `npc_trainer` VALUES (200014, 62757, 3000, 0, 0, 60, 0);

-- Soul Shards
UPDATE `item_template` SET `maxcount` = 0, `stackable` = 250 WHERE `entry` = 6265;

-- Reagents
UPDATE `item_template` SET `maxcount` = 0, `stackable` = 250 WHERE `entry` IN (5565,16583,17020,17021,17026,17028,17029,17030,17031,17032,17033,17034,17035,17036,17037,17038,21177,22147,22148,37201,44605,44605,44614,44615);

-- Rogue poisons and Thistle Tea
UPDATE `item_template` SET `maxcount` = 0, `stackable` = 250 WHERE `entry` IN (7676, 2892, 2893, 8984, 8985, 20844, 22053, 22054, 43232, 43233, 3775, 3776, 5237, 6951, 9186, 6947, 6949, 6950, 8926, 8927, 8928, 21927, 43230, 43231, 10918, 10920, 10921, 10922, 22055, 43234, 43235, 21835, 43237);
