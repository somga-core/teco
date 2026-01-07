#include "teco_gui.hpp"
#include <cmath>

void tick();
void draw();

enum {
    IDLE,
    WALK_LEFT,
    WALK_RIGHT,
    RUN_LEFT,
    RUN_RIGHT
};

teco::Screen screen;
teco::Sprite player;

int player_y_offset = 3;
int player_x_offset = 1;

float friction_strengh = .1f;
float run_strengh = .4f;
float walk_strengh = .2f;

float gravity_strengh = .4f;
float jump_strengh = 2.f;

float player_x = 32.f;
float player_y = 32.f;

float player_x_velocity = 0.f;
float player_y_velocity = 0.f;

int main() {
    teco::init(&screen);
    teco::init_gui();

    teco::keybinds = std::map<int, char> {
        {SDLK_w, 'w'},
        {SDLK_a, 'a'},
        {SDLK_d, 'd'},
        {SDLK_LSHIFT, '$'}
    };

    screen = teco::Screen(64, 32, &tick, &draw);

    player = teco::Sprite(
        std::vector<teco::Animation> {
            teco::Animation(
                std::vector<teco::Source> {
                    teco::Source("assets/idle/1.tcsb", "assets/empty.tccl", "assets/empty.tccl"),
                    teco::Source("assets/idle/2.tcsb", "assets/empty.tccl", "assets/empty.tccl")
                },
                teco::LOOPING,
                6
            ),
            teco::Animation(
                std::vector<teco::Source> {
                    teco::Source("assets/walk/left/1.tcsb", "assets/empty.tccl", "assets/empty.tccl"),
                    teco::Source("assets/walk/left/2.tcsb", "assets/empty.tccl", "assets/empty.tccl")
                },
                teco::LOOPING,
                2
            ),
            teco::Animation(
                std::vector<teco::Source> {
                    teco::Source("assets/walk/right/1.tcsb", "assets/empty.tccl", "assets/empty.tccl"),
                    teco::Source("assets/walk/right/2.tcsb", "assets/empty.tccl", "assets/empty.tccl")
                },
                teco::LOOPING,
                2
            ),
            teco::Animation(
                std::vector<teco::Source> {
                    teco::Source("assets/run/left/1.tcsb", "assets/empty.tccl", "assets/empty.tccl"),
                    teco::Source("assets/run/left/2.tcsb", "assets/empty.tccl", "assets/empty.tccl")
                },
                teco::LOOPING,
                2
            ),
            teco::Animation(
                std::vector<teco::Source> {
                    teco::Source("assets/run/right/1.tcsb", "assets/empty.tccl", "assets/empty.tccl"),
                    teco::Source("assets/run/right/2.tcsb", "assets/empty.tccl", "assets/empty.tccl")
                },
                teco::LOOPING,
                2
            ),
        }
    );

    teco::mainloop_gui();

    return 0;
}

void draw() {
    screen.draw_sprite((int) player_x - player_x_offset, (int) player_y - player_y_offset, player);
}

void tick() {
    if (std::abs(player_x_velocity) <= friction_strengh) {
        player.set_animation(IDLE);
        player_x_velocity = 0.f;
    } else if (player_x_velocity > 0) {
        player_x_velocity -= friction_strengh;
    } else if (player_x_velocity < 0) {
        player_x_velocity += friction_strengh;
    }

    player.update_animations();

    bool is_shift_pressed = std::count(teco::pressed_keys.begin(), teco::pressed_keys.end(), '$') >= 1;

    for (auto key : teco::pressed_keys) {
        switch (key) {
            case 'w':
                if (player_y == 32) {
                    player_y_velocity -= jump_strengh;
                }
                break;

            case 'a':
                if (is_shift_pressed) {
                    player_x_velocity -= run_strengh;
                    player.set_animation(RUN_LEFT);
                } else {
                    player_x_velocity -= walk_strengh;
                    player.set_animation(WALK_LEFT);
                }
                break;

            case 'd':
                if (is_shift_pressed) {
                    player_x_velocity += run_strengh;
                    player.set_animation(RUN_RIGHT);
                } else {
                    player_x_velocity += walk_strengh;
                    player.set_animation(WALK_RIGHT);
                }
                break;
        }
    }

    if (player_y < 32) {
        player_y_velocity += gravity_strengh;
    }

    player_x += player_x_velocity;
    player_y += player_y_velocity;

    if (player_y > 32) {
        player_y = 32.f;
        player_y_velocity = 0.f;
    }
}
