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

namespace teco {

// classes

class Source {
public:
    std::vector<std::string> symbols;
    std::vector<std::string> colors;

    Source(std::string symbols_path, std:string colors_path);
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
	
};

class Subscreen {
public:
	
};

class Screen {
public:
	
};

// consts

// enums

enum {TUI, GUI};
enum {LOOPING, STOP_ON_FIRST_FRAME, STOP_ON_LAST_FRAME};

// variables
int window_width;
int window_height;
int window_width_in_symbols;
int window_height_in_symbols;

std::string title;

int fps;
int tps;

int graphics_type;

SDL_DisplayMode display_mode;
SDL_Event event;
SDL_Renderer *renderer = NULL;
SDL_Window *window = NULL;
SDL_Surface *window_surface = NULL;

std::vector<int> pressed_keys;

auto tick_slice = unfduration::zero();
auto draw_slice = unfduration::zero();

auto last_update_time = unftime();
auto accumulator = unfduration::zero();

bool run = true;

// functions

void init(int _graphics_type = GUI; int _fps = 60, int _tps = 20, std::string& _title);
void mainloop();
void handle_events_tui();
void handle_events_gui();
void draw_tui();
void draw_gui();
void playsonds();
void exit();

bool is_key_pressed(int key);

}

