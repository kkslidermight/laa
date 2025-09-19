@echo off
REM laa.bat — mark a PE file as Large Address Aware
REM Usage: drag-and-drop an EXE onto this batch file, or run from cmd

if "%~1"=="" (
    echo Usage: %~nx0 file.exe
    pause
    exit /b 1
)

REM Call the compiled laa.exe in the same folder as this script
"%~dp0laa.exe" "%~1"
pause
