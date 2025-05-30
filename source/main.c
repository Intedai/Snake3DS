
#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

#define SQUARE_SIZE 20

#define DRAW_SQUARE(x,y,z,size,color) C2D_DrawRectangle(x,y,z,size,size,color,color,color,color)

void draw_background() {
    u32 light_color = C2D_Color32(170, 215, 81, 255);
    u32 dark_color = C2D_Color32(162, 209, 73, 255);

	for (int i = 1; i < SCREEN_HEIGHT / SQUARE_SIZE - 1; i++)
	{
		for(int j = 1; j < SCREEN_WIDTH / SQUARE_SIZE - 1; j++)
		{
			u32 curr = j % 2 == i % 2 ? light_color : dark_color;
			DRAW_SQUARE(j * SQUARE_SIZE,   i * SQUARE_SIZE, 0, SQUARE_SIZE, curr);
		}
	}
}

int main(int argc, char **argv)
{

	// Initialize services
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);

	C2D_Prepare();

	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	u32 border_color = C2D_Color32(87, 138, 52, 255);

	while (aptMainLoop())
	{
		hidScanInput();
		u32 kDown = hidKeysDown();

		if (kDown & KEY_START) break;

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		// Draw top
		C2D_TargetClear(top, border_color);
		C2D_SceneBegin(top);
		draw_background();


		// Draw bottom
		C2D_TargetClear(bottom, border_color);
		C2D_SceneBegin(bottom);

		C3D_FrameEnd(0);
	}

	C2D_Fini();
	C3D_Fini();
	gfxExit();

	return 0;
}
