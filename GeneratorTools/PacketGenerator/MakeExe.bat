pushd %~dp0
pyinstaller --onefile PacketGenerator.py
MOVE .\dist\PacketGenerator.exe ..\..\ProtobufMake\bin\GenPacketsHandler.exe
XCOPY .\Templates\*.h ..\..\ProtobufMake\bin\Templates /Y
@RD /S /Q .\build
@RD /S /Q .\dist
DEL /S /F /Q .\PacketGenerator.spec
PAUSE