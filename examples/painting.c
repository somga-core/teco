#define TECO_NCURSESW
#include "../teco.h"

int main(int argc, char *argv[]) {
    teco_image painting = teco_image_import("examples/assets/ship.tci", 2);
    teco_image guy = teco_image_import("examples/assets/guy.tci", 2);

	initscr();

	if (has_colors() == FALSE) return 1;
	start_color();
    use_default_colors();

    teco_palette_import("examples/assets/core.tcp", 0);

    WINDOW* window = newwin(10, 11, 0, 2);
    refresh();

    teco_image_draw_on_window(&painting, window, 0, 0);

    box(window, 0, 0);

    teco_image_draw(&guy, 6, 10);

    teco_image_resize(&guy, 1, guy.width, guy.height);

    teco_image_draw(&guy, 11, 10);
    teco_image_draw(&guy, 1, 10);
    
	wrefresh(window);

	getch();

    delwin(window);
	endwin();

    return 0;
}