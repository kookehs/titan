@echo off

set RootDirectory=%~dp0
set RootDirectory=%RootDirectory:\=/%

set DllName=titan
set DllSourceFiles=character.cpp enemy.cpp hashmap.cpp list.cpp misc.cpp physics.cpp quadtree.cpp titan.cpp
set DllExportFunctions=/EXPORT:api

set ExecutableName=win32_titan
set ExecutableSourceFiles=root.cpp

set CommonCompilerFlags=/FAs /FC /MDd /nologo /Od /Oi /WX /Zi /I%RootDirectory%include/
set CommonLinkerFlags=/DEBUG /INCREMENTAL:NO /LIBPATH:%RootDirectory%lib/ csfml-audio.lib csfml-graphics.lib csfml-network.lib csfml-system.lib csfml-window.lib

if not exist "%RootDirectory%bin/" mkdir "%RootDirectory%bin/"

pushd "%RootDirectory%src/titan/core/"
copy * "%RootDirectory%bin/"
popd

pushd "%RootDirectory%src/titan/utility/"
copy * "%RootDirectory%bin/"
popd

echo ________________________________________________________________________

pushd "%RootDirectory%bin/"

if exist *.asm del *.asm
if exist *.exp del *.exp
if exist %DllName% del %DllName%
if exist *.i del *.i
if exist *.ilk del *.ilk
if exist *.lib del *.lib
if exist *.map del *.map
if exist *.obj del *.obj
if exist *.pdb del *.pdb > NUL 2> NUL

cl %CommonCompilerFlags% /LDd /Fm%DllName%.map /Fe%DllName%.dll %DllSourceFiles% /link /PDB:%DllName%_%random%.pdb %CommonLinkerFlags% %DllExportFunctions%
echo ________________________________________________________________________
tasklist /fi "imagename eq %ExecutableName%.exe" |find /I "%ExecutableName%.exe" || cl %CommonCompilerFlags% -Fe%ExecutableName%.exe %ExecutableSourceFiles% /link /INCREMENTAL:NO

if exist *.cpp del *.cpp
popd
