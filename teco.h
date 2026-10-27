#include <stdio.h>
#include <stdlib.h>

#ifdef TECO_GUI

// DECLARATION!!!!!
typedef struct teco_image {
    unsigned width; unsigned height;
    char* symbols; char* colors; char* effects;
} teco_image;

teco_image teco_image_create(unsigned _width, unsigned _height);
teco_image teco_image_copy(teco_image* image);
void teco_image_delete(teco_image* image);

void teco_image_resize(teco_image* image, unsigned new_width, unsigned new_height);

teco_image teco_image_import(char* path); // Maybe call this "load" (For no reason)
void teco_image_export(teco_image* image, char* path);
void teco_image_apply(teco_image* image_to_apply_on, teco_image* image_to_apply, long x, long y);

void teco_image_apply_symbols(teco_image* image_to_apply_on, teco_image* image_to_apply, long x, long y);

char teco_image_get_symbol(teco_image* image, unsigned x, unsigned y);
void teco_image_set_symbol(teco_image* image, char symbol, unsigned x, unsigned y);

void teco_image_rect_symbol(teco_image* image, char symbol, long x1, long y1, long x2, long y2);
void teco_image_fill_symbol(teco_image* image, char symbol);

void teco_image_draw(teco_image* image);
void teco_image_print(teco_image* image);

// IMPLEMENTATION!!!!!
teco_image teco_image_create(unsigned _width, unsigned _height) {
    teco_image image = {.width=_width, .height=_height};

    unsigned long long size = _width * _height;

    image.symbols = malloc(size);
    image.colors = malloc(size);
    image.effects = malloc(size);

    teco_image_fill_symbol(&image, '\0');

    return image;
};

teco_image teco_image_copy(teco_image* image) {
    teco_image new_image = {.width=image->width, .height=image->height};

    for (unsigned y = 0; y < image->height; y++) {
        for (unsigned x = 0; x < image->width; x++) {
            teco_image_set_symbol(
                &new_image,
                teco_image_get_symbol(image, x, y),
                x, y
            );
        }
    }

    return new_image;
};

void teco_image_delete(teco_image* image) {
    free(image->symbols);
    free(image->colors);
    free(image->effects);
};

void teco_image_resize(teco_image* image, unsigned new_width, unsigned new_height) {
    teco_image new_image = teco_image_create(new_width, new_height);

    teco_image_apply(&new_image, image, 0, 0);

    teco_image_delete(image);

    *image = new_image;
};

teco_image teco_image_import(char* path) {
	teco_image image;
	char buffer = 0;
	FILE* file_stream = fopen(path, "r");

	if (file_stream) {
        // This approach feels a bit dirty, but i don't know how to make it better
		unsigned width_length = 0;
		unsigned height_length = 0;

		while ((buffer = getc(file_stream)) != '\n');
		width_length = ftell(file_stream) - 1;
		while ((buffer = getc(file_stream)) != '\n');
		height_length = ftell(file_stream) - width_length - 2;

		char width_string[width_length+1];
		char height_string[height_length+1];

		fseek(file_stream, 0, SEEK_SET);
		while ((buffer = getc(file_stream)) != '\n')
			width_string[ftell(file_stream)-1] = buffer;
		while ((buffer = getc(file_stream)) != '\n')
			height_string[ftell(file_stream)-width_length-2] = buffer;

		width_string[width_length] = '\0';
		height_string[height_length] = '\0';

		image = teco_image_create(strtoull(width_string, NULL, 10), strtoull(height_string, NULL, 10));

        // If there is not enough symbols in a row, all of the data would shift
        // Can also save two processor ticks by pre-computing image.width*image.height in a separate variable
		for (unsigned long long position = 0; position < image.width*image.height; position++) {
			buffer = getc(file_stream);
			if (buffer == '\n')
				position--;
			else
				image.symbols[position] = buffer;
		}

		for (unsigned long long position = 0; position < image.width*image.height; position++) {
			buffer = getc(file_stream);
			if (buffer == '\n')
				position--;
			else
				image.colors[position] = buffer;
		}

		for (unsigned long long position = 0; position < image.width*image.height; position++) {
			buffer = getc(file_stream);
			if (buffer == '\n')
				position--;
			else
				image.effects[position] = buffer;
		}

		fclose(file_stream);

		return image;
	}

	else { // Unnecessary else statement
		image = teco_image_create(0, 0);
		return image;
	}
};

char teco_image_get_symbol(teco_image* image, unsigned x, unsigned y) {
    return (image->symbols)[image->width * y + x];
};

void teco_image_set_symbol(teco_image* image, char symbol, unsigned x, unsigned y) {
    (image->symbols)[image->width * y + x] = symbol;
};

void teco_image_apply(teco_image* canvas, teco_image* image, long x, long y) {
    if (
        x >= (long) canvas->width ||
        y >= (long) canvas->height ||
        x + ((long) image->width) <= 0 ||
        y + ((long) image->height) <= 0
    ) {
        return;
    };

    unsigned image_xl = 0, image_xr = 0, canvas_x = 0;
    unsigned image_yu = 0, image_yd = 0, canvas_y = 0;

    // Crop x
    if (x < 0) {
        image_xl = -x;
    } else {
        canvas_x = x;
    }

    if (x + image->width > canvas->width) {
        image_xr = canvas->width - x;
    } else {
        image_xr = image->width;
    }

    // Crop y
    if (y < 0) {
        image_yu = -y;
    } else {
        canvas_y = y;
    }

    if (y + image->height > canvas->height) {
        image_yd = canvas->height - y;
    } else {
        image_yd = image->height;
    }

    // Draw
    for (unsigned dy = image_yu; dy < image_yd; dy++) {
        for (unsigned dx = image_xl; dx < image_xr; dx++) {
            char symbol = teco_image_get_symbol(image, dx, dy);

            if (symbol != '\0') {
                teco_image_set_symbol(
                    canvas,
                    symbol,
                    dx + canvas_x - image_xl,
                    dy + canvas_y - image_yu
                );
            }
        }
    }
};

void teco_image_rect_symbol(teco_image* image, char symbol, long xl, long yu, long xr, long yd) {
    if (xl < 0) xl = 0;
    if (xr >= image->width) xr = image->width - 1;
    if (yu < 0) yu = 0;
    if (yd >= image->height) yd = image->height - 1;

    for (unsigned y = yu; y <= yd; y++) {
        for (unsigned x = xl; x <= xr; x++) {
            teco_image_set_symbol(image, symbol, x, y);
        }
    }
};

void teco_image_fill_symbol(teco_image* image, char symbol) {
    for (unsigned y = 0; y < image->height; y++) {
        for (unsigned x = 0; x < image->width; x++) {
            teco_image_set_symbol(image, symbol, x, y);
        }
    }
};

void teco_image_print(teco_image* image) {
    for (unsigned y = 0; y < image->height; y++) {
        for (unsigned x = 0; x < image->width; x++) {
            printf("%c", teco_image_get_symbol(image, x, y));
        }
        printf("\n");
    }
};

#endif
