@echo on

set CUR_DIR="%~dp0"
mkdir "%CUR_DIR%\obj"

copy "%CUR_DIR%\ModInfo.xml" "%CUR_DIR%\obj\"

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x86

devenv "%~dp0\SporeCreationZIPPorter\SporeCreationZIPPorter.sln" /Build "Release DLL"

copy "%programdata%\SporeModManagerStorage\mLibs\SporeCreationZIPPorter.dll" "%CUR_DIR%\obj\"

smfx pack "%~dp0\SMFX\CreationZIPPorter-AssetExportConfig"  "%~dp0\obj\!CreationZIPPorter-AssetExportConfig.package"

smfx pack "%~dp0\SMFX\CreationZIPPorter-CreationAutoDownload"  "%~dp0\obj\!CreationZIPPorter-CreationAutoDownload.package"

cd "%CUR_DIR%\obj\"

"C:\Program Files\7-Zip\7z" a -tzip %CUR_DIR%\SporeCreationZIPPorter.sporemod *

cd "%CUR_DIR%"

rmdir /s /q "%CUR_DIR%\obj\"