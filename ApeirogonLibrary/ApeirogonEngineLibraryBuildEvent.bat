@echo off
pushd %~dp0
SET Project_MMORPG2D_PATH=P:\Project_LD_Server

rem delete file
DEL /Q /F "%Project_MMORPG2D_PATH%\ThirdParty\Apeirogon\include\Apeirogon\*.h"
rem DEL /Q /F "%Project_MMORPG2D_PATH%\Builds\include\apeirogon\*.cpp"
DEL /Q /F ".\..\Builds\include\apeirogon\*.h"
rem DEL /Q /F ".\..\Builds\include\apeirogon\*.cpp"
IF ERRORLEVEL 1 PAUSE

rem copy include header files
ROBOCOPY "./" ".\..\Builds\include\apeirogon" "*.h" /S
rem XCOPY /F /S "./*.h" ".\..\Builds\include\apeirogon" /Y
rem XCOPY /F "./*.cpp" ".\..\Builds\include\apeirogon" /Y
XCOPY /F /E ".\..\Builds\include\apeirogon\*" "%Project_MMORPG2D_PATH%\ThirdParty\Apeirogon\include\Apeirogon" /Y
IF ERRORLEVEL 1 PAUSE

rem copy lib files
XCOPY ".\..\Builds\release\ApeirogonLibrary.dll" "%Project_MMORPG2D_PATH%\Builds\Release\Client" /Y
XCOPY ".\..\Builds\release\ApeirogonLibrary.dll" "%Project_MMORPG2D_PATH%\Builds\Release\Server" /Y
XCOPY ".\..\Builds\release\ApeirogonLibrary.exp" "%Project_MMORPG2D_PATH%\ThirdParty\Apeirogon\bin\Release" /Y
XCOPY ".\..\Builds\release\ApeirogonLibrary.lib" "%Project_MMORPG2D_PATH%\ThirdParty\Apeirogon\lib\Release" /Y
XCOPY ".\..\Builds\release\ApeirogonLibrary.pdb" "%Project_MMORPG2D_PATH%\ThirdParty\Apeirogon\bin\Release" /Y
						
XCOPY ".\..\Builds\debug\ApeirogonLibrary.dll" "%Project_MMORPG2D_PATH%\Builds\Debug\Client" /Y
XCOPY ".\..\Builds\debug\ApeirogonLibrary.dll" "%Project_MMORPG2D_PATH%\Builds\Debug\Server" /Y
XCOPY ".\..\Builds\debug\ApeirogonLibrary.exp" "%Project_MMORPG2D_PATH%\ThirdParty\Apeirogon\bin\Debug" /Y
XCOPY ".\..\Builds\debug\ApeirogonLibrary.lib" "%Project_MMORPG2D_PATH%\ThirdParty\Apeirogon\lib\Debug" /Y
XCOPY ".\..\Builds\debug\ApeirogonLibrary.pdb" "%Project_MMORPG2D_PATH%\ThirdParty\Apeirogon\bin\Debug" /Y						
IF ERRORLEVEL 1 PAUSE