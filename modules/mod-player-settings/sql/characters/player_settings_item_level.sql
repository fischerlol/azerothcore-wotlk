CREATE TABLE IF NOT EXISTS `item_level` (
    `id` SERIAL PRIMARY KEY,
    `player` BIGINT,
    `item` BIGINT,
    `ilvl` SMALLINT
) ENGINE=InnoDB DEFAULT CHARSET=utf8;