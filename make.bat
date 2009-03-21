@echo off

:: This is just a little helper batch file to allow you to build/clean the
:: project with `make' and `make clean' like in Linux (after symlinking a
:: Makefile, that is).
if [%1] == [-f] (
    mingw32-make %*
) else (
    mingw32-make -f Makefile.bam %*
)

