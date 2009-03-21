@echo off

if [%MINGDIR%] == [] (
    echo Please create the MINGDIR environment variable, pointing to your MinGW
	echo installation.
	echo.
    exit /b -1
)

:: This is just a little helper batch file to allow you to build/clean the
:: project with `make' and `make clean' like in Linux (after symlinking a
:: Makefile, that is).
if exist "%MINGDIR%\bin\mingw32-make.exe" (
    if [%1] == [-f] (
        "%MINGDIR%\bin\mingw32-make" %* WINDOWS=YES
    ) else (
        "%MINGDIR%\bin\mingw32-make" -f Makefile.bam %* WINDOWS=YES
    )
) else (
    if [%1] == [-f] (
        "%MINGDIR%\bin\make" %* WINDOWS=YES
    ) else (
        "%MINGDIR%\bin\make" -f Makefile.bam %* WINDOWS=YES
    )
)
