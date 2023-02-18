SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Description:	이메일 인증
--
-- Params
-- @global_id		: 글로벌 아이디
-- @verfiy			: 인증키
-- =============================================
USE IdentityDB;
DROP PROCEDURE IF EXISTS email_verfiy;
GO

CREATE PROCEDURE email_verfiy
	@global_id		NVARCHAR(24),
	@in_verify		NVARCHAR(24)

AS
BEGIN TRY
	BEGIN TRANSACTION
		SET NOCOUNT ON;
		DECLARE @temp_verify AS NVARCHAR(6);

		SELECT @temp_verify=temp_verify FROM dbo.user_email WHERE global_id=@global_id; 

		IF (@temp_verify=@in_verify)
			BEGIN
				UPDATE user_email SET verify = 1;
			END
		ELSE
			BEGIN
				RETURN 1;
			END

		COMMIT;
		RETURN 1;
END TRY
BEGIN CATCH
	ROLLBACK;
	RETURN 0;
END CATCH
GO