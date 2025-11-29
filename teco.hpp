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
    std::vector<char> symbols;
    std::vector<char> colors;
	std::vector<char> effects;

	int width;
	int height;

    Source(std::string symbols_path, std:string colors_path, std::string effects_path);
    std::vector<std::string> read_file(std::string file_name);
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
	int current_animation_index = 0;
	int current_animation_frame_index = 0;
};

class Subscreen {
public:
	std::vector<char> symbols;
	std::vector<char> colors;
	std::vector<char> effects;	

	int width;
	int height;

	void (*tick) ();
	void (*draw) ();

	void clear();
	void draw_all(int x, int y, std::vector<char> symbols_to_draw, std::vector<char> colors_to_draw, std::vector<char> effects_to_draw);
	void draw_symbols(int x, int y, std::vector<char> symbols_to_draw);
	void draw_colors(int x, int y, std::vectgor<char> colors_to_draw);
	void draw_effects(int x, int y, std::vector<char> effects_to_draw);
};

class Screen {
public:
	std::vector<char> symbols;
	int width;
	int height;

	void (*tick) ();
	void (*draw) ();

	void clear();
	void draw_all(int x, int y, std::vector<char> symbols_to_draw, std::vector<char> colors_to_draw, std::vector<char> effects_to_draw);
	void draw_symbols(int x, int y, std::vector<char> symbols_to_draw);
	void draw_colors(int x, int y, std::vectgor<char> colors_to_draw);
	void draw_effects(int x, int y, std::vector<char> effects_to_draw);
};

// variables
int window_width;
int window_height;
int window_width_in_symbols;
int window_height_in_symbols;

std::string title;

int fps;
int tps;

int graphics_type;

SDL_Event event;
SDL_Renderer *renderer = NULL;
SDL_Window *window = NULL;
SDL_Surface *window_surface = NULL;

std::vector<int> pressed_keys;

auto tick_slice = unfduration::zero();
auto draw_slice = unfduration::zero();

auto last_update_time = unftime();
auto time_accumulator = unfduration::zero();

bool run = true;

Screen* current_screen = NULL;

// functions

void init(int _graphics_type = GUI; int _fps = 60, int _tps = 20, int _window_width_in_symbols = 128, int _window_height_in_symbols = 128, int _window_width, int _window_height, std::string& _title);
void mainloop();
void handle_events_tui();
void handle_events_gui();
void draw_tui();
void draw_gui();
void playsonds();
void exit();

bool is_key_pressed(int key);

}

