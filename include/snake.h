#ifndef SNAKE_H
#define SNAKE_H

#include <stdlib.h>
#include <stdbool.h>
#include <citro2d.h>
#include "point.h"
#include "square_utils.h"

typedef enum {LEFT, RIGHT, UP, DOWN} Direction;

typedef struct Node {
    Point position;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct {
    int size;
    int score;
    u32 color;
    Direction direction;
    Node *head;
    Node *tail;
} Snake;

Node *new_node(Point position);
Snake *create_snake();
bool init_snake(Snake* snake, Point starting_position, int starting_size, u32 color, int row_squares, int col_squares);
bool insert_at_front(Snake *snake, Point position);
void remove_tail(Snake* snake);
bool is_snake_dead(Snake *snake, int row_squares, int col_squares);
void move_snake(Snake* snake, bool ate_fruit);
bool push_back_node(Snake *snake, Point position);
bool change_direction(Snake* snake, Direction direction);
void free_snake(Snake *snake);

#endif