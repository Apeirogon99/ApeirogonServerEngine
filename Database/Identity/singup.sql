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
-- @email_local		: 유저 이메일 로컬
-- @email_domain	: 유저 이메일 도메인
-- =============================================
USE IdentityDB;
DROP PROCEDURE IF EXISTS singup;
GO

CREATE PROCEDURE singup
	@id				NVARCHAR(24),
	@password		NVARCHAR(24),
	@local			NVARCHAR(64),
	@domain			NVARCHAR(255),
	@output			INT OUTPUT
AS
BEGIN TRY
	BEGIN TRANSACTION
		SET NOCOUNT ON;

		DECLARE @cur_date AS SMALLDATETIME = CAST(GETUTCDATE() AS SMALLDATETIME);
		INSERT INTO user_identity (id, password) VALUES (@id, @password);
		DECLARE @cur_ident AS INT = @@IDENTITY
		INSERT INTO user_email(global_id, local, domain) VALUES (@cur_ident, @local, @domain);
		INSERT INTO user_info (global_id, create_date, update_date) VALUES (@cur_ident, @cur_date, @cur_date);
		SET @output=0;
		COMMIT;
END TRY
BEGIN CATCH
	SET @output=-1;
	print('FAILURE');
	ROLLBACK;
END CATCH
GO

--TEST SUCCESS
DELETE FROM user_info WHERE global_id=5001;
DELETE FROM user_email WHERE global_id=5001;
DELETE FROM user_identity WHERE global_id=5001;

DECLARE @cur_id AS NVARCHAR = CAST(IDENT_CURRENT('user_identity') AS NVARCHAR)

DECLARE @id AS NVARCHAR(24)= 'TEST_' + RIGHT('0000' + @cur_id, 4)
DECLARE @password AS NVARCHAR(24)='1234';
DECLARE @local AS NVARCHAR(64)= 'TEST_' + RIGHT('0000' + @cur_id, 4)
DECLARE @domain AS NVARCHAR(255)='example.com';
DECLARE @output AS INT;

--EXEC [dbo].[singup] @id, @password,@local, @domain, @output OUTPUT
--SELECT @output AS OUTPUT

EXEC [dbo].[singup] 'LGW1', '1234','LGW1', 'example.com', @output OUTPUT
SELECT @output AS OUTPUT
GO

SELECT * FROM user_identity
SELECT * FROM user_email
SELECT * FROM user_info