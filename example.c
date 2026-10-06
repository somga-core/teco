#define TECO_GUI
#include "teco.h"

int main() {
    teco_image image1 = teco_image_create(10, 10);
    teco_image image2 = teco_image_create(2, 2);

    teco_image_set_symbol(&image1, '@', 0, 0);
    teco_image_set_symbol(&image1, '@', 9, 9);

    teco_image_print(&image1);

    teco_image_set_symbol(&image2, '#', 0, 0);
    teco_image_set_symbol(&image2, '#', 0, 1);
    teco_image_set_symbol(&image2, '#', 1, 0);
    teco_image_set_symbol(&image2, '#', 1, 1);

    teco_image_apply(&image1, &image2, 10, 7);

    teco_image_print(&image1);

    return 0;
}