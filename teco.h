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
void teco_image_apply(teco_image* image_to_apply_on, teco_image* image_to_apply, long x, long y);

char teco_image_get_symbol(teco_image* image, unsigned x, unsigned y);

void teco_image_set_symbol(teco_image* image, char symbol, unsigned x, unsigned y);
void teco_image_rect_symbol(teco_image* image, char symbol, long x1, long y1, long x2, long y2);
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

char teco_image_get_symbol(teco_image* image, unsigned x, unsigned y) {
    return (image->symbols)[image->width * y + x];
};

void teco_image_set_symbol(teco_image* image, char symbol, unsigned x, unsigned y) {
    (image->symbols)[image->width * y + x] = symbol;
};

void teco_image_apply(teco_image* canvas, teco_image* image, long x, long y) {
    unsigned canvas_x, canvas_y = 0;
    unsigned image_xr, image_yu, image_xl, image_yd = 0;

    if (x < 0) {
        canvas_x = 0;
        image_xl = -x;
    } else {
        canvas_x = x;
        image_xl = 0;
    }
    
    if (y < 0) {
        canvas_y = 0;
        image_yu = -y;
    } else {
        canvas_y = y;
        image_yu = 0;
    }

    if (y + image->height < canvas->height) {
        image_yd = image->height;
    } else {
        image_yd = image->height - (y + image->height - canvas->height);
    }
    
    if (x + image->width < canvas->width) {
        image_xr = image->width;
    } else {
        image_xr = image->width - (x + image->width - canvas->width);
    }

    printf("c: %u %u iul: %u %u idr: %u %u\n", canvas_x, canvas_y, image_xl, image_yu, image_xr, image_yd);

    for (unsigned y = image_yu; y < image_yd; y++) {
        for (unsigned x = image_xl; x < image_xr; x++) {
            teco_image_set_symbol(
                canvas, teco_image_get_symbol(image, x, y),
                x + canvas_x - image_xl, y + canvas_y - image_yu
            );
        }
    }
};

void teco_image_rect_symbol(teco_image* image, char symbol, long xl, long yu, long xr, long yd) {
    if (xl < 0) xl = 0;
    if (xr >= image->width) xr = image->width - 1;
    if (yu < 0) yu = 0;
    if (yd >= image->height) yd = image->height - 1;

    for (unsigned y = yu; y <= yd; y++) {
        for (unsigned x = xl; x <= xr; x++) {
            teco_image_set_symbol(image, symbol, x, y);
        }
    }
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