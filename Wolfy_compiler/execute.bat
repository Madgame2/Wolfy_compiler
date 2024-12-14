@echo off
set assembler=ml.exe
set linker=link.exe
set flags=/c /coff
set linker_flags=/subsystem:console

set VCToolsInstallDir=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.31.31103\
set WindowsSdkDir=C:\Program Files (x86)\Windows Kits\10\
set UCRTVersion=10.0.19041.0

rem Устанавливаем пути для 32-битных библиотек
set LIB=%VCToolsInstallDir%lib\x86
set SDK_LIB=%WindowsSdkDir%Lib\%UCRTVersion%\um\x86
set UCRT_LIB=%WindowsSdkDir%Lib\%UCRTVersion%\ucrt\x86

rem Печатаем значения переменных
echo VCToolsInstallDir=%VCToolsInstallDir%
echo WindowsSdkDir=%WindowsSdkDir%
echo UCRTVersion=%UCRTVersion%

rem ���������� ���� � ������� ���������� ��� ���������
set CURRENT_DIR=%CD%

rem ����������
echo compiling
%assembler% %flags% File.wolf.asm MAIN.asm 
if %errorlevel% neq 0 (
pause
exit /b %errorlevel%
)

rem ��������
echo linking object files
%linker% %linker_flags% /LIBPATH:"%LIB%" /LIBPATH:"%SDK_LIB%" /LIBPATH:"%UCRT_LIB%" /LIBPATH:"%CURRENT_DIR%" File.wolf.obj MAIN.obj WolfyConsoleLib.lib Wolfy_standart_lib.lib kernel32.lib /out:main.exe
if %errorlevel% neq 0 (
echo Linking error!
pause
exit /b %errorlevel%
)

echo compiling finished
pause