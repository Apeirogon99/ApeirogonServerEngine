pushd %~dp0
pyinstaller --onefile PacketGenerator.py
MOVE .\dist\PacketGenerator.exe ..\..\ProtobufMake\bin\GenPacketHandler\GenPacketsHandler.exe
XCOPY .\Templates\*.h ..\..\ProtobufMake\bin\GenPacketHandler\Templates /Y
@RD /S /Q .\build
@RD /S /Q .\dist
DEL /S /F /Q .\PacketGenerator.spec
PAUSE