pushd %~dp0
pyinstaller --onefile PacketGenerator.py
MOVE .\dist\PacketGenerator.exe ..\..\Protobuf\Generator\bin\GenPacketsHandler.exe
XCOPY .\Templates\*.h ..\..\Protobuf\Generator\bin\Templates /Y
@RD /S /Q .\build
@RD /S /Q .\dist
DEL /S /F /Q .\PacketGenerator.spec
PAUSE