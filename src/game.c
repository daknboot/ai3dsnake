#include "game.h"

#include <stdlib.h>
#include <time.h>

static bool is_cell_free(const GameState *game, Vec3i pos)
{
    return !snake_intersects(&game->snake, pos, true);
}

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
        if (is_cell_free(game, pos))
        {
            game->food = pos;
            game->food_available = true;
            return;
        }
    } while (attempts < 1024);

    for (int x = 0; x < game->grid_size; ++x)
    {
        for (int y = 0; y < game->grid_size; ++y)
        {
            for (int z = 0; z < game->grid_size; ++z)
            {
                Vec3i candidate = { x, y, z };
                if (is_cell_free(game, candidate))
                {
                    game->food = candidate;
                    game->food_available = true;
                    return;
                }
            }
        }
    }

    game->food_available = false;
    game->status = GAME_OVER;
}

void game_init(GameState *game, int grid_size)
{
    if (!game)
    {
        return;
    }

    srand((unsigned int)time(NULL));

    game->grid_size = grid_size > 3 ? grid_size : 10;
    game->base_move_interval = 0.25f;
    game->move_interval = game->base_move_interval;
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
    game->move_interval = game->base_move_interval;
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

    bool ate_food = (game->food_available &&
                     next.x == game->food.x &&
                     next.y == game->food.y &&
                     next.z == game->food.z);

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
        if (!game->food_available)
        {
            return;
        }
    }
}
