SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Description:	sp_test(테스트 전용)
--
-- Params
-- @input
-- =============================================
USE IdentityDB;
DROP PROCEDURE IF EXISTS sp_test;
GO

CREATE PROCEDURE sp_test
	@input			NVARCHAR(24) = ''
AS
BEGIN TRY
	BEGIN TRANSACTION
		SET NOCOUNT ON;

		COMMIT;
END TRY
BEGIN CATCH
	ROLLBACK;
END CATCH
GO


--TEST SUCCESS
DECLARE @input AS NVARCHAR(24)='TEST_1234';
