#ifndef TECO
#define TECO

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// DECLARATION!!!
#define TECO_LAYER_BITS 2
#define TECO_LAYER_MASK 0b11
#define TECO_DEFAULT_FILL "\0\0\0"

#define TECO_S 0b01
#define TECO_C 0b10
#define TECO_E 0b11

#define TECO_SC 0b1001
#define TECO_SE 0b1101
#define TECO_CE 0b1110

#define TECO_SCE 0b111001

#define TECO_ALL 0b111001

#define TECO_SQRT_2 1.41421
#define TECO_SWAP(type, a, b) {type tmp = a; a = b; b = tmp;}

#define TECO_FOR_LAYERS(layers, layer) for ( \
	unsigned char layer, shifted = layers; \
	(layer = shifted & TECO_LAYER_MASK) != 0; \
	shifted >>= TECO_LAYER_BITS \
)

#define TECO_FOR_LAYERS_CHARS(layers, layer, char_index) for ( \
	unsigned char char_index = 0, layer, shifted = layers; \
	(layer = shifted & TECO_LAYER_MASK) != 0; \
	shifted >>= TECO_LAYER_BITS, ++char_index \
)

typedef struct teco_image {
	unsigned width; unsigned height;
	unsigned char layer_count;
	char* data;
} teco_image;

teco_image teco_image_create(unsigned char layer_count, unsigned width, unsigned height);
teco_image teco_image_copy(teco_image* image, unsigned char layers);
teco_image teco_image_import(char* path, unsigned char layer_count);

void teco_image_delete(teco_image* image);
void teco_image_resize(teco_image* image, unsigned char new_layer_count, unsigned new_width, unsigned new_height);

void teco_image_export(teco_image* image, char* path, char space_symbol);

void teco_image_apply(teco_image* canvas, teco_image* image, unsigned char layers, long x, long y);

char teco_image_get(teco_image* image, unsigned char layer, unsigned x, unsigned y);
void teco_image_set(teco_image* image, unsigned char layer, char character, unsigned x, unsigned y);
int teco_image_get_in_bounds(teco_image* image, long x, long y);

void teco_image_line(teco_image* image, unsigned char layers, char* characters, long x1, long y1, long x2, long y2);
void teco_image_rect(teco_image* image, unsigned char layers, char* characters, long x1, long y1, long x2, long y2);
void teco_image_hrect(teco_image* image, unsigned char layers, char* characters, long x1, long y1, long x2, long y2);
void teco_image_circle(teco_image* image, unsigned char layers, char* characters, long x, long y, unsigned radius);
void teco_image_hcircle(teco_image* image, unsigned char layers, char* characters, long x, long y, unsigned radius);
void teco_image_fill(teco_image* image, unsigned char layers, char* characters);

void teco_image_print(teco_image* image, unsigned char layers);

#if defined(TECO_SDL2) || defined(TECO_RAYLIB)

#include <stdarg.h>

#if defined(TECO_SDL2)

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define TECO_TEXTURE SDL_Texture*
#define TECO_RECT SDL_Rect
#define TECO_COLOR SDL_Color
#define TECO_TEXTURE_DELETE SDL_DestroyTexture

#ifdef TECO_RENDER_TEXT_SOLID
#define TECO_RENDER_TEXT TTF_RenderText_Solid
#else
#define TECO_RENDER_TEXT TTF_RenderText_Blended
#endif

#elif defined(TECO_RAYLIB)

#include <raylib.h>

#define TECO_TEXTURE RenderTexture2D
#define TECO_RECT Rectangle
#define TECO_COLOR Color
#define TECO_TEXTURE_DELETE UnloadRenderTexture

#endif

typedef struct teco_palette {
	unsigned char indices[256];
	unsigned char* colors;
	unsigned short size;
} teco_palette;

teco_palette teco_palette_import(char* path);
teco_palette teco_palette_create();
void teco_palette_delete(teco_palette* palette);

void teco_palette_set_rgb(teco_palette* palette, char character, unsigned char red, unsigned char green, unsigned char blue);
void teco_palette_set_hex(teco_palette* palette, char character, char* hex);

unsigned short teco_palette_get_index(teco_palette* palette, char character);
void teco_palette_get_color(teco_palette* palette, unsigned short index, unsigned char* red, unsigned char* green, unsigned char* blue);

typedef void (*teco_effect) (float*, float*, float*, float*, unsigned, unsigned, va_list args);
typedef struct teco_bulb {
	unsigned char indices[256];
	teco_effect* effects;
	unsigned short size;
} teco_bulb;

teco_bulb teco_bulb_create();
void teco_bulb_delete(teco_bulb* bulb);

void teco_bulb_set(teco_bulb* bulb, char character, teco_effect effect);

unsigned short teco_bulb_get_index(teco_bulb* bulb, char character);
teco_effect teco_bulb_get_effect(teco_bulb* bulb, unsigned short index);

typedef struct teco_cache {
	unsigned char indices[256];
	TECO_TEXTURE* textures;
	unsigned short size;
} teco_cache;

teco_cache teco_cache_create();
void teco_cache_delete(teco_cache* cache);

void teco_cache_add(teco_cache* cache, char symbol, TECO_TEXTURE texture);

unsigned short teco_cache_get_index(teco_cache* cache, char symbol);
TECO_TEXTURE teco_cache_get_texture(teco_cache* cache, unsigned short index);

#if defined(TECO_SDL2)

void teco_image_draw(teco_image* image, teco_palette* palette, teco_bulb* bulb, SDL_Renderer* renderer, teco_cache* cache, TECO_RECT draw_rect, TTF_Font* font, ...);

#elif defined(TECO_RAYLIB)

void teco_image_draw(teco_image* image, teco_palette* palette, teco_bulb* bulb, teco_cache* cache, TECO_RECT draw_rect, Font font, ...);

#endif

#elif defined(TECO_NCURSES) || defined(TECO_NCURSESW)

#if defined(TECO_NCURSES)
#include <ncurses.h>
#elif defined(TECO_NCURSESW)
#include <ncursesw/ncurses.h>
#endif

void teco_image_draw(teco_image* image, unsigned dx, unsigned dy);
void teco_image_draw_on_window(teco_image* image, WINDOW* window, unsigned dx, unsigned dy);

void teco_palette_import(char* path, short start);

#endif

// IMPLEMENTATION!!!
teco_image teco_image_create(unsigned char layer_count, unsigned width, unsigned height) {
	teco_image image = {.layer_count=layer_count, .width=width, .height=height};

	image.data = malloc(width * height * layer_count);

	teco_image_fill(&image, TECO_ALL & (unsigned) (pow(TECO_LAYER_MASK + 1, layer_count) - 1), TECO_DEFAULT_FILL);

	return image;
};

teco_image teco_image_copy(teco_image* image, unsigned char layers) {
	teco_image new_image = teco_image_create(image->layer_count, image->width, image->height);

	TECO_FOR_LAYERS(layers, layer) {
		for (unsigned y = 0; y < image->height; y++) {
			for (unsigned x = 0; x < image->width; x++) {
				teco_image_set(
					&new_image, layer,
					teco_image_get(image, layer, x, y),
					x, y
				);
			}
		}
	}

	return new_image;
};

teco_image teco_image_import(char* path, unsigned char layer_count) {
	FILE* file_stream = fopen(path, "r");
	if (!file_stream) return teco_image_create(1, 0, 0);
	
	unsigned width = 0, height = 0;
	char buffer;
	
	char space_symbol;
	// All "space symbols" in file will become spaces in image,
	// all spaces in file will become '\0' in image

	fscanf(file_stream, "%u %u %c", &width, &height, &space_symbol);

	while (getc(file_stream) != '\n') {};

	teco_image image = teco_image_create(layer_count, width, height);

	for (unsigned char layer = 1; layer <= layer_count; layer++) {
		for (unsigned y = 0; y < image.height; y++) {
			for (
				unsigned x = 0;
				(buffer = getc(file_stream)) != '\n' && x < image.width;
				x++
			) {
				if (buffer == EOF) goto teco_image_import_end;

				if (buffer == space_symbol)
					teco_image_set(&image, layer, ' ', x, y);
				else if (buffer != ' ')
					teco_image_set(&image, layer, buffer, x, y);
			}
		}
	}

teco_image_import_end:
	fclose(file_stream);
	return image;
};

void teco_image_delete(teco_image* image) {
	free(image->data);

	image->width = 0;
	image->height = 0;
};

void teco_image_resize(teco_image* image, unsigned char new_layer_count, unsigned new_width, unsigned new_height) {
	teco_image new_image = teco_image_create(new_layer_count, new_width, new_height);

	unsigned char copy_layer_count = new_layer_count;
	if (copy_layer_count > image->layer_count) copy_layer_count = image->layer_count;

	teco_image_apply(&new_image, image, TECO_ALL & (unsigned) (pow(TECO_LAYER_MASK + 1, copy_layer_count) - 1), 0, 0);

	teco_image_delete(image);

	*image = new_image;
};

void teco_image_export(teco_image* image, char* path, char space_symbol) {
	FILE* file_stream = fopen(path, "w");
	
	if (!file_stream) return;

	fprintf(file_stream, "%u %u %c\n", image->width, image->height, space_symbol);

	for (unsigned char layer = 1; layer <= image->layer_count; layer++) {
		for (unsigned y = 0; y < image->height; y++) {
			for (unsigned x = 0; x < image->width; x++) {
				char character = teco_image_get(image, layer, x, y);

				if (character == '\0')
					fputc(' ', file_stream);
				else if (character == ' ')
					fputc(space_symbol, file_stream);
				else
					fputc(character, file_stream);
			}
			fputc('\n', file_stream);
		}
	}
	
	fclose(file_stream);
};

void teco_image_apply(teco_image* canvas, teco_image* image, unsigned char layers, long x, long y) {
	if (
		x >= (long) canvas->width ||
		y >= (long) canvas->height ||
		x + ((long) image->width) <= 0 ||
		y + ((long) image->height) <= 0
	) return;

	unsigned image_xl = 0, image_xr = 0, canvas_x = 0;
	unsigned image_yu = 0, image_yd = 0, canvas_y = 0;

	// Crop x
	if (x < 0)
		image_xl = -x;
	else
		canvas_x = x;

	if (x + image->width > canvas->width)
		image_xr = canvas->width - x;
	else
		image_xr = image->width;

	// Crop y
	if (y < 0)
		image_yu = -y;
	else
		canvas_y = y;

	if (y + image->height > canvas->height)
		image_yd = canvas->height - y;
	else
		image_yd = image->height;

	// Plot
	TECO_FOR_LAYERS(layers, layer) {
		for (unsigned dy = image_yu; dy < image_yd; dy++) {
			for (unsigned dx = image_xl; dx < image_xr; dx++) {
				char character = teco_image_get(image, layer, dx, dy);

				if (character != '\0')
					teco_image_set(
						canvas,
						layer,
						character,
						dx + canvas_x - image_xl,
						dy + canvas_y - image_yu
					);
			}
		}
	}
};

char teco_image_get(teco_image* image, unsigned char layer, unsigned x, unsigned y) {
	return image->data[image->width * image->height * (layer - 1) + image->width * y + x];
};

void teco_image_set(teco_image* image, unsigned char layer, char character, unsigned x, unsigned y) {
	image->data[image->width * image->height * (layer - 1) + image->width * y + x] = character;
};

int teco_image_get_in_bounds(teco_image* image, long x, long y) {
	return x >= 0 && y >= 0 && x < image->width && y < image->height;
};

void teco_image_line(teco_image* image, unsigned char layers, char* characters, long x1, long y1, long x2, long y2) {
	long long_lenght = x2 - x1;
	long short_lenght = y2 - y1;

	char plot_along_x = 1;

	if (labs(long_lenght) < labs(short_lenght)) {
		TECO_SWAP(long, long_lenght, short_lenght)
		plot_along_x = 0;
	}

	float step_along_short = (float) short_lenght / (float) long_lenght;
	char step_along_long = 1;

	if (long_lenght < 0) {step_along_long = -1; step_along_short = -step_along_short;}

	long_lenght += step_along_long;

	if (plot_along_x) {
		float y = .5 * step_along_long;
		for (long x = 0; x != long_lenght; x += step_along_long) {
			if (teco_image_get_in_bounds(image, x1 + x, y1 + (long) y))
				TECO_FOR_LAYERS_CHARS(layers, layer, char_index)
					teco_image_set(image, layer, characters[char_index], x1 + x, y1 + (long) y);
			
			y += step_along_short;
		}
	} else {
		float x = .5 * step_along_long;
		for (long y = 0; y != long_lenght; y += step_along_long) {
			if (teco_image_get_in_bounds(image, x1 + (long) x, y1 + y))
				TECO_FOR_LAYERS_CHARS(layers, layer, char_index)
					teco_image_set(image, layer, characters[char_index], x1 + (long) x, y1 + y);

			x += step_along_short;
		}
	}
}

void teco_image_rect(teco_image* image, unsigned char layers, char* characters, long x1, long y1, long x2, long y2) {
	if (x1 > x2) TECO_SWAP(long, x1, x2)

	if (x1 >= image->width || x2 < 0) return;
	
	if (y1 > y2) TECO_SWAP(long, y1, y2)
	
	if (y1 >= image->height || y2 < 0) return;

	if (x1 < 0) x1 = 0;
	if (x2 >= image->width) x2 = image->width - 1;
	if (y1 < 0) y1 = 0;
	if (y2 >= image->height) y2 = image->height - 1;

	
	TECO_FOR_LAYERS_CHARS(layers, layer, char_index)
		for (unsigned y = y1; y <= y2; y++)
			for (unsigned x = x1; x <= x2; x++)
				teco_image_set(image, layer, characters[char_index], x, y);
};

void teco_image_hrect(teco_image* image, unsigned char layers, char* characters, long x1, long y1, long x2, long y2) {
	if (x1 > x2) TECO_SWAP(long, x1, x2)

	if (x1 >= image->width || x2 < 0) return;
	
	if (y1 > y2) TECO_SWAP(long, y1, y2)
	
	if (y1 >= image->height || y2 < 0) return;

	char draw_top = 1, draw_bottom = 1, draw_left = 1, draw_right = 1;

	if (x1 < 0) {draw_left = 0; x1 = 0;}
	if (x2 >= image->width) {draw_right = 0; x2 = image->width - 1;}
	if (y1 < 0) {draw_top = 0; y1 = 0;}
	if (y2 >= image->height) {draw_bottom = 0; y2 = image->height - 1;}

	if (draw_left) for (unsigned y = y1; y <= y2; ++y)
		TECO_FOR_LAYERS_CHARS(layers, layer, char_index)
			teco_image_set(image, layer, characters[char_index], x1, y);
			
	if (draw_right) for (unsigned y = y1; y <= y2; ++y)
		TECO_FOR_LAYERS_CHARS(layers, layer, char_index)
			teco_image_set(image, layer, characters[char_index], x2, y);
			
	if (draw_top) for (unsigned x = x1; x <= x2; ++x)
		TECO_FOR_LAYERS_CHARS(layers, layer, char_index)
			teco_image_set(image, layer, characters[char_index], x, y1);
			
	if (draw_bottom) for (unsigned x = x1; x <= x2; ++x)
		TECO_FOR_LAYERS_CHARS(layers, layer, char_index)
			teco_image_set(image, layer, characters[char_index], x, y2);
};

void teco_image_circle(teco_image* image, unsigned char layers, char* characters, long x, long y, unsigned radius) {
	for (long i = 0; i <= radius / TECO_SQRT_2 + 1; ++i) {
		long max_j = sqrt(radius * radius - i * i);
		
		for (long j = 0; j <= max_j; ++j)
			for (char im = -1; im < 3; im += 2) {
				for (char jm = -1; jm < 3; jm += 2) {
					// Grossy looking
					long dx = x + im * i;
					long dy = y + jm * j;
					if (teco_image_get_in_bounds(image, dx, dy))
						TECO_FOR_LAYERS_CHARS(layers, layer, char_index)
							teco_image_set(image, layer, characters[char_index], dx, dy);

					dx = x + jm * j;
					dy = y + im * i;
					if (teco_image_get_in_bounds(image, dx, dy))
						TECO_FOR_LAYERS_CHARS(layers, layer, char_index)
							teco_image_set(image, layer, characters[char_index], dx, dy);
				}
			}
	}
};

void teco_image_hcircle(teco_image* image, unsigned char layers, char* characters, long x, long y, unsigned radius) {
	for (long i = 0; i <= radius / TECO_SQRT_2 + 1; ++i) {
		long j = sqrt(radius * radius - i * i) + .5;

		for (char im = -1; im < 3; im += 2) {
			for (char jm = -1; jm < 3; jm += 2) {
				// Grossy looking
				long dx = x + im * i;
				long dy = y + jm * j;
				if (teco_image_get_in_bounds(image, dx, dy))
					TECO_FOR_LAYERS_CHARS(layers, layer, char_index)
						teco_image_set(image, layer, characters[char_index], dx, dy);
					
				dx = x + jm * j;
				dy = y + im * i;
				if (teco_image_get_in_bounds(image, dx, dy))
					TECO_FOR_LAYERS_CHARS(layers, layer, char_index)
						teco_image_set(image, layer, characters[char_index], dx, dy);
			}
		}
	}
};

void teco_image_fill(teco_image* image, unsigned char layers, char* characters) {
	TECO_FOR_LAYERS_CHARS(layers, layer, char_index)
		for (unsigned y = 0; y < image->height; y++)
			for (unsigned x = 0; x < image->width; x++)
				teco_image_set(image, layer, characters[char_index], x, y);
};

void teco_image_print(teco_image* image, unsigned char layers) {
	TECO_FOR_LAYERS(layers, layer) {
		for (unsigned y = 0; y < image->height; y++) {
			for (unsigned x = 0; x < image->width; x++) {
				char character = teco_image_get(image, layer, x, y);
				
				if (character == '\0') 
					printf(" ");
				else
					printf("%c", character);
			}
			printf("\n");
		}
		printf("\n");
	}
};

#if defined(TECO_SDL2) || defined(TECO_RAYLIB)

teco_palette teco_palette_import(char* path) {
	teco_palette palette = teco_palette_create();

	FILE* file_stream = fopen(path, "r");

	if (!file_stream) return palette;

	char buffer;
	unsigned short red = 0, green = 0, blue = 0;

	while ((buffer = fgetc(file_stream)) != EOF) {
		fseek(file_stream, 1, SEEK_CUR);

		if (fgetc(file_stream) == '#') {
			fscanf(file_stream, "%2hx%2hx%2hx\n", &red, &green, &blue);
		} else {
			fseek(file_stream, -1, SEEK_CUR);
			fscanf(file_stream, "%hu %hu %hu\n", &red, &green, &blue);
		}
		
		teco_palette_set_rgb(&palette, buffer, red, green, blue);
	}
	
	fclose(file_stream);

	return palette;
};

teco_palette teco_palette_create() {
	teco_palette palette = {.size=0, .colors=NULL};

	for (unsigned short i = 0; i < 256; i++) {
		palette.indices[i] = 255;
	}

	return palette;
};

void teco_palette_delete(teco_palette* palette) {
	free(palette->colors);

	palette->size = 0;
	palette->colors = NULL;
};

void teco_palette_set_rgb(teco_palette* palette, char character, unsigned char red, unsigned char green, unsigned char blue) {
	unsigned short color_index;

	unsigned short index = teco_palette_get_index(palette, character);

	if (index != palette->size) {
		color_index = index * 3;
	} else {
		palette->indices[(unsigned char) character] = palette->size;
		color_index = (palette->size) * 3;

		++(palette->size);

		palette->colors = realloc(palette->colors, palette->size * 3);
	}

	palette->colors[color_index] = red;
	palette->colors[color_index + 1] = green;
	palette->colors[color_index + 2] = blue;
};

void teco_palette_set_hex(teco_palette* palette, char character, char* hex) {
	unsigned short red, green, blue;

	sscanf(hex, "#%2hx%2hx%2hx", &red, &green, &blue);

	teco_palette_set_rgb(palette, character, red, green, blue);
};

unsigned short teco_palette_get_index(teco_palette* palette, char character) {
	unsigned short index = palette->indices[(unsigned char) character];

	if (index == 255) return palette->size;
	return index;
};

void teco_palette_get_color(teco_palette* palette, unsigned short index, unsigned char* red, unsigned char* green, unsigned char* blue) {
	unsigned short color_index = index * 3;

	*red = palette->colors[color_index];
	*green = palette->colors[color_index + 1];
	*blue = palette->colors[color_index + 2];
};

teco_bulb teco_bulb_create() {
	teco_bulb bulb = {.size=0, .effects=NULL};

	for (unsigned short i = 0; i < 256; i++) {
		bulb.indices[i] = 255;
	}

	return bulb;
};

void teco_bulb_delete(teco_bulb* bulb) {
	free(bulb->effects);

	bulb->size = 0;
	bulb->effects = NULL;
};

void teco_bulb_set(teco_bulb* bulb, char character, teco_effect effect) {
	unsigned short index = teco_bulb_get_index(bulb, character);

	if (index == bulb->size) {
		bulb->indices[(unsigned char) character] = bulb->size;

		++(bulb->size);

		bulb->effects = realloc(bulb->effects, bulb->size * sizeof(teco_effect));
	}

	bulb->effects[index] = effect;
};

unsigned short teco_bulb_get_index(teco_bulb* bulb, char character) {
	unsigned short index = bulb->indices[(unsigned char) character];

	if (index == 255) return bulb->size;
	return index;
};

teco_effect teco_bulb_get_effect(teco_bulb* bulb, unsigned short index) {
	return bulb->effects[index];
};

teco_cache teco_cache_create() {
	teco_cache cache = {.size=0, .textures=NULL};

	for (unsigned short i = 0; i < 256; i++) {
		cache.indices[i] = 255;
	}

	return cache;
};

void teco_cache_delete(teco_cache* cache) {
	for (unsigned short index = 0; index < cache->size; index++) {
		TECO_TEXTURE_DELETE(cache->textures[index]);
	}

	free(cache->textures);

	cache->size = 0;
	cache->textures = NULL;
};

void teco_cache_add(teco_cache* cache, char symbol, TECO_TEXTURE texture) {
	cache->indices[(unsigned char) symbol] = cache->size;

	++(cache->size);

	cache->textures = realloc(cache->textures, cache->size * sizeof(TECO_TEXTURE));

	cache->textures[cache->size - 1] = texture;
};

unsigned short teco_cache_get_index(teco_cache* cache, char symbol) {
	unsigned short index = cache->indices[(unsigned char) symbol];

	if (index == 255) return cache->size;
	return index;
};

TECO_TEXTURE teco_cache_get_texture(teco_cache* cache, unsigned short index) {
	return cache->textures[index];
};

#if defined(TECO_SDL2)

void teco_image_draw(teco_image* image, teco_palette* palette, teco_bulb* bulb, SDL_Renderer* renderer, teco_cache* cache, TECO_RECT draw_rect, TTF_Font* font, ...) {
	unsigned symbol_width = draw_rect.w / image->width;
	unsigned symbol_height = draw_rect.h / image->height;

#elif defined(TECO_RAYLIB)

void teco_image_draw(teco_image* image, teco_palette* palette, teco_bulb* bulb, teco_cache* cache, TECO_RECT draw_rect, Font font, ...) {
	unsigned symbol_width = draw_rect.width / image->width;
	unsigned symbol_height = draw_rect.height / image->height;

#endif
	
	va_list effect_args;

	if (image->layer_count >= 3) {
		va_start(effect_args, font);
	}
	
	TECO_COLOR color = {255, 255, 255, 255};
	TECO_COLOR render_color = {255, 255, 255, 255};

	for (unsigned y = 0; y < image->height; y++) {
		for (unsigned x = 0; x < image->width; x++) {
			char render_text[2] = " ";

			char symbol = teco_image_get(image, TECO_S, x, y);
			if (symbol == '\0' || symbol == ' ') continue;

			TECO_RECT symbol_rect = {
				draw_rect.x + symbol_width * x, draw_rect.y + symbol_height * y,
				symbol_width, symbol_height
			};

			if (image->layer_count >= 2) {
				char color_char = teco_image_get(image, TECO_C, x, y);
				if (color_char != '\0') {
					unsigned short color_index = teco_palette_get_index(palette, color_char);
					if (color_index != palette->size)
						teco_palette_get_color(
							palette, color_index,
							&color.r, &color.g, &color.b
						);
				}

				if (image->layer_count >= 3) {
					char effect_char = teco_image_get(image, TECO_E, x, y);
		
					if (effect_char != '\0') {
						unsigned short effect_index = teco_bulb_get_index(bulb, effect_char);
		
						if (effect_index != bulb->size) {
							teco_effect effect = teco_bulb_get_effect(bulb, effect_index);
		
							float dx = 0, dy = 0, scale_x = 1, scale_y = 1;
							
							va_list effect_args_temp;
							va_copy(effect_args_temp, effect_args);
							effect(&dx, &dy, &scale_x, &scale_y, x, y, effect_args_temp);
		
							symbol_rect.x += symbol_width * (dx + (1 - scale_x) * .5);
							symbol_rect.y += symbol_height * (dy + (1 - scale_y) * .5);
							
							#if defined(TECO_SDL2)
							symbol_rect.w *= scale_x;
							symbol_rect.h *= scale_y;
							#elif defined(TECO_RAYLIB)
							symbol_rect.width *= scale_x;
							symbol_rect.height *= scale_y;
							#endif
						}
					}
				}
			}

			unsigned short texture_index = teco_cache_get_index(cache, symbol);
			TECO_TEXTURE texture;
			if (texture_index == cache->size) {
				#if defined(TECO_SDL2)
				
				render_text[0] = symbol;
				
				SDL_Surface* surface = TECO_RENDER_TEXT(font, render_text, render_color);
				texture = SDL_CreateTextureFromSurface(renderer, surface);

				teco_cache_add(cache, symbol, texture);

				SDL_FreeSurface(surface);
				
				#elif defined(TECO_RAYLIB)

				int glyph_index = GetGlyphIndex(font, symbol);
				
				texture = LoadRenderTexture(
					(font.glyphs[glyph_index].advanceX == 0 ?
						font.recs[glyph_index].width :
						font.glyphs[glyph_index].advanceX
					),
					font.baseSize
				);

				BeginTextureMode(texture);
					DrawTextCodepoint(font, symbol, (Vector2) {0, 0}, font.baseSize, render_color);
				EndTextureMode();

				teco_cache_add(cache, symbol, texture);

				#endif
			} else {
				texture = teco_cache_get_texture(cache, texture_index);
			}

			#if defined(TECO_SDL2)
			
			SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
			SDL_RenderCopy(renderer, texture, NULL, &symbol_rect);

			#elif defined(TECO_RAYLIB)
			
			DrawTexturePro(
				texture.texture,
				(TECO_RECT) {0, 0, texture.texture.width, -texture.texture.height},
				symbol_rect,
				(Vector2) {0, 0}, 0.0f,
				color
			);

			#endif
		}
	}

	va_end(effect_args);
};

#elif defined(TECO_NCURSES) || defined(TECO_NCURSESW)

void teco_image_draw(teco_image* image, unsigned dx, unsigned dy) {
	for (unsigned y = 0; y < image->height; y++) {
		for (unsigned x = 0; x < image->width; x++) {
			char symbol = teco_image_get(image, TECO_S, x, y);

			if (symbol == ' ' || symbol == '\0') continue;

			if (image->layer_count >= 2) {
				int color_pair = COLOR_PAIR(teco_image_get(image, TECO_C, x, y));
				attron(color_pair);
			
				mvaddch(dy + y, dx + x, symbol);

				attroff(color_pair);
			} else {
				mvaddch(dy + y, dx + x, symbol);
			}
		}
	}
};

void teco_image_draw_on_window(teco_image* image, WINDOW* window, unsigned dx, unsigned dy) {
	for (unsigned y = 0; y < image->height; y++) {
		for (unsigned x = 0; x < image->width; x++) {
			char symbol = teco_image_get(image, TECO_S, x, y);

			if (symbol == ' ' || symbol == '\0') continue;

			if (image->layer_count >= 2) {
				int color_pair = COLOR_PAIR(teco_image_get(image, TECO_C, x, y));
				wattron(window, color_pair);
			
				mvwaddch(window, dy + y, dx + x, symbol);

				wattroff(window, color_pair);
			} else {
				mvwaddch(window, dy + y, dx + x, symbol);
			}
		}
	}
};

void teco_palette_import(char* path, short start) {
	FILE* file_stream = fopen(path, "r");

	if (!file_stream) return;

	char buffer;
	unsigned i = 0;
	unsigned short red = 0, green = 0, blue = 0;

	while ((buffer = fgetc(file_stream)) != EOF) {
		fseek(file_stream, 1, SEEK_CUR);

		if (fgetc(file_stream) == '#') {
			fscanf(file_stream, "%2hx%2hx%2hx\n", &red, &green, &blue);
		} else {
			fseek(file_stream, -1, SEEK_CUR);
			fscanf(file_stream, "%hu %hu %hu\n", &red, &green, &blue);
		}
		
		init_color(start + i, red * 1000 / 255, green * 1000 / 255, blue * 1000 / 255);
		init_pair(buffer, start + i, -1);

		++i;
	}
	
	fclose(file_stream);
};

#endif

#endif
