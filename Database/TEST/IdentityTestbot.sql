USE IdentityDB;
GO

DROP TABLE IF EXISTS user_info;
DROP TABLE IF EXISTS user_email;
DROP TABLE IF EXISTS user_identity;
CREATE TABLE user_identity
(
	global_id		INT IDENTITY(1, 1) NOT NULL,
	id				NVARCHAR(24) NOT NULL,
	password		NVARCHAR(24) NOT NULL,

	PRIMARY KEY(global_id),
	UNIQUE (id)
);

CREATE TABLE user_email
(
	global_id		INT NOT NULL,
	local			NVARCHAR(64) NOT NULL,
	domain			NVARCHAR(255) NOT NULL,
	verify			INT DEFAULT 0 CHECK (verify = 1 OR verify = 0),

	PRIMARY KEY (global_id),
);

CREATE TABLE user_info
(
	global_id		INT NOT NULL,
	confirm			INT DEFAULT 0 CHECK (confirm = 1 OR confirm = 0),
	create_date		SMALLDATETIME NOT NULL,
	update_date		SMALLDATETIME NOT NULL,

	PRIMARY KEY (global_id),
);
GO

SET NOCOUNT ON
DECLARE @max_num AS INT = 5000;
DECLARE @cur_index AS INT = IDENT_CURRENT('user_identity');

BEGIN TRY
	BEGIN TRANSACTION
	WHILE @cur_index <= @max_num
		BEGIN

		DECLARE @cur_id AS NVARCHAR = CAST(IDENT_CURRENT('user_identity') AS NVARCHAR)
		DECLARE @id AS NVARCHAR(24)= 'TEST_' + RIGHT('0000' + @cur_id, 4)
		DECLARE @password AS NVARCHAR(24)='1234';
		DECLARE @local AS NVARCHAR(64)= 'TEST_' + RIGHT('0000' + @cur_id, 4)
		DECLARE @domain AS NVARCHAR(255)='example.com';
		DECLARE @output AS INT;

		EXEC [dbo].[singup] @id, @password,@local, @domain, @output OUTPUT
		SET @cur_index = @cur_index + 1;

		END;

		SELECT * FROM user_identity;
		COMMIT TRANSACTION;

END TRY
BEGIN CATCH
	print('생성 불가능')
	ROLLBACK TRANSACTION;
END CATCH

GO