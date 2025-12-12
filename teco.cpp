#include "teco.hpp"

// class methods
teco::Source::Source(std::string symbols_path, std::string colors_path, std::string effects_path) {
	symbols = read_file(symbols_path);
	colors = read_file(colors_path);
	effects = read_file(effects_path);
}

std::vector<std::vector<char>> teco::Source::read_file(std::string file_name) {
	std::vector<std::vector<char>> result;
	std::string line;
	int line_index = 0;

	std::ifstream in = std::ifstream(file_name);

	if (in.is_open()) {
		while (std::getline(in, line)) {
			result.push_back(std::vector<char>());

			for (char symbol : line) {
				result[line_index].push_back(symbol);
			}

			line_index++;
		}

		in.close();
	}

	return result;
}

teco::Animation::Animation(std::vector<Source> _sources, int _loop_mode, int _ticks_per_frame) {
	sources = _sources;
	loop_mode = _loop_mode;
	ticks_per_frame = _ticks_per_frame;
}

teco::Sprite::Sprite(std::vector<Animation> _animations, int _current_animation_index, int _current_animation_frame_index) {
	animations = _animations;

	force_animation(_current_animation_index);

	current_animation_frame_index = _current_animation_frame_index;
}

void teco::Sprite::set_animation(int animation_index) {
	if (animation_index != current_animation_index) {
		force_animation(animation_index);
	}
}

void teco::Sprite::force_animation(int animation_index) {
	current_animation_index = animation_index;
	is_playing_animations = true;
	current_animation_frame_index = 0;
	current_animation_tick = 0;
}

void teco::Sprite::update_animations() {
	if (
		is_playing_animations && 
		++current_animation_tick == animations[current_animation_index].ticks_per_frame
	) {
		if (++current_animation_frame_index >= animations[current_animation_index].sources.size()) {
			switch (animations[current_animation_index].loop_mode) {
				case LOOPING:
					current_animation_frame_index = 0;
					break;
				case STOP_ON_LAST_FRAME:
					is_playing_animations = false;
					current_animation_frame_index--;
					break;
				case STOP_ON_FIRST_FRAME:
					is_playing_animations = false;
					current_animation_frame_index = 0;
					break;
			}
		}
	}
	if (current_animation_tick >= animations[current_animation_index].ticks_per_frame) {
		current_animation_tick = 0;
	}
}

teco::Subscreen::Subscreen(int _width, int _height) {
	width = _width;
	height = _height;

	for (int line = 0; line < height; line++) {
		symbols.push_back(std::vector<char>());
		symbols.back().resize(width, ' ');
		colors.push_back(std::vector<char>());
		colors.back().resize(width, ' ');
		effects.push_back(std::vector<char>());
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

void teco::Subscreen::draw_sprite(int x, int y, teco::Sprite sprite_to_draw) {
	draw_all(x, y, SPRITE_SYMBOLS(sprite_to_draw), SPRITE_COLORS(sprite_to_draw), SPRITE_EFFECTS(sprite_to_draw));
}

void teco::Subscreen::draw_all(int x, int y, std::vector<std::vector<char>> symbols_to_draw, std::vector<std::vector<char>> colors_to_draw, std::vector<std::vector<char>> effects_to_draw) {
	teco::draw_chars_on_something(x, y, symbols, symbols_to_draw);
	teco::draw_chars_on_something(x, y, colors, colors_to_draw);
	teco::draw_chars_on_something(x, y, effects, effects_to_draw);
}

teco::Screen::Screen(int _width, int _height, void (*_tick) (), void (*_draw) ()) {
	width = _width;
	height = _height;
	tick = _tick;
	draw = _draw;

	for (int line = 0; line < height; line++) {
		symbols.push_back(std::vector<char>());
		symbols.back().resize(width, ' ');
		colors.push_back(std::vector<char>());
		colors.back().resize(width, ' ');
		effects.push_back(std::vector<char>());
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

void teco::Screen::draw_sprite(int x, int y, teco::Sprite sprite_to_draw) {
	draw_all(x, y, SPRITE_SYMBOLS(sprite_to_draw), SPRITE_COLORS(sprite_to_draw), SPRITE_EFFECTS(sprite_to_draw));
}

void teco::Screen::draw_all(int x, int y, std::vector<std::vector<char>> symbols_to_draw, std::vector<std::vector<char>> colors_to_draw, std::vector<std::vector<char>> effects_to_draw) {
	teco::draw_chars_on_something(x, y, symbols, symbols_to_draw);
	teco::draw_chars_on_something(x, y, colors, colors_to_draw);
	teco::draw_chars_on_something(x, y, effects, effects_to_draw);
}

// variables
int teco::graphics_type;

std::string teco::title;

int teco::fps;
int teco::tps;

int teco::window_width_in_symbols;
int teco::window_height_in_symbols;
int teco::window_width;
int teco::window_height;

int teco::tick_count;

SDL_Event teco::event;
SDL_Renderer *teco::renderer = NULL;
SDL_Window *teco::window = NULL;
SDL_Surface *teco::window_surface = NULL;
TTF_Font *teco::font;

std::vector<int> teco::pressed_keys;

std::map<char, std::map<char, SDL_Texture*>> teco::saved_textures;

std::map<char, SDL_Color> teco::colors;
char teco::default_color;

std::map<char, std::vector<float> (*) (int, int, int)> teco::effects;

unfduration teco::tick_slice = unfduration::zero();
unfduration teco::draw_slice = unfduration::zero();
unfduration teco::time_accumulator = unfduration::zero();
unftimepoint teco::last_update_time = unftime();

bool teco::run = true;

teco::Screen *teco::current_screen;

// functions
void teco::init(
	Screen *_current_screen,
	int _graphics_type,
	std::string _title,
	int _fps,
	int _tps,
	int _window_width,
	int _window_height,
	std::string font_path,
	int font_size,
	std::map<char,
	std::vector<float> (*) (int, int, int)> effects,
	std::map<char, SDL_Color> _colors,
	char _default_color,
	int background_red, 
	int background_green,
	int background_blue
) {
	graphics_type = _graphics_type;

	title = _title;

	fps = _fps;
	tps = _tps;

	window_width = _window_width;
	window_height = _window_height;

	current_screen = _current_screen;

	colors = _colors;
	default_color = _default_color;
	
	draw_slice = unfduration(second_ratio / fps);
	tick_slice = unfduration(second_ratio / tps);
	
	if (graphics_type == GUI) {
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			exit();
		}
		
		window = SDL_CreateWindow(
			title.c_str(),
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			window_width, window_height,
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
		);
		
		if (window == NULL) {
			exit();
		}
		
		renderer = SDL_CreateRenderer(
			window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
		);
		
		if (renderer == NULL) {
			exit();
		}
		
		if (TTF_Init() == -1) {
			exit();
		}

		font = TTF_OpenFont(font_path.c_str(), font_size);
		
		SDL_SetRenderDrawColor(renderer, background_red, background_green, background_blue, 0x00);
	}

	else if (graphics_type == TUI) {

	}
}

void teco::mainloop() {
	while (run) {
		auto delta_time = unftime() - last_update_time;
		last_update_time = unftime();
		time_accumulator += delta_time;

		if (graphics_type == GUI)
			handle_events_gui();
		else if (graphics_type == TUI)
			handle_events_tui();
			
		while (time_accumulator > tick_slice) {
			current_screen->tick();
			tick_count++;
	
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

	current_screen->draw();

	char current_symbol[2] = " ";

	for (int line = 0; line < current_screen->height; line++) {
		for (int column = 0; column < current_screen->width; column++) {
			if (current_screen->symbols[line][column] != ' ') {
				current_symbol[0] = current_screen->symbols[line][column];
				char current_color = current_screen->colors[line][column];
				char current_effect = current_screen->effects[line][column];

				if (current_color == ' ' || colors.count(current_color) == 0)
					current_color = default_color;

				if (saved_textures.count(current_symbol[0]) == 0 || saved_textures[current_symbol[0]].count(current_color) == 0) {
					SDL_Surface *text_surface = TTF_RenderText_Solid(font, current_symbol, colors[current_color]);
					saved_textures[current_screen->symbols[line][column]][current_color] = SDL_CreateTextureFromSurface(renderer, text_surface);
                    SDL_FreeSurface(text_surface);
				}

				SDL_Rect text_rectangle = {
					column * window_width / current_screen->width,
					line * window_height / current_screen->height,
					(window_width / current_screen->width),
					(window_height / current_screen->height)
				};
				
				if (current_effect != ' ' && effects.count(current_effect) >= 1) {
					std::vector<float> offsets = effects[current_effect](column, line, tick_count);
				
					text_rectangle.x += offsets[0] * (window_width / current_screen->width);
					text_rectangle.y += offsets[1] * (window_height / current_screen->height);
					text_rectangle.w *= offsets[2];
					text_rectangle.h *= offsets[3];
				}

				SDL_RenderCopy(renderer, saved_textures[current_symbol[0]][current_color], NULL, &text_rectangle);
			}
		}
	}

	SDL_RenderPresent(renderer);
}

void teco::draw_tui() {

}

void teco::draw_chars_on_something(int x, int y, std::vector<std::vector<char>> &something_to_draw_on, std::vector<std::vector<char>> chars_to_draw) {
	for (int line = 0; line < chars_to_draw.size(); line++) {
		if (y+line <= something_to_draw_on.size()) {
			for (int column = 0; column < chars_to_draw[line].size(); column++) {
				if (x+column <= something_to_draw_on[column].size()) {
					if (chars_to_draw[line][column] != ' ')
						something_to_draw_on[y+line][x+column] = chars_to_draw[line][column];
				}
			}
		}
	}
}

void teco::play_sounds() {

}

void teco::exit() {
	run = false;

	for (const auto& [symbol, colors] : saved_textures) {
		for (const auto& [color, texture] : colors) {
			SDL_DestroyTexture(texture);
	}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
