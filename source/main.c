#include <3ds.h>
#include <citro2d.h>
#include <string.h>
#include <stdbool.h>
#include "point.h"
#include "snake.h"
#include "fruit.h"
#include "game_graphics.h"

typedef enum {GAME, MENU} State;

void quit_game() {
	cfguExit();
	C2D_Fini();
	C3D_Fini();
	gfxExit();
}

void free_game_resources(Snake* snake, Fruit* fruits) {
	free_snake(snake);
	free(fruits);
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

void draw_menu(
		C3D_RenderTarget* top,
		C3D_RenderTarget* bottom
) {
	u32 bg_color = C2D_Color32(60, 60, 60, 255);
	
	// Draw top
	C2D_SceneBegin(top);
	C2D_TargetClear(top, bg_color);
	
	// Draw bottom
	C2D_SceneBegin(bottom);
	C2D_TargetClear(bottom, bg_color);
}

State read_menu_input(u32 kDown) {
	if (kDown & KEY_START)
		return GAME;
	return MENU;
}

void init_game(
	Snake** snake,
	Fruit** fruits,
	Point starting_position,
	int starting_size,
	u32 snake_color,
	int row_squares,
	int col_squares,
	int fruit_count,
	u32 fruit_color,
	int fruit_value,
	bool* chose_direction
) {
	*snake = create_snake();

	if (!(*snake)) {
		quit_game();
		exit(1);
	}

	bool init_success = init_snake(
		*snake,
		starting_position,
		starting_size,
		snake_color,
		row_squares,
		col_squares
	);

	if (!init_success) {
		free_snake(*snake);
		quit_game();
		exit(1);	
	}

	*fruits = create_fruits(fruit_count, fruit_color, fruit_value);

	if(!fruits) {
		free_snake(*snake);
		quit_game();
		exit(1);
	}

	init_fruits(*fruits, fruit_count, *snake, row_squares, col_squares);
	
	*chose_direction = false;
}

State process_game_frame(
		bool* choseDirection,
		Snake* snake,
		u32 kDown,
		int move_delay,
		u64* last_move_time,
		Fruit *fruits,
		int fruit_count,
		int row_squares,
		int col_squares
) {
	if (!(*choseDirection) && update_direction(snake, kDown)) {
		*choseDirection = true;
	}

	u64 now = osGetTime();
	if (now - *last_move_time >= move_delay) {
		move_snake(snake, fruit_eaten(snake, fruits, fruit_count, row_squares, col_squares));
		*last_move_time = now;
		*choseDirection = false;
	}

	if(is_snake_dead(snake, row_squares, col_squares)) {
		return MENU;
	}
	else {
		return GAME;
	}
}

void draw_game_graphics(
		C3D_RenderTarget* top,
		C3D_RenderTarget* bottom,
		u32 border_color,
		int square_size,
		int row_squares,
		int col_squares,
		u32 light_color,
		u32 dark_color,
		Snake* snake,
		Fruit *fruits,
		int fruit_count,
		int fruit_margin,
		int border_size,
		u32 bottom_color
) {
	// Draw top
	C2D_SceneBegin(top);
	C2D_TargetClear(top, border_color);
	draw_background(square_size, row_squares, col_squares, light_color, dark_color);
		
	draw_snake(snake, square_size);

	for(int i = 0; i < fruit_count; i++) {
		draw_fruit(fruits[i], square_size, fruit_margin);
	}

	// Draw bottom
	C2D_SceneBegin(bottom);
	C2D_TargetClear(bottom, border_color);
	draw_bottom(border_size, bottom_color);
} 


int main(int argc, char **argv) {
	u64 last_move_time = osGetTime();
	const u64 move_delay = 100;

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
	int fruit_count = 5;
	int fruit_value = 10;

	const int border_size = 30;

	Point starting_position = {4, 4};
	int starting_size = 3;

	State current_state = MENU;
	bool chose_direction = false;

	Snake* snake = NULL;
	Fruit* fruits = NULL;

	while (aptMainLoop()) {
		hidScanInput();
		u32 kDown = hidKeysDown();

		switch (current_state)
		{
		case MENU:
			current_state = read_menu_input(kDown);
			if (current_state == GAME) {
				init_game(
					&snake,
					&fruits,
					starting_position,
					starting_size,
					snake_color,
					row_squares,
					col_squares,
					fruit_count,
					fruit_color,
					fruit_value,
					&chose_direction
				);
				break;
			}
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			draw_menu(top, bottom);
			break;
		
		case GAME:
			current_state = process_game_frame(
				&chose_direction,
				snake,
				kDown,
				move_delay,
				&last_move_time,
				fruits, fruit_count,
				row_squares,
				col_squares
			);

			if (current_state == MENU) {
				free_game_resources(snake, fruits);
				break;
			}
		
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			draw_game_graphics(
				top,
				bottom,
				border_color,
				square_size,
				row_squares,
				col_squares,
				light_color,
				dark_color,
				snake,
				fruits,
				fruit_count,
				fruit_margin,
				border_size,
				bottom_color
			);
			break;
		}


		C3D_FrameEnd(0);
	}

	free_game_resources(snake, fruits);
	quit_game();

	return 0;
}
