# Navigation
- [Quck start](#quick-start)
  - [Includes](#includes)
  - [Initialization](#initialization)
  - [Screens](#screens)
  - [Sources](#sources)
  - [Sprites](#sprites)
    - [Animations](#animations)
  - [Tick and draw in mainloop](#tick-and-draw-in-mainloop)
  - [Keybinds](#keybinds)
  - [Compilation](#compilation)
  - [Result](#result)

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
`Source` is a class that stores ASCII-image. ASCII-image is composed of `symbols`, `colors` and `effects`:
- `symbols` represent which symbol would be displayed
- `colors` specify in which color from lookup table symbol would be painted
- `effects` specify which effect would be applied on symbol. They work only in GUI (More on that system later)

`colors` lookup table can be specified in `init()` or by setting `colors` variable, `effects` lookup table can be specified in `init_gui()` or by setting `effects` variable:

```c++
// ∅

teco::init(..., ..., ..., ..., colors);
teco::init_gui(..., ..., ..., ..., effects);

// ∅
```

```c++
// ∅

teco::init(...);
teco::init_gui(..);

teco::colors = ...;
teco::effects = ...;

// ∅
```

## Sprites
`Sprites` is a class for storing and processing `Animations` (More on them later). You can draw it on `Screen`. To init sprite you need to specify `vector` of `Animations`. 

### Animations


## Tick and draw in mainloop


## Keybinds


## Compilation


## Result
