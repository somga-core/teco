#ifndef TECO
#define TECO

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define TECO_LAYERS 3
#define TECO_LAYER_BITS 2
#define TECO_LAYER_MASK 0b11

#define TECO_S 0b01
#define TECO_C 0b10
#define TECO_E 0b11

#define TECO_SC 0b1001
#define TECO_SE 0b1101
#define TECO_CE 0b1110

#define TECO_SCE 0b111001

// DECLARATION!!!!!
typedef struct teco_image {
    unsigned width; unsigned height;
    char* data[TECO_LAYERS];
} teco_image;

teco_image teco_image_create(unsigned width, unsigned height);
teco_image teco_image_copy(teco_image* image, unsigned char layers);
teco_image teco_image_import(char* path, unsigned char layers);

void teco_image_delete(teco_image* image);
void teco_image_resize(teco_image* image, unsigned new_width, unsigned new_height);

void teco_image_export(teco_image* image, char* path, char space_symbol);

void teco_image_apply(teco_image* image_to_apply_on, teco_image* image_to_apply, unsigned char layers, long x, long y);

char teco_image_get(teco_image* image, unsigned char layer, unsigned x, unsigned y);
void teco_image_set(teco_image* image, unsigned char layer, char character, unsigned x, unsigned y);

void teco_image_rect(teco_image* image, unsigned char layers, char* characters, long x1, long y1, long x2, long y2);
void teco_image_line(teco_image* image, unsigned char layers, char* characters, long x1, long y1, long x2, long y2);
void teco_image_fill(teco_image* image, unsigned char layers, char* characters);

void teco_image_print(teco_image* image, unsigned char layers);

// IMPLEMENTATION!!!!!
teco_image teco_image_create(unsigned width, unsigned height) {
    teco_image image = {.width=width, .height=height};

    unsigned long long size = width * height;

    for (char layer = 0; layer < TECO_LAYERS; layer++)
        image.data[layer] = malloc(size);

    teco_image_fill(&image, TECO_SCE, "\0\0\0");

    return image;
};

teco_image teco_image_copy(teco_image* image, unsigned char layers) {
    teco_image new_image = teco_image_create(image->width, image->height);

    for (
        unsigned char layer, shifted = layers;
        (layer = shifted & TECO_LAYER_MASK) != 0;
        shifted >>= TECO_LAYER_BITS
    ) {
        for (unsigned y = 0; y < image->height; y++) {
            for (unsigned x = 0; x < image->width; x++) {
                teco_image_set(
                    &new_image, layer,
                    teco_image_get(image, layer, x, y),
                    x, y
                );
            }
        }
    }

    return new_image;
};

teco_image teco_image_import(char* path, unsigned char layers) {
    unsigned width = 0, height = 0;
    char buffer;
    // All "space" synbols in file will become spaces in image,
    // all spaces in file will become '\0' in image
    char space_symbol;
    FILE* file_stream = fopen(path, "r");

    if (!file_stream) return teco_image_create(0, 0);

    fscanf(file_stream, "%u %u %c", &width, &height, &space_symbol);

    while (getc(file_stream) != '\n') {};

    teco_image image = teco_image_create(width, height);

    for (
        unsigned char layer, shifted = layers;
        (layer = shifted & TECO_LAYER_MASK) != 0;
        shifted >>= TECO_LAYER_BITS
    ) {
        for (unsigned y = 0; y < image.height; y++) {
            for (
                unsigned x = 0;
                (buffer = getc(file_stream)) != '\n' && x < image.width;
                x++
            ) {
                if (buffer == EOF) goto teco_image_import_end;

                // Maybe invert this behavour and specify in a
                // file not a space symbol, but an empty symbol
                if (buffer == space_symbol)
                    teco_image_set(&image, layer, ' ', x, y);
                else if (buffer != ' ')
                    teco_image_set(&image, layer, buffer, x, y);
            }
        }
    }

teco_image_import_end:
    fclose(file_stream);
    return image;
};

void teco_image_delete(teco_image* image) {
    for (char layer = 0; layer < TECO_LAYERS; layer++)
        free(image->data[layer]);

    image->width = 0;
    image->height = 0;
};

void teco_image_resize(teco_image* image, unsigned new_width, unsigned new_height) {
    teco_image new_image = teco_image_create(new_width, new_height);

    teco_image_apply(&new_image, image, TECO_SCE, 0, 0);

    teco_image_delete(image);

    *image = new_image;
};

void teco_image_export(teco_image* image, char* path, char space_symbol) {
    FILE* file_stream = fopen(path, "w");
    
    if (!file_stream) return;

    fprintf(file_stream, "%u %u %c\n", image->width, image->height, space_symbol);

    for (unsigned char layer = 1; layer <= TECO_LAYERS; layer++) {
        for (unsigned y = 0; y < image->height; y++) {
            for (unsigned x = 0; x < image->width; x++) {
                char character = teco_image_get(image, layer, x, y);

                if (character == '\0')
                    fputc(' ', file_stream);
                else if (character == ' ')
                    fputc(space_symbol, file_stream);
                else
                    fputc(character, file_stream);
            }
            fputc('\n', file_stream);
        }
    }
    
    fclose(file_stream);
};

void teco_image_apply(teco_image* canvas, teco_image* image, unsigned char layers, long x, long y) {
    if (
        x >= (long) canvas->width ||
        y >= (long) canvas->height ||
        x + ((long) image->width) <= 0 ||
        y + ((long) image->height) <= 0
    ) return;

    unsigned image_xl = 0, image_xr = 0, canvas_x = 0;
    unsigned image_yu = 0, image_yd = 0, canvas_y = 0;

    // Crop x
    if (x < 0)
        image_xl = -x;
    else
        canvas_x = x;

    if (x + image->width > canvas->width)
        image_xr = canvas->width - x;
    else
        image_xr = image->width;

    // Crop y
    if (y < 0)
        image_yu = -y;
    else
        canvas_y = y;

    if (y + image->height > canvas->height)
        image_yd = canvas->height - y;
    else
        image_yd = image->height;

    // Draw
    for (
        unsigned char layer, shifted = layers;
        (layer = shifted & TECO_LAYER_MASK) != 0;
        shifted >>= TECO_LAYER_BITS
    ) {
        for (unsigned dy = image_yu; dy < image_yd; dy++) {
            for (unsigned dx = image_xl; dx < image_xr; dx++) {
                char character = teco_image_get(image, layer, dx, dy);

                if (character != '\0')
                    teco_image_set(
                        canvas,
                        layer,
                        character,
                        dx + canvas_x - image_xl,
                        dy + canvas_y - image_yu
                    );
            }
        }
    }
};

char teco_image_get(teco_image* image, unsigned char layer, unsigned x, unsigned y) {
    return image->data[layer - 1][image->width * y + x];
};

void teco_image_set(teco_image* image, unsigned char layer, char character, unsigned x, unsigned y) {
    image->data[layer - 1][image->width * y + x] = character;
};

void teco_image_rect(teco_image* image, unsigned char layers, char* characters, long xl, long yu, long xr, long yd) {
    if (xl < 0) xl = 0;
    if (xr >= image->width) xr = image->width - 1;
    if (yu < 0) yu = 0;
    if (yd >= image->height) yd = image->height - 1;

    for (
        unsigned char i = 0, layer, shifted = layers;
        (layer = shifted & TECO_LAYER_MASK) != 0;
        shifted >>= TECO_LAYER_BITS
    ) {
        for (unsigned y = yu; y <= yd; y++)
            for (unsigned x = xl; x <= xr; x++)
                teco_image_set(image, layer, characters[i], x, y);
        
        i++;
    }
};

void teco_image_fill(teco_image* image, unsigned char layers, char* characters) {
    for (
        unsigned char i = 0, layer, shifted = layers;
        (layer = shifted & TECO_LAYER_MASK) != 0;
        shifted >>= TECO_LAYER_BITS
    ) {
        for (unsigned y = 0; y < image->height; y++)
            for (unsigned x = 0; x < image->width; x++)
                teco_image_set(image, layer, characters[i], x, y);

        i++;
    }
};

void teco_image_print(teco_image* image, unsigned char layers) {
    for (
        unsigned char layer, shifted = layers;
        (layer = shifted & TECO_LAYER_MASK) != 0;
        shifted >>= TECO_LAYER_BITS
    ) {
        for (unsigned y = 0; y < image->height; y++) {
            for (unsigned x = 0; x < image->width; x++) {
                char character = teco_image_get(image, layer, x, y);
                
                if (character == '\0') 
                    printf(" ");
                else
                    printf("%c", character);
            }
            printf("\n");
        }
        printf("\n");
    }
};

#if defined(TECO_SDL2)

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// DECLARATION!!!
typedef struct teco_palette {
    char* characters;
    unsigned char* colors;
    unsigned short size;
} teco_palette;

teco_palette teco_palette_import(char* path);
teco_palette teco_palette_create();
void teco_palette_delete(teco_palette* palette);

void teco_palette_set_rgb(teco_palette* palette, char character, unsigned char red, unsigned char green, unsigned char blue);
void teco_palette_set_hex(teco_palette* palette, char character, char* hex); // Maybe this function is not needed

unsigned short teco_palette_get_index(teco_palette* palette, char character);
char teco_palette_get_character(teco_palette* palette, unsigned short index);
void teco_palette_get_color(teco_palette* palette, unsigned short index, unsigned char* red, unsigned char* green, unsigned char* blue);

typedef struct teco_cache {
    char* symbols; // Previously this was named charcol (Short for character & color) (Pun intended)
    SDL_Texture** textures;
    unsigned short size;
    // Maybe replace "symbols" with "keys" variable: 256 elements long array of chars, that stores indices of textures
} teco_cache;

teco_cache teco_cache_create();
void teco_cache_delete(teco_cache* cache);

void teco_cache_add(teco_cache* cache, char symbol, SDL_Texture* texture);

unsigned short teco_cache_get_index(teco_cache* cache, char symbol);
char teco_cache_get_symbol(teco_cache* cache, unsigned short index);
SDL_Texture* teco_cache_get_texture(teco_cache* cache, unsigned short index);

typedef void (*teco_effect) (float*, float*, float*, float*, unsigned, unsigned, va_list args);

typedef struct teco_bulb {
    char* characters;
    teco_effect* effects;
    unsigned short size;
} teco_bulb; // Maybe call this shaderpack. Sane name is needed

teco_bulb teco_bulb_create();
void teco_bulb_delete(teco_bulb* bulb);

void teco_bulb_set(teco_bulb* bulb, char character, teco_effect effect);

unsigned short teco_bulb_get_index(teco_bulb* bulb, char character);
char teco_bulb_get_character(teco_bulb* bulb, unsigned short index);
teco_effect teco_bulb_get_effect(teco_bulb* bulb, unsigned short index);
// Maybe condence theese similar functions somehow
// Maybe add function to delete only one element from palette or bulb

// Maybe add another parameter that defines how to render symbols: smoothly or solidly
void teco_image_draw(teco_image* image, teco_palette* palette, teco_bulb* bulb, SDL_Renderer* renderer, teco_cache* cache, TTF_Font* font, ...);

// IMPLEMENTATION!!!
teco_palette teco_palette_import(char* path) {
    teco_palette palette = teco_palette_create();

    FILE* file_stream = fopen(path, "r");

    if (!file_stream) return palette;

    char buffer;
    unsigned short red = 0, green = 0, blue = 0;

    while ((buffer = fgetc(file_stream)) != EOF) {
        fseek(file_stream, 1, SEEK_CUR);

        if (fgetc(file_stream) == '#') { // Maybe this option is not needed
            fscanf(file_stream, "%2hx%2hx%2hx\n", &red, &green, &blue);
        } else {
            fseek(file_stream, -1, SEEK_CUR);
            fscanf(file_stream, "%hu %hu %hu\n", &red, &green, &blue);
        }
        
        teco_palette_set_rgb(&palette, buffer, red, green, blue);
    }
    
    fclose(file_stream);

    return palette;
};

teco_palette teco_palette_create() {
    teco_palette palette = {.size=0, .characters=NULL, .colors=NULL};
    return palette;
};

void teco_palette_delete(teco_palette* palette) {
    free(palette->characters);
    free(palette->colors);

    palette->size = 0;
    palette->characters = NULL;
    palette->colors = NULL;
};

void teco_palette_set_rgb(teco_palette* palette, char character, unsigned char red, unsigned char green, unsigned char blue) {
    unsigned short color_index;

    unsigned short index = teco_palette_get_index(palette, character);

    if (index != palette->size) {
        color_index = index * 3;
    } else {
        ++(palette->size);

        palette->characters = realloc(palette->characters, palette->size);
        palette->colors = realloc(palette->colors, palette->size * 3);

        palette->characters[palette->size - 1] = character;

        color_index = (palette->size - 1) * 3;
    }

    palette->colors[color_index] = red;
    palette->colors[color_index + 1] = green;
    palette->colors[color_index + 2] = blue;
};

void teco_palette_set_hex(teco_palette* palette, char character, char* hex) {
    unsigned short red, green, blue;

    sscanf(hex, "#%2hx%2hx%2hx", &red, &green, &blue);

    teco_palette_set_rgb(palette, character, red, green, blue);
};

unsigned short teco_palette_get_index(teco_palette* palette, char character) {
    unsigned short index = 0;

    while (index < palette->size && palette->characters[index] != character)
        ++index;

    return index;
};

char teco_palette_get_character(teco_palette* palette, unsigned short index) {
    return palette->characters[index];
}

void teco_palette_get_color(teco_palette* palette, unsigned short index, unsigned char* red, unsigned char* green, unsigned char* blue) {
    unsigned short color_index = index * 3;

    *red = palette->colors[color_index];
    *green = palette->colors[color_index + 1];
    *blue = palette->colors[color_index + 2];
};

teco_cache teco_cache_create() {
    teco_cache cache = {.size=0, .symbols=NULL, .textures=NULL};

    return cache;
};

void teco_cache_delete(teco_cache* cache) {
    for (unsigned short index = 0; index < cache->size; index++) {
        SDL_DestroyTexture(cache->textures[index]);
    }

    free(cache->symbols);
    free(cache->textures);

    cache->size = 0;
    cache->symbols = NULL;
    cache->textures = NULL;
};

void teco_cache_add(teco_cache* cache, char symbol, SDL_Texture* texture) {
    ++(cache->size);

    cache->symbols = realloc(cache->symbols, cache->size);
    cache->textures = realloc(cache->textures, cache->size * sizeof(SDL_Texture*));

    cache->textures[cache->size - 1] = texture;
    cache->symbols[cache->size - 1] = symbol;
};

unsigned short teco_cache_get_index(teco_cache* cache, char symbol) {
    unsigned short index = 0;

    while (index < cache->size && cache->symbols[index] != symbol)
        ++index;

    return index;
};

char teco_cache_get_symbol(teco_cache* cache, unsigned short index) {
    return cache->symbols[index];
};

SDL_Texture* teco_cache_get_texture(teco_cache* cache, unsigned short index) {
    return cache->textures[index];
};

teco_bulb teco_bulb_create() {
    teco_bulb bulb = {.size=0, .characters=NULL, .effects=NULL};

    return bulb;
};

void teco_bulb_delete(teco_bulb* bulb) {
    free(bulb->characters);
    free(bulb->effects);

    bulb->size = 0;
    bulb->characters = NULL;
    bulb->effects = NULL;
};

void teco_bulb_set(teco_bulb* bulb, char character, teco_effect effect) {
    unsigned short index = teco_bulb_get_index(bulb, character);

    if (index == bulb->size) {
        ++(bulb->size);

        bulb->characters = realloc(bulb->characters, bulb->size);
        bulb->effects = realloc(bulb->effects, bulb->size * sizeof(teco_effect));

        bulb->characters[bulb->size - 1] = character;
    }

    bulb->effects[index] = effect;
};

unsigned short teco_bulb_get_index(teco_bulb* bulb, char character) {
    unsigned short index = 0;

    while (index < bulb->size && bulb->characters[index] != character)
        ++index;

    return index;
};

char teco_bulb_get_character(teco_bulb* bulb, unsigned short index) {
    return bulb->characters[index];
};

teco_effect teco_bulb_get_effect(teco_bulb* bulb, unsigned short index) {
    return bulb->effects[index];
};

void teco_image_draw(teco_image* image, teco_palette* palette, teco_bulb* bulb, SDL_Renderer* renderer, teco_cache* cache, TTF_Font* font, ...) {
    unsigned render_width, render_height;
    SDL_GetRendererOutputSize(renderer, &render_width, &render_height);

    unsigned symbol_width = render_width / image->width;
    unsigned symbol_height = render_height / image->height;

    char color_char, symbol, effect_char;
    unsigned short color_index, texture_index, effect_index;

    unsigned char red = 255, green = 255, blue = 255;

    teco_effect effect;
    float dx = 0, dy = 0, scale_x = 1, scale_y = 1;

    va_list effect_args, effect_args_temp;
    va_start(effect_args, font);

    SDL_Texture* texture;
    SDL_Surface* surface;
    SDL_Rect rectangle = {0, 0, symbol_width, symbol_height};
    SDL_Color foreground = {255, 255, 255};

    char render_text[2] = " ";

    for (unsigned y = 0; y < image->height; y++) {
        for (unsigned x = 0; x < image->width; x++) {
            symbol = teco_image_get(image, TECO_S, x, y);
            if (symbol == '\0' || symbol == ' ') continue;

            color_char = teco_image_get(image, TECO_C, x, y);
            if (color_char != '\0') {
                color_index = teco_palette_get_index(palette, color_char);
                if (color_index != palette->size)
                    teco_palette_get_color(
                        palette, color_index,
                        &red, &green, &blue
                    );
            }

            texture_index = teco_cache_get_index(cache, symbol);
            if (texture_index == cache->size) {
                render_text[0] = symbol;
                
                surface = TTF_RenderText_Solid(font, render_text, foreground);
                // Or TTF_RenderText_Blended
                texture = SDL_CreateTextureFromSurface(renderer, surface);

                teco_cache_add(cache, symbol, texture);

                SDL_FreeSurface(surface);
            } else {
                texture = teco_cache_get_texture(cache, texture_index);
            }

            rectangle.x = symbol_width * x;
            rectangle.y = symbol_height * y;
            rectangle.w = symbol_width;
            rectangle.h = symbol_height;

            effect_char = teco_image_get(image, TECO_E, x, y);

            if (effect_char != '\0') {
                effect_index = teco_bulb_get_index(bulb, effect_char);

                if (effect_index != bulb->size) {
                    effect = teco_bulb_get_effect(bulb, effect_index);

                    dx = 0; dy = 0; scale_x = 1; scale_y = 1;
                    va_copy(effect_args_temp, effect_args);
                    effect(&dx, &dy, &scale_x, &scale_y, x, y, effect_args_temp);

                    rectangle.x += symbol_width * (dx + (1 - scale_x) * .5);
                    rectangle.y += symbol_height * (dy + (1 - scale_y) * .5);
                    rectangle.w *= scale_x;
                    rectangle.h *= scale_y;
                }
            }

            SDL_SetTextureColorMod(texture, red, green, blue);

            SDL_RenderCopy(renderer, texture, NULL, &rectangle);
        }
    }

    va_end(effect_args);
};

#endif

#endif