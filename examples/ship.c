#define TECO_SDL2
// #define TECO_RENDER_TEXT_SOLID
#include "../teco.h"
#include <math.h>

#define FPS 60

void effect_wave(float* dx, float* dy, float* scale_x, float* scale_y, unsigned x, unsigned y, va_list args) {
    *dy = sin(x / 2. + (va_arg(args, int) / 500.)) / 4 - .15;
}

void effect_ship(float* dx, float* dy, float* scale_x, float* scale_y, unsigned x, unsigned y, va_list args) {
    unsigned time = va_arg(args, unsigned);

    *dy = (-sin(time / 500. + 2) * (((int) x) - 7) / 20.) - .2 * sin(time / 500.);
}

void effect_blink(float* dx, float* dy, float* scale_x, float* scale_y, unsigned x, unsigned y, va_list args) {
    *scale_x = sin(y * 5. + x * 10. + (va_arg(args, int) / 500.));
    *scale_y = pow(*scale_x, 3);
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow(
        "TeCo example",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        640, 480,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    SDL_SetRenderDrawColor(renderer, 0x12, 0x12, 0x12, 0x00);

    TTF_Font* font = TTF_OpenFont("examples/assets/font.ttf", 30);

    char run = 1;

    SDL_Event event;

    teco_image image = teco_image_import("examples/assets/ship.tci", TECO_SCE);
    teco_palette palette = teco_palette_import("examples/assets/core.tcp");
    teco_cache cache = teco_cache_create();

    teco_bulb bulb = teco_bulb_create();
    teco_bulb_set(&bulb, '#', &effect_wave);
    teco_bulb_set(&bulb, '/', &effect_ship);
    teco_bulb_set(&bulb, '+', &effect_blink);

    while (run) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                run = 0;
            }
        }

        SDL_RenderClear(renderer);

        teco_image_draw(&image, &palette, &bulb, renderer, &cache, font, SDL_GetTicks());

        SDL_RenderPresent(renderer);

        SDL_Delay(1000 / FPS);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    teco_image_delete(&image);
    teco_cache_delete(&cache);
    teco_palette_delete(&palette);

    SDL_Quit();

    return 0;
}