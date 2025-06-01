#ifndef POINT_H
#define POINT_H

#include <stdbool.h>

typedef struct
{
	int x;
	int y;
} Point;

static inline bool equal_points(Point a, Point b) { return a.x == b.x && a.y == b.y; }

#endif