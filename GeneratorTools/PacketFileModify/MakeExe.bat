pushd %~dp0
pyinstaller --onefile PacketFileModify.py
MOVE .\dist\PacketFileModify.exe ..\..\Protobuf\Generator\bin\PacketFileModify.exe
@RD /S /Q .\build
@RD /S /Q .\dist
DEL /S /F /Q .\PacketFileModify.spec
PAUSE