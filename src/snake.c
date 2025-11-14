#include "snake.h"

static bool is_opposite(Direction a, Direction b)
{
    return (a == DIR_POS_X && b == DIR_NEG_X) || (a == DIR_NEG_X && b == DIR_POS_X) ||
           (a == DIR_POS_Y && b == DIR_NEG_Y) || (a == DIR_NEG_Y && b == DIR_POS_Y) ||
           (a == DIR_POS_Z && b == DIR_NEG_Z) || (a == DIR_NEG_Z && b == DIR_POS_Z);
}

void snake_init(Snake *snake, int start_length, Vec3i start_pos, Direction dir)
{
    if (!snake)
    {
        return;
    }

    if (start_length < 1)
    {
        start_length = 1;
    }

    snake->length = start_length;
    snake->direction = dir;
    snake->grow = false;

    for (int i = 0; i < start_length; ++i)
    {
        snake->segments[i] = start_pos;
        switch (dir)
        {
        case DIR_POS_X:
            snake->segments[i].x -= i;
            break;
        case DIR_NEG_X:
            snake->segments[i].x += i;
            break;
        case DIR_POS_Y:
            snake->segments[i].y -= i;
            break;
        case DIR_NEG_Y:
            snake->segments[i].y += i;
            break;
        case DIR_POS_Z:
            snake->segments[i].z -= i;
            break;
        case DIR_NEG_Z:
            snake->segments[i].z += i;
            break;
        }
    }
}

void snake_set_direction(Snake *snake, Direction dir)
{
    if (!snake)
    {
        return;
    }

    if (dir != snake->direction && !is_opposite(dir, snake->direction))
    {
        snake->direction = dir;
    }
}

Vec3i snake_next_head_position(const Snake *snake)
{
    Vec3i next = snake->segments[0];

    switch (snake->direction)
    {
    case DIR_POS_X:
        next.x += 1;
        break;
    case DIR_NEG_X:
        next.x -= 1;
        break;
    case DIR_POS_Y:
        next.y += 1;
        break;
    case DIR_NEG_Y:
        next.y -= 1;
        break;
    case DIR_POS_Z:
        next.z += 1;
        break;
    case DIR_NEG_Z:
        next.z -= 1;
        break;
    }

    return next;
}

void snake_step(Snake *snake, Vec3i next_head)
{
    if (!snake)
    {
        return;
    }

    for (int i = snake->length - 1; i > 0; --i)
    {
        snake->segments[i] = snake->segments[i - 1];
    }

    snake->segments[0] = next_head;

    if (snake->grow && snake->length < MAX_SNAKE_LENGTH)
    {
        snake->segments[snake->length] = snake->segments[snake->length - 1];
        snake->length += 1;
        snake->grow = false;
    }
}

bool snake_intersects(const Snake *snake, Vec3i point, bool include_head)
{
    if (!snake)
    {
        return false;
    }

    int start = include_head ? 0 : 1;
    for (int i = start; i < snake->length; ++i)
    {
        if (snake->segments[i].x == point.x && snake->segments[i].y == point.y && snake->segments[i].z == point.z)
        {
            return true;
        }
    }

    return false;
}
