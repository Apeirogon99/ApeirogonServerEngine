USE game_database

DROP TABLE IF EXISTS character_table;
DROP TABLE IF EXISTS appearance_table;
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

CREATE TABLE appearance_table
(
	character_id		INT NOT NULL PRIMARY KEY,
	body_color			INT NOT NULL,
	hair_color			INT NOT NULL,
	eye_color			INT NOT NULL,
	
	head				INT	NULL DEFAULT 0,
	ears				INT	NULL DEFAULT 0,
	feet				INT NULL DEFAULT 0,
	hair				INT NULL DEFAULT 0,
	facials_01			INT NULL DEFAULT 0,
	facials_02			INT NULL DEFAULT 0,
	helmet				INT NULL DEFAULT 0,
	shoulders			INT NULL DEFAULT 0,
	skirt				INT NULL DEFAULT 0,
	legs				INT NULL DEFAULT 0,
	legs_add			INT NULL DEFAULT 0,
	hands				INT NULL DEFAULT 0,
	hands_add			INT NULL DEFAULT 0,
	chest				INT NULL DEFAULT 0,
	chest_add			INT NULL DEFAULT 0,
	cape				INT NULL DEFAULT 0,
	bracers				INT NULL DEFAULT 0,
	bracers_add			INT NULL DEFAULT 0,
	boots				INT NULL DEFAULT 0,
	belt				INT NULL DEFAULT 0,
	tabard				INT NULL DEFAULT 0,

	back_2hl			INT NULL DEFAULT 0,
	back_shield			INT NULL DEFAULT 0,
	back_weapon_l		INT NULL DEFAULT 0,
	back_weapon_r		INT NULL DEFAULT 0,
	back_bow			INT NULL DEFAULT 0,
	quiver				INT NULL DEFAULT 0,
	weapon_r_arrow		INT NULL DEFAULT 0,
	weapon_shield		INT NULL DEFAULT 0,
	weapon_l			INT NULL DEFAULT 0,
	weapon_r			INT NULL DEFAULT 0,
	hip_l				INT NULL DEFAULT 0,
	hip_r				INT NULL DEFAULT 0,
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