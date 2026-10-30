#define TECO_SDL2
#include "../teco.h"

#define FPS 60

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

    TTF_Font* font = TTF_OpenFont("examples/assets/font.ttf", 20);

    bool run = true;

    SDL_Event event;

    teco_image image = teco_image_import("examples/assets/ship.tci", TECO_SCE);
    teco_palette palette = teco_palette_import("examples/assets/core.tcp");
    teco_cache cache = teco_cache_create();

    while (run) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                run = false;
            }
        }

        SDL_RenderClear(renderer);

        teco_image_draw(&image, &palette, renderer, &cache, font);

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