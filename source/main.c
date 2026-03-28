#include <3ds.h>
#include <citro2d.h>
#include <string.h>
#include <stdbool.h>
#include "point.h"
#include "snake.h"
#include "fruit.h"
#include "game_graphics.h"

void quit_game() {
	cfguExit();
	C2D_Fini();
	C3D_Fini();
	gfxExit();
}

bool update_direction(Snake* snake, u32 mask) {
	Direction direction;
	if (mask & KEY_RIGHT || mask & KEY_CSTICK_RIGHT)
		direction = RIGHT;
	else if (mask & KEY_LEFT || mask & KEY_CSTICK_LEFT)
		direction = LEFT;
	else if (mask & KEY_DOWN || mask & KEY_CSTICK_DOWN)
		direction = DOWN;
	else if (mask & KEY_UP || mask & KEY_CSTICK_UP)
		direction = UP;
	else
		return false;

	return change_direction(snake, direction);
}

int main(int argc, char **argv) {
	u64 last_move_time = osGetTime();
	const u64 moveDelay = 100;

	// Initialize services
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);

	C2D_Prepare();
	

	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
	

	int square_size = 20;
	int row_squares = total_squares_in_row(square_size);
	int col_squares = total_squares_in_column(square_size);

	u32 snake_color = C2D_Color32(78,124,246,255);
	u32 fruit_color = C2D_Color32(231, 71, 29, 255);
	u32 border_color = C2D_Color32(87, 138, 52, 255);
    u32 light_color = C2D_Color32(170, 215, 81, 255);
    u32 dark_color = C2D_Color32(162, 209, 73, 255);
	u32 bottom_color = C2D_Color32(74, 117, 44, 255);

	int fruit_margin = square_size / 5;
	int fruit_value = 10;

	const int border_size = 30;

	Point starting_position = {4, 4};
	int starting_size = 3;

	Snake *snake = create_snake();

	if (!snake) {
		quit_game();
		return 1;
	}

	bool init_success = init_snake(
		snake,
		starting_position,
		starting_size,
		snake_color,
		row_squares,
		col_squares
	);

	if (!init_success) {
		quit_game();
		free_snake(snake);
		return 1;	
	}

	int fruit_count = 5;
	Fruit *fruits = create_fruits(fruit_count, fruit_color, fruit_value);

	if(!fruits) {
		quit_game();
		free_snake(snake);
		return 1;
	}

	init_fruits(fruits, fruit_count, snake, row_squares, col_squares);
	
	bool choseDirection = false;

	while (aptMainLoop()) {
		hidScanInput();
		u32 kDown = hidKeysDown();

		if (kDown & KEY_START) break;

		if (!choseDirection && update_direction(snake, kDown)) {
			choseDirection = true;
		}

		u64 now = osGetTime();
		if (now - last_move_time >= moveDelay) {
			move_snake(snake, fruit_eaten(snake, fruits, fruit_count, row_squares, col_squares));
			last_move_time = now;
			choseDirection = false;
		}

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		if(is_snake_dead(snake, row_squares, col_squares)) {
			border_color = C2D_Color32(255, 0, 0, 255);
		};
		
		// Draw top
		C2D_SceneBegin(top);
		C2D_TargetClear(top, border_color);
		draw_background(square_size, row_squares, col_squares, light_color, dark_color);
		
		draw_snake(snake, square_size);

		for(int i = 0; i < fruit_count; i++) {
			draw_fruit(fruits[i], square_size, fruit_margin);
		}

		consoleClear();

		// Draw bottom
		C2D_SceneBegin(bottom);
		C2D_TargetClear(bottom, border_color);
		draw_bottom(border_size, bottom_color);

		C3D_FrameEnd(0);
	}

	quit_game();
	free_snake(snake);
	free(fruits);

	return 0;
}
