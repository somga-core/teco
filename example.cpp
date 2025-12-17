#include "teco.hpp"

void tick_tock();
void draw_drew();
std::vector<float> wave_effect(int x, int y, int tick_count);
std::vector<float> squish_effect(int x, int y, int tick_count);
int animation_switcher;

teco::Screen mein_screen = teco::Screen(192 / 4, 56 / 4, &tick_tock, &draw_drew);

teco::Sprite test_sprite = teco::Sprite(
    std::vector<teco::Animation> {
        teco::Animation(
            std::vector<teco::Source> {
                teco::Source("assets/example_1.tcsb", "assets/example_1.tccl", "assets/example_1.tcef"),
            },
            teco::LOOPING,
            2
        ),
        teco::Animation(
            std::vector<teco::Source> {
                teco::Source("assets/example_2.tcsb", "assets/example_2.tccl", "assets/example_2.tcef")
            },
            teco::LOOPING,
            10
        )
    }
);

int main(int argc, char const *argv[])
{
    teco::init(&mein_screen, teco::TUI, "z", 60, 20);

    teco::effects['#'] = wave_effect;
    teco::effects['&'] = squish_effect;

    teco::keybinds = std::map<int, char> {
        {'w', 'w'}
    };

    animation_switcher = 0;

    teco::mainloop();

    return 0;
}

void draw_drew() {
    mein_screen.draw_sprite(0, 0, test_sprite);
}

void tick_tock() {
    test_sprite.update_animations();

    if (std::find(teco::pressed_keys.begin(), teco::pressed_keys.end(), 'w') != teco::pressed_keys.end())
        animation_switcher = abs(animation_switcher - 1);
        test_sprite.set_animation(animation_switcher);

    teco::pressed_keys.clear();
}

std::vector<float> wave_effect(int x, int y, int tick_count) {
    return std::vector<float> {
        0,
        (float) std::sin((tick_count * .5 + (x * 4 + y * 6) * (-1 + (y % 2) * 2)) / 4.0) / 8,
        1,
        1
    };
};

std::vector<float> squish_effect(int x, int y, int tick_count) {
    float mult_x = (float) .5 + std::abs(std::sin(tick_count / 4.0 + x + y) - 1) / 4;
    float mult_y = (float) .5 + (std::sin(tick_count / 4.0 + x + y) + 1) / 4;

    return std::vector<float> {
        (1 - mult_x) / 2,
        (1 - mult_y) / 2,
        mult_x,
        mult_y
    };
};
