 ////// ////// ////// //////    ////// //  // ////// // //  // //////
  //   ////   //     //  //    ////   /// // //     // /// // ////
 //   //     //     //  //    //     // /// //  // // // /// //
//   ////// ////// //////    ////// //  // ////// // //  // //////

#include "teco_engine.hpp"

// class methods
teco::Source::Source(std::string symbols_path, std::string colors_path, std::string effects_path) {
	symbols = teco::get_chars_from_file(symbols_path);
	colors = teco::get_chars_from_file(colors_path);
	effects = teco::get_chars_from_file(effects_path);
}

teco::Source::Source(std::vector<std::vector<char>> _symbols, std::vector<std::vector<char>> _colors, std::vector<std::vector<char>> _effects) {
	symbols = _symbols;
	colors = _colors;
	effects = _effects;
}

teco::Source::Source(std::vector<std::string> symbol_strings, std::vector<std::string> color_strings, std::vector<std::string> effect_strings) {
	symbols = teco::get_chars_from_strings(symbol_strings);
	colors = teco::get_chars_from_strings(color_strings);
	effects = teco::get_chars_from_strings(effect_strings);
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

void teco::Screen::draw_sprite(int x, int y, teco::Sprite& sprite_to_draw) {
	draw_all(x, y, SPRITE_SYMBOLS(sprite_to_draw), SPRITE_COLORS(sprite_to_draw), SPRITE_EFFECTS(sprite_to_draw));
}

void teco::Screen::draw_screen(int x, int y, teco::Screen& screen_to_draw) {
	draw_all(x, y, screen_to_draw.symbols, screen_to_draw.colors, screen_to_draw.effects);
}

void teco::Screen::draw_char(int x, int y, char symbol_to_draw, char color_to_draw, char effect_to_draw) {
	symbols[y][x] = symbol_to_draw;
	symbols[y][x] = color_to_draw;
	symbols[y][x] = effect_to_draw;
}

void teco::Screen::draw_all(int x, int y, std::vector<std::vector<char>>& symbols_to_draw, std::vector<std::vector<char>>& colors_to_draw, std::vector<std::vector<char>>& effects_to_draw) {
	teco::draw_chars_on_something(x, y, symbols, symbols_to_draw);
	teco::draw_chars_on_something(x, y, colors, colors_to_draw);
	teco::draw_chars_on_something(x, y, effects, effects_to_draw);
}

// variables
std::string teco::title;

int tick_count;

int teco::fps;
int teco::tps;

int teco::window_width_in_symbols;
int teco::window_height_in_symbols;

std::vector<char> teco::pressed_keys;

std::map<char, std::vector<unsigned char>> teco::colors;
int teco::background_red;
int teco::background_green;
int teco::background_blue;
char teco::default_color;

std::map<int, char> teco::keybinds;

unfduration teco::tick_slice = unfduration::zero();
unfduration teco::draw_slice = unfduration::zero();
unfduration teco::time_accumulator = unfduration::zero();
unftimepoint teco::last_update_time = unftime();

bool teco::run = true;

teco::Screen *teco::current_screen;

// engine functions
void teco::init(
	Screen *_current_screen,
	std::string _title,
	int _fps,
	int _tps,
	std::map<int, char> _keybinds,
	std::map<char, std::vector<unsigned char>> _colors,
	char _default_color,
	int _background_red,
	int _background_green,
	int _background_blue
) {
	title = _title;

	fps = _fps;
	tps = _tps;

	current_screen = _current_screen;

	colors = _colors;
	default_color = _default_color;
	
	keybinds = _keybinds;

	background_red = _background_red;
	background_green = _background_green;
	background_blue = _background_blue;
}

void teco::mainloop(void (*draw) (), void (*handle_events) ()) {
	while (run) {
		auto delta_time = unftime() - last_update_time;
		last_update_time = unftime();
		time_accumulator += delta_time;

		handle_events();
			
		while (time_accumulator > tick_slice) {
			current_screen->tick();
			tick_count++;
	
			time_accumulator -= tick_slice;
		}

		current_screen->clear();
		current_screen->draw();

		draw();

		if (delta_time < draw_slice)
			unfsleep((draw_slice - delta_time).count());
	}
}

// utility functions
void teco::draw_chars_on_something(int x, int y, std::vector<std::vector<char>> &something_to_draw_on, std::vector<std::vector<char>> chars_to_draw) {
	for (int line = 0; line < chars_to_draw.size(); line++) {
		if (y+line <= something_to_draw_on.size()) {
			for (int column = 0; column < chars_to_draw[line].size(); column++) {
				if (x+column <= something_to_draw_on[line].size()) {
					if (chars_to_draw[line][column] != ' ')
						something_to_draw_on[y+line][x+column] = chars_to_draw[line][column];
				}
			}
		}
	}
}

std::vector<std::vector<char>> teco::get_chars_from_file(std::string path) {
	std::vector<std::vector<char>> result;
	std::string line;
	int line_index = 0;

	std::ifstream in = std::ifstream(path);

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

std::vector<std::vector<char>> teco::get_chars_from_strings(std::vector<std::string> strings) {
	std::vector<std::vector<char>> result = std::vector<std::vector<char>>();

	for (std::string string : strings)
		result.push_back(std::vector<char>(string.begin(), string.end()));

	return result;
}

