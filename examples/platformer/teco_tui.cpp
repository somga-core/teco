 ////// ////// ////// //////    ////// //  // //
  //   ////   //     //  //      //   //  // //
 //   //     //     //  //      //   //  // //
//   ////// ////// //////      //   ////// //

#include "teco_tui.hpp"

// engine functions
void teco::init_tui() {
	draw_slice = unfduration(second_ratio / fps);
	tick_slice = unfduration(second_ratio / tps);
	
	initscr();
    start_color();

	if (has_colors() == FALSE) {
		exit_tui();
	}
        
	curs_set(0);
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);

	if (can_change_color()) {
		init_color(0, background_red*1000/255, background_green*1000/255, background_blue*1000/255);

		for (const auto& [symbol, color] : colors) {
			init_color(symbol, color[0]*1000/255, color[1]*1000/255, color[2]*1000/255);
			init_pair(symbol, symbol, 0);
		}
	}
}

void teco::mainloop_tui() {
	mainloop(draw_tui, handle_events_tui);
}

void teco::handle_events_tui() {
	int ch = getch();
	pressed_keys.push_back(keybinds[ch]);
	flushinp();
}

void teco::draw_tui() {
	int x;
	int y;

	getmaxyx(stdscr, y, x);

	if (y >= current_screen->height) {
		for (int line = 0; line < current_screen->height; line++) {
			for (int column = 0; column < current_screen->width; column++) {
				char current_color = current_screen->colors[line][column];
				attron(COLOR_PAIR(current_color));
				mvprintw(line+(y-current_screen->height)/2, column+(x-current_screen->width)/2, "%c", current_screen->symbols[line][column]);
				attroff(COLOR_PAIR(current_color));
			}
		}
	}

	else {
		mvprintw(y/2-13, x/2-1, "Screen resolution is too small");
	}

	refresh();
	clear();
}

void teco::exit_tui() {
	run = false;

	endwin();
}


