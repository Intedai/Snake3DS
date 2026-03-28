#include "square_utils.h"

int total_squares_in_row(int square_size) {
    // Not including the borders (border len = square_size)
    return TOP_SCREEN_WIDTH / square_size - 2;
}

int total_squares_in_column(int square_size) {
    // Not including the borders (border len = square_size)
    return TOP_SCREEN_HEIGHT / square_size - 2;
}