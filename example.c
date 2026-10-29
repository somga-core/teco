#define TECO_SDL2
#include "teco.h"

int main(int argc, char *argv[]) {
    teco_image image1 = teco_image_create(10, 10);
    teco_image_fill(&image1, TECO_SC, ".'");

    teco_image image2 = teco_image_create(11, 11);
    for (char i = 0; i < 11; i++) {
        teco_image_set(&image2, TECO_S, i + 97, i, i);
    }
    teco_image_resize(&image2, 10, 10);

    printf("image1 v1:\n");
    teco_image_print(&image1, TECO_SC);

    teco_image_apply(&image1, &image2, TECO_SCE, -1, -1);
    teco_image_rect(&image1, TECO_C, "$", 1, 1, 8, 8);

    printf("image1 v2:\n");
    teco_image_print(&image1, TECO_SC);

    teco_image background = teco_image_copy(&image1, TECO_SC);
    teco_image_fill(&background, TECO_E, "&");

    teco_image object = teco_image_import("example.tci", TECO_SCE);

    teco_image_apply(&background, &object, TECO_SCE, 1, 1);

    printf("background:\n");
    teco_image_print(&background, TECO_SCE);

    teco_image_export(&background, "super_export.tci", ':');

    teco_palette palette = teco_palette_import("example.tcp");
    teco_palette_add_hex(&palette, 'z', "#ffffff");
    teco_palette_add_rgb(&palette, 's', 10, 10, 10);
    teco_palette_add_rgb(&palette, 't', 13, 12, 11);

    for (unsigned i = 0; i < palette.size; i++)
        printf("%d %c %d-%d-%d\n", palette.size, palette.characters[i], palette.colors[i*3], palette.colors[i*3+1], palette.colors[i*3+2]);

    unsigned char color[3];
    unsigned index = teco_palette_get_index(&palette, 'z');

    teco_palette_get_color(&palette, index, color);

    printf("%d %d-%d-%d", index, color[0], color[1], color[2]);

    return 0;
}