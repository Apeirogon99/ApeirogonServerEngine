USE game_database

DROP TABLE IF EXISTS class_table;
GO


CREATE TABLE class_table
(
	id					INT NOT NULL IDENTITY(0,1) PRIMARY KEY,
	
	physical_attack		FLOAT NOT NULL,
	magical_attack		FLOAT NOT NULL,

	physical_defenses	FLOAT NOT NULL,
	magical_defenses	FLOAT NOT NULL,

	critical_hit		FLOAT NOT NULL,
	critical_percent	FLOAT NOT NULL,

	hit_point			FLOAT NOT NULL,
	magic_point			FLOAT NOT NULL,

	attack_speed		FLOAT NOT NULL,
	travel_speed		FLOAT NOT NULL,
)
INSERT INTO class_table VALUES (10, 3, 20 , 8, 10, 15, 300, 100, 1, 100); 
GO

SELECT * FROM class_table