@echo off

set flags=

if "%2"=="sdl2" set flags=-lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

gcc examples\%1.c %flags% -o exc.exe && .\exc.exe