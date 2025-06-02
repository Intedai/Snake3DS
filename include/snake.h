#ifndef SNAKE_H
#define SNAKE_H

#include <stdlib.h>
#include <stdbool.h>
#include "point.h"
#include "globals.h"

#define START_SIZE 3

typedef enum {LEFT, RIGHT, UP, DOWN} Direction;

typedef struct Node
{
    Point position;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct
{
    int size;
    int score;
    Direction direction;
    Node *head;
    Node *tail;
} Snake;

Node *new_node(Point position);
Snake *create_snake();
bool init_snake(Snake* snake, Point starting_position);
bool insert_at_front(Snake *snake, Point position);
void remove_tail(Snake* snake);
bool end_game(Snake *snake);
void move_snake(Snake* snake, bool ate_fruit);
void foreach_node(Snake *snake, void (*func)(Node *));
bool push_back_node(Snake *snake, Point position);
void change_direction(Snake* snake, Direction direction);
void free_snake(Snake *snake);

#endif