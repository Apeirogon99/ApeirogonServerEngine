USE account_database

DROP TABLE IF EXISTS email_table;
DROP TABLE IF EXISTS information_table;
DROP TABLE IF EXISTS account_table;
GO

CREATE TABLE account_table
(
	global_id		INT IDENTITY(1, 1) NOT NULL PRIMARY KEY,
	id				NVARCHAR(24) NOT NULL UNIQUE,
	password		NVARCHAR(24) NOT NULL,
	ticket			UNIQUEIDENTIFIER NULL,
)
GO

CREATE TABLE email_table
(
	global_id			INT NOT NULL REFERENCES account_table(global_id),
	local				NVARCHAR(64) NOT NULL,
	domain				NVARCHAR(255) NOT NULL,
	verify				INT DEFAULT 0 CHECK (verify = 1 OR verify = 0),
	temp_verify			INT NULL,
)
GO

CREATE TABLE information_table
(
	global_id		INT NOT NULL REFERENCES account_table(global_id),
	confirm			INT DEFAULT 0 CHECK (confirm = 1 OR confirm = 0),
	create_date		SMALLDATETIME NOT NULL,
	update_date		SMALLDATETIME NOT NULL,
)
GO