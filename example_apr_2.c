#define TECO_GUI
#include "teco_apr_2.h"

int main() {
    teco_image image1 = teco_image_create(10, 10);
    teco_image_fill(&image1, TECO_S, ".");

    teco_image image2 = teco_image_create(11, 11);
    for (char i = 0; i < 11; i++) {
        teco_image_set(&image2, TECO_S, i + 97, i, i);
    }
    teco_image_resize(&image2, 10, 10);

    teco_image_print(&image1, TECO_S);

    teco_image_apply(&image1, &image2, TECO_SCE, -1, -1);

    teco_image_print(&image1, TECO_S);

    printf("DAMN!!! Pure gambling: s%u e%u c%u x%u\n", 's' % 3, 'e' % 3, 'c' % 3, 'x' % 3);

    return 0;
}