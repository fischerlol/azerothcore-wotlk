SET @SATCHEL = 52003;
SET @POOLA = 108455;
SET @POOLB = 108456;

DELETE FROM `item_loot_template` WHERE `Entry` = @SATCHEL AND `Reference` IN (@POOLA, @POOLB);
INSERT INTO `item_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`)
VALUES (@SATCHEL, 5, @POOLA, 100, 0, 1, 5, 1, 1, 'Satchel of Helpful Goods - (ReferenceTable)');
INSERT INTO `item_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`)
VALUES (@SATCHEL, 6, @POOLB, 100, 0, 1, 6, 1, 1, 'Satchel of Helpful Goods - (ReferenceTable)');

DELETE FROM `reference_loot_template` WHERE `Entry` IN (@POOLA);
INSERT INTO `reference_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`)
VALUES (@POOLA, 12811, 0, 24, 0, 1, 5, 2, 4, 'Righteous Orb');
INSERT INTO `reference_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`)
VALUES (@POOLA, 12809, 0, 10, 0, 1, 5, 2, 4, 'Guardian Stone');
INSERT INTO `reference_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`)
VALUES (@POOLA, 43123, 0, 20, 0, 1, 5, 3, 6, 'Ink of the Sky');
INSERT INTO `reference_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`)
VALUES (@POOLA, 20520, 0, 25, 0, 1, 5, 3, 6, 'Dark Rune');
INSERT INTO `reference_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`)
VALUES (@POOLA, 12363, 0, 20, 0, 1, 5, 2, 5, 'Arcane Crystal');
INSERT INTO `reference_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`)
VALUES (@POOLA, 17203, 0, 1, 0, 1, 5, 1, 1, 'Sulfuron Ingot');

DELETE FROM `reference_loot_template` WHERE `Entry` IN (@POOLB);
INSERT INTO `reference_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`)
VALUES (@POOLB, 13512, 0, 20, 0, 1, 6, 1, 2, 'Flask of Supreme Power');
INSERT INTO `reference_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`)
VALUES (@POOLB, 13511, 0, 20, 0, 1, 6, 1, 2, 'Flask of Distilled Wisdom');
INSERT INTO `reference_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`)
VALUES (@POOLB, 13510, 0, 20, 0, 1, 6, 1, 2, 'Flask of the Titans');
INSERT INTO `reference_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`)
VALUES (@POOLB, 9030, 0, 20, 0, 1, 6, 4, 6, 'Restorative Potion');
INSERT INTO `reference_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`)
VALUES (@POOLB, 5634, 0, 20, 0, 1, 6, 4, 6, 'Free Action Potion');
