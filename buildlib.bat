@echo off

REM Searching for a toolset. Preferring the newest

if defined VS140COMNTOOLS (
    set VCVARSALLPATH= "%VS140COMNTOOLS%..\..\VC\vcvarsall"
) else if defined VS120COMNTOOLS (
    set VCVARSALLPATH= "%VS120COMNTOOLS%..\..\VC\vcvarsall"
) else if defined VS110COMNTOOLS (
    set VCVARSALLPATH= "%VS110COMNTOOLS%..\..\VC\vcvarsall"
) else if defined VS100COMNTOOLS (
    set VCVARSALLPATH= "%VS100COMNTOOLS%..\..\VC\vcvarsall"
) else if defined VS90COMNTOOLS (
    set VCVARSALLPATH= "%VS90COMNTOOLS%..\..\VC\vcvarsall"
)

if not defined VCVARSALLPATH (
    echo No build system been found, abort.
    exit /b 1
)

if not defined DevEnvDir (call %VCVARSALLPATH%)

REM =============================================

set LIBNAME=aurora

REM Collect all source files
setlocal enableextensions enabledelayedexpansion
set SOURCES=
for /F %%A in ('dir /b /S *.cpp *.c *.res') do set SOURCES=!SOURCES! "%%A"

set INCLUDE_PATHS= ^
/I C:\MyDocuments\Projects\Aurora\include ^
/I C:\MyDocuments\Projects\Aurora\external

if exist build.txt (
    set /p BUILDINDEX=<build.txt
    set BUILDDIR=lib\!BUILDINDEX!
) else (
    set BUILDDIR=lib
)


mkdir %BUILDDIR%
mkdir obj
pushd obj
cl %INCLUDE_PATHS% ^
/D "_UNICODE" ^
/D "UNICODE" ^
/GS ^
/GL ^
/analyze- ^
/W3 ^
/Gy ^
/Zc:wchar_t ^
/EHsc ^
/MT ^
/WX- ^
/Zc:forScope ^
/Gd ^
/Oy- ^
/Oi ^
/Gm- ^
/O2 ^
/nologo ^
/c ^
%SOURCES%

REM Collect all obj files
set OBJS=
for /F %%A in ('dir /b /S *.obj') do set OBJS=!OBJS! "%%A"

lib /OUT:..\lib\%LIBNAME%.lib %OBJS% OpenGL32.lib /LTCG

REM remember! you can do this to get rid of thousands of dependencies
REM lib /OUT:..\lib\%LIBNAME%.lib  ..\lib\_%LIBNAME%.lib ..\external\soil.lib opengl32.lib

if exist ..\build.txt (
    if %ERRORLEVEL% EQU 0 (
        popd
        set /a BUILDINDEX=!BUILDINDEX!+1
        >build.txt echo !BUILDINDEX!
    ) else (
        popd
    )
) else (
    popd
)