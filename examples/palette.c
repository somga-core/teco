#define TECO_SDL2
#include "../teco.h"

int main(int argc, char *argv[]) {
    teco_palette palette = teco_palette_import("examples/assets/test.tcp");
    teco_palette_set_hex(&palette, 'z', "#ffffff");
    teco_palette_set_rgb(&palette, 's', 10, 10, 10);
    teco_palette_set_rgb(&palette, 't', 13, 12, 11);
    teco_palette_set_hex(&palette, 'a', "#ff0000");

    for (unsigned i = 0; i < palette.size; i++)
        printf("%d %c %d-%d-%d\n", palette.size, palette.characters[i], palette.colors[i*3], palette.colors[i*3+1], palette.colors[i*3+2]);

    unsigned char red, green, blue;
    unsigned index = teco_palette_get_index(&palette, 'z');

    teco_palette_get_color(&palette, index, &red, &green, &blue);

    printf("%d %d-%d-%d", index, red, green, blue);
    
    teco_palette_delete(&palette);

    return 0;
}