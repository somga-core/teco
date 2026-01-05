# Navigation
- [Quck start](#quick-start)
  - [Includes](#includes)
  - [Initialization](#initialization)
  - [Screens](#screens)
  - [Sprites](#sprites)
    - [Animations](#animations)
    - [Sources](#sources)
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
#include "teco_gui.hpp" // or "teco_tui.hpp"

// ∅
```

## Initialization
Then you need to run init functions. Always run `teco::init()`. Use `teco::init_gui()` for GUI and `teco::init_tui()` for TUI:

```c++
#include "teco_engine.hpp"
#include "teco_gui.hpp" // or "teco_tui.hpp"

int main() {
    teco::init();
    teco::init_gui(); // or teco::init_tui();

    return 0;
}
```

## Screens
In `init` function you must pass `current_screen` argument. It has `Screen` type. It's a 2D plane that will be displayed on window or in terminal and on which you will be drawing symbols

To init `Screen` you need to specify width and height as well as tick and draw functions (More on that later):

```c++
#include "teco_engine.hpp"
#include "teco_gui.hpp" // or "teco_tui.hpp"

void tick() {};
void draw() {};

teco::Screen screen;

int main() {
    teco::init(screen);
    teco::init_gui(); // or teco::init_tui();

    screen = teco::Screen(48, 14, &tick, &draw);

    return 0;
}
```

You can also init several `Screens` and switch to them later:

```c++
#include "teco_engine.hpp"
#include "teco_gui.hpp" // or "teco_tui.hpp"

void main_tick() {};
void main_draw() {};

void sec_tick() {};
void sec_draw() {};

teco::Screen main_screen;
teco::Screen sec_screen;

int main() {
    teco::init(screen);
    teco::init_gui(); // or teco::init_tui();
    
    // ∅

    screen = teco::Screen(48, 14, &tick, &draw);

    return 0;
}
```

## Sprites


### Animations


### Sources


## Tick and draw in mainloop


## Keybinds


## Compilation


## Result
