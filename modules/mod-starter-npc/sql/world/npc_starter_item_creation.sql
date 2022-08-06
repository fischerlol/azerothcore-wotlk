-- class
set @class_weapon = 2;
set @class_armor = 4;

-- class id
set @class_warrior = 1;
set @class_paladin = 2;
set @class_hunter = 3;
set @class_rogue = 4;
set @class_priest = 5;
set @class_shaman = 7;
set @class_mage = 8;
set @class_warlock = 9;
set @class_druid = 11;

-- subclass weapon
set @subclass_axe_one_hand = 0;
set @subclass_axe_two_hand = 1;
set @subclass_bow = 2;
set @subclass_gun = 3;
set @subclass_mace_one_hand = 4;
set @sublcass_mace_two_hand = 5;
set @subclass_polearm = 6;
set @subclass_sword_one_hand = 7;
set @subclass_sword_two_hand = 8;
set @subclass_staff = 10;
set @subclass_dagger = 15;
set @subclass_crossbow = 18;
set @subclass_wand = 19;

-- sublcass armor
set @subclass_miscellaneous = 0;
set @subclass_cloth = 1;
set @subclass_leather = 2;
set @subclass_mail = 3;
set @subclass_shield = 6;

-- inventorytype
set @inventorytype_shoulders = 3;
set @inventorytype_chest = 5;
set @inventorytype_waist = 6;
set @inventorytype_legs = 7;
set @inventorytype_feet = 8;
set @inventorytype_wrists = 9;
set @inventorytype_hands = 10;
set @inventorytype_one_hand = 13;
set @inventorytype_shield = 14;
set @inventorytype_bow = 15;
set @inventorytype_two_hand = 17;
set @inventorytype_tabard = 19;
set @inventorytype_robe = 20;
set @inventorytype_main_hand = 21;
set @inventorytype_off_hand = 22;
set @inventorytype_holdable = 23;
set @inventorytype_ranged = 26;

-- sheath 
set @sheath_ranged_and_frill = 0;
set @sheath_two_hand_weapon = 1;
set @sheath_staff = 2;
set @sheath_one_handed = 3;
set @sheath_shield = 4;
set @sheath_off_hand = 6;

-- material
set @material_metal = 1;
set @material_wood = 2;
set @material_liquid = 3;
set @material_chain = 5;
set @material_cloth = 7;
set @material_leather = 8;

-- stats
set @agility = 3;
set @strength = 4;
set @intellect = 5;
set @spirit = 6;
set @stamina = 7;

-- other
set @itemlevel = 20;
set @flags_default = 32768;
set @quality = 2;
set @bind_on_pickup = 1;
set @sell_price = 0;

-- container
set @all_races = 0;

-- base id
set @base_item_container = 90000;
set @base_item_armor = 91000;
set @base_item_weapon = 92000;

DELIMITER //

DROP PROCEDURE IF EXISTS `create_armor` //
CREATE PROCEDURE create_armor(IN entry MEDIUMINT(7), IN name VARCHAR(255),  IN description VARCHAR(255), IN class TINYINT(3), IN subclass TINYINT(3), IN displayid MEDIUMINT(7), IN inventorytype TINYINT(3), IN quality TINYINT(3), IN itemlevel SMALLINT(5), IN flags INT(10), IN material TINYINT(10), IN bonding TINYINT(3), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5)) 
BEGIN
    REPLACE INTO item_template(entry, name, description, class, subclass, displayid, inventorytype, quality, itemlevel, flags, material, bonding, sellprice, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3) VALUES
      (entry, name, description, class, subclass, displayid, inventorytype, quality, itemlevel, flags, material, bonding, sellprice, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DROP PROCEDURE IF EXISTS `create_shoulders` //
CREATE PROCEDURE create_shoulders(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN subclass TINYINT(3), IN displayid MEDIUMINT(7), IN flags INT(10), IN material TINYINT(10), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5))
BEGIN
    CALL create_armor(entry, name, description, @class_armor, subclass, displayid, @inventorytype_shoulders, @quality, @itemlevel, flags, material, @bind_on_pickup, @sell_price, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DROP PROCEDURE IF EXISTS `create_robe` //
CREATE PROCEDURE create_robe(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN subclass TINYINT(3), IN displayid MEDIUMINT(7), IN flags INT(10), IN material TINYINT(10), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5))
BEGIN
    CALL create_armor(entry, name, description, @class_armor, subclass, displayid, @inventorytype_robe, @quality, @itemlevel, flags, material, @bind_on_pickup, @sell_price, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DROP PROCEDURE IF EXISTS `create_chest` //
CREATE PROCEDURE create_chest(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN subclass TINYINT(3), IN displayid MEDIUMINT(7), IN flags INT(10), IN material TINYINT(10), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5))
BEGIN
    CALL create_armor(entry, name, description, @class_armor, subclass, displayid, @inventorytype_chest, @quality, @itemlevel, flags, material, @bind_on_pickup, @sell_price, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DROP PROCEDURE IF EXISTS `create_bracers` //
CREATE PROCEDURE create_bracers(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN subclass TINYINT(3), IN displayid MEDIUMINT(7), IN flags INT(10), IN material TINYINT(10), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5))
BEGIN
    CALL create_armor(entry, name, description, @class_armor, subclass, displayid, @inventorytype_wrists, @quality, @itemlevel, flags, material, @bind_on_pickup, @sell_price, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DROP PROCEDURE IF EXISTS `create_gloves` //
CREATE PROCEDURE create_gloves(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN subclass TINYINT(3), IN displayid MEDIUMINT(7), IN flags INT(10), IN material TINYINT(10), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5))
BEGIN
    CALL create_armor(entry, name, description, @class_armor, subclass, displayid, @inventorytype_hands, @quality, @itemlevel, flags, material, @bind_on_pickup, @sell_price, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DROP PROCEDURE IF EXISTS `create_belt` //
CREATE PROCEDURE create_belt(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN subclass TINYINT(3), IN displayid MEDIUMINT(7), IN flags INT(10), IN material TINYINT(10), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5))
BEGIN
    CALL create_armor(entry, name, description, @class_armor, subclass, displayid, @inventorytype_waist, @quality, @itemlevel, flags, material, @bind_on_pickup, @sell_price, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DROP PROCEDURE IF EXISTS `create_legs` //
CREATE PROCEDURE create_legs(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN subclass TINYINT(3), IN displayid MEDIUMINT(7), IN flags INT(10), IN material TINYINT(10), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5))
BEGIN
    CALL create_armor(entry, name, description, @class_armor, subclass, displayid, @inventorytype_legs, @quality, @itemlevel, flags, material, @bind_on_pickup, @sell_price, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DROP PROCEDURE IF EXISTS `create_boots` //
CREATE PROCEDURE create_boots(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN subclass TINYINT(3), IN displayid MEDIUMINT(7), IN flags INT(10), IN material TINYINT(10), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5))
BEGIN
    CALL create_armor(entry, name, description, @class_armor, subclass, displayid, @inventorytype_feet, @quality, @itemlevel, flags, material, @bind_on_pickup, @sell_price, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DROP PROCEDURE IF EXISTS `create_weapon` //
CREATE PROCEDURE create_weapon(IN entry MEDIUMINT(7), IN name VARCHAR(255),  IN description VARCHAR(255), IN class TINYINT(3), IN subclass TINYINT(3), IN displayid MEDIUMINT(7), IN inventorytype TINYINT(3), IN sheath TINYINT(3), IN quality TINYINT(3), IN itemlevel SMALLINT(5), IN flags INT(10), IN material TINYINT(10), IN bonding TINYINT(3), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5)) 
BEGIN
    REPLACE INTO item_template(entry, name, description, class, subclass, displayid, inventorytype, sheath, quality, itemlevel, flags, material, bonding, sellprice, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3) VALUES
      (entry, name, description, class, subclass, displayid, inventorytype, sheath, @quality, itemlevel, flags, material, bonding, sellprice, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DROP PROCEDURE IF EXISTS `create_two_hand_axe` //
CREATE PROCEDURE create_two_hand_axe(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, @subclass_axe_two_hand, displayid, @inventorytype_two_hand, @sheath_two_hand_weapon, @quality, @itemlevel, flags, @material_metal, @bind_on_pickup, @sell_price, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DROP PROCEDURE IF EXISTS `create_two_hand_mace` //
CREATE PROCEDURE create_two_hand_mace(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, @sublcass_mace_two_hand, displayid, @inventorytype_two_hand, @sheath_two_hand_weapon, @quality, @itemlevel, flags, @material_metal, @bind_on_pickup, @sell_price, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DROP PROCEDURE IF EXISTS `create_two_hand_sword` //
CREATE PROCEDURE create_two_hand_sword(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, @subclass_sword_two_hand, displayid, @inventorytype_two_hand, @sheath_two_hand_weapon, @quality, @itemlevel, flags, @material_metal, @bind_on_pickup, @sell_price, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DROP PROCEDURE IF EXISTS `create_one_hand_dagger` //
CREATE PROCEDURE create_one_hand_dagger(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, @subclass_dagger, displayid, @inventorytype_one_hand, @sheath_one_handed, @quality, @itemlevel, flags, @material_metal, @bind_on_pickup, @sell_price, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DROP PROCEDURE IF EXISTS `create_one_hand_mace` //
CREATE PROCEDURE create_one_hand_mace(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, @subclass_mace_one_hand, displayid, @inventorytype_one_hand, @sheath_one_handed, @quality, @itemlevel, flags, @material_metal, @bind_on_pickup, @sell_price, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DROP PROCEDURE IF EXISTS `create_one_hand_sword` //
CREATE PROCEDURE create_one_hand_sword(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, @subclass_sword_one_hand, displayid, @inventorytype_one_hand, @sheath_one_handed, @quality, @itemlevel, flags, @material_metal, @bind_on_pickup, @sell_price, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DROP PROCEDURE IF EXISTS `create_one_hand_axe` //
CREATE PROCEDURE create_one_hand_axe(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, @subclass_axe_one_hand, displayid, @inventorytype_one_hand, @sheath_one_handed, @quality, @itemlevel, flags, @material_metal, @bind_on_pickup, @sell_price, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DROP PROCEDURE IF EXISTS `create_staff` //
CREATE PROCEDURE create_staff (IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, @subclass_staff, displayid, @inventorytype_two_hand, @sheath_staff, @quality, @itemlevel, flags, @material_metal, @bind_on_pickup, @sell_price, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DROP PROCEDURE IF EXISTS `create_wand` //
CREATE PROCEDURE create_wand(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, @subclass_wand, displayid, @inventorytype_ranged, @sheath_ranged_and_frill, @quality, @itemlevel, flags, @material_wood, @bind_on_pickup, @sell_price, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DROP PROCEDURE IF EXISTS `create_gun` //
CREATE PROCEDURE create_gun(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, @subclass_gun, displayid, @inventorytype_ranged, @sheath_ranged_and_frill, @quality, @itemlevel, flags, @material_metal, @bind_on_pickup, @sell_price, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DROP PROCEDURE IF EXISTS `create_crossbow` //
CREATE PROCEDURE create_crossbow(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, @subclass_crossbow, displayid, @inventorytype_ranged, @sheath_ranged_and_frill, @quality, @itemlevel, flags, @material_wood, @bind_on_pickup, @sell_price, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DROP PROCEDURE IF EXISTS `create_bow` //
CREATE PROCEDURE create_bow(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, @subclass_bow, displayid, @inventorytype_bow, @sheath_ranged_and_frill, @quality, @itemlevel, flags, @material_wood, @bind_on_pickup, @sell_price, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DROP PROCEDURE IF EXISTS `create_off_hand_weapon` //
CREATE PROCEDURE create_off_hand_weapon(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, subclass, displayid, @inventorytype_off_hand, @sheath_one_handed, @quality, @itemlevel, flags, @material_metal, @bind_on_pickup, @bind_on_pickup, @sell_price, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DROP PROCEDURE IF EXISTS `create_off_hand_frill` //
CREATE PROCEDURE create_off_hand_frill(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5))
BEGIN
    CALL create_weapon(entry, name, description, @class_armor, @subclass_miscellaneous, displayid, @inventorytype_holdable, @sheath_ranged_and_frill, @quality, @itemlevel, flags, @material_liquid, @bind_on_pickup, @sell_price, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DROP PROCEDURE IF EXISTS `create_shield` //
CREATE PROCEDURE create_shield(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN sellprice INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5))
BEGIN
    CALL create_weapon(entry, name, description, @class_armor, @subclass_shield, displayid, @inventorytype_shield, @sheath_shield, @quality, @itemlevel, flags, @material_metal, @bind_on_pickup, @sell_price, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3);
END//

DELIMITER ;

-- Weapons
CALL create_two_hand_axe(@base_item_weapon, "Initiate's Battleaxe", '', @base_item_weapon, @flags_default, 0, 3, 0, 3, 0, 3, 0);
CALL create_two_hand_sword(@base_item_weapon+1, "Initiate's Greatsword", '', @base_item_weapon+1, @flags_default, 0, 3, 0, 3, 0, 3, 0);
CALL create_two_hand_mace(@base_item_weapon+2, "Initiate's Hammer", '', @base_item_weapon+2, @flags_default, 0, 3, 0, 3, 0, 3, 0);
CALL create_one_hand_dagger(@base_item_weapon+3, "Initiate's Dagger", '', @base_item_weapon+3, @flags_default, 0, 3, 0, 3, 0, 3, 0);
call create_bow(@base_item_weapon+4, "Initiate's Shortbow", '', @base_item_weapon+4, @flags_default, 0, 3, 0, 3, 0, 3, 0);
call create_one_hand_mace(@base_item_weapon+5, "Initiate's Mace", '', @base_item_weapon+5, @flags_default, 0, 3, 0, 3, 0, 3, 0);
call create_one_hand_sword(@base_item_weapon+6, "Initiate's Shortsword", '', @base_item_weapon+6, @flags_default, 0, 3, 0, 3, 0, 3, 0);
call create_crossbow(@base_item_weapon+7, "Initiate's Crossbow", '', @base_item_weapon+7, @flags_default, 0, 3, 0, 3, 0, 3, 0);
call create_wand(@base_item_weapon+8, "Initiate's Wand", '', @base_item_weapon+8, @flags_default, 0, 3, 0, 3, 0, 3, 0);
call create_one_hand_axe(@base_item_weapon+9, "Initiate's Axe", '', @base_item_weapon+9, @flags_default, 0, 3, 0, 3, 0, 3, 0);
call create_staff(@base_item_weapon+10, "Initiate's Staff", '', @base_item_weapon+10, @flags_default, 0, 3, 0, 3, 0, 3, 0);
call create_shield(@base_item_weapon+11, "Initiate's Wooden Shield", '', @base_item_weapon+11, @flags_default, 0, 3, 0, 3, 0, 3, 0);
call create_gun(@base_item_weapon+12, "Initiate's Blunderbuss", '', @base_item_weapon+12, @flags_default, 0, 3, 0, 3, 0, 3, 0);

/*
* Warrior
*/
call create_shoulders(@base_item_armor, "Initiate's Plate Pauldrons", '', @subclass_mail, @base_item_armor, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);
call create_chest(@base_item_armor+1, "Initiate's Plate Chestpiece", '', @subclass_mail, @base_item_armor+1, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);
call create_bracers(@base_item_armor+2, "Initiate's Plate Wristguards", '', @subclass_mail, @base_item_armor+2, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);
call create_gloves(@base_item_armor+3, "Initiate's Plate Gauntlets", '', @subclass_mail, @base_item_armor+3, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);
call create_belt(@base_item_armor+4, "Initiate's Plate Girdle", '', @subclass_mail, @base_item_armor+4, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);
call create_legs(@base_item_armor+5, "Initiate's Plate Leggguards", '', @subclass_mail, @base_item_armor+5, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);
call create_boots(@base_item_armor+6, "Initiate's Plate Greaves", '', @subclass_mail, @base_item_armor+6, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);

/*
* Paladin
*/
call create_shoulders(@base_item_armor+7, "Initiate's Scaled Pauldrons", '', @subclass_mail, @base_item_armor+7, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);
call create_chest(@base_item_armor+8, "Initiate's Scaled Chestpiece", '', @subclass_mail, @base_item_armor+8, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);
call create_bracers(@base_item_armor+9, "Initiate's Scaled Wristguards", '', @subclass_mail, @base_item_armor+9, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);
call create_gloves(@base_item_armor+10, "Initiate's Scaled Gauntlets", '', @subclass_mail, @base_item_armor+10, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);
call create_belt(@base_item_armor+11, "Initiate's Scaled Girdle", '', @subclass_mail, @base_item_armor+11, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);
call create_legs(@base_item_armor+12, "Initiate's Scaled Legguards", '', @subclass_mail, @base_item_armor+12, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);
call create_boots(@base_item_armor+13, "Initiate's Scaled Greaves", '', @subclass_mail, @base_item_armor+13, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);

/*
* Hunter
*/
call create_shoulders(@base_item_armor+14, "Initiate's Chain Spaulders", '', @subclass_leather, @base_item_armor+14, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);
call create_chest(@base_item_armor+15, "Initiate's Chain Armor", '', @subclass_leather, @base_item_armor+15, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);
call create_bracers(@base_item_armor+16, "Initiate's Chain Wristguards", '', @subclass_leather, @base_item_armor+16, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);
call create_gloves(@base_item_armor+17, "Initiate's Chain Gauntlets", '', @subclass_leather, @base_item_armor+17, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);
call create_belt(@base_item_armor+18, "Initiate's Chain Links", '', @subclass_leather, @base_item_armor+18, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);
call create_legs(@base_item_armor+19, "Initiate's Chain Legguards", '', @subclass_leather, @base_item_armor+19, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);
call create_boots(@base_item_armor+20, "Initiate's Chain Sabatons", '', @subclass_leather, @base_item_armor+20, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);

/*
* Rogue
*/
call create_shoulders(@base_item_armor+21, "Initiate's Leather Spaulders", '', @subclass_leather, @base_item_armor+21, @flags_default, @material_leather, 0, 3, 0, 3, 0, 3, 0);
call create_chest(@base_item_armor+22, "Initiate's Leather Tunic", '', @subclass_leather, @base_item_armor+22, @flags_default, @material_leather, 0, 3, 0, 3, 0, 3, 0);
call create_bracers(@base_item_armor+23, "Initiate's Leather Wristguards", '', @subclass_leather, @base_item_armor+23, @flags_default, @material_leather, 0, 3, 0, 3, 0, 3, 0);
call create_gloves(@base_item_armor+24, "Initiate's Leather Gloves", '', @subclass_leather, @base_item_armor+24, @flags_default, @material_leather, 0, 3, 0, 3, 0, 3, 0);
call create_belt(@base_item_armor+25, "Initiate's Leather Waistband", '', @subclass_leather, @base_item_armor+25, @flags_default, @material_leather, 0, 3, 0, 3, 0, 3, 0);
call create_legs(@base_item_armor+26, "Initiate's Leather Legguards", '', @subclass_leather, @base_item_armor+26, @flags_default, @material_leather, 0, 3, 0, 3, 0, 3, 0);
call create_boots(@base_item_armor+27, "Initiate's Leather Boots", '', @subclass_leather, @base_item_armor+27, @flags_default, @material_leather, 0, 3, 0, 3, 0, 3, 0);

/*
* Priest
*/
call create_shoulders(@base_item_armor+28, "Initiate's Satin Mantle", '', @subclass_cloth, @base_item_armor+28, @flags_default, @material_cloth, 0, 3, 0, 3, 0, 3, 0);
call create_robe(@base_item_armor+29, "Initiate's Satin Tunic", '', @subclass_cloth, @base_item_armor+29, @flags_default, @material_cloth, 0, 3, 0, 3, 0, 3, 0);
call create_bracers(@base_item_armor+30, "Initiate's Satin Cuffs", '', @subclass_cloth, @base_item_armor+30, @flags_default, @material_cloth, 0, 3, 0, 3, 0, 3, 0);
call create_gloves(@base_item_armor+31, "Initiate's Satin Gloves", '', @subclass_cloth, @base_item_armor+31, @flags_default, @material_cloth, 0, 3, 0, 3, 0, 3, 0);
call create_belt(@base_item_armor+32, "Initiate's Satin Cord", '', @subclass_cloth, @base_item_armor+32, @flags_default, @material_cloth, 0, 3, 0, 3, 0, 3, 0);
call create_legs(@base_item_armor+33, "Initiate's Satin Leggings", '', @subclass_cloth, @base_item_armor+33, @flags_default, @material_cloth, 0, 3, 0, 3, 0, 3, 0);
call create_boots(@base_item_armor+34, "Initiate's Satin Treads", '', @subclass_cloth, @base_item_armor+34, @flags_default, @material_cloth, 0, 3, 0, 3, 0, 3, 0);

/*
* Shaman
*/
call create_shoulders(@base_item_armor+35, "Initiate's Mail Spaulders", '', @subclass_leather, @base_item_armor+35, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);
call create_robe(@base_item_armor+36, "Initiate's Mail Armor", '', @subclass_leather, @base_item_armor+36, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);
call create_bracers(@base_item_armor+37, "Initiate's Mail Cuffs", '', @subclass_leather, @base_item_armor+37, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);
call create_gloves(@base_item_armor+38, "Initiate's Mail Gauntlets", '', @subclass_leather, @base_item_armor+38, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);
call create_belt(@base_item_armor+39, "Initiate's Mail Waistguard", '', @subclass_leather, @base_item_armor+39, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);
call create_legs(@base_item_armor+40, "Initiate's Mail Leggings", '', @subclass_leather, @base_item_armor+40, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);
call create_boots(@base_item_armor+41, "Initiate's Mail Sabatons", '', @subclass_leather, @base_item_armor+41, @flags_default, @material_chain, 0, 3, 0, 3, 0, 3, 0);

/*
* Mage
*/
call create_shoulders(@base_item_armor+42, "Initiate's Silk Amice", '', @subclass_cloth, @base_item_armor+42, @flags_default, @material_cloth, 0, 3, 0, 3, 0, 3, 0);
call create_robe(@base_item_armor+43, "Initiate's Silk Tunic", '', @subclass_cloth, @base_item_armor+43, @flags_default, @material_cloth, 0, 3, 0, 3, 0, 3, 0);
call create_bracers(@base_item_armor+44, "Initiate's Silk Cuffs", '', @subclass_cloth, @base_item_armor+44, @flags_default, @material_cloth, 0, 3, 0, 3, 0, 3, 0);
call create_gloves(@base_item_armor+45, "Initiate's Silk Handguards", '', @subclass_cloth, @base_item_armor+45, @flags_default, @material_cloth, 0, 3, 0, 3, 0, 3, 0);
call create_belt(@base_item_armor+46, "Initiate's Silk Cord", '', @subclass_cloth, @base_item_armor+46, @flags_default, @material_cloth, 0, 3, 0, 3, 0, 3, 0);
call create_legs(@base_item_armor+47, "Initiate's Silk Trousers", '', @subclass_cloth, @base_item_armor+47, @flags_default, @material_cloth, 0, 3, 0, 3, 0, 3, 0);
call create_boots(@base_item_armor+48, "Initiate's Silk Treads", '', @subclass_cloth, @base_item_armor+48, @flags_default, @material_cloth, 0, 3, 0, 3, 0, 3, 0);

/*
* Warlock
*/
call create_shoulders(@base_item_armor+49, "Initiate's Felweave Amice", '', @subclass_cloth, @base_item_armor+49, @flags_default, @material_cloth, 0, 3, 0, 3, 0, 3, 0);
call create_robe(@base_item_armor+50 , "Initiate's Felweave Tunic", '', @subclass_cloth, @base_item_armor+50, @flags_default, @material_cloth, 0, 3, 0, 3, 0, 3, 0);
call create_bracers(@base_item_armor+51, "Initiate's Felweave Cuffs", '', @subclass_cloth, @base_item_armor+51, @flags_default, @material_cloth, 0, 3, 0, 3, 0, 3, 0);
call create_gloves(@base_item_armor+52, "Initiate's Felweave Handguards", '', @subclass_cloth, @base_item_armor+52, @flags_default, @material_cloth, 0, 3, 0, 3, 0, 3, 0);
call create_belt(@base_item_armor+53, "Initiate's Felweave Cord", '', @subclass_cloth, @base_item_armor+53, @flags_default, @material_cloth, 0, 3, 0, 3, 0, 3, 0);
call create_legs(@base_item_armor+54, "Initiate's Felweave Trousers", '', @subclass_cloth, @base_item_armor+54, @flags_default, @material_cloth, 0, 3, 0, 3, 0, 3, 0);
call create_boots(@base_item_armor+55, "Initiate's Felweave Treads", '', @subclass_cloth, @base_item_armor+55, @flags_default, @material_cloth, 0, 3, 0, 3, 0, 3, 0);

/*
* Druid
*/
call create_shoulders(@base_item_armor+56, "Initiate's Dragonhide Spaulders", '', @subclass_leather, @base_item_armor+56, @flags_default, @material_leather, 0, 3, 0, 3, 0, 3, 0);
call create_robe(@base_item_armor+57, "Initiate's Dragonhide Robes", '', @subclass_leather, @base_item_armor+57, @flags_default, @material_leather, 0, 3, 0, 3, 0, 3, 0);
call create_bracers(@base_item_armor+58, "Initiate's Dragonhide Bindings", '', @subclass_leather, @base_item_armor+58, @flags_default, @material_leather, 0, 3, 0, 3, 0, 3, 0);
call create_gloves(@base_item_armor+59, "Initiate's Dragonhide Gloves", '', @subclass_leather, @base_item_armor+59, @flags_default, @material_leather, 0, 3, 0, 3, 0, 3, 0);
call create_belt(@base_item_armor+60, "Initiate's Dragonhide Belt", '', @subclass_leather, @base_item_armor+60, @flags_default, @material_leather, 0, 3, 0, 3, 0, 3, 0);
call create_legs(@base_item_armor+61, "Initiate's Dragonhide Legguards", '', @subclass_leather, @base_item_armor+61, @flags_default, @material_leather, 0, 3, 0, 3, 0, 3, 0);
call create_boots(@base_item_armor+62, "Initiate's Dragonhide Footguards", '', @subclass_leather, @base_item_armor+62, @flags_default, @material_leather, 0, 3, 0, 3, 0, 3, 0);


-- Class containers

/*
* Warrior
*/
-- DELETE FROM `item_template` WHERE `entry` = @base_item_container;
-- INSERT INTO `item_template` (`entry`, `class`, `subclass`, `SoundOverrideSubclass`, `name`, `displayid`, `Quality`, `Flags`, `FlagsExtra`, `BuyCount`, `BuyPrice`, `SellPrice`, `InventoryType`, `AllowableClass`, `AllowableRace`, `ItemLevel`, `RequiredLevel`, `RequiredSkill`, `RequiredSkillRank`, `requiredspell`, `requiredhonorrank`, `RequiredCityRank`, `RequiredReputationFaction`, `RequiredReputationRank`, `maxcount`, `stackable`, `ContainerSlots`, `StatsCount`, `stat_type1`, `stat_value1`, `stat_type2`, `stat_value2`, `stat_type3`, `stat_value3`, `stat_type4`, `stat_value4`, `stat_type5`, `stat_value5`, `stat_type6`, `stat_value6`, `stat_type7`, `stat_value7`, `stat_type8`, `stat_value8`, `stat_type9`, `stat_value9`, `stat_type10`, `stat_value10`, `ScalingStatDistribution`, `ScalingStatValue`, `dmg_min1`, `dmg_max1`, `dmg_type1`, `dmg_min2`, `dmg_max2`, `dmg_type2`, `armor`, `holy_res`, `fire_res`, `nature_res`, `frost_res`, `shadow_res`, `arcane_res`, `delay`, `ammo_type`, `RangedModRange`, `spellid_1`, `spelltrigger_1`, `spellcharges_1`, `spellppmRate_1`, `spellcooldown_1`, `spellcategory_1`, `spellcategorycooldown_1`, `spellid_2`, `spelltrigger_2`, `spellcharges_2`, `spellppmRate_2`, `spellcooldown_2`, `spellcategory_2`, `spellcategorycooldown_2`, `spellid_3`, `spelltrigger_3`, `spellcharges_3`, `spellppmRate_3`, `spellcooldown_3`, `spellcategory_3`, `spellcategorycooldown_3`, `spellid_4`, `spelltrigger_4`, `spellcharges_4`, `spellppmRate_4`, `spellcooldown_4`, `spellcategory_4`, `spellcategorycooldown_4`, `spellid_5`, `spelltrigger_5`, `spellcharges_5`, `spellppmRate_5`, `spellcooldown_5`, `spellcategory_5`, `spellcategorycooldown_5`, `bonding`, `description`, `PageText`, `LanguageID`, `PageMaterial`, `startquest`, `lockid`, `Material`, `sheath`, `RandomProperty`, `RandomSuffix`, `block`, `itemset`, `MaxDurability`, `area`, `Map`, `BagFamily`, `TotemCategory`, `socketColor_1`, `socketContent_1`, `socketColor_2`, `socketContent_2`, `socketColor_3`, `socketContent_3`, `socketBonus`, `GemProperties`, `RequiredDisenchantSkill`, `ArmorDamageModifier`, `duration`, `ItemLimitCategory`, `HolidayId`, `ScriptName`, `DisenchantID`, `FoodType`, `minMoneyLoot`, `maxMoneyLoot`, `flagsCustom`, `VerifiedBuild`) VALUES
-- (@entry_id, 0, 0, -1, "Initiate\'s Equipment", @base_item_container, @quality, 4, 0, 1, 0, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, @description, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, '', 0, 3, 0, 3, 0, 3, 0);

-- DELETE FROM `item_loot_template` WHERE `entry` = @entry_id;
-- INSERT INTO `item_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
-- (@entry_id, @dk_reagent_1, 0, @percent, 0, 1, 0, 1, 1, '');

/*
* Paladin
*/