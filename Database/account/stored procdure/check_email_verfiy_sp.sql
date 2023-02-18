SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Description:	check_email_verify(이메일 체크)
--
-- Params
-- @global_id				: 글로벌 아이디
-- @verify					: 이메일 인증 번호
-- =============================================
USE account_database;
DROP PROCEDURE IF EXISTS check_email_verify_sp;
GO

CREATE PROCEDURE check_email_verify_sp
	@global_id		INT,
	@verify			INT
AS
BEGIN TRY
	BEGIN TRANSACTION
		SET NOCOUNT ON;

		--이메일 인증 중복 확인
		IF (SELECT verify FROM email_table WHERE global_id=@global_id) = 1
			BEGIN
				ROLLBACK TRANSACTION;
				RETURN 1005
			END

		--이메일 인증 확인
		IF (SELECT temp_verify FROM email_table WHERE global_id=@global_id) <> @verify
			BEGIN
				ROLLBACK TRANSACTION;
				RETURN 1006
			END

		--인증 번호 맞음
		UPDATE email_table SET verify=1 WHERE global_id=@global_id
		UPDATE email_table SET temp_verify=NULL WHERE global_id=@global_id 
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
	USE account_database;

	--UPDATE email_table SET verify=0 WHERE global_id=2

	EXEC dbo.check_email_verify_sp 3, '901139'

	SELECT * FROM account_table
	SELECT * FROM email_table
	SELECT * FROM information_table
END
GO