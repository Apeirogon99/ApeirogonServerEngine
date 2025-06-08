#pragma once

//==========================//
//		   DataBase			//
//==========================//

#import "C:\Program Files (x86)\Common Files\System\ado\msado15.dll" no_namespace rename("EOF", "adoEOF")
#include "./Database/msado15.tlh"
#include <oledb.h>

#include "Database/ADOVariant.h"
#include "Database/ADOUtils.h"
#include "Database/ADOConnection.h"
#include "Database/ADORecordset.h"
#include "Database/ADOCommand.h"
#include "Database/ADOEvent.h"
#include "Database/ADOTask.h"
#include "Database/DatabaseManager.h"