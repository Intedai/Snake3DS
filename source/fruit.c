#include "fruit.h"

Fruit *create_fruits(int fruit_amount, u32 color, int value) {
    Fruit *fruits = (Fruit *)malloc(fruit_amount * sizeof(Fruit));
    
    if (!fruits)
        return NULL;

    for(int i = 0; i < fruit_amount; i++) {
        fruits[i].color = color;
        fruits[i].value = value;
        fruits[i].position.x = -1;
        fruits[i].position.y = -1;
    }

    return fruits;
}

void place_fruit(int fruit_idx, Fruit *fruits, int fruit_amount, Snake *snake, int row_squares, int col_squares) {
    // If there's no space to place anymore fruits
    if (snake->size > row_squares * col_squares - fruit_amount)
        return;

    Point new_position;
    bool can_place;

    do {
        can_place = true;

        new_position.x = 1 + rand() % (row_squares);
        new_position.y = 1 + rand() % (col_squares);
        
        Node *current = snake->head;
        while(current != NULL) {
            if (equal_points(current->position, new_position)) {
                can_place = false;
                break;
            }
            current = current->next;
        }

        for(int i = 0; i < fruit_amount; i++) {
            if (equal_points(fruits[i].position, new_position)) {
                can_place = false;
                break;
            }
        }
    } while(!can_place);

    fruits[fruit_idx].position = new_position;
}

void init_fruits(Fruit* fruits, int fruit_amount, Snake* snake, int row_squares, int col_squares)
{
    for(int i = 0; i < fruit_amount; i++) {
        place_fruit(i, fruits, fruit_amount, snake, row_squares, col_squares);
    }
}

bool fruit_eaten (Snake *snake, Fruit* fruits, int fruit_amount, int row_squares, int col_squares)
{
    for (int i = 0; i < fruit_amount; i++) {
        if(equal_points(snake->head->position, fruits[i].position)) {
            place_fruit(i,fruits,fruit_amount,snake, row_squares, col_squares);
            snake->score += fruits[i].value;
            return true;
        }
    }
    return false;
}