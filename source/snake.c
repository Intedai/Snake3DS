#include "snake.h"

Node* new_node(Point position) {
    Node *node = (Node *) malloc(sizeof(Node));
    
    if(!node)
        return NULL;

    node->position = position; 
    node->next = NULL;
    node->prev = NULL;
    
    return node;
}

Snake *create_snake() {
    Snake *snake = (Snake *)malloc(sizeof(Snake));

    // Will return NULL if allocation fails
    return snake;
}

bool insert_at_front(Snake *snake, Point position) {
    Node *node = new_node(position);

    if (!node)
        return false;

    if (snake->tail == NULL) {
        snake->head = node;
        snake->tail = node;
    }
    else {
        node->next = snake->head;
        snake->head->prev = node;
        snake->head = node; 
    }

    snake->size++;

    return true;
}

bool change_direction(Snake* snake, Direction direction) {
    if (
        (snake->direction == RIGHT && direction == LEFT) ||
        (snake->direction == LEFT && direction == RIGHT) ||
        (snake->direction == UP && direction == DOWN) ||
        (snake->direction == DOWN && direction == UP)
    )
        return false;
    snake->direction = direction;
    return true;
}
bool push_back_node(Snake *snake, Point position) {
    Node *node = new_node(position);

    if (!node)
        return false;

    if (snake->tail == NULL) {
        snake->head = node;
        snake->tail = node;
    }
    else {
        snake->tail->next = node;
        node->prev = snake->tail;
        snake->tail =node;
    }
    snake->size++;

    return true;
}

bool init_snake(Snake* snake, Point starting_position, int starting_size, u32 color, int row_squares, int col_squares) {
    if (
        starting_position.x  < starting_size ||
        starting_position.x > row_squares ||
        starting_position.y < 1 ||
        starting_position.y > col_squares
    )
        return false;
    snake->size = 0;
    snake->score = 0;
    snake->color = color;
    snake->direction = RIGHT;
    snake->head = NULL;
    snake->tail = NULL;

    for(int i = 0; i < starting_size; i++) {
        Point position = {starting_position.x - i, starting_position.y}; 
        if(!push_back_node(snake, position))
            return false;
    }
    return true;
}

void remove_tail(Snake* snake) {
    Node *tail = snake-> tail;

    // Set tail to the previous node
    snake->tail = tail->prev;
    snake->tail->next = NULL;
    snake->size--;
    free(tail);
}

void move_snake(Snake* snake, bool ate_fruit) {
    Point new_head_position;
    Point old_head_position = snake->head->position;

    switch (snake->direction) {
        case LEFT:
            new_head_position.x = old_head_position.x - 1;
            new_head_position.y = old_head_position.y + 0;
            break;
        case RIGHT:
            new_head_position.x = old_head_position.x + 1;
            new_head_position.y = old_head_position.y + 0;
            break;
        case UP:
            new_head_position.x = old_head_position.x + 0;
            new_head_position.y = old_head_position.y - 1;
            break;
        case DOWN:
            new_head_position.x = old_head_position.x + 0;
            new_head_position.y = old_head_position.y + 1;
            break;     
        default:
            return; //impossible
    }
    
    if (!insert_at_front(snake,new_head_position))
        return;
    
    if(!ate_fruit) {
        remove_tail(snake);
    }
}

bool is_snake_dead(Snake *snake, int row_squares, int col_squares) {
    Point head_position = snake->head->position;
    // Check if collides with walls
    if (
            head_position.x < 1 ||
            head_position.x > row_squares ||
            head_position.y < 1 ||
            head_position.y > col_squares
       )
       return true;
       
    // Check if collides with itself
    Node *current = snake->head->next;
    while(current != NULL) {
        if (equal_points(head_position, current->position))
            return true;
            
        current = current->next;
    }

    return false;
}

void free_snake(Snake *snake) {
    Node *current = snake-> head;
    Node *next;

    while(current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    free(snake);
}