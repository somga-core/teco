#include <stdio.h>
#include <stdlib.h>

#ifdef TECO_GUI

// DECLARATION!!!!!
typedef struct teco_image {
    unsigned width; unsigned height;
    char* symbols; char* colors; char* effects;
} teco_image;

teco_image teco_image_create(unsigned _width, unsigned _height);
void teco_image_delete(teco_image* image);

void teco_image_resize(teco_image* image, unsigned new_height, unsigned new_width);

void teco_image_import(teco_image* image, char* path);
void teco_image_apply(teco_image* image_to_apply_on, teco_image* image_to_apply, unsigned x, unsigned y);

void teco_image_set_symbol(teco_image* image, char symbol, unsigned x, unsigned y);
void teco_image_rect_symbol(teco_image* image, char symbol, unsigned x1, unsigned y1, unsigned x2, unsigned y2);
void teco_image_fill_symbol(teco_image* image, char symbol);

void teco_image_draw(teco_image* image);
void teco_image_print(teco_image* image);

// IMPLEMENTATION!!!!!
teco_image teco_image_create(unsigned _width, unsigned _height) {
    teco_image image = {.width=_width, .height=_height};

    unsigned long long size = _width * _height;

    image.symbols = malloc(size);
    image.colors = malloc(size);
    image.effects = malloc(size);

    teco_image_fill_symbol(&image, ' ');

    return image;
};

void teco_image_set_symbol(teco_image* image, char symbol, unsigned x, unsigned y) {
    (image->symbols)[image->width * y + x] = symbol;
};

void teco_image_fill_symbol(teco_image* image, char symbol) {
    for (unsigned y = 0; y < image->height; y++) {
        for (unsigned x = 0; x < image->width; x++) {
            teco_image_set_symbol(image, symbol, x, y);
        }
    }
};

void teco_image_print(teco_image* image) {
    for (unsigned y = 0; y < image->height; y++) {
        for (unsigned x = 0; x < image->width; x++) {
            printf("%c", (image->symbols)[image->width * y + x]);
        }
        printf("\n");
    }
};

#endif