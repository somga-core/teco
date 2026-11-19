#define TECO_RAYLIB
#include "../teco.h"

#include <stdlib.h>

#define VORONOI_DOT_COUNT 30

#define VORONOI_CHARS " .:~*"
#define VORONOI_CHAR_COUNT 5
#define VORONOI_START_DISTANCE 1
#define VORONOI_END_DISTANCE 6

#define VORONOI_SPEED_MAX_CHANGE .1
#define VORONOI_CHANGE_DIRECTION_TIME_SECONDS_WOW_THATS_A_VERY_LONG_VARIABLE_NAME_BY_THE_WAY 2

#define BIG 1000
#define SMALL .01

typedef struct voronoi_dot {
	float x, y;
	float speed_x, speed_y;	
} voronoi_dot;

void effect_wave(float* dx, float* dy, float* scale_x, float* scale_y, unsigned x, unsigned y, va_list args) {
	float time = va_arg(args, double);
    *dy = sin(x / 2. + y / 1.5 + time) / 4 + sin(x / 9. + y / 2 + time / 2) / 2 + sin(x + y + time * 4) / 20;
}

// WARNING: Close your eyes and don't open them while reading this function
// This is the grossiest implementation of voronoi nose
void voronoi_generate(teco_image* image, voronoi_dot dots[VORONOI_DOT_COUNT]) {
	for (unsigned y = 0; y < image->height; ++y) {
		for (unsigned x = 0; x < image->width; ++x) {
			float min_distance = BIG;

			for (unsigned i = 0; i < VORONOI_DOT_COUNT; ++i) {
				float distance = sqrt(
					pow(dots[i].x - x, 2) +
					pow(dots[i].y - y, 2)
				);

				if (distance < min_distance) min_distance = distance;
			}

			if (min_distance >= VORONOI_END_DISTANCE) min_distance = VORONOI_END_DISTANCE - SMALL;
			if (min_distance < VORONOI_START_DISTANCE) min_distance = VORONOI_START_DISTANCE;
			
			unsigned char_index = VORONOI_CHAR_COUNT * (min_distance - VORONOI_START_DISTANCE) / (VORONOI_END_DISTANCE - VORONOI_START_DISTANCE);
			
			teco_image_set(image, TECO_S, VORONOI_CHARS[char_index], x, y);
		}
	}
}

void voronoi_move_dots(teco_image* image, voronoi_dot dots[VORONOI_DOT_COUNT], float delta_time) {
	for (unsigned i = 0; i < VORONOI_DOT_COUNT; ++i) {		
		dots[i].x += dots[i].speed_x * delta_time;
		dots[i].y += dots[i].speed_y * delta_time;
		
		if (
			(dots[i].x >= image->width && dots[i].speed_x > 0) ||
			(dots[i].x < 0 && dots[i].speed_x < 0)
		) dots[i].speed_x = -dots[i].speed_x;
		
		if (
			(dots[i].y >= image->height && dots[i].speed_y > 0) ||
			(dots[i].y < 0 && dots[i].speed_y < 0)
		) dots[i].speed_y = -dots[i].speed_y;
	}
}

void voronoi_change_dots_directions(voronoi_dot dots[VORONOI_DOT_COUNT]) {
	for (unsigned i = 0; i < VORONOI_DOT_COUNT; ++i) {
		dots[i].speed_x += VORONOI_SPEED_MAX_CHANGE * (float) (rand() % (BIG * 2) - BIG) / BIG;
		dots[i].speed_y += VORONOI_SPEED_MAX_CHANGE * (float) (rand() % (BIG * 2) - BIG) / BIG;
	}
}

int main() {
	// Raylib stuff
    InitWindow(640, 480, "Waves effect");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    
	SetTargetFPS(60);
	
	Font font = LoadFontEx("examples/assets/font.ttf", 50, 0, 0);
    Rectangle window_rect = {0, 0, 0, 0};

	// TeCo stuff
    teco_image image = teco_image_create(3, 50, 25);
	teco_image_fill(&image, TECO_CE, "6~");
    
    teco_palette palette = teco_palette_import("examples/assets/core.tcp");
    teco_cache cache = teco_cache_create();    
    teco_bulb bulb = teco_bulb_create();
    teco_bulb_set(&bulb, '~', &effect_wave);
    
	// Generate dots for voronoi noise
	srand(10);
	
	voronoi_dot dots[VORONOI_DOT_COUNT];
	for (unsigned i = 0; i < VORONOI_DOT_COUNT; ++i) {
		voronoi_dot dot = {rand() % image.width, rand() % image.height, 0, 0};
		
		dots[i] = dot;
	}

	voronoi_change_dots_directions(dots);
	
	float time_accumulator = 0;
	
    // Mainloop
    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground((Color) {19, 68, 125, 0x00});

			window_rect.width = GetScreenWidth();
			window_rect.height = GetScreenHeight();

			time_accumulator += GetFrameTime();

			if (time_accumulator > VORONOI_CHANGE_DIRECTION_TIME_SECONDS_WOW_THATS_A_VERY_LONG_VARIABLE_NAME_BY_THE_WAY) {
				time_accumulator -= VORONOI_CHANGE_DIRECTION_TIME_SECONDS_WOW_THATS_A_VERY_LONG_VARIABLE_NAME_BY_THE_WAY;
				voronoi_change_dots_directions(dots);
			}
			
			voronoi_move_dots(&image, dots, GetFrameTime());
			
			voronoi_generate(&image, dots);
			
			teco_image_draw(&image, &palette, &bulb, &cache, window_rect, font, GetTime());
        EndDrawing();
    }

	// Cleaning that is technically not neccesary
    UnloadFont(font);

    teco_image_delete(&image);
    teco_palette_delete(&palette);
    teco_bulb_delete(&bulb);
    teco_cache_delete(&cache);

    CloseWindow();

    return 0;
}
