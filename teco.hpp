#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> 
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
enum {TUI, GUI};
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
    std::vector<std::vector<char>> read_file(std::string file_name);
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

	Sprite(std::vector<Animation> _animations, int _currrent_animation_index = 0, int _current_animation_frame_index = 0);
	void play_animation(int animation_index);
	void update_animations();
};

class Subscreen {
public:
	std::vector<std::vector<char>> symbols;
	std::vector<std::vector<char>> colors;
	std::vector<std::vector<char>> effects;	

	int width;
	int height;

	void (*tick) ();
	void (*draw) ();

	Subscreen(int _width, int _height);
	void clear();
	void draw_sprite(int x, int y, Sprite sprite_to_draw);
	void draw_all(int x, int y, std::vector<std::vector<char>> symbols_to_draw, std::vector<std::vector<char>> colors_to_draw, std::vector<std::vector<char>> effects_to_draw);
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

	Screen(int _width, int _height, void (*_tick) (), void (*_draw) ());
	void clear();
	void draw_sprite(int x, int y, Sprite sprite_to_draw);
	void draw_all(int x, int y, std::vector<std::vector<char>> symbols_to_draw, std::vector<std::vector<char>> colors_to_draw, std::vector<std::vector<char>> effects_to_draw);
};

// variables
extern int graphics_type;

extern std::string title;

extern int fps;
extern int tps;

extern int window_width_in_symbols;
extern int window_height_in_symbols;
extern int window_width;
extern int window_height;

extern SDL_Event event;
extern SDL_Renderer *renderer;
extern SDL_Window *window;
extern SDL_Surface *window_surface;
extern TTF_Font *font;

extern std::vector<int> pressed_keys;
extern std::map<char, std::map<char, SDL_Texture*>> saved_textures;
extern std::map<char, SDL_Color> colors;
extern char default_color;

extern unfduration tick_slice;
extern unfduration draw_slice;
extern unfduration time_accumulator;
extern unftimepoint last_update_time;

extern bool run;

extern Screen *current_screen;

// functions
void init(
	std::string font_path = "assets/JetBrainsMono-Regular.ttf",
	int font_size = 20,
	std::map<char, SDL_Color> _colors = std::map<char, SDL_Color> {
		{'0', SDL_Color {229,229,229}},
		{'1', SDL_Color {160,160,160}},
		{'2', SDL_Color {10,162,146}},
		{'3', SDL_Color {0,133,102}},
		{'4', SDL_Color {165,89,177}},
		{'5', SDL_Color {102,0,153}},
		{'6', SDL_Color {42,111,189}},
		{'7', SDL_Color {19,68,125}},
		{'8', SDL_Color {209,148,23}},
		{'9', SDL_Color {178,94,13}},
		{'A', SDL_Color {135,173,0}},
		{'B', SDL_Color {94,117,0}},
		{'C', SDL_Color {171,0,0}},
		{'D', SDL_Color {127,0,31}},
		{'E', SDL_Color {103,103,103}},
		{'F', SDL_Color {0,0,0}}
	},
	char _default_color = '0',
	Screen *_current_screen = NULL,
	int _graphics_type = GUI,
	std::string _title = "TeCo",
	int _fps = 60,
	int _tps = 20, 
	int _window_width = 640,
	int _window_height = 480
);

void mainloop();

void handle_events_gui();
void handle_events_tui();

void draw_gui();
void draw_tui();

void draw_chars_on_something(int x, int y, std::vector<std::vector<char>> &something_to_draw_on, std::vector<std::vector<char>> chars_to_draw);

void play_sounds();

void exit();

}

