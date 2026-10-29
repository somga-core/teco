#ifndef TECO
#define TECO

#include <stdio.h>
#include <stdlib.h>

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

    fscanf(file_stream, "%u %u %c\n", &width, &height, &space_symbol);
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

// DECLARATION!!!
typedef struct teco_palette {
    char* characters;
    unsigned char* colors;
    unsigned size;
} teco_palette;

typedef struct teco_sdl_buffer {
    char* characters;
    SDL_Texture* sdl_textures;
    unsigned size;
} teco_sdl_buffer;

teco_palette teco_palette_import(char* path);
teco_palette teco_palette_create();
void teco_palette_delete(teco_palette* palette);
void teco_palette_set_rgb(teco_palette* palette, char character, unsigned char red, unsigned char green, unsigned char blue);
void teco_palette_set_hex(teco_palette* palette, char character, char* hex);
unsigned teco_palette_get_index(teco_palette* palette, char character);
char teco_palette_get_character(teco_palette* palette, unsigned index);
void teco_palette_get_color(teco_palette* palette, unsigned index, unsigned char color[3]);

void teco_image_draw(teco_image* image, teco_palette palette, SDL_Surface* sdl_surface, teco_sdl_buffer* sdl_buffer);

// IMPLEMENTATION!!!
teco_palette teco_palette_import(char* path) {
    teco_palette palette = teco_palette_create();

    FILE* file_stream = fopen(path, "r");

    if (!file_stream) return palette;

    char buffer;
    unsigned red, green, blue;

    while ((buffer = fgetc(file_stream)) != EOF) {
        fseek(file_stream, 1, SEEK_CUR);

        if (fgetc(file_stream) == '#') {
            fscanf(file_stream, "%2x%2x%2x\n", &red, &green, &blue);
        } else {
            fseek(file_stream, -1, SEEK_CUR);
            fscanf(file_stream, "%u %u %u\n", &red, &green, &blue);
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
    unsigned long long colors_index;

    unsigned index = teco_palette_get_index(palette, character);

    if (index != palette->size) {
        colors_index = index * 3;
    } else {
        ++(palette->size);

        palette->characters = realloc(palette->characters, palette->size);
        palette->colors = realloc(palette->colors, palette->size * 3);

        palette->characters[palette->size - 1] = character;

        colors_index = (palette->size - 1) * 3;
    }

    palette->colors[colors_index] = red;
    palette->colors[colors_index + 1] = green;
    palette->colors[colors_index + 2] = blue;
};

void teco_palette_set_hex(teco_palette* palette, char character, char* hex) {
    unsigned red, green, blue;

    sscanf(hex, "#%2x%2x%2x", &red, &green, &blue);

    teco_palette_set_rgb(palette, character, red, green, blue);
};

unsigned teco_palette_get_index(teco_palette* palette, char character) {
    int index = 0;

    while (index < palette->size && palette->characters[index] != character)
        ++index;

    return index;
};

char teco_palette_get_character(teco_palette* palette, unsigned index) {
    return palette->characters[index];
}

void teco_palette_get_color(teco_palette* palette, unsigned index, unsigned char color[3]) {
    unsigned long long colors_index = index * 3;

    color[0] = palette->colors[colors_index];
    color[1] = palette->colors[colors_index + 1];
    color[2] = palette->colors[colors_index + 2];
};

#endif

#endif