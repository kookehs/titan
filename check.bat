@echo off

set RootDirectory=%~dp0
set RootDirectory=%RootDirectory:\=/%

if not exist "%RootDirectory%/logs/" mkdir "%RootDirectory%/logs/"

pushd "%RootDirectory%bin/"
drmemory.exe -brief -logdir %RootDirectory%/logs/ -- titan.exe
popd
