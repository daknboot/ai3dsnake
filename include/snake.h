#ifndef SNAKE_H
#define SNAKE_H

#include <stdbool.h>

#define MAX_SNAKE_LENGTH 512

typedef struct
{
    int x;
    int y;
    int z;
} Vec3i;

typedef enum
{
    DIR_POS_X,
    DIR_NEG_X,
    DIR_POS_Y,
    DIR_NEG_Y,
    DIR_POS_Z,
    DIR_NEG_Z
} Direction;

typedef struct
{
    Vec3i segments[MAX_SNAKE_LENGTH];
    int length;
    Direction direction;
    bool grow;
} Snake;

void snake_init(Snake *snake, int start_length, Vec3i start_pos, Direction dir);
void snake_set_direction(Snake *snake, Direction dir);
Vec3i snake_next_head_position(const Snake *snake);
void snake_step(Snake *snake, Vec3i next_head);
bool snake_intersects(const Snake *snake, Vec3i point, bool include_head);

#endif /* SNAKE_H */
