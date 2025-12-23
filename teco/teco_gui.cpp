 ////// ////// ////// //////    ////// //  // //
  //   ////   //     //  //    //     //  // //
 //   //     //     //  //    //  // //  // //
//   ////// ////// //////    ////// ////// //

#include "teco_gui.hpp"

// variables
int teco::window_width;
int teco::window_height;

int teco::tick_count;

SDL_Event teco::event;
SDL_Renderer *teco::renderer = NULL;
SDL_Window *teco::window = NULL;
SDL_Surface *teco::window_surface = NULL;
TTF_Font *teco::font;

std::map<char, std::map<char, SDL_Texture*>> teco::saved_textures;

std::map<char, std::vector<float> (*) (int, int, int)> teco::effects;

// engine functions
void teco::init_gui(
	int _window_width,
	int _window_height,
	std::string font_path,
	int font_size,
	std::map<char, std::vector<float> (*) (int, int, int)> _effects
) {
	window_width = _window_width;
	window_height = _window_height;
	
	effects = _effects;
	
	draw_slice = unfduration(second_ratio / fps);
	tick_slice = unfduration(second_ratio / tps);
	
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		exit_gui();
	}
	
	window = SDL_CreateWindow(
		title.c_str(),
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		window_width, window_height,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);
	
	if (window == NULL) {
		exit_gui();
	}
	
	renderer = SDL_CreateRenderer(
		window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
		
	if (renderer == NULL) {
		exit_gui();
	}
	
	if (TTF_Init() == -1) {
		exit_gui();
	}

	font = TTF_OpenFont(font_path.c_str(), font_size);
	
	SDL_SetRenderDrawColor(renderer, background_red, background_green, background_blue, 0x00);
}

void teco::mainloop_gui() {
	mainloop(draw_gui, handle_events_gui);
}

void teco::handle_events_gui() {
	while (SDL_PollEvent(&event) != 0) {
		if (event.type == SDL_QUIT) {
			exit_gui();
		}

		else if (event.type == SDL_WINDOWEVENT) {
			if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
				window_width = event.window.data1;
				window_height = event.window.data2;
			}
		}

		else if (event.type == SDL_KEYDOWN) {
			if (keybinds.count(event.key.keysym.sym) != 0)
				pressed_keys.push_back(keybinds[event.key.keysym.sym]);
		}
	}
}

void teco::draw_gui() {
	SDL_RenderClear(renderer);

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
					SDL_Color text_color {colors[current_color][0], colors[current_color][1], colors[current_color][2]};
					SDL_Surface *text_surface = TTF_RenderText_Solid(font, current_symbol, text_color);
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

void teco::exit_gui() {
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

