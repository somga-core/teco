# Navigation
- [GUI](#gui)
  - [Variables](#variables)
  - [Functions](#functions)

# GUI
GUI part (`teco_gui.cpp` and `teco_gui.hpp`) is needed for window input and output. It's handled mainly by `SDL2` lib. Using this graphics type allows you to use `effects` system

## Variables
- `std::map<char, std::vector<float> (*) (int, int, int)> effects` - lookup table of `effects`, where key is `char` and value is function with arguments: x position of symbol, y position of symbol and tick count, that must return `vector` with 4 elements: relative x position in symbols, relative y position in symbols (0 for no symbol offset), width of symbol, height of symbol (1 for default symbol scale)
- `int window_width` - width of window in pixels
- `int window_height` - height of window in pixels
- `TTF_Font *font` - font, used to display text

## Functions
- `init_gui` - initialize all systems needed for GUI input and output
  - `int _window_width` - will be directlry assigned to `window_width`. Default - `640`
  - `int _window_height` - will be directlry assigned to `window_height`. Default - `480`
  - `std::string font_path` - path to the font. Default - `"assets/font.ttf"`
  - `int font_size` - size of the font. Default - `20`
  - `std::map<char, std::vector<float> (*) (int, int, int)> _effects` - will be directlry assigned to `effects`. Default - empty `map`

- `mainloop_gui` - start main loop with all GUI systems

- `exit_gui` - safely exit the programm when window is showing