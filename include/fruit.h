#ifndef FRUIT_H
#define FRUIT_H

#include <stdlib.h>
#include <citro2d.h>
#include "globals.h"
#include "point.h"
#include "snake.h"

#define NORMAL_FRUIT_COLOR C2D_Color32(231, 71, 29, 255)

#define NORMAL_FRUIT_VALUE 10

typedef struct {
    int value;
    u32 color;
    Point position;
} Fruit;

Fruit *create_fruits(int fruit_amount);
void place_fruit(int fruit_idx, Fruit *fruits, int fruit_amount, Snake *snake);
void init_fruits(Fruit* fruits, int fruit_amount, Snake* snake);
bool fruit_eaten (Snake *snake, Fruit* fruits, int fruit_amount);

#endif