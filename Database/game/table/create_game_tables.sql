USE game_database

DROP TABLE IF EXISTS character_table;
DROP TABLE IF EXISTS custom_table;
DROP TABLE IF EXISTS inventory_table;
DROP TABLE IF EXISTS game_table;
GO

CREATE TABLE character_table
(
	id					INT NOT NULL IDENTITY(0,1) PRIMARY KEY,
	name				NVARCHAR(10) NOT NULL,
	class				INT NOT NULL,
	tribe				INT NOT NULL,
	level				INT NOT NULL,
	position			INT NOT NULL,
	global_id			INT NOT NULL,
	server_id			INT NOT NULL,
)
GO

CREATE TABLE custom_table
(
	character_id		INT NOT NULL PRIMARY KEY,
	skin				INT NOT NULL,
	hair				INT NOT NULL,
	eye					INT NOT NULL,
	eyebrow				INT NOT NULL,
)
GO

CREATE TABLE inventory_table
(
	character_id		INT NOT NULL,
	quantity			INT NOT NULL,
	item_id				INT NOT NULL
)
GO

CREATE TABLE game_table
(
	character_id		INT NOT NULL PRIMARY KEY,
)
GO