#include "teco.hpp"

void tick_tock();
void draw_drew();

teco::Screen mein_screen = teco::Screen(192, 56, &tick_tock, &draw_drew);

teco::Sprite test_sprite = teco::Sprite(
    std::vector<teco::Animation> {
        teco::Animation(
            std::vector<teco::Source> {
                teco::Source("assets/example.tcsb", "assets/example.tccl", "assets/example.tcef")
            }
        )
    }
);

int main(int argc, char const *argv[])
{
    teco::init();

    teco::current_screen = &mein_screen;

    teco::mainloop();

    return 0;
}

void draw_drew() {
    mein_screen.draw_sprite(0, 0, test_sprite);
}

void tick_tock() {
}