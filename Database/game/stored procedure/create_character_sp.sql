SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Description:	create_character_sp(새로운 캐릭터 생성)
--
-- Params
-- @name				: 유저 닉네임
-- @class				: 유저 클래스
-- @position			: 위치
-- @global_id			: 유저 글로벌 아이디
-- @server_id			: 유저 서버 아이디
--
-- @skin				: 피부색
-- @hair				: 머리색
-- @eye					: 눈색
-- @eyebrow				: 눈섭색
-- =============================================
USE game_database;
DROP PROCEDURE IF EXISTS create_character_sp;
GO

CREATE PROCEDURE create_character_sp
	@name			NVARCHAR(10),
	@class			INT,
	@position		INT,
	@tribe			INT,
	@global_id		INT,
	@server_id		INT,

	@body_color		INT,
	@hair_color		INT,
	@eye_color		INT,
	
	@head			INT,
	@ears			INT,
	@feet			INT,
	@hair			INT,
	@facials_02		INT,
	@legs			INT,
	@hands			INT,
	@chest			INT,
	@bracers		INT,
	@boots			INT

AS
BEGIN TRY
	BEGIN TRANSACTION
		SET NOCOUNT ON;

		-- 초과된 캐릭터 수를 소유중
		DECLARE @current_character_count AS INT;
		DECLARE @charcater_max_count AS INT;
		DECLARE @level AS INT = 1;

		SET @charcater_max_count = 3;
		SELECT @current_character_count=COUNT(*) FROM character_table WHERE global_id=@global_id AND server_id=@server_id;
		IF (@current_character_count > @charcater_max_count)
			BEGIN
				ROLLBACK TRANSACTION
				RETURN 2001
			END

		-- 위치 중복
		IF EXISTS (SELECT 1 FROM character_table WHERE global_id=@global_id AND server_id=@server_id AND position=@position)
			BEGIN
				ROLLBACK TRANSACTION
				RETURN 2004
			END

		-- 닉네임 중복
		IF EXISTS (SELECT 1 FROM character_table WHERE name=@name)
			BEGIN
				ROLLBACK TRANSACTION
				RETURN 2003
			END
		
		--캐릭터 생성
		DECLARE @character_id AS INT
		INSERT INTO character_table VALUES (@name, @class, @tribe, @level, @position, @global_id, @server_id);
		SELECT @character_id=id FROM character_table WHERE global_id=@global_id AND name=@name;

		INSERT INTO appearance_table (character_id, body_color, hair_color, eye_color, head, ears, feet, hair, facials_02, legs, hands, chest, bracers, boots) 
		VALUES (@character_id, @body_color, @hair_color, @eye_color, @head, @ears, @feet, @hair, @facials_02, @legs, @hands, @chest, @bracers, @boots);
		COMMIT TRANSACTION
		RETURN 0

END TRY
BEGIN CATCH
	ROLLBACK TRANSACTION
	RETURN -1
END CATCH
GO

--TEST
BEGIN
	DECLARE @test_character_id AS INT;
	DECLARE @count AS INT;

	--EXEC dbo.create_character_sp '하이요', 1, 0, 1, 1, 0, 0, 0, 0
	--EXEC dbo.create_character_sp 'name1', 1, 1, 1, 1, 0, 0, 0, 0
	--EXEC dbo.create_character_sp 'name2', 1, 2, 1, 1, 0, 0, 0, 0

	--EXEC dbo.create_character_sp 'name3', 1, 0, 38, 1, 0, 0, 0, 0
	--EXEC dbo.create_character_sp 'name4', 1, 1, 38, 1, 0, 0, 0, 0
	--EXEC dbo.create_character_sp 'name5', 1, 2, 38, 1, 0, 0, 0, 0


	SELECT * FROM character_table
	SELECT * FROM appearance_table

	SELECT @count=COUNT(*) FROM character_table;
	
	--WHILE (@count > 0)
	--	BEGIN
	--		SELECT @test_character_id=id FROM character_table
	--		DELETE FROM dbo.character_table WHERE id=@test_character_id
	--		DELETE FROM dbo.custom_table WHERE character_id=@test_character_id
	--		SET @count = @count -1
	--	END
	----
	--SELECT * FROM character_table
	--SELECT * FROM custom_table
END
GO