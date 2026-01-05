# Navigation
- [Quck start](#quick-start)
  - [Includes](#includes)
  - [Initialization](#initialization)
  - [ASCII images](#ascii-images)
  - [Screens](#screens)
  - [Sources](#sources)
  - [Sprites](#sprites)
    - [Animations](#animations)
  - [Tick and draw in mainloop](#tick-and-draw-in-mainloop)
  - [Keybinds](#keybinds)
  - [Compilation](#compilation)

# Quick start
To start using `TeCo` you need to copy it's engine files into your project. They are located in [`teco`](/teco/) folder. You always want to copy [`teco_engine.cpp`](/teco/teco_engine.cpp) and [`teco_engine.hpp`](/teco/teco_engine.hpp)

If you want to use window input and output (Next, GUI) you must copy [`teco_gui.cpp`](/teco/teco_gui.cpp) and [`teco_gui.hpp`](/teco/teco_gui.hpp)

If you want to use terminal input and output (Next, TUI) you must copy [`teco_tui.cpp`](/teco/teco_tui.cpp) and [`teco_tui.hpp`](/teco/teco_tui.hpp)

## Includes
You must include into your project `TeCo` files to use it (Obviously). You always need to include [`teco_engine.hpp`](/teco/teco_engine.hpp). Include [`teco_gui.hpp`](/teco/teco_gui.hpp) if you want to use GUI or [`teco_tui.hpp`](/teco/teco_tui.hpp) if you want to use TUI

So your includes must look something like that:

```c++
#include "teco_engine.hpp"
#include "teco_gui.hpp" // Or "teco_tui.hpp"

// ∅
```

## Initialization
Then you need to run init functions. Always run `init()`. Use `init_gui()` for GUI and `init_tui()` for TUI:

```c++
#include "teco_engine.hpp"
#include "teco_gui.hpp" // Or "teco_tui.hpp"

int main() {
    teco::init();
    teco::init_gui(); // Or teco::init_tui();

    return 0;
}
```

In `init()` you must specify starting screen (More on that later) and also can specify, FPS and TPS (More on that later), keybinds (More on that later) and a few color options (More on that later):

```c++
// ∅

teco::init(screen, 60, 20, keybinds, colors, default_color, background_red, background_green, background_blue);

// ∅
```

In `init_gui()` you can specify default window width and height, path to font and it's size, as well as effects lookup table (More on that later):

```c++
// ∅

teco::init_gui(600, 400, "path/to/font.ttf", 20, effects);

// ∅
```

## ASCII images
`TeCo` ASCII image (Used in `Sources` and `Screens`) is stored using `symbols`, `colors` and `effects`:
- `symbols` represent which symbol would be displayed
- `colors` specify in which color from lookup table symbol would be painted. Colors are vectors with 3 elements: red, green and blue channels
- `effects` specify which effect would be applied on symbol. They work only in GUI. Effects are functions with arguments: x position of symbol, y position of symbol and tick count, that must return `vector` with 4 elements: relative x position in symbols, relative y position in symbols (0 for no symbol offset), width of symbol, height of symbol (1 for default symbol scale)

`colors` lookup table can be specified in `init()` or by setting `colors` variable, `effects` lookup table can be specified in `init_gui()` or by setting `effects` variable:

```c++
// ∅

teco::init(..., ..., ..., ..., colors);
teco::init_gui(..., ..., ..., ..., effects);

// ∅
```

Or:

```c++
// ∅

teco::init(...);
teco::init_gui(..);

teco::colors = ...;
teco::effects = ...;

// ∅
```

Here is default `colors` lookup table:

```c++
teco::colors = std::map<char, std::vector<unsigned char>> {
		{'0', std::vector<unsigned char> {229, 229, 229}},
		{'1', std::vector<unsigned char> {160, 160, 160}},
		{'2', std::vector<unsigned char> {10, 162, 146}},
		{'3', std::vector<unsigned char> {0, 133, 102}},
		{'4', std::vector<unsigned char> {165, 89, 177}},
		{'5', std::vector<unsigned char> {102, 0, 153}},
		{'6', std::vector<unsigned char> {42, 111, 189}},
		{'7', std::vector<unsigned char> {19, 68, 125}},
		{'8', std::vector<unsigned char> {209, 148, 23}},
		{'9', std::vector<unsigned char> {178, 94, 13}},
		{'A', std::vector<unsigned char> {135, 173, 0}},
		{'B', std::vector<unsigned char> {94, 117, 0}},
		{'C', std::vector<unsigned char> {171, 0, 0}},
		{'D', std::vector<unsigned char> {127, 0, 31}},
		{'E', std::vector<unsigned char> {103, 103, 103}},
		{'F', std::vector<unsigned char> {0, 0, 0}}
}
```

And here is example `effects`:

```c++
std::vector<float> some_effect(int x, int y, int tick_count);
std::vector<float> other_effect(int x, int y, int tick_count);

// ∅

teco::effects = std::map<char, std::vector<float> (*) (int, int, int)> {
  {'#', some_effect},
  {'&', other_effect}
}
```

You can find effect examples in [`examples/effect_switcher`](/examples/effect_switcher)

In addition to `colors` lookup table in `init()` you need to pass `default_color`, which is key in `colors` lookup table of color, that will be applied to symbol wihout specified color and `background_red`, `background_green`, `background_blue` which are channels of background color

## Screens
In `init` function you must pass `current_screen` argument. It has `Screen` type. It's a 2D plane that will be displayed on window or in terminal and on which you will be drawing symbols

To init `Screen` you need to specify width and height as well as tick and draw functions (More on that later):

```c++
#include "teco_engine.hpp"
#include "teco_gui.hpp" // Or "teco_tui.hpp"

void tick() {};
void draw() {};

teco::Screen screen;

int main() {
    teco::init(screen);
    teco::init_gui(); // Or teco::init_tui();

    screen = teco::Screen(48, 14, &tick, &draw);

    return 0;
}
```

You can also init several `Screens` and switch between them later:

```c++
#include "teco_engine.hpp"
#include "teco_gui.hpp" // Or "teco_tui.hpp"

void main_tick() {};
void main_draw() {};

void sec_tick() {};
void sec_draw() {};

teco::Screen main_screen;
teco::Screen sec_screen;

int main() {
    teco::init(main_screen);
    teco::init_gui(); // Or teco::init_tui();
    
    main_screen = teco::Screen(48, 14, &tick, &draw);
    sec_screen = teco::Screen(48, 14, &tick, &draw);

    // ∅

    teco::current_screen = sec_screen; // Switching to different screen

    return 0;
}
```

## Sources
To init `Source` you need to specify path to files with `symbols`, `colors` and `effects`. Extentions for them are: `.tcsb`, `.tccl`, `tcef` respectively

For example, to display this image:

```
 o
(|)
/ \
```

Wully painted blue (Using default lookup table) and with `some_effect` from example earlier, you need to create theese files:

`source.tcsb`:
```
 o
(|)
/ \
```

`source.tccl`:
```
 7
777
7 7
```

`source.tcef`:
```
 #
###
# #
```

`some_cpp_file.cpp`:
```c++
// ∅

teco::Source source = teco::Source("source.tcsb", "source.tccl", "source.tcef")

// ∅
```

Alternatively you can init `Source` in code:

```c++
// ∅

teco::Source source = teco::Source(
    std::vector<std::vector<char>> {
        std::vector<char>{' ', 'o', ' '},
        std::vector<char>{'(', '|', ')'},
        std::vector<char>{'/', ' ', '\\'}
    },
    std::vector<std::vector<char>> {
        std::vector<char>{' ', '7', ' '},
        std::vector<char>{'7', '7', '7'},
        std::vector<char>{'7', ' ', '7'}
    },
    std::vector<std::vector<char>> {
        std::vector<char>{' ', '#', ' '},
        std::vector<char>{'#', '#', '#'},
        std::vector<char>{'#', ' ', '#'}
    }
)

// ∅
```

Or:

```c++
// ∅

teco::Source source = teco::Source(
    std::vector<std::string> {
        " o ",
        "(|)",
        "/ \\"
    },
    std::vector<std::string> {
        " 7 ",
        "777",
        "7 7"
    },
    std::vector<std::string> {
        " # ",
        "###",
        "# #"
    }
)

// ∅
```

## Sprites
`Sprite` is a class for storing and processing `Animations` (More on them later). To init sprite you need to specify `vector` of `Animations` and can specify index of `Animation` in that `vector` that will be played first and it's starting frame

### Animations
`Animation` is a class for storing sequence of `Sources`

## Tick and draw in mainloop


## Keybinds


## Compilation
