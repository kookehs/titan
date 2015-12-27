@echo off

set RootDirectory=%~dp0
set RootDirectory=%RootDirectory:\=/%

pushd "%RootDirectory%bin/"
if exist "win32_titan.exe" devenv win32_titan.exe
popd
