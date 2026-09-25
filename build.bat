@echo off
setlocal

set "MSYS2=C:\msys64"
set "UCRT64=%MSYS2%\ucrt64\bin"
set "MSYSBIN=%MSYS2%\usr\bin"
set "PATH=%UCRT64%;%MSYSBIN%;%PATH%"

set "MAKE=%UCRT64%\mingw32-make.exe"
set "TASKS=3"

echo (1/%TASKS%) Setting up MSYS2 UCRT64 environment...
if not exist "%UCRT64%\g++.exe" (
    echo Error: g++.exe was not found.
    pause
    exit /b 1
)

echo (2/%TASKS%) Building outputproject.exe...
"%MAKE%"
if errorlevel 1 (
    echo Build failed.
    pause
    exit /b 1
)

echo (3/%TASKS%) Build completed successfully.
echo outputproject.exe is ready.

outputproject.exe

pause