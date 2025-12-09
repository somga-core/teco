#include "teco.hpp"

// class methods

teco::Source::Source(std::string symbols_path, std:string colors_path, std::string effects_path, int _width) {
	symbols = read_file(symblos_path);
	colors = read_file(colors_path);
	effects = read_file(effects_path);

	width = _width;
	height = symbols.size();
}

std::vector<std::vector<char>> teco::Source::read_file(std::string file_name) {
	std::vector<std::vector<char>> result;
	std::string line;
	int line_index = 0;
	std::ifstream in;

	in.open(file_name);

	if (in.is_open()) {
		while (std::getline(in, line)) {
			result.push_back(std::vector<char>);

			for (char symbol : line) {
				result[line_index].push_back(symbol);
			}

			line_index++;
		}

		in.close();
	}

	return result;
}

teco::Animation::Animation(std::vector<Source> _sources, int _loop_mode = STOP_ON_FIRST_FRAME, int _ticks_per_frame = 2) {
	sources = _sources;
	loop_mode = _loop_mode;
	ticks_per_frame = _ticks_per_frame;
}

teco::Sprite::Sprite(std::vector<Animation> _animations, int _currrent_animation_index = 0, int _current_animation_frame_index = 0) {
	animations = _animations;
	current_animation_index = _current_animation_index;
	current_animation_frame_index = _current_animation_frame_index;
}

teco::Subscreen::Subscreen(int _width, int _height) {
	width = _width;
	height = _height;

	for (int line = 0; line < height; line++) {
		symbols.push_back(std::vector<char>);
		symbols.back().resize(width, ' ');
		colors.push_back(std::vector<char>);
		colors.back().resize(width, ' ');
		effects.push_back(std::vector<char>);
		effects.back().resize(width, ' ');
	}
}

void teco::Subscreen::clear() {
	for (int line = 0; line < height; line++) {
		for (int column = 0; column < width; column++) {
			symbols[line][column] = ' ';
			colors[line][column] = ' ';
			effects[line][column] = ' ';
		}
	} 
}

void teco::Subscreen::draw_all(int x, int y, std::vector<std::vector<char>> symbols_to_draw, std::vector<std::vector<char>> colors_to_draw, std::vector<std::vector<char>> effects_to_draw) {
	draw_symbols(x, y, symbols_to_draw);
	draw_colors(x, y, colors_to_draw);
	draw_effects(x, y, effects_to_draw);
}

void teco::Subscreen::draw_symbols(int x, int y, std::vector<std::vector<char>> symbols_to_draw) {
	for (int line = 0; line < symbols_to_draw.size(); line++) {
		if (y+line <= height) {
			for (int column = 0; column < symbols_to_draw[line].size(); column++) {
				if (x+column <= width) {
					symbols[y+line][x+column] = symbols_to_draw[line][column];
				}
			}
		}
	}
}

void teco::Subscreen::draw_colors(int x, int y, std::vectgor<std::vector<char>> colors_to_draw) {
	for (int line = 0; line < colors_to_draw.size(); line++) {
		if (y+line <= height) {
			for (int column = 0; column < colors_to_draw[line].size(); column++) {
				if (x+column <= width) {
					symbols[y+line][x+column] = colors_to_draw[line][column];
				}
			}
		}
	}
}

void teco::Subscreen::draw_effects(int x, int y, std::vector<std::vector<char>> effects_to_draw) {
	for (int line = 0; line < effects_to_draw.size(); line++) {
		if (y+line <= height) {
			for (int column = 0; column < effects_to_draw[line].size(); column++) {
				if (x+column <= width) {
					symbols[y+line][x+column] = effects_to_draw[line][column];
				}
			}
		}
	}
}

teco::Screen::Screen(int _width, int _height) {
	width = _width;
	height = _height;

	for (int line = 0; line < height; line++) {
		symbols.push_back(std::vector<char>);
		symbols.back().resize(width, ' ');
		colors.push_back(std::vector<char>);
		colors.back().resize(width, ' ');
		effects.push_back(std::vector<char>);
		effects.back().resize(width, ' ');
	}
}

void teco::Screen::clear() {
	for (int line = 0; line < height; line++) {
		for (int column = 0; column < width; column++) {
			symbols[line][column] = ' ';
			colors[line][column] = ' ';
			effects[line][column] = ' ';
		}
	} 
}

void teco::Screen::draw_all(int x, int y, std::vector<std::vector<char>> symbols_to_draw, std::vector<std::vector<char>> colors_to_draw, std::vector<std::vector<char>> effects_to_draw) {
	draw_symbols(x, y, symbols_to_draw);
	draw_colors(x, y, colors_to_draw);
	draw_effects(x, y, effects_to_draw);
}

void teco::Screen::draw_symbols(int x, int y, std::vector<std::vector<char>> symbols_to_draw) {
	for (int line = 0; line < symbols_to_draw.size(); line++) {
		if (y+line <= height) {
			for (int column = 0; column < symbols_to_draw[line].size(); column++) {
				if (x+column <= width) {
					symbols[y+line][x+column] = symbols_to_draw[line][column];
				}
			}
		}
	}
}

void teco::Screen::draw_colors(int x, int y, std::vectgor<std::vector<char>> colors_to_draw) {
	for (int line = 0; line < colors_to_draw.size(); line++) {
		if (y+line <= height) {
			for (int column = 0; column < colors_to_draw[line].size(); column++) {
				if (x+column <= width) {
					symbols[y+line][x+column] = colors_to_draw[line][column];
				}
			}
		}
	}
}

void teco::Screen::draw_effects(int x, int y, std::vector<std::vector<char>> effects_to_draw) {
	for (int line = 0; line < effects_to_draw.size(); line++) {
		if (y+line <= height) {
			for (int column = 0; column < effects_to_draw[line].size(); column++) {
				if (x+column <= width) {
					symbols[y+line][x+column] = effects_to_draw[line][column];
				}
			}
		}
	}
}

// variables

int graphics type;

std::string title;

int fps;
int tps;

int window_width_in_symbols;
int window_height_in_symbols;
int window_width;
int window_height;

SDL_Event event;
SDL_Renderer *renderer = NULL;
SDL_Window *window = NULL;
SDL_Surface *window_surface = NULL;

std::vector<int> pressed_keys;
std::vector<char, SDL_Texture*> symbol_textures;

unfduration tick_slice = unfduration::zero();
unfduration draw_slice = unfduration::zero();
unfduration time_accumulator = unfduration::zero();
std::chrono::time_point_cast<unfduration> last_update_time = unftime();

bool run = true;

Screen *current_screen;

// functions

void teco::init(int _graphics_type = GUI; std::string& _title, int _fps = 60, int _tps = 20, int _window_width_in_symbols = 128, int _window_height_in_symbols = 128, int _window_width = 640, int _window_height = 480, Screen *_current_screen) {
	graphics_type = _graphics_type;

	title = _title;

	fps = _fps;
	tps = _tps;

	window_width_in_symbols = _window_width_in_symbols;
	window_height_in_symbols = _window_height_in_symbols;
	window_width = _window_width;
	window_height = _window_height;

	current_screen = _current_screen;

	draw_slice = unfduration(second_ratio / fps);
	tick_slice = unfduration(second_ratio / tps);

	if (graphics_type == GUI) {
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
			exit();
		}

		window = SDL_CreateWindow(
			title,
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			window_width, window_height,
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
		);

		if (window == NULL) {
			std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
			exit();
		}

		renderer = SDL_CreateRenderer(
			window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
		);

		if (renderer == NULL) {
			std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
			exit();
		}

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	}

	else if (graphics_type == TUI) {

	}
}

void teco::mainloop() {
	while (run) {
		auto delta_time = unftime() - last_update_time;
		last_update_time = unftime();
		time_accumulator += delta_time();

		if (graphics_type == GUI)
			handle_events_gui();

		else if (graphics_type == TUI)
			handle_events_tui();

		while (time_accumulator > tick_slice) {
			current_screen->tick();
			time_accumulator -= tick_slice;
		}

		if (graphics_type == GUI) {
			draw_gui();
			play_sounds();
		}

		else if (graphics_type == TUI)
			draw_tui();

		if (delta_time < draw_slice)
			unfsleep((draw_slice - delta_time).count());
	}
}

void teco::handle_events_gui() {
	while (SDL_PollEvent(&event) != 0) {
		if (event.type == SDL_QUIT) {
			exit();
		}

		else if (event.type == SDL_WINDOWEVENT) {
			if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
				window_width = event.window.data1;
				window_height = event.window.data2;
			}
		}

		else if (event.type == SDL_KEYDOWN) {
			pressed_keys.push_back(event.key.keysym.sym);
		}
	}
}

void teco::handle_events_tui() {

}

void teco::draw_gui() {
	SDL_RenderClear(renderer);

	current_screen.draw();

	for (int line = 0; line < current_screen.height; line++) {
		for (int column = 0; column < current_screen.width; column++) {
			if (current_screen.symbols[line][column] != ' ') {
				if (symbol_textures.count(current_screen.symbols[line][column]) == 0) {
					SDL_Color text_color = {229, 229, 229};
					SDL_Surface *test_surface = TTF_RenderText_Solid(font, current_symbol, text_color);
					symbol_textures[current_screen.symbols[line][column]] = SDL_CreateTextureFromSurface(renderer, text_surface);
                    SDL_FreeSurface(text_surface);
				}

				SDL_Rect text_rectangle = {
					column*window_width/current_screen.width,
					line*window_height/current_screen.height,
					window_width/current_screen.width,
					window_height/current_screen.height
				};

				SDL_RenderCopy(renderer, symbol_textures[current_screen.symbols[line][column]], NULL, &text_rectangle);
			}
		}
	}
}

void teco::draw_tui() {

}

void teco::play_sounds() {

}

void teco::exit() {
	run = false;

	for (const auto& [symbol, texture] : symbol_textures) {
		SDL_DestroyTexture(texture);
		symbol_textures.erase(symbol);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

