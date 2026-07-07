@echo off
setlocal
where g++ >nul 2>nul
if errorlevel 1 (
    echo g++ not found. Please install MinGW or MSYS2 and ensure g++ is on PATH.
    exit /b 1
)
g++ src\main.cpp src\snake_game.cpp -Iinclude -o snake.exe
if errorlevel 1 (
    echo Build failed.
    exit /b 1
)
echo Build succeeded.
