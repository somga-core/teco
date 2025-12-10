#include "teco.hpp"

void tick_tock() {
    std::cout << "TICKIFICATION!!!" << std::endl;
}

void draw_drew() {
    std::cout << "DRAWIFICATION!!!" << std::endl;
}

int main(int argc, char const *argv[])
{
    teco::init();

    teco::Screen mein_screen = teco::Screen(100, 100, &tick_tock, &draw_drew);

    teco::current_screen = &mein_screen;

    teco::mainloop();

    return 0;
}
