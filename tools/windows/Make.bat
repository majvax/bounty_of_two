@echo off
echo Generating GNU Make project files...

cd ..\..\

set "PREMAKE_PATH=tools\windows\premake5.exe"

if exist "%PREMAKE_PATH%" (
    echo Found premake5 executable at %PREMAKE_PATH%
) else (
    echo Error: premake5 executable not found at %PREMAKE_PATH%
    exit /b 1
)

"%PREMAKE_PATH%" gmake

