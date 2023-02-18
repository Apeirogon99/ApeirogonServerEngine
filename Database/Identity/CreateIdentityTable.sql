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