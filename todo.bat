@echo off

set RootDirectory=%~dp0
set RootDirectory=%RootDirectory:\=/%

@echo.
@echo -----------
@echo.

set Wildcard=*.c *.cpp *.h *.hpp

@echo TODOS FOUND:
findstr /S /N /I /L /D:%RootDirectory% "TODO" %Wildcard%

@echo.
@echo -----------
@echo.
