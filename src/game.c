#include "game.h"

#include <stdlib.h>
#include <time.h>

static void spawn_food(GameState *game)
{
    if (!game)
    {
        return;
    }

    Vec3i pos;
    int attempts = 0;
    do
    {
        pos.x = rand() % game->grid_size;
        pos.y = rand() % game->grid_size;
        pos.z = rand() % game->grid_size;
        attempts++;
    } while (snake_intersects(&game->snake, pos, true) && attempts < 1024);

    game->food = pos;
}

void game_init(GameState *game, int grid_size)
{
    if (!game)
    {
        return;
    }

    srand((unsigned int)time(NULL));

    game->grid_size = grid_size > 3 ? grid_size : 10;
    game->move_interval = 0.25f;
    game_reset(game);
}

void game_reset(GameState *game)
{
    if (!game)
    {
        return;
    }

    Vec3i start = { game->grid_size / 2, game->grid_size / 2, game->grid_size / 2 };
    snake_init(&game->snake, 4, start, DIR_POS_X);

    game->snake.grow = false;
    game->score = 0;
    game->move_timer = 0.0f;
    game->status = GAME_RUNNING;

    spawn_food(game);
}

void game_change_direction(GameState *game, Direction direction)
{
    if (!game || game->status != GAME_RUNNING)
    {
        return;
    }

    snake_set_direction(&game->snake, direction);
}

void game_toggle_pause(GameState *game)
{
    if (!game)
    {
        return;
    }

    if (game->status == GAME_RUNNING)
    {
        game->status = GAME_PAUSED;
    }
    else if (game->status == GAME_PAUSED)
    {
        game->status = GAME_RUNNING;
    }
}

void game_update(GameState *game, float dt)
{
    if (!game || game->status != GAME_RUNNING)
    {
        return;
    }

    game->move_timer += dt;

    if (game->move_timer < game->move_interval)
    {
        return;
    }

    game->move_timer = 0.0f;

    Vec3i next = snake_next_head_position(&game->snake);

    if (next.x < 0 || next.y < 0 || next.z < 0 ||
        next.x >= game->grid_size || next.y >= game->grid_size || next.z >= game->grid_size)
    {
        game->status = GAME_OVER;
        return;
    }

    if (snake_intersects(&game->snake, next, false))
    {
        game->status = GAME_OVER;
        return;
    }

    bool ate_food = (next.x == game->food.x && next.y == game->food.y && next.z == game->food.z);

    snake_step(&game->snake, next);

    if (ate_food)
    {
        game->snake.grow = true;
        game->score += 10;
        if (game->move_interval > 0.08f)
        {
            game->move_interval *= 0.97f;
        }
        spawn_food(game);
    }
}
