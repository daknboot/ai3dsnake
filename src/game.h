#ifndef GAME_H
#define GAME_H

#include "snake.h"
#include <stdbool.h>

typedef enum
{
    GAME_RUNNING,
    GAME_PAUSED,
    GAME_OVER
} GameStatus;

typedef struct
{
    Snake snake;
    Vec3i food;
    int grid_size;
    int score;
    float move_timer;
    float move_interval;
    GameStatus status;
} GameState;

void game_init(GameState *game, int grid_size);
void game_reset(GameState *game);
void game_update(GameState *game, float dt);
void game_change_direction(GameState *game, Direction direction);
void game_toggle_pause(GameState *game);

#endif /* GAME_H */
