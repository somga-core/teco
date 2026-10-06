#define TECO_GUI
#include "teco.h"

int main() {
    teco_image image = teco_image_create(10, 10);

    teco_image_set_symbol(&image, '@', 0, 0);

    teco_image_set_symbol(&image, '@', 9, 9);

    teco_image_print(&image);

    return 0;
}