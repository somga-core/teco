#define TECO_GUI
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
    // teco_image_print(&image1, TECO_SC);

    teco_image_apply(&image1, &image2, TECO_SCE, -1, -1);
    teco_image_rect(&image1, TECO_C, "$", 1, 1, 8, 8);

    printf("image1 v2:\n");
    // teco_image_print(&image1, TECO_SC);

    teco_image background = teco_image_copy(&image1, TECO_SC);
    teco_image_fill(&background, TECO_E, "&");

    teco_image object = teco_image_import("example.tci", TECO_SCE);

    teco_image_apply(&background, &object, TECO_SCE, 1, 1);

    printf("background:\n");
    // teco_image_print(&background, TECO_SCE);

    teco_image_export(&background, "super_export.tci", ':');

    return 0;
}