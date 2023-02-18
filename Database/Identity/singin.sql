SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Description:	singup(회원가입)
--
-- Params
-- @id				: 유저 아이디
-- @password		: 유저 패스워드
-- =============================================
USE IdentityDB;
DROP PROCEDURE IF EXISTS singin;
GO

CREATE PROCEDURE singin
	@id				NVARCHAR(24),
	@password		NVARCHAR(24),
	@global_id		INT OUTPUT,
	@output			INT OUTPUT
AS
BEGIN TRY
	BEGIN TRANSACTION
		SET NOCOUNT ON;
		DECLARE @temp_global_id AS INT;
		DECLARE @temp_password AS NVARCHAR(24);
		SELECT @temp_global_id=global_id, @temp_password=password FROM user_identity WHERE id=@id; 

		IF (@temp_password=@password)
			BEGIN
				SET @global_id=@temp_global_id;
				SET @output=1;
				print('SUCCESS')
			END
		ELSE
			BEGIN
				SET @global_id=-1
				SET @output=0;
				print('FAILURE')
			END

		COMMIT;
END TRY
BEGIN CATCH
	SET @output=-1;
	ROLLBACK;
END CATCH
GO


--TEST SUCCESS
DECLARE @id AS NVARCHAR(24)='TEST_1234';
DECLARE @password AS NVARCHAR(24)='1234';
DECLARE @global_id AS INT;
DECLARE @output AS INT;

EXEC [dbo].[singin] @id, @password, @global_id OUTPUT, @output OUTPUT
SELECT @global_id AS GLOBAL, @output AS OUTPUT
GO

--TEST FAILED
DECLARE @id AS NVARCHAR(24)='FAILED';
DECLARE @password AS NVARCHAR(24)='1234';
DECLARE @global_id AS INT;
DECLARE @output AS INT;

EXEC [dbo].[singin] @id, @password, @global_id OUTPUT, @output OUTPUT
SELECT @global_id AS GLOBAL, @output AS OUTPUT
GO