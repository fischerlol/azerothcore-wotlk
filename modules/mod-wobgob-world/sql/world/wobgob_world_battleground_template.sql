-- Update required players to start each bg
UPDATE `battleground_template` SET `MinPlayersPerTeam` = `MaxPlayersPerTeam` / 5;