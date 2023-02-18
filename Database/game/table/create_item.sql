USE game_database

DROP TABLE IF EXISTS item_table;
GO

CREATE TABLE item_table
(
	id					INT NOT NULL IDENTITY(0,1) PRIMARY KEY,
	type				INT NOT NULL,
	name				NVARCHAR(32) NOT NULL,
	cost				INT NOT NULL,
	minimum_level		INT NOT NULL,
)
GO

INSERT INTO item_table VALUES (0, 'µ¹', 3, 0);

SELECT * FROM item_table;