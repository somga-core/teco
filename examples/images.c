#include "../teco.h"

int main(int argc, char *argv[]) {
    teco_image image1 = teco_image_create(3, 10, 10);
    teco_image_fill(&image1, TECO_SC, ".'");

    teco_image image2 = teco_image_create(3, 11, 11);
    for (char i = 0; i < 11; i++) {
        teco_image_set(&image2, TECO_S, i + 97, i, i);
    }
    teco_image_resize(&image2, 3, 10, 10);

    printf("image1 v1:\n");
    teco_image_print(&image1, TECO_SCE);

    teco_image_apply(&image1, &image2, TECO_SCE, -1, -1);
    teco_image_rect(&image1, TECO_C, "$", 1, 1, 8, 8);

    printf("image1 v2:\n");
    teco_image_print(&image1, TECO_SC);

    teco_image background = teco_image_copy(&image1, TECO_SC);
    teco_image_fill(&background, TECO_E, "&");

    teco_image object = teco_image_import("examples/assets/test.tci", 3);

    teco_image_apply(&background, &object, TECO_SCE, 1, 1);

    printf("background:\n");
    teco_image_print(&background, TECO_SCE);

    teco_image_export(&background, "examples/temp/export.tci", ':');

    teco_image primitives = teco_image_create(3, 40, 20);

    teco_image_fill(&primitives, TECO_SCE, "\"^'");
    teco_image_rect(&primitives, TECO_SCE, "@#$", 9, 5, -100, 100);
    teco_image_hcircle(&primitives, TECO_SCE, "*&!", 15, 4, 4);
    teco_image_circle(&primitives, TECO_SCE, "*&!", 39, 11, 6);
    
    teco_image_line(&primitives, TECO_C, "a", 0, 19, 39, 0);
    teco_image_line(&primitives, TECO_C, "b", 10, 19, 39, 0);
    teco_image_line(&primitives, TECO_C, "c", 20, 19, 39, 0);
    teco_image_line(&primitives, TECO_C, "d", 30, 19, 39, 0);
    teco_image_line(&primitives, TECO_C, "e", 39, 19, 39, 0);
    
    teco_image_line(&primitives, TECO_E, " ", 0, 19, 0, 0);
    teco_image_line(&primitives, TECO_E, " ", 10, 19, 0, 0);
    teco_image_line(&primitives, TECO_E, " ", 20, 19, 0, 0);
    teco_image_line(&primitives, TECO_E, " ", 30, 19, 0, 0);
    teco_image_line(&primitives, TECO_E, " ", 39, 19, 0, 0);
        
    printf("primitives:\n");
    teco_image_print(&primitives, TECO_SCE);

    teco_image_delete(&image1);
    teco_image_delete(&image2);
    teco_image_delete(&background);
    teco_image_delete(&object);
    teco_image_delete(&primitives);

    return 0;
}
