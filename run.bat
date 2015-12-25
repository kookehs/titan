@echo off

set RootDirectory=%~dp0
set RootDirectory=%RootDirectory:\=/%

pushd "%RootDirectory%bin/"
if exist "titan.exe" start titan.exe
popd
