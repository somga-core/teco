#define TECO_GUI
#include "teco.h"

int main() {
	/*
    teco_image image1 = teco_image_create(10, 10);
    teco_image_fill_symbol(&image1, '.');

    teco_image image2 = teco_image_create(11, 11);
    for (char i = 0; i < 11; i++) {
        teco_image_set_symbol(&image2, i + 97, i, i);
    }
    teco_image_resize(&image2, 10, 10);

    teco_image_print(&image1);

    printf("\n");

    teco_image_apply(&image1, &image2, -1, -1);

    teco_image_print(&image1);
	*/

	teco_image cartinca = teco_image_import("example.thebestfuckingformat");
	teco_image_print(&cartinca);

    return 0;
}
