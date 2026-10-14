#define TECO_GUI
#include "teco_different_approach.h"

int main() {
    teco_image image1 = teco_image_create(10, 10);
    teco_image_fill(&image1, "s", ".");

    teco_image image2 = teco_image_create(11, 11);
    for (char i = 0; i < 11; i++) {
        teco_image_set(&image2, 's', i + 97, i, i);
    }
    teco_image_resize(&image2, 10, 10);

    teco_image_print(&image1, "s");

    teco_image_apply(&image1, &image2, TECO_LAYERS_ALL, -1, -1);

    teco_image_print(&image1, "s");

    printf("DAMN!!! Pure gambling: s%u e%u c%u x%u\n", 's' % 3, 'e' % 3, 'c' % 3, 'x' % 3);

    return 0;
}