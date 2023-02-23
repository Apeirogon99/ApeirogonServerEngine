SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Description:	load_character_sp(캐릭터 불러오기)
--
-- Params
-- @global_id			: 유저 글로벌 아이디
-- @server_id			: 유저 서버 아이디
-- =============================================
USE game_database;
DROP PROCEDURE IF EXISTS load_character_sp;
GO

CREATE PROCEDURE load_character_sp
	@global_id		INT,
	@server_id		INT
AS
BEGIN TRY
	BEGIN TRANSACTION
		SET NOCOUNT ON;

		DECLARE @character_id AS INT;

		IF NOT EXISTS (SELECT 1 FROM character_table WHERE global_id=@global_id AND server_id=@server_id)
			BEGIN
				ROLLBACK TRANSACTION;
				RETURN 2003
			END

		--커스텀 정보 및 위치정보
		SELECT name, class, tribe, level, position, body_color, hair_color, eye_color, head, ears, feet, hair, facials_01, facials_02, helmet, shoulders, skirt, legs, legs_add, hands, hands_add, chest, chest_add, cape, bracers, bracers_add, boots, belt, tabard, back_2hl, back_shield, back_weapon_l, back_weapon_r, back_bow, quiver, weapon_r_arrow, weapon_shield, weapon_l, weapon_r, hip_l, hip_r FROM character_table AS character INNER JOIN appearance_table AS custom ON (character.id = custom.character_id) WHERE character.global_id=@global_id AND character.server_id=@server_id
		COMMIT TRANSACTION;
		RETURN 0

END TRY
BEGIN CATCH
	ROLLBACK TRANSACTION;
	RETURN -1
END CATCH
GO

--TEST
BEGIN
	USE game_database;
	--EXEC dbo.load_character_sp 37, 1
END
GO