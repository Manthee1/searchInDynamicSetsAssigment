@echo off
setlocal enabledelayedexpansion enableextensions
@REM varibles TARGET_EXEC, TARGET_LIB, TARGET_DLL, TARGET_LIB_DLL, TARGET_LIB_DLL_STATIC, TARGET_LIB_STATIC, TARGET_DLL_STATIC


set TARGET_EXEC=searchInDSA.exe
set SRC_DIR=src
set BUILD_DIR=bin
set BUILD_SRC_DIR=bin\win_src
set CURRENT_DIR=%cd%
set CXX=g++

set LIBS=
set CXXFLAGS=-g -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-field-initializers

set SRCS=

@REM Create the windows build directory
mkdir %BUILD_DIR% > nul 2>&1
mkdir %BUILD_SRC_DIR% > nul 2>&1

@REM Check if the user wants to compile the gui or not
@REM Check if %1 is empty
if [%1]==[] goto :nogui
if %1 == clean goto :clean
if %1 == nogui goto :nogui
if %1 == gui goto :gui

echo Invalid argument %1
echo Usage: "build.bat [32|64] [gui|nogui|clean]"
echo If no argument is specified, It assumes 'nogui'
goto :end


:nogui

dir /b /s %SRC_DIR%\*.cpp | findstr -v gui > %BUILD_SRC_DIR%\SRCS 
set CXXFLAGS=%CXXFLAGS% -DNO_GUI
goto :precompile

:gui


if [%2]==[] (
    echo You must specify the architecture of the g++ compiler to build the gui
    goto :architecture-error
)
if %2 == 32 goto :continue-gui
if %2 == 64 goto :continue-gui

echo Invalid argument architecture '%1'

:architecture-error
@REM Figure out which architecture the g++ compiler is
set ARCH=
for /f "tokens=1,2 delims= " %%a in ('g++ -v 2^>^&1 ^| findstr Target') do (
    set ARCH=%%b
)
echo Usage: "build.bat gui [32|64]"
echo Architecture hint: %ARCH%
goto :end

:continue-gui
@REM Set the architecture specific variables. Essentailly just append 32 or 64 to the end of the library folder
set LIBFOLDER=%SRC_DIR%\gui\lib\GLFW\lib-mingw-w%2
if not exist %LIBFOLDER% (
    echo The library folder %LIBFOLDER% does not exist
    goto :end
)
dir /b /s %SRC_DIR%\*.cpp > %BUILD_SRC_DIR%\SRCS
@REM set CXXFLAGS=%CXXFLAGS%  -L%SRC_DIR%\gui\lib\GLFW\lib-mingw-w64\glfw3.lib
set LIBS=-L%LIBFOLDER% -lglfw3 -lm -lopengl32 -lgdi32 -lwinmm
goto :precompile

:precompile
@REM Get all the subdirectories of the SRC directory
set INCLUDE_FLAGS=
set NEWDIR=
for /f "delims=" %%i in ('dir /b /s %SRC_DIR%') do (
    if exist %%i\*.cpp (
        @REM Set NEWDIR to the current directory
        set NEWDIR=%NEWDIR%%%~i
        @REM Remove the CURRENT_DIR from NEWDIR
        set NEWDIR=!NEWDIR:%CURRENT_DIR%\%SRC_DIR%\=!

        mkdir %BUILD_SRC_DIR%\!NEWDIR! > nul 2>&1
        set INCLUDE_FLAGS=!INCLUDE_FLAGS! -I%SRC_DIR%\!NEWDIR!
    )
)

@REM If SRCS_TIME does not exist, create it
if not exist %BUILD_SRC_DIR%\SRCS_TIME (
    echo.> %BUILD_SRC_DIR%\SRCS_TIME
)

@REM Compile all the source files
set NEWOBJ=
set OBJS=
set CUR_TIME=
for /f "tokens=1,* delims=:" %%a in ('findstr /n "^" "%BUILD_SRC_DIR%\SRCS"') do (

    @REM Get the file path and current time of the source file
    set FILE_PATH=%%b
    set CUR_TIME=%%~tb

    @REM Remove the CURRENT_DIR from NEWOBJ
    set NEWOBJ=%NEWOBJ%%%~b.o
    set NEWOBJ=!NEWOBJ:%CURRENT_DIR%\%SRC_DIR%\=!
    set "OBJS=!OBJS! %BUILD_SRC_DIR%\!NEWOBJ!"

    set /a line=%%a+1 
    (for /L %%i in (1,1,!line!) do set /P SAVED_TIME=) <%BUILD_SRC_DIR%\SRCS_TIME

    @REM Check if the source file has been modified
    if not "!SAVED_TIME!"=="!CUR_TIME!" (
        echo Compiling %%~b
        %CXX% %CXXFLAGS% %INCLUDE_FLAGS% -c "%%~b" -o "%CURRENT_DIR%\%BUILD_SRC_DIR%\!NEWOBJ!"
        echo %CXX% %CXXFLAGS% %INCLUDE_FLAGS% -c "%%~b" -o "%CURRENT_DIR%\%BUILD_SRC_DIR%\!NEWOBJ!"
    )
)

:updatetime
@REM Update the modified time of the source files
echo.> %BUILD_SRC_DIR%\SRCS_TIME
for /f "delims=" %%i in (%BUILD_SRC_DIR%\SRCS) do (
    echo %%~ti>> %BUILD_SRC_DIR%\SRCS_TIME
)

:link
@REM Link all the object files
echo %CXX% %CXXFLAGS% %INCLUDE_FLAGS% %OBJS% -o %BUILD_DIR%\%TARGET_EXEC% %LIBS%
%CXX% %CXXFLAGS% %INCLUDE_FLAGS% %OBJS% -o %BUILD_DIR%\%TARGET_EXEC% %LIBS%
goto :end

:clean
@REM Clean the build directory
rmdir /s /q %BUILD_SRC_DIR%


:end