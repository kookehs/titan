@echo off

set RootDirectory=%~dp0
set RootDirectory=%RootDirectory:\=/%

set DllMapName=titan.map
set DllName=titan.dll
set DllSourceFiles=titan.cpp
set DllExportFunctions=/EXPORT:game_init /EXPORT:game_update

set ExecutableName=titan.exe
set ExecutableSourceFiles=root.cpp player_handler.cpp

set CommonCompilerFlags=/FAs /FC /MTd /nologo /WX /Zi /I%RootDirectory%include/
set CommonLinkerFlags=/INCREMENTAL:NO /LIBPATH:%RootDirectory%lib/ csfml-audio.lib csfml-graphics.lib csfml-network.lib csfml-system.lib csfml-window.lib

if not exist "%RootDirectory%bin/" mkdir "%RootDirectory%bin/"

pushd "%RootDirectory%src/core/"
copy * "%RootDirectory%bin/"
popd

pushd "%RootDirectory%bin/"

if exist *.i del *.i
if exist *.pdb del *.pdb
if exist *.map del *.map
if exist %DllName% del %DllName%
if exist *.exp del *.exp
if exist *.ilk del *.ilk
if exist *.obj del *.obj
if exist *.lib del *.lib

cl %CommonCompilerFlags% /LD /Fm%DllMapName% /Fe%DllName% %DllSourceFiles% /link %CommonLinkerFlags% %DllExportFunctions%
tasklist /fi "imagename eq %ExecutableName%" |find /I "%ExecutableName%" || cl %CommonCompilerFlags% -Fe%ExecutableName% %ExecutableSourceFiles% /link %CommonLinkerFlags%

if exist *.cpp del *.cpp
popd
