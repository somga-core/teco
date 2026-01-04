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
	int _window_wifth = 640,
	int _window_height = 480,
	std::string font_path = "assets/font.ttf",
	int font_size = 20,
	std::map<char, std::vector<float> (*) (int, int, int)> effects = std::map<char, std::vector<float> (*) (int, int, int)>()
);
void mainloop_gui();
void handle_events_gui();
void draw_gui();
void exit_gui();

}

