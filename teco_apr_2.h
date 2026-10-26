#include <stdio.h>
#include <stdlib.h>

#ifdef TECO_GUI

#define TECO_LAYERS 3
#define TECO_LAYER_BITS 2
#define TECO_LAYER_MASK 0b11

// Maybe use this...
#define TECO_S 0b01
#define TECO_C 0b10
#define TECO_E 0b11

#define TECO_SE 0b1101
#define TECO_ES 0b0111
#define TECO_SC 0b1001
#define TECO_CS 0b0110
#define TECO_EC 0b1011
#define TECO_CE 0b1110

#define TECO_SCE 0b111001
#define TECO_SEC 0b101101
#define TECO_CSE 0b110110
#define TECO_ESC 0b100111
#define TECO_CES 0b001110
#define TECO_ECS 0b001011

// ...or this. But readability with this way is worse
#define TECO_S 0b01
#define TECO_C 0b10
#define TECO_E 0b11

#define TECO_XS 0b0100
#define TECO_XC 0b1000
#define TECO_XE 0b1100

#define TECO_XXS 0b0100
#define TECO_XXC 0b1000
#define TECO_XXE 0b1100

// DECLARATION!!!!!
typedef struct teco_image {
    unsigned width; unsigned height;
    char* data[TECO_LAYERS];
} teco_image;

teco_image teco_image_create(unsigned _width, unsigned _height);
void teco_image_delete(teco_image* image);

void teco_image_resize(teco_image* image, unsigned new_width, unsigned new_height);

void teco_image_apply(teco_image* image_to_apply_on, teco_image* image_to_apply, unsigned char layers, long x, long y);

char teco_image_get(teco_image* image, unsigned char layer, unsigned x, unsigned y);
void teco_image_set(teco_image* image, unsigned char layer, char character, unsigned x, unsigned y);

void teco_image_rect(teco_image* image, unsigned char layers, char* characters, long x1, long y1, long x2, long y2);
void teco_image_fill(teco_image* image, unsigned char layers, char* characters);

void teco_image_print(teco_image* image, unsigned char layers);

// IMPLEMENTATION!!!!!
teco_image teco_image_create(unsigned _width, unsigned _height) {
    teco_image image = {.width=_width, .height=_height};

    unsigned long long size = _width * _height;

    for (char layer = 0; layer < TECO_LAYERS; layer++) {
        image.data[layer] = malloc(size);
    }
    
    teco_image_fill(&image, TECO_SCE, "\0\0\0");

    return image;
};

void teco_image_delete(teco_image* image) {
    for (char layer = 0; layer < TECO_LAYERS; layer++) {
        free(image->data[layer]);
    }
};

void teco_image_resize(teco_image* image, unsigned new_width, unsigned new_height) {
    teco_image new_image = teco_image_create(new_width, new_height);

    teco_image_apply(&new_image, image, TECO_SCE, 0, 0);

    teco_image_delete(image);

    *image = new_image;
};

char teco_image_get(teco_image* image, unsigned char layer, unsigned x, unsigned y) {
    return image->data[layer - 1][image->width * y + x];
};

void teco_image_set(teco_image* image, unsigned char layer, char character, unsigned x, unsigned y) {
    image->data[layer - 1][image->width * y + x] = character;
};

void teco_image_apply(teco_image* canvas, teco_image* image, unsigned char layers, long x, long y) {
    if (
        x >= (long) canvas->width ||
        y >= (long) canvas->height ||
        x + ((long) image->width) <= 0 ||
        y + ((long) image->height) <= 0
    ) {
        return;
    };

    unsigned image_xl = 0, image_xr = 0, canvas_x = 0;
    unsigned image_yu = 0, image_yd = 0, canvas_y = 0;

    // Crop x
    if (x < 0) {
        image_xl = -x;
    } else {
        canvas_x = x;
    }

    if (x + image->width > canvas->width) {
        image_xr = canvas->width - x;
    } else {
        image_xr = image->width;
    }

    // Crop y
    if (y < 0) {
        image_yu = -y;
    } else {
        canvas_y = y;
    }

    if (y + image->height > canvas->height) {
        image_yd = canvas->height - y;
    } else {
        image_yd = image->height;
    }

    // Draw
    for (
        unsigned char layer, shifted = layers;
        (layer = shifted & TECO_LAYER_MASK) != 0;
        shifted = shifted >> TECO_LAYER_BITS
    ) {
        for (unsigned dy = image_yu; dy < image_yd; dy++) {
            for (unsigned dx = image_xl; dx < image_xr; dx++) {
                char character = teco_image_get(image, layer, dx, dy);

                if (character != '\0') {
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
    }
};

void teco_image_rect(teco_image* image, unsigned char layers, char* characters, long xl, long yu, long xr, long yd) {
    if (xl < 0) xl = 0;
    if (xr >= image->width) xr = image->width - 1;
    if (yu < 0) yu = 0;
    if (yd >= image->height) yd = image->height - 1;
    
    for (
        unsigned char i = 0, layer, shifted = layers;
        (layer = shifted & TECO_LAYER_MASK) != 0;
        shifted = shifted >> TECO_LAYER_BITS
    ) {
        for (unsigned y = yu; y <= yd; y++) {
            for (unsigned x = xl; x <= xr; x++) {
                teco_image_set(image, layer, characters[i], x, y);
            }
        }
        
        i++;
    }
};

void teco_image_fill(teco_image* image, unsigned char layers, char* characters) {
    for (
        unsigned char i = 0, layer, shifted = layers;
        (layer = shifted & TECO_LAYER_MASK) != 0;
        shifted = shifted >> TECO_LAYER_BITS
    ) {
        for (unsigned y = 0; y < image->height; y++) {
            for (unsigned x = 0; x < image->width; x++) {
                teco_image_set(image, layer, characters[i], x, y);
            }
        }

        i++;
    }
};

void teco_image_print(teco_image* image, unsigned char layers) {

    for (
        unsigned char layer, shifted = layers;
        (layer = shifted & TECO_LAYER_MASK) != 0;
        shifted = shifted >> TECO_LAYER_BITS
    ) {
        for (unsigned y = 0; y < image->height; y++) {
            for (unsigned x = 0; x < image->width; x++) {
                char character = teco_image_get(image, layer, x, y);
                
                (character == '\0') ? printf(" ") : printf("%c", character);
            }
            printf("\n");
        }
        printf("\n");
    }
};

#endif