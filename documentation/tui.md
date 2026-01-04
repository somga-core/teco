# Navigation
- [TUI](#tui)
  - [Functions](#functions)

# TUI
TUI part (`teco_tui.cpp` and `teco_tui.hpp`) is needed for terminal input and output. It's handled mainly by `NCurses` lib. `effects` don't affect this graphics type

## Functions
- `init_tui` - initialize all systems needed for TUI input and output

- `mainloop_tui` - start main loop with all TUI systems

- `exit_tui` - safely exit the programm when printing in terminal

