# Navigation
- [Quck start](#quick-start)
  - [Includes](#includes)
  - [Initialization](#initialization)
  - [ASCII images](#ascii-images)
  - [Screens](#screens)
  - [Sources](#sources)
  - [Animations](#animations)
  - [Sprites](#sprites)
  - [Tick and draw in mainloop](#tick-and-draw-in-mainloop)
  - [Keybinds](#keybinds)
  - [Compilation](#compilation)

# Quick start
To start using `TeCo` you need to copy it's engine files into your project. They are located in [`teco`](/teco/) folder. You always want to copy [`teco_engine.cpp`](/teco/teco_engine.cpp) and [`teco_engine.hpp`](/teco/teco_engine.hpp)

If you want to use window input and output (Next, GUI) you must copy [`teco_gui.cpp`](/teco/teco_gui.cpp) and [`teco_gui.hpp`](/teco/teco_gui.hpp)

If you want to use terminal input and output (Next, TUI) you must copy [`teco_tui.cpp`](/teco/teco_tui.cpp) and [`teco_tui.hpp`](/teco/teco_tui.hpp)

## Includes
You must include into your project `TeCo` files to use it (Obviously). Include [`teco_gui.hpp`](/teco/teco_gui.hpp) if you want to use GUI or [`teco_tui.hpp`](/teco/teco_tui.hpp) if you want to use TUI

So your includes must look something like that:

```c++
#include "teco_gui.hpp" // Or "teco_tui.hpp"

// ∅
```

## Initialization
Then you need to run init functions. Always run `init()`. Use `init_gui()` for GUI and `init_tui()` for TUI:

```c++
#include "teco_gui.hpp" // Or "teco_tui.hpp"

int main() {
    teco::init();
    teco::init_gui(); // Or teco::init_tui();

    return 0;
}
```

In `init()` you must specify starting screen (More on that later) and can specify FPS and TPS (More on that later), keybinds (More on that later) and a few color options (More on that later):

```c++
// ∅

teco::init(&screen, 60, 20, keybinds, colors, default_color, background_red, background_green, background_blue);

// ∅
```

In `init_gui()` you can specify default window width and height, path to font and it's size, as well as effects lookup table (More on that later):

```c++
// ∅

teco::init_gui(600, 400, "path/to/font.ttf", 20, effects);

// ∅
```

In `init_tui()` there are no arguments

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
// ∅

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

// ∅
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
In `init` function you must pass `current_screen` argument. It has `Screen` type. It's a 2D plane (ACSII image) that will be displayed on window or in terminal and on which you will be drawing symbols, `Sources` or `Sprites` (More on them later)

To init `Screen` you need to specify width and height as well as tick and draw functions (More on that later):

```c++
#include "teco_gui.hpp" // Or "teco_tui.hpp"

void tick() {};
void draw() {};

teco::Screen screen;

int main() {
    teco::init(&screen);
    teco::init_gui(); // Or teco::init_tui();

    screen = teco::Screen(48, 14, &tick, &draw);

    return 0;
}
```

You can also init several `Screens` and switch between them later:

```c++
#include "teco_gui.hpp" // Or "teco_tui.hpp"

void main_tick() {};
void main_draw() {};

void sec_tick() {};
void sec_draw() {};

teco::Screen main_screen;
teco::Screen sec_screen;

int main() {
    teco::init(&main_screen);
    teco::init_gui(); // Or teco::init_tui();
    
    main_screen = teco::Screen(48, 14, &tick, &draw);
    sec_screen = teco::Screen(48, 14, &tick, &draw);

    // ∅

    teco::current_screen = &sec_screen; // Switching to different screen

    return 0;
}
```

## Sources
Source is a class that stores ASCII images. To init `Source` you need to specify path to files with `symbols`, `colors` and `effects`. Extentions for them are: `.tcsb`, `.tccl`, `tcef` respectively

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

## Animations
`Animation` is a class for storing sequence of `Sources`. It's needed mainly to pass as argument in `Sprites`. To initialize it you need to specify `vector` of `Sources`, and can specify loop mode and number of ticks for frame (More on that later)

Loop mode is defined using enum:
- `LOOPING` - after animation is completed, it will be repeated
- `STOP_ON_FIRST_FRAME` - after animation is completed, it will stop on first frame of this animations
- `STOP_ON_LAST_FRAME` - after animation is completed, it will stop on last frame of this animations

So, here is example `Animation`:

```c++
// ∅

teco::Animation animation = teco::Animation(
    std::vector<teco::Source> {
        teco::Source("source1.tcsb", "source1.tccl", "source1.tcef"),
        teco::Source("source2.tcsb", "source2.tccl", "source2.tcef"),
        teco::Source("source3.tcsb", "source3.tccl", "source3.tcef"),
        teco::Source("source4.tcsb", "source4.tccl", "source4.tcef")
    },
    teco::LOOPING, // Or teco::STOP_ON_FIRST_FRAME or teco::STOP_ON_LAST_FRAME
    2 // This means that between animation frame switches there will be 2 tick delay
)

// ∅
```

## Sprites
`Sprite` is a class for storing and processing `Animations`. To init sprite you need to specify `vector` of `Animations` and can specify index of `Animation` from that `vector` that will be played first and it's starting frame

Here is example `Sprite` initialization:

```c++
// ∅

teco::Sprite sprite = teco::Sprite(
    std::vector<teco::Animation> {
        teco::Animation(
            std::vector<teco::Source> {
                teco::Source("source1.tcsb", "source1.tccl", "source1.tcef"),
                teco::Source("source2.tcsb", "source2.tccl", "source2.tcef")
            },
            teco::LOOPING,
            4
        ),
        teco::Animation(
            std::vector<teco::Source> {
                teco::Source("source3.tcsb", "source3.tccl", "source3.tcef"),
                teco::Source("source4.tcsb", "source4.tccl", "source4.tcef")
            },
            teco::STOP_ON_LAST_FRAME,
            4
        )
    },
    1, // This means that, when sprite would be initialized, second animation would be played
    0 // This mens that, when sprite would be initialized, it's animation will start at first frame
);

// ∅
```

You can switch `Sprite` animation by using `set_animation()` or `force_animation()` and specifying animation index

```c++
// ∅

sprite.set_animation(0); // This will set animation to first, only if it's not currently playing
sprite.force_animation(0); // This will set animation to first no matter what

// ∅
```

## Tick and draw in mainloop
Next you want to start mainloop. Run `mainloop_tui()` if you are using TUI, and `mainloop_gui()` if you are using GUI

This mainloop is needed for updating window or terminal printing with frequency of `fps`. It's also will run `tick` and `draw` function of `current_screen`. `tick` would be executed with frequency of `tps` and `draw` with frequency of `fps`

In your `draw` function you want to draw all `Sprites` and `Sources` (Or even other `Screens`) on `Screen`

In your `tick` function you want to update `Sprites` animations, detect keybinds (More on them later) and run your game logic

Here is an example:

```c++
#include "teco_gui.hpp" // Or "teco_tui.hpp"

void tick();
void draw();

teco::Screen screen;
teco::Sprite sprite;

int sprite_x;

int main() {
    teco::init(&screen);
    teco::init_gui(); // Or teco::init_tui();

    screen = teco::Screen(48, 14, &tick, &draw);
    sprite = teco::Sprite(
        std::vector<teco::Animation> {
            teco::Animation(
                std::vector<teco::Source> {
                    teco::Source("source1.tcsb", "source1.tccl", "source1.tcef"),
                    teco::Source("source2.tcsb", "source2.tccl", "source2.tcef"),
                    teco::Source("source3.tcsb", "source3.tccl", "source3.tcef"),
                    teco::Source("source4.tcsb", "source4.tccl", "source4.tcef")
                },
                teco::LOOPING,
                2
            )
        }
    );

    sprite_x = 0;

    return 0;
}

void tick() {
    sprite.update_animations();
    sprite_x++; // This will move sprite by 1 symbol to the right every tick
};

void draw() {
    screen.draw_sprite(sprite_x, 6);
};
```

On `Screen` you can also draw `Sources`, other `Screens`, as well as individual `chars`:

```c++
// ∅

screen.draw_source(x, y, source);
screen.draw_sprite(x, y, sprite);
screen.draw_screen(x, y, another_screen);
screen.draw_char(x, y, symbol, color, effect); // symbol, color and effect need to be chars
screen.draw_all(x, y, symbols, colors, effects); // symbols, colors and effects need to be 2D vectors of chars

// ∅
```

## Keybinds
To check for pressed keys you need to check variable `pressed_keys`, which is `vector` with values of `keybinds` lookup table. After keypress of any key in `keybinds` table will be detected, in `pressed_keys` would be added corresponding value

So, code to handle keybinds may look like this:

```C++
#include "teco_gui.hpp"

// ∅

int main () {
    teco::init(...);
    teco::init_gui();

    teco::keybinds = std::map<int, char> {
        {SDLK_UP, '^'},
        {SDLK_DOWN, 'v'},
        {SDLK_LEFT, '>'},
        {SDLK_RIGHT, '<'}
    };

    // ∅

};

void tick() {
    for (auto key : teco::pressed_keys) {
        switch (key) {
            case '^':
                std::cout << "Up arrow is pressed!!!" << std::endl;
                break;
            case 'v':
                std::cout << "Down arrow is pressed!!!" << std::endl;
                break;
            case '>':
                std::cout << "Right arrow is pressed!!!" << std::endl;
                break;
            case '<':
                std::cout << "Left arrow is pressed!!!" << std::endl;
                break;
        }
    }

    // ∅
};

// ∅
```

If you are using TUI you have to clear `pressed_keys` array when you are done checking it:

```c++
// ∅
void tick() {

    // ∅

    teco::pressed_keys.clear();
}

// ∅
```

## Compilation
For compilation see [`compilation.md`](compilation.md)