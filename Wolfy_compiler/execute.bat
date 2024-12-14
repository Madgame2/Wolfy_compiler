@echo off

rem Настраиваем среду через vcvars32.bat
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat"
if %errorlevel% neq 0 (
    echo Error setting up environment!
    pause
    exit /b %errorlevel%
)

set assembler=ml.exe
set linker=link.exe
set flags=/c /coff
set linker_flags=/subsystem:console

rem Текущая директория
set CURRENT_DIR=%CD%

rem Компиляция
echo compiling
%assembler% %flags% "File.wolf.asm" "MAIN.asm" 
if %errorlevel% neq 0 (
    pause
    exit /b %errorlevel%
)

rem Линковка
echo linking object files
%linker% %linker_flags% "File.wolf.obj" "MAIN.obj" "WolfyConsoleLib.lib" "Wolfy_standart_lib.lib" /out:main.exe
if %errorlevel% neq 0 (
    echo Linking error!
    pause
    exit /b %errorlevel%
)

echo compiling finished
pause
