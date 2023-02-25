@echo off
pushd %~dp0
SET Project_LD_Server_PATH=P:\Project_LD_Server

rem copy include header files
XCOPY /F "./*.h" ".\..\Builds\include\apeirogon" /Y
XCOPY /F ".\..\Builds\include\apeirogon" "%Project_LD_Server_PATH%\Builds\include\apeirogon" /Y
IF ERRORLEVEL 1 PAUSE

rem copy lib files
XCOPY ".\..\Builds\release\ApeirogonLibrary.dll" "%Project_LD_Server_PATH%\Builds\release" /Y
XCOPY ".\..\Builds\release\ApeirogonLibrary.exp" "%Project_LD_Server_PATH%\Builds\release" /Y
XCOPY ".\..\Builds\release\ApeirogonLibrary.lib" "%Project_LD_Server_PATH%\Builds\release" /Y
XCOPY ".\..\Builds\release\ApeirogonLibrary.pdb" "%Project_LD_Server_PATH%\Builds\release" /Y
						
XCOPY ".\..\Builds\debug\ApeirogonLibrary.dll" "%Project_LD_Server_PATH%\Builds\debug" /Y
XCOPY ".\..\Builds\debug\ApeirogonLibrary.exp" "%Project_LD_Server_PATH%\Builds\debug" /Y
XCOPY ".\..\Builds\debug\ApeirogonLibrary.lib" "%Project_LD_Server_PATH%\Builds\debug" /Y
XCOPY ".\..\Builds\debug\ApeirogonLibrary.pdb" "%Project_LD_Server_PATH%\Builds\debug" /Y						
IF ERRORLEVEL 1 PAUSE