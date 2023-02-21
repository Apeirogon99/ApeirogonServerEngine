@echo off
pushd %~dp0
SET COMMON_PLUGIN_PATH=P:\Project_LD\Plugins\CommonLibrary\Source\ThirdParty\CommonLibrary

rem copy include header files
XCOPY /F "./../CommonLibrary/*.h" "%COMMON_PLUGIN_PATH%\include" /Y
IF ERRORLEVEL 1 PAUSE

rem copy lib files
XCOPY ".\..\ApeirogonBuilds\Release\CommonLibrary.lib" "%COMMON_PLUGIN_PATH%\release\lib" /Y
XCOPY ".\..\ApeirogonBuilds\Release\CommonLibrary.pdb" "%COMMON_PLUGIN_PATH%\release\bin" /Y
						   
XCOPY ".\..\ApeirogonBuilds\Debug\CommonLibrary.lib" "%COMMON_PLUGIN_PATH%\debug\lib" /Y
XCOPY ".\..\ApeirogonBuilds\Debug\CommonLibrary.pdb" "%COMMON_PLUGIN_PATH%\debug\bin" /Y
XCOPY ".\..\ApeirogonBuilds\Debug\CommonLibrary.idb" "%COMMON_PLUGIN_PATH%\debug\bin" /Y
IF ERRORLEVEL 1 PAUSE