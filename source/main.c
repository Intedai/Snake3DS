#include <3ds.h>
#include <citro2d.h>
#include <string.h>
#include <stdbool.h>
#include "globals.h"
#include "point.h"
#include "snake.h"

#define SQUARE_SIZE 20

#define DRAW_SQUARE(x,y,z,size,color) C2D_DrawRectSolid(x,y,z,size,size,color)
#define shadow_color C2D_Color32(148, 189, 70, 255)

void draw_background() {
    u32 light_color = C2D_Color32(170, 215, 81, 255);
    u32 dark_color = C2D_Color32(162, 209, 73, 255);

	for (int i = 1; i < TOP_SCREEN_HEIGHT / SQUARE_SIZE - 1; i++)
	{
		for(int j = 1; j < TOP_SCREEN_WIDTH / SQUARE_SIZE - 1; j++)
		{
			u32 curr = j % 2 == i % 2 ? light_color : dark_color;
			DRAW_SQUARE(j * SQUARE_SIZE,   i * SQUARE_SIZE, 0, SQUARE_SIZE, curr);
		}
	}
}

void draw_bottom()
{
	u32 bottom_color = C2D_Color32(74, 117, 44, 255);
	const int dist = 30;

	C2D_DrawRectSolid(dist,dist, 0, BOTTOM_SCREEN_WIDTH - dist * 2, BOTTOM_SCREEN_HEIGHT - dist * 2, bottom_color);
}

void exit_all()
{
	cfguExit();
	C2D_Fini();
	C3D_Fini();
	gfxExit();
}

void draw_fruit(Point position)
{
	int dist = SQUARE_SIZE / 5;
	u32 fruit_color = C2D_Color32(231, 71, 29, 255);
	DRAW_SQUARE(position.x * SQUARE_SIZE + dist, position.y * SQUARE_SIZE + dist, 0.1, SQUARE_SIZE - dist * 2, fruit_color);
	C2D_DrawRectSolid(position.x * SQUARE_SIZE + dist, SQUARE_SIZE*position.y +  SQUARE_SIZE - dist, 0.1,SQUARE_SIZE - dist * 2, SQUARE_SIZE / 7, shadow_color);
}

void draw_snake_node(Node* node)
{
	Point position = node->position;
	DRAW_SQUARE(SQUARE_SIZE*position.x,SQUARE_SIZE*position.y,1,SQUARE_SIZE,C2D_Color32(78,124,246,255));
	
	if (position.y != TOP_SCREEN_HEIGHT / SQUARE_SIZE - 2)
		C2D_DrawRectSolid(SQUARE_SIZE*position.x, SQUARE_SIZE*position.y + SQUARE_SIZE,0.5,SQUARE_SIZE, SQUARE_SIZE / 5,shadow_color);
}

void update_direction(Snake* snake, u32 mask)
{
	if (mask & KEY_RIGHT || mask & KEY_CSTICK_RIGHT)
		change_direction(snake, RIGHT);
	else if (mask & KEY_LEFT || mask & KEY_CSTICK_LEFT)
		change_direction(snake, LEFT);
	else if (mask & KEY_DOWN || mask & KEY_CSTICK_DOWN)
		change_direction(snake, DOWN);
	else if (mask & KEY_UP || mask & KEY_CSTICK_UP)
		change_direction(snake, UP);
}

int main(int argc, char **argv)
{
	C2D_Font font = C2D_FontLoadSystem(CFG_REGION_USA);
	u64 last_move_time = osGetTime();
	const u64 moveDelay = 100;

	// Initialize services
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);

	C2D_Prepare();
	

	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
	

	u32 border_color = C2D_Color32(87, 138, 52, 255);

	Point start_position = {8,8};

	Snake *snake = create_snake();
	init_snake(snake,start_position);

	Point fruit = {3,3};
	while (aptMainLoop())
	{
		hidScanInput();
		u32 kDown = hidKeysDown();

		if (kDown & KEY_START) {exit_all(); break;}
		update_direction(snake,kDown);

		u64 now = osGetTime();
		if (now - last_move_time >= moveDelay)
		{
			if (equal_points(fruit, snake->head->position))
				move_snake(snake, true);
			else
				move_snake(snake, false);
			last_move_time = now;
		}

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		if(end_game(snake)) {
			border_color = C2D_Color32(255, 0, 0, 255);
		};
		
		// Draw top
		C2D_SceneBegin(top);
		C2D_TargetClear(top, border_color);
		draw_background();


		foreach_node(snake, draw_snake_node);
		draw_fruit(fruit);
		consoleClear();


		C2D_SceneBegin(bottom);
		C2D_TargetClear(bottom, border_color);
		draw_bottom();
		C3D_FrameEnd(0);
	}
	exit_all();
	return 0;
}
