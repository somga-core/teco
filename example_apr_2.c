#define TECO_GUI
#include "teco_apr_2.h"

int main() {
    teco_image image1 = teco_image_create(10, 10);
    teco_image_fill(&image1, TECO_SC, ".'"); // Demonstration of the first way to represent layers

    teco_image image2 = teco_image_create(11, 11);
    for (char i = 0; i < 11; i++) {
        teco_image_set(&image2, TECO_S, i + 97, i, i);
    }
    teco_image_resize(&image2, 10, 10);

    teco_image_print(&image1, TECO_S | TECO_XC); // Demonstration of the second way to represent layers

    teco_image_apply(&image1, &image2, TECO_SCE, -1, -1);
    teco_image_rect(&image1, TECO_C, "$", 1, 1, 8, 8);

    teco_image_print(&image1, TECO_S | TECO_XC);

    return 0;
}