# Navigation
- [Compilation](#compilation)
  - [TeCo files](#teco-files)
  - [Libs](#libs)
  - [Makefile](#makefile)

# Compilation
## TeCo files
Besides your project files, you also need co compile [`teco_engine.cpp`](/teco/teco_engine.cpp) and [`teco_gui.cpp`](/teco/teco_gui.cpp) if you are using GUI or [`teco_tui.cpp`](/teco/teco_tui.cpp) if you are using TUI

## Libs
You need to compile your project with different graphics-type-dependend libs:
- If you are using GUI, you must install `SLD2` and `SDL2_ttf`
- If you are using TUI, you must install `ncurses`

## Makefile
If you feel lazy about writing commands yourself, you can use `Makefile` template in `teco` folder