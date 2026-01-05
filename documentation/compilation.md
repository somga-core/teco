# Navigation
- [Compilation](#compilation)
  - [TeCo files](#teco-files)
  - [Libs](#libs)
  - [Makefile](#makefile)

# Compilation
## TeCo files
Besides your project files, you also need co compile `teco.cpp` and `teco_gui.cpp` if you are using GUI or `teco_tui.cpp` if you are using TUI

## Libs
You need to compile your project with connection of different graphics-type-dependend libs:
- If you are using GUI, you must connect `SLD2` and `SDL2_ttf`
- If you are using TUI, you must connect `ncurses`

## Makefile
If you feel lazy about writing commands yourself, you can use `Makefile` template in `teco` folder