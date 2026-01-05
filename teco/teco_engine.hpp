 ////// ////// ////// //////    ////// //  // ////// // //  // //////
  //   ////   //     //  //    ////   /// // //     // /// // ////
 //   //     //     //  //    //     // /// //  // // // /// //
//   ////// ////// //////    ////// //  // ////// // //  // //////

#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <string>
#include <fstream>

#define unfduration std::chrono::nanoseconds
#define second_ratio 1000000000L
#define unftimepoint std::enable_if<true, std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::_V2::system_clock::duration>>::type
#define unftime() std::chrono::time_point_cast<unfduration>(std::chrono::system_clock::now())

#ifdef _WIN32
#include <Windows.h>
#define unfsleep(sleep) usleep((sleep * 1000L) / second_ratio)

#else
#include <unistd.h>
#define unfsleep(sleep) usleep((sleep * 1000000L) / second_ratio)

#endif

#define SPRITE_SYMBOLS(sprite) sprite.animations[sprite.current_animation_index].sources[sprite.current_animation_frame_index].symbols
#define SPRITE_COLORS(sprite) sprite.animations[sprite.current_animation_index].sources[sprite.current_animation_frame_index].colors
#define SPRITE_EFFECTS(sprite) sprite.animations[sprite.current_animation_index].sources[sprite.current_animation_frame_index].effects

namespace teco {

// enums
enum {LOOPING, STOP_ON_FIRST_FRAME, STOP_ON_LAST_FRAME};

// classes
class Source {
public:
    std::vector<std::vector<char>> symbols;
    std::vector<std::vector<char>> colors;
	std::vector<std::vector<char>> effects;

	int width;
	int height;

    Source(std::string symbols_path, std::string colors_path, std::string effects_path);
    Source(std::vector<std::vector<char>> _symbols, std::vector<std::vector<char>> _colors, std::vector<std::vector<char>> _effects);
    Source(std::vector<std::string> symbol_strings, std::vector<std::string> color_strings, std::vector<std::string> effect_strings);
};

class Animation {
public:
	std::vector<Source> sources;

	int loop_mode;
	int ticks_per_frame;

	Animation(std::vector<Source> _sources, int _loop_mode = STOP_ON_FIRST_FRAME, int _ticks_per_frame = 2);
};

class Sprite {
public:
	std::vector<Animation> animations;

	int current_animation_index;
	int current_animation_frame_index;
    int current_animation_tick;
    bool is_playing_animations;
	
	Sprite();
	Sprite(std::vector<Animation> _animations, int _currrent_animation_index = 0, int _current_animation_frame_index = 0);
	void set_animation(int animation_index);
	void force_animation(int animation_index);
	void update_animations();
};

class Screen {
public:
	std::vector<std::vector<char>> symbols;
	std::vector<std::vector<char>> colors;
	std::vector<std::vector<char>> effects;

	int width;
	int height;

	void (*tick) ();
	void (*draw) ();

	Screen();
	Screen(int _width, int _height, void (*_tick) (), void (*_draw) ());
	void clear();
	void draw_source(int x, int y, Source& source_to_draw);
	void draw_sprite(int x, int y, Sprite& sprite_to_draw);
	void draw_screen(int x, int y, Screen& screen_to_draw);
	void draw_char(int x, int y, char symbol_to_draw = ' ', char color_to_draw = ' ', char effect_to_draw = ' ');
	void draw_all(int x, int y, std::vector<std::vector<char>>& symbols_to_draw, std::vector<std::vector<char>>& colors_to_draw, std::vector<std::vector<char>>& effects_to_draw);
};

// variables
extern std::string title;

extern unsigned long long tick_count;

extern int fps;
extern int tps;

extern std::vector<char> pressed_keys;

extern std::map<char, std::vector<unsigned char>> colors;
extern int background_red;
extern int background_green;
extern int background_blue;
extern char default_color;

extern std::map<int, char> keybinds;

extern unfduration tick_slice;
extern unfduration draw_slice;
extern unfduration time_accumulator;
extern unftimepoint last_update_time;

extern bool run;

extern Screen *current_screen;

// engine functions
void init(
	Screen *_current_screen = NULL,
	std::string _title = "TeCo",
	int _fps = 60,
	int _tps = 20,
	std::map<int, char> _keybinds = std::map<int, char> {},
	std::map<char, std::vector<unsigned char>> _colors = std::map<char, std::vector<unsigned char>> {
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
	},
	char _default_color = '0',
	int _background_red = 0x12,
	int _background_green = 0x12,
	int _background_blue = 0x12
);

void mainloop(void (*draw) (), void (*handle_events) ());

// untility functions
void draw_chars_on_something(int x, int y, std::vector<std::vector<char>> &something_to_draw_on, std::vector<std::vector<char>> chars_to_draw);

std::vector<std::vector<char>> get_chars_from_file(std::string path);
std::vector<std::vector<char>> get_chars_from_strings(std::vector<std::string> strings);

}
