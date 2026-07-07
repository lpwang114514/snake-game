@echo off
if exist snake.exe (
    snake.exe
) else (
    echo Executable not found. Please build the project first.
    exit /b 1
)
