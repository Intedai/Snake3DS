#ifndef FRUIT_H
#define FRUIT_H

#include <stdlib.h>
#include <citro2d.h>
#include "point.h"
#include "snake.h"

typedef struct {
    int value;
    u32 color;
    Point position;
} Fruit;

Fruit *create_fruits(int fruit_amount, u32 color, int value);
void place_fruit(int fruit_idx, Fruit *fruits, int fruit_amount, Snake *snake, int row_squares, int col_squares);
void init_fruits(Fruit* fruits, int fruit_amount, Snake* snake, int row_squares, int col_squares);
bool fruit_eaten (Snake *snake, Fruit* fruits, int fruit_amount, int row_squares, int col_squares);

#endif