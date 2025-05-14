@echo off

if exist "..\..\tools" (
    cd "..\..\"
)
rd /S /Q build
rd /s /Q bin
