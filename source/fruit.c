#include "fruit.h"

Fruit *create_fruits(int fruit_amount)
{
    Fruit *fruits = (Fruit *)malloc(fruit_amount * sizeof(Fruit));
    
    if (!fruits)
        return NULL;

    for(int i = 0; i < fruit_amount; i++)
    {
        fruits[i].color = NORMAL_FRUIT_COLOR;
        fruits[i].value = NORMAL_FRUIT_VALUE;
        fruits[i].position.x = -1;
        fruits[i].position.y = -1;
    }
    return fruits;
}

void place_fruit(int fruit_idx, Fruit *fruits, int fruit_amount, Snake *snake)
{
    // If there's no space to place anymore fruits
    if (snake->size >= TOTAL_SQUARES - fruit_amount)
        return;

    Point new_position;
    bool can_place;

    do {
        can_place = true;

        new_position.x = 1 + rand() % (RIGHT_BORDER - 1);
        new_position.y = 1 + rand() % (BOTTOM_BORDER - 1);
        
        Node *current = snake->head;
        while(current != NULL)
        {
            if (equal_points(current->position, new_position))
            {
                can_place = false;
                break;
            }
            current = current->next;
        }

        for(int i = 0; i < fruit_amount; i++)
        {
            if (equal_points(fruits[i].position, new_position))
            {
                can_place = false;
                break;
            }
        }
    } while(!can_place);
    fruits[fruit_idx].position = new_position;
}

void init_fruits(Fruit* fruits, int fruit_amount, Snake* snake)
{
    for(int i = 0; i < fruit_amount; i++)
    {
        place_fruit(i, fruits, fruit_amount, snake);
    }
}

bool fruit_eaten (Snake *snake, Fruit* fruits, int fruit_amount)
{
    for (int i = 0; i < fruit_amount; i++)
    {
        if(equal_points(snake->head->position, fruits[i].position))
        {
            place_fruit(i,fruits,fruit_amount,snake);
            snake->score += fruits[i].value;
            return true;
        }
    }
    return false;
}