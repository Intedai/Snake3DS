#include "game_graphics.h"

#define DRAW_SQUARE(x,y,z,size,color) C2D_DrawRectSolid(x,y,z,size,size,color)
#define SHADOW_COLOR C2D_Color32(148, 189, 70, 255)

void draw_background(int square_size, int row_squares, int col_squares, u32 light_color, u32 dark_color) {
	for (int i = 1; i <= col_squares; i++) {
		for(int j = 1; j <= row_squares; j++) {
			u32 curr = j % 2 == i % 2 ? light_color : dark_color;
			DRAW_SQUARE(j * square_size,   i * square_size, 0, square_size, curr);
		}
	}
}

void draw_bottom(int border_size, u32 color) {
	C2D_DrawRectSolid(border_size,border_size, 0, BOTTOM_SCREEN_WIDTH - border_size * 2, BOTTOM_SCREEN_HEIGHT - border_size * 2, color);
}

void draw_fruit(Fruit fruit, int square_size, int margin) {
	DRAW_SQUARE(fruit.position.x * square_size + margin, fruit.position.y * square_size + margin, 0.1, square_size - margin * 2, fruit.color);
	C2D_DrawRectSolid(fruit.position.x * square_size + margin, square_size*fruit.position.y +  square_size - margin, 0.1,square_size - margin * 2, square_size / 7, SHADOW_COLOR);
}

void _draw_snake_node(Node* node, int square_size, u32 color) {
	Point position = node->position;
	DRAW_SQUARE(
		square_size*position.x,
		square_size*position.y,
		1,
		square_size,
		color
	);
	
	if (position.y != TOP_SCREEN_HEIGHT / square_size - 2)
		C2D_DrawRectSolid(square_size*position.x, square_size*position.y + square_size,0.5,square_size, square_size / 5,SHADOW_COLOR);
}

void draw_snake(Snake *snake, int square_size) {
    Node *current = snake-> head;
    while (current != NULL) {
        _draw_snake_node(current, square_size, snake->color);
        current = current -> next;
    }
}