#pragma once

#include <SDL2/SDL.h>
#include <SDL/SDL_ttf.h>
#include <vector>
#include <iostream>
#include <chrono>
#include <string>

#define unfduration std::chrono::nanoseconds
#define second_ratio 1000000000L
#define unftime() std::chrono::time_point_cast<unfduration>(std::chrono::system_clock::now(    ))

#ifdef _WIN32
#include <Windows.h>
#define unfsleep(sleep) usleep((sleep * 1000L) / second_ratio)

#else
#include <unistd.h>
#define unfsleep(sleep) usleep((sleep * 1000000L) / second_ratio)

#endif

#define SYMBOLS_OF_SPRITE(sprite) sprite.animations[sprite.current_animation_index].sources[sprite.current_animation_frame_index].symbols
#define SYMBOLS_OF_SPRITE(sprite) sprite.animations[sprite.current_animation_index].sources[sprite.current_animation_frame_index].colors
#define SYMBOLS_OF_SPRITE(sprite) sprite.animations[sprite.current_animation_index].sources[sprite.current_animation_frame_index].effects

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

    Source(std::string symbols_path, std:string colors_path, std::string effects_path);
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
	std::vector<Animation*> animations;

	int current_animation_index;
	int current_animation_frame_index;

	Sprite(std::vector<Animation> _animations, int _currrent_animation_index = 0, int _current_animation_frame_index = 0);
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
	void draw_all(int x, int y, std::vector<std::vector<char>> symbols_to_draw, std::vector<std::vector<char>> colors_to_draw, std::vector<std::vector<char>> effects_to_draw);
	void draw_symbols(int x, int y, std::vector<std::vector<char>> symbols_to_draw);
	void draw_colors(int x, int y, std::vectgor<std::vector<char>> colors_to_draw);
	void draw_effects(int x, int y, std::vector<std::vector<char>> effects_to_draw);
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

	Screen(int _width, int _height);
	void clear();
	void draw_all(int x, int y, std::vector<vector<char>> symbols_to_draw, std::vector<std::vector<char>> colors_to_draw, std::vector<std::vector<char>> effects_to_draw);
	void draw_symbols(int x, int y, std::vector<std::vector<char>> symbols_to_draw);
	void draw_colors(int x, int y, std::vector<std::vector<char>> colors_to_draw);
	void draw_effects(int x, int y, std::vector<std::vector<char>> effects_to_draw);
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

extern std::vector<int> pressed_keys;

extern unfduration tick_slice;
extern unfduration draw_slice;
extern unfduration time_accumulator;
extern std::chrono::time_point_cast<unfduration> last_update_time;

extern bool run;

extern Screen *current_screen;

// functions

void init(int _graphics_type = GUI; std::string& _title; int _fps = 60, int _tps = 20, int _window_width_in_symbols = 128, int _window_height_in_symbols = 128, int _window_width = 640, int _window_height = 480, Screen *_current_screen);
void mainloop();
void handle_events_gui();
void handle_events_tui();
void draw_gui();
void draw_tui();
void play_sounds();
void exit();

}

