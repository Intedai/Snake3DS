#ifndef GAME_GRAPHICS_H
#define GAME_GRAPHICS_H

#include <3ds.h>
#include <citro2d.h>
#include <string.h>
#include <stdbool.h>
#include "dimensions.h"
#include "square_utils.h"
#include "point.h"
#include "snake.h"
#include "fruit.h"

void draw_background(int square_size, int row_squares, int col_squares, u32 light_color, u32 dark_color);
void draw_bottom(int border_size, u32 color);
void draw_fruit(Fruit fruit, int square_size, int margin);
void draw_snake(Snake *snake, int square_size);

#endif