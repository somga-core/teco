#include "teco_gui.hpp"

void tick();
void draw();
std::vector<float> wave_effect(int x, int y, int tick_count);
std::vector<float> squish_effect(int x, int y, int tick_count);
int animation_switcher;

teco::Screen main_screen;
teco::Sprite test_sprite;

int main() {
    // initializing teco
    teco::init(&main_screen);
    teco::init_gui();

    teco::effects['#'] = wave_effect;
    teco::effects['&'] = squish_effect;

    teco::keybinds = std::map<int, char> {
        {'w', 'w'}
    };

    // setting variables
    animation_switcher = 0;

    main_screen = teco::Screen(48, 14, &tick, &draw);
    
    // setting up sprite with 2 animations
    test_sprite = teco::Sprite(
        std::vector<teco::Animation> {
            teco::Animation(
                std::vector<teco::Source> {
                    teco::Source("assets/squish/def.tcsb", "assets/squish/def.tccl", "assets/squish/def.tcef"),
                }
            ),
            teco::Animation(
                std::vector<teco::Source> {
                    teco::Source("assets/wave/def.tcsb", "assets/wave/def.tccl", "assets/wave/def.tcef")
                }
            )
        }
    );

    // starting main loop (obviously)
    teco::mainloop_gui();

    return 0;
}

void draw() {
    main_screen.draw_sprite(0, 0, test_sprite);
}

void tick() {
    test_sprite.update_animations();

    // checking key presses
    if (std::find(teco::pressed_keys.begin(), teco::pressed_keys.end(), 'w') != teco::pressed_keys.end())
        animation_switcher = abs(animation_switcher - 1);
        test_sprite.set_animation(animation_switcher);

    teco::pressed_keys.clear();
}

std::vector<float> wave_effect(int x, int y, int tick_count) {
    return std::vector<float> {
        0,
        (float) std::sin((tick_count * .5 + (x * 4 + y * 6) * (-1 + (y % 2) * 2)) / 4.0) / 8, // calculating y position using sin on calibrated x, y, and tick_count 
        1,
        1
    };
};

std::vector<float> squish_effect(int x, int y, int tick_count) {
    // calculating size with position using sin on calibrated x, y, and tick_count
    float mult_x = (float) .5 + std::abs(std::sin(tick_count / 4.0 + x + y) - 1) / 4;
    float mult_y = (float) .5 + (std::sin(tick_count / 4.0 + x + y) + 1) / 4;
    
    return std::vector<float> {
        (1 - mult_x) / 2,
        (1 - mult_y) / 2,
        mult_x,
        mult_y
    };
};
