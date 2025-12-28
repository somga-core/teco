# Engine
The engine part of TeCo (`teco_engine.cpp` and `teco_engine.hpp`) is needed basically for placing ASCII image information on a 2D plane, as well as handling mainloop logic and storing global varibles for GUI and TUI parts

## Source
`Source` is a class that stores ASCII images

ASCII images are stored using three sets of 2D `vectors` of `chars`:
- `symbols` are what you will see on a screen, 
- `colors` are keys for the lookup table, that correspond to specific RGB values. They paint symbols in place of which they stand
- `effects` are kays for the lookup table, that correspond to functions, that return x and y offset as well as x and y size multipier for symbols. They apply to symbols in place of which they stand. More on that system in [`gui.md`](gui.md)

Standard file types for ASCII images are:
- `.tcsb` for `symbols`
- `.tccl` for `colors`
- `.tcef` for `effects`

### Variables
`std::vector<std::vector<char>> symbols` - ASCII image `symbols`

`std::vector<std::vector<char>> colors` - ASCII image `colors`

`std::vector<std::vector<char>> effects` - ASCII image `effects`

`int width` - width of ASCII image

`int height` - height of ASCII image 

### Functions
`Source` - define `Source` with paths to files from which ASCII image would be generated:
- `std::string symbols_path` - path to `.tcsb` file
- `std::string colors_path` - path to `.tccl` file
- `std::string effects_path` - path to `.tcef` file

`Source` - define `Source` with 2D `vectors` of `chars`:
- `std::vector<std::vector<char>> _symbols` - will be directly assigned to `symbols`
- `std::vector<std::vector<char>> _colors` - will be directly assigned to `colors`
- `std::vector<std::vector<char>> _effects` - will be directly assigned to `effects`

`Source` - define `Source` with `vectors` of `strings`:
- `std::vector<std::string> symbol_strings` - will be converted to `symbols`
- `std::vector<std::string> color_strings` - will be converted to `colors`
- `std::vector<std::string> effect_strings` - will be converted to `effects`

## Animation
`Animations` is a class that stores `Sources` sequence, loop mode, and number of ticks per frame

Loop mode is defined using variables from enum:
- `LOOPING` - after animation is completed, it will be repeated
- `STOP_ON_FIRST_FRAME` - after animation is completed, it will stop on first frame of this animations
- `STOP_ON_LAST_FRAME` - after animation is completed, it will stop on last frame of this animations

### Variables
`std::vector<Source> sources` - sequence of `Sources`

`int loop_mode` - loop mode from enum that is described earlier

`int ticks_per_frame` - number of ticks that frame will wait before changing

### Functions
`Animation` - define `Animation`:
- `std::vector<Source> _sources` - will be directlry assigned to `sources`
- `int _loop_mode` - will be directlry assigned to `loop_mode`. Default - `STOP_ON_FIRST_FRAME`
- `int _ticks_per_frame` - will be directlry assigned to `ticks_per_frame`. Default - `2`

## Sprite
`Sprite` is a class for storing and manipulating `Animations`

### Variables
`std::vector<Animation> animations` - `Animations` that will be stored in that `Sprite`

`int current_animation_index` - index of `Animation` that is currently playing from `vector` of `Animations`

`int current_animation_frame_index` - index of `Source` that is currently shown from `vector` of `Sources` in current `Animations`

`int current_animation_tick` - tick counter

`bool is_playing_animations` - the name describes it very well

### Functions
`Sprite` - define `Sprite`:
- `std::vector<Animation> _animations` - will be directlry assigned to `animations`
- `int _currrent_animation_index` - will be directlry assigned to `current_animation_index`. Default - `0`
- `int _current_animation_frame_index` - will be directlry assigned to `current_animation_frame_index`. Default - `0`

`set_animation` - changes animation and resets `current_animation_frame_index` only if `current_animation_index` doesn't equal to new animation index:
- `int animation_index` - will be directlry assigned to `current_animation_index`

`force_animation` - changes animation and resets `current_animation_frame_index`:
- `int animation_index` - will be directlry assigned to `current_animation_index`

`update_animations` - counts ticks, switches frames, handles loop mode logic

## Screen
`Screen` is a class that stores ASCII image on which you can draw `chars` using `Sprites` or other `Screens`. It can be displayed in a terminal or a window

### Variables
`std::vector<std::vector<char>> symbols` - ASCII image `symbols`

`std::vector<std::vector<char>> colors` - ASCII image `colors`

`std::vector<std::vector<char>> effects` - ASCII image `effects`

`int width` - width of ASCII image

`int height` - height of ASCII image

`void (*tick) ()` - function that will be called every engine tick. More on that system later

`void (*draw) ()` - function that will be called every engine frame. More on that system later

### Functions
`Screen` - define `Screen`:
- `int _width` - will be directlry assigned to `width`
- `int _height` - will be directlry assigned to `height`
- `void (*_tick) ()` - will be directlry assigned to `tick`
- `void (*_draw) ()` - will be directlry assigned to `draw`

`clear` - fill all the image with spaces

`draw_source` - place `Source` on the image:
- `int x` - x coordinate of left-right corner of the `Source`
- `int y` - y coordinate of left-right corner of the `Source`
- `Source& source_to_draw` - `Source` which will be placed on the image

`draw_sprite` - place current `Sprite` frame on the image:
- `int x` - x coordinate of left-right corner of the `Sprite`
- `int y` - y coordinate of left-right corner of the `Sprite`
- `Sprite& sprite_to_draw` - `Sprite` which will be placed on the image

`draw_screen` - place `Screen` contents on the image:
- `int x` - x coordinate of left-right corner of the `Screen`
- `int y` - y coordinate of left-right corner of the `Screen`
- `Screen& screen_to_draw` - `Screen` which will be placed on the image

`draw_char` - place `char` on the image:
- `int x` - x coordinate of the `char`
- `int y` - y coordinate of the `char`
- `char symbol_to_draw` - `symbol` that will be placed on the image
- `char color_to_draw` - `color` that will be placed on the image
- `char effect_to_draw` - `effect` that will be placed on the image

`draw_all` - place `chars` on the image:
- `int x` - x coordinate of left-right corner of the `chars`
- `int y` - y coordinate of left-right corner of the `chars`
- `std::vector<std::vector<char>>& symbols_to_draw` - `symbols` that will be placed on the image
- `std::vector<std::vector<char>>& colors_to_draw` - `colors` that will be placed on the image
- `std::vector<std::vector<char>>& effects_to_draw` - `effects` that will be placed on the image

## Init

## Mainloop

## Current screen