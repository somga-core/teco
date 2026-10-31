#ifndef TECO
#define TECO

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

// Maybe add background layer

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
    unsigned char layer_count;
    char* data;
} teco_image;

teco_image teco_image_create(unsigned char layer_count, unsigned width, unsigned height);
teco_image teco_image_copy(teco_image* image, unsigned char layers);
teco_image teco_image_import(char* path, unsigned char layer_count);

void teco_image_delete(teco_image* image);
void teco_image_resize(teco_image* image, unsigned char new_layer_count, unsigned new_width, unsigned new_height);

void teco_image_export(teco_image* image, char* path, char space_symbol);

void teco_image_apply(teco_image* image_to_apply_on, teco_image* image_to_apply, unsigned char layers, long x, long y);

char teco_image_get(teco_image* image, unsigned char layer, unsigned x, unsigned y);
void teco_image_set(teco_image* image, unsigned char layer, char character, unsigned x, unsigned y);

// Maybe specify in rect function two parameters: fill char and border char
void teco_image_rect(teco_image* image, unsigned char layers, char* characters, long x1, long y1, long x2, long y2);
void teco_image_line(teco_image* image, unsigned char layers, char* characters, long x1, long y1, long x2, long y2);
void teco_image_fill(teco_image* image, unsigned char layers, char* characters);

void teco_image_print(teco_image* image, unsigned char layers);

// IMPLEMENTATION!!!!!
teco_image teco_image_create(unsigned char layer_count, unsigned width, unsigned height) {
    teco_image image = {.layer_count=layer_count, .width=width, .height=height};

    image.data = malloc(width * height * layer_count);

    teco_image_fill(&image, TECO_SCE & (unsigned) (pow(TECO_LAYER_MASK + 1, layer_count) - 1), "\0\0\0");

    return image;
};

teco_image teco_image_copy(teco_image* image, unsigned char layers) {
    teco_image new_image = teco_image_create(image->layer_count, image->width, image->height);

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

teco_image teco_image_import(char* path, unsigned char layer_count) {
    unsigned width = 0, height = 0;
    char buffer;
    // All "space" synbols in file will become spaces in image,
    // all spaces in file will become '\0' in image
    char space_symbol;
    FILE* file_stream = fopen(path, "r");

    if (!file_stream) return teco_image_create(1, 0, 0);

    fscanf(file_stream, "%u %u %c", &width, &height, &space_symbol);

    while (getc(file_stream) != '\n') {};

    teco_image image = teco_image_create(layer_count, width, height);

    for (unsigned char layer = 1; layer <= layer_count; layer++) {
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
    free(image->data);

    image->width = 0;
    image->height = 0;
};

void teco_image_resize(teco_image* image, unsigned char new_layer_count, unsigned new_width, unsigned new_height) {
    teco_image new_image = teco_image_create(new_layer_count, new_width, new_height);

    unsigned char copy_layer_count = new_layer_count;
    if (copy_layer_count > image->layer_count) copy_layer_count = image->layer_count;

    teco_image_apply(&new_image, image, TECO_SCE & (unsigned) (pow(TECO_LAYER_MASK + 1, copy_layer_count) - 1), 0, 0);

    teco_image_delete(image);

    *image = new_image;
};

void teco_image_export(teco_image* image, char* path, char space_symbol) {
    FILE* file_stream = fopen(path, "w");
    
    if (!file_stream) return;

    fprintf(file_stream, "%u %u %c\n", image->width, image->height, space_symbol);

    for (unsigned char layer = 1; layer <= image->layer_count; layer++) {
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
    return image->data[image->width * image->height * (layer - 1) + image->width * y + x];
};

void teco_image_set(teco_image* image, unsigned char layer, char character, unsigned x, unsigned y) {
    image->data[image->width * image->height * (layer - 1) + image->width * y + x] = character;
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

#ifdef TECO_RENDER_TEXT_SOLID
#define TECO_RENDER_TEXT TTF_RenderText_Solid
#else
#define TECO_RENDER_TEXT TTF_RenderText_Blended
#endif

// DECLARATION!!!
typedef struct teco_palette {
    unsigned char indices[256];
    unsigned char* colors;
    unsigned short size;
} teco_palette;

teco_palette teco_palette_import(char* path);
teco_palette teco_palette_create();
void teco_palette_delete(teco_palette* palette);

void teco_palette_set_rgb(teco_palette* palette, char character, unsigned char red, unsigned char green, unsigned char blue);
void teco_palette_set_hex(teco_palette* palette, char character, char* hex); // Maybe this function is not needed

unsigned short teco_palette_get_index(teco_palette* palette, char character);
void teco_palette_get_color(teco_palette* palette, unsigned short index, unsigned char* red, unsigned char* green, unsigned char* blue);

typedef struct teco_cache {
    unsigned char indices[256];
    SDL_Texture** textures;
    unsigned short size;
} teco_cache;

teco_cache teco_cache_create();
void teco_cache_delete(teco_cache* cache);

void teco_cache_add(teco_cache* cache, char symbol, SDL_Texture* texture);

unsigned short teco_cache_get_index(teco_cache* cache, char symbol);
SDL_Texture* teco_cache_get_texture(teco_cache* cache, unsigned short index);

typedef void (*teco_effect) (float*, float*, float*, float*, unsigned, unsigned, va_list args);

typedef struct teco_bulb {
    unsigned char indices[256];
    teco_effect* effects;
    unsigned short size;
} teco_bulb; // Maybe call this shaderpack. Sane name is needed

teco_bulb teco_bulb_create();
void teco_bulb_delete(teco_bulb* bulb);

void teco_bulb_set(teco_bulb* bulb, char character, teco_effect effect);

unsigned short teco_bulb_get_index(teco_bulb* bulb, char character);
teco_effect teco_bulb_get_effect(teco_bulb* bulb, unsigned short index);
// Maybe condence theese similar functions somehow
// Maybe add function to delete only one element from palette or bulb

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
    teco_palette palette = {.size=0, .colors=NULL};

    for (unsigned short i = 0; i < 256; i++) {
        palette.indices[i] = 255;
    }

    return palette;
};

void teco_palette_delete(teco_palette* palette) {
    free(palette->colors);

    palette->size = 0;
    palette->colors = NULL;
};

void teco_palette_set_rgb(teco_palette* palette, char character, unsigned char red, unsigned char green, unsigned char blue) {
    unsigned short color_index;

    unsigned short index = teco_palette_get_index(palette, character);

    if (index != palette->size) {
        color_index = index * 3;
    } else {
        palette->indices[(unsigned char) character] = palette->size;
        color_index = (palette->size) * 3;

        ++(palette->size);

        palette->colors = realloc(palette->colors, palette->size * 3);
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
    unsigned short index = palette->indices[(unsigned char) character];

    if (index == 255) return palette->size;
    return index;
};

void teco_palette_get_color(teco_palette* palette, unsigned short index, unsigned char* red, unsigned char* green, unsigned char* blue) {
    unsigned short color_index = index * 3;

    *red = palette->colors[color_index];
    *green = palette->colors[color_index + 1];
    *blue = palette->colors[color_index + 2];
};

teco_cache teco_cache_create() {
    teco_cache cache = {.size=0, .textures=NULL};

    for (unsigned short i = 0; i < 256; i++) {
        cache.indices[i] = 255;
    }

    return cache;
};

void teco_cache_delete(teco_cache* cache) {
    for (unsigned short index = 0; index < cache->size; index++) {
        SDL_DestroyTexture(cache->textures[index]);
    }

    free(cache->textures);

    cache->size = 0;
    cache->textures = NULL;
};

void teco_cache_add(teco_cache* cache, char symbol, SDL_Texture* texture) {
    cache->indices[(unsigned char) symbol] = cache->size;

    ++(cache->size);

    cache->textures = realloc(cache->textures, cache->size * sizeof(SDL_Texture*));

    cache->textures[cache->size - 1] = texture;
};

unsigned short teco_cache_get_index(teco_cache* cache, char symbol) {
    unsigned short index = cache->indices[(unsigned char) symbol];

    if (index == 255) return cache->size;
    return index;
};

SDL_Texture* teco_cache_get_texture(teco_cache* cache, unsigned short index) {
    return cache->textures[index];
};

teco_bulb teco_bulb_create() {
    teco_bulb bulb = {.size=0, .effects=NULL};

    for (unsigned short i = 0; i < 256; i++) {
        bulb.indices[i] = 255;
    }

    return bulb;
};

void teco_bulb_delete(teco_bulb* bulb) {
    free(bulb->effects);

    bulb->size = 0;
    bulb->effects = NULL;
};

void teco_bulb_set(teco_bulb* bulb, char character, teco_effect effect) {
    unsigned short index = teco_bulb_get_index(bulb, character);

    if (index == bulb->size) {
        bulb->indices[(unsigned char) character] = bulb->size;

        ++(bulb->size);

        bulb->effects = realloc(bulb->effects, bulb->size * sizeof(teco_effect));
    }

    bulb->effects[index] = effect;
};

unsigned short teco_bulb_get_index(teco_bulb* bulb, char character) {
    unsigned short index = bulb->indices[(unsigned char) character];

    if (index == 255) return bulb->size;
    return index;
};

teco_effect teco_bulb_get_effect(teco_bulb* bulb, unsigned short index) {
    return bulb->effects[index];
};

void teco_image_draw(teco_image* image, teco_palette* palette, teco_bulb* bulb, SDL_Renderer* renderer, teco_cache* cache, TTF_Font* font, ...) {
    unsigned render_width, render_height;
    SDL_GetRendererOutputSize(renderer, &render_width, &render_height);

    unsigned symbol_width = render_width / image->width;
    unsigned symbol_height = render_height / image->height;
    
    va_list effect_args;

    if (image->layer_count >= 3) {
        va_start(effect_args, font);
    }
    
    unsigned char red = 255, green = 255, blue = 255;
    SDL_Color render_color = {255, 255, 255};

    for (unsigned y = 0; y < image->height; y++) {
        for (unsigned x = 0; x < image->width; x++) {
            char render_text[2] = " ";

            char symbol = teco_image_get(image, TECO_S, x, y);
            if (symbol == '\0' || symbol == ' ') continue;

            SDL_Rect rectangle = {
                symbol_width * x, symbol_height * y,
                symbol_width, symbol_height
            };

            if (image->layer_count >= 2) {
                char color_char = teco_image_get(image, TECO_C, x, y);
                if (color_char != '\0') {
                    unsigned short color_index = teco_palette_get_index(palette, color_char);
                    if (color_index != palette->size)
                        teco_palette_get_color(
                            palette, color_index,
                            &red, &green, &blue
                        );
                }

                if (image->layer_count >= 3) {
                    char effect_char = teco_image_get(image, TECO_E, x, y);
        
                    if (effect_char != '\0') {
                        unsigned short effect_index = teco_bulb_get_index(bulb, effect_char);
        
                        if (effect_index != bulb->size) {
                            teco_effect effect = teco_bulb_get_effect(bulb, effect_index);
        
                            float dx = 0, dy = 0, scale_x = 1, scale_y = 1;
                            
                            va_list effect_args_temp;
                            va_copy(effect_args_temp, effect_args);
                            effect(&dx, &dy, &scale_x, &scale_y, x, y, effect_args_temp);
        
                            rectangle.x += symbol_width * (dx + (1 - scale_x) * .5);
                            rectangle.y += symbol_height * (dy + (1 - scale_y) * .5);
                            rectangle.w *= scale_x;
                            rectangle.h *= scale_y;
                        }
                    }
                }
            }

            unsigned short texture_index = teco_cache_get_index(cache, symbol);
            SDL_Texture* texture;
            if (texture_index == cache->size) {
                render_text[0] = symbol;
                
                SDL_Surface* surface = TECO_RENDER_TEXT(font, render_text, render_color);
                texture = SDL_CreateTextureFromSurface(renderer, surface);

                teco_cache_add(cache, symbol, texture);

                SDL_FreeSurface(surface);
            } else {
                texture = teco_cache_get_texture(cache, texture_index);
            }

            SDL_SetTextureColorMod(texture, red, green, blue);

            SDL_RenderCopy(renderer, texture, NULL, &rectangle);
        }
    }

    va_end(effect_args);
};

#elif defined(TECO_NCURSES) || defined(TECO_NCURSESW)

#ifdef TECO_NCURSES
#include <ncurses.h>
#else
#include <ncursesw/ncurses.h>
#endif

void teco_image_draw(teco_image* image, unsigned dx, unsigned dy);
void teco_image_draw_on_window(teco_image* image, WINDOW* window, unsigned dx, unsigned dy);

void teco_palette_import(char* path, short start);

void teco_image_draw(teco_image* image, unsigned dx, unsigned dy) {
    for (unsigned y = 0; y < image->height; y++) {
        for (unsigned x = 0; x < image->width; x++) {
            char symbol = teco_image_get(image, TECO_S, x, y);

            if (symbol == ' ' || symbol == '\0') continue;

            if (image->layer_count >= 2) {
                int color_pair = COLOR_PAIR(teco_image_get(image, TECO_C, x, y));
                attron(color_pair);
            
                mvaddch(dy + y, dx + x, symbol);

                attroff(color_pair);
            } else {
                mvaddch(dy + y, dx + x, symbol);
            }
        }
    }
};

void teco_image_draw_on_window(teco_image* image, WINDOW* window, unsigned dx, unsigned dy) {
    for (unsigned y = 0; y < image->height; y++) {
        for (unsigned x = 0; x < image->width; x++) {
            char symbol = teco_image_get(image, TECO_S, x, y);

            if (symbol == ' ' || symbol == '\0') continue;

            if (image->layer_count >= 2) {
                int color_pair = COLOR_PAIR(teco_image_get(image, TECO_C, x, y));
                wattron(window, color_pair);
            
                mvwaddch(window, dy + y, dx + x, symbol);

                wattroff(window, color_pair);
            } else {
                mvwaddch(window, dy + y, dx + x, symbol);
            }
        }
    }
};

void teco_palette_import(char* path, short start) {
    FILE* file_stream = fopen(path, "r");

    if (!file_stream) return;

    char buffer;
    unsigned i = 0;
    unsigned short red = 0, green = 0, blue = 0;

    while ((buffer = fgetc(file_stream)) != EOF) {
        fseek(file_stream, 1, SEEK_CUR);

        if (fgetc(file_stream) == '#') { // Maybe this option is not needed
            fscanf(file_stream, "%2hx%2hx%2hx\n", &red, &green, &blue);
        } else {
            fseek(file_stream, -1, SEEK_CUR);
            fscanf(file_stream, "%hu %hu %hu\n", &red, &green, &blue);
        }
        
        init_color(start + i, red * 1000 / 255, green * 1000 / 255, blue * 1000 / 255);
        init_pair(buffer, start + i, -1);

        ++i;
    }
    
    fclose(file_stream);
};

#endif

#endif