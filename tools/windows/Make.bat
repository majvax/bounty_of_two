@echo off

setlocal
call :setESC


echo %ESC%[92m -^> Generating Makefile Windows... %ESC%[0m

if exist "..\..\tools" (
    echo %ESC%[91m -^> Warning Running script from the wrong directory, changing to root dir%ESC%[0m
    cd "..\..\"
) 

if exist "tools" (
    echo %ESC%[92m -^> Found "tools" directory%ESC%[0m
    set "PREMAKE_PATH=tools\windows\premake5.exe"
) else (
    echo %ESC%[91m -^> Error: "tools" directory does not exist.%ESC%[0m
    exit /b 1
)

if exist "%PREMAKE_PATH%" (
    echo %ESC%[92m -^> Found premake5 executable at %cd%\%PREMAKE_PATH%%ESC%[0m
) else (
    echo %ESC%[91m -^> Error: premake5 executable not found at %cd%\%PREMAKE_PATH%%ESC%[0m
    exit /b 1
)

"%PREMAKE_PATH%" gmake

:setESC
for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do rem"') do (
  set ESC=%%b
  exit /B 0
)
exit /B 0
