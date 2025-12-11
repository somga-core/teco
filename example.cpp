#include "teco.hpp"

void tick_tock();
void draw_drew();
int c = 0;

teco::Screen mein_screen = teco::Screen(192 / 2, 56 / 2, &tick_tock, &draw_drew);

teco::Sprite test_sprite = teco::Sprite(
    std::vector<teco::Animation> {
        teco::Animation(
            std::vector<teco::Source> {
                teco::Source("assets/example_1_1.tcsb", "assets/example_1.tccl", "assets/example.tcef"),
                teco::Source("assets/example_1_2.tcsb", "assets/example_1.tccl", "assets/example.tcef"),
                teco::Source("assets/example_1_3.tcsb", "assets/example_1.tccl", "assets/example.tcef"),
                teco::Source("assets/example_1_4.tcsb", "assets/example_1.tccl", "assets/example.tcef")
            },
            teco::LOOPING,
            2
        ),
        teco::Animation(
            std::vector<teco::Source> {
                teco::Source("assets/example_2_1.tcsb", "assets/example_2.tccl", "assets/example.tcef"),
                teco::Source("assets/example_2_2.tcsb", "assets/example_2.tccl", "assets/example.tcef")
            },
            teco::LOOPING,
            10
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
    test_sprite.update_animations();

    // std::cout << c << std::endl;

    if (c++ == 100)
        test_sprite.play_animation(1);
}