 ////// ////// ////// //////    ////// //  // //
  //   ////   //     //  //    //     //  // //
 //   //     //     //  //    //  // //  // //
//   ////// ////// //////    ////// ////// //

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "teco_engine.hpp"

namespace teco {

// variables

extern int window_width;
extern int window_height;

extern SDL_Event event;
extern SDL_Renderer *renderer;
extern SDL_Window *window;
extern SDL_Surface *window_surface;
extern TTF_Font *font;

extern std::map<char, std::map<char, SDL_Texture*>> saved_textures;

extern std::map<char, std::vector<float> (*) (int, int, int)> effects;

// engine functions
void init_gui(
	Screen *current_screen = NULL,
	std::string _title = "TeCo",
	int _fps = 60,
	int _tps = 20,
	int _window_wifth = 640,
	int _window_height = 480,
	std::map<int, char> _keybinds = std::map<int, char> {},
	std::string font_path = "assets/font.ttf",
	int font_size = 20,
	std::map<char, std::vector<float> (*) (int, int, int)> effects = std::map<char, std::vector<float> (*) (int, int, int)>(),
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
	int background_red = 0x12,
	int background_green = 0x12,
	int background_blue = 0x12
);
void mainloop_gui();
void handle_events_gui();
void draw_gui();
void exit_gui();

}

