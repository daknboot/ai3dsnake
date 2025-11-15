#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <math.h>
#include <stdio.h>

#include "game.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static void set_perspective(int width, int height)
{
    if (height == 0)
    {
        height = 1;
    }

    const float aspect = (float)width / (float)height;
    const float fov = 60.0f;
    const float near_plane = 0.1f;
    const float far_plane = 500.0f;
    const float top = near_plane * tanf(fov * 0.5f * (float)M_PI / 180.0f);
    const float bottom = -top;
    const float right = top * aspect;
    const float left = -right;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(left, right, bottom, top, near_plane, far_plane);
}

static void draw_cube(float size)
{
    const float half = size * 0.5f;
    glBegin(GL_QUADS);

    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-half, -half, half);
    glVertex3f(half, -half, half);
    glVertex3f(half, half, half);
    glVertex3f(-half, half, half);

    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-half, half, -half);
    glVertex3f(half, half, -half);
    glVertex3f(half, -half, -half);
    glVertex3f(-half, -half, -half);

    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-half, half, half);
    glVertex3f(half, half, half);
    glVertex3f(half, half, -half);
    glVertex3f(-half, half, -half);

    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-half, -half, -half);
    glVertex3f(half, -half, -half);
    glVertex3f(half, -half, half);
    glVertex3f(-half, -half, half);

    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(half, -half, half);
    glVertex3f(half, -half, -half);
    glVertex3f(half, half, -half);
    glVertex3f(half, half, half);

    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-half, -half, -half);
    glVertex3f(-half, -half, half);
    glVertex3f(-half, half, half);
    glVertex3f(-half, half, -half);

    glEnd();
}

static void render_grid(const GameState *game)
{
    glColor3f(0.25f, 0.25f, 0.25f);
    glBegin(GL_LINES);

    const float max = (float)game->grid_size;

    for (int i = 0; i <= game->grid_size; ++i)
    {
        float coord = (float)i;

        glVertex3f(0.0f, coord, 0.0f);
        glVertex3f(max, coord, 0.0f);

        glVertex3f(coord, 0.0f, 0.0f);
        glVertex3f(coord, max, 0.0f);

        glVertex3f(0.0f, 0.0f, coord);
        glVertex3f(max, 0.0f, coord);

        glVertex3f(coord, 0.0f, 0.0f);
        glVertex3f(coord, 0.0f, max);

        glVertex3f(0.0f, coord, 0.0f);
        glVertex3f(0.0f, coord, max);

        glVertex3f(0.0f, 0.0f, coord);
        glVertex3f(0.0f, max, coord);

        glVertex3f(max, coord, 0.0f);
        glVertex3f(max, coord, max);

        glVertex3f(coord, max, 0.0f);
        glVertex3f(coord, max, max);

        glVertex3f(0.0f, max, coord);
        glVertex3f(max, max, coord);
    }

    glEnd();
}

static void render_snake(const Snake *snake)
{
    glColor3f(0.1f, 0.8f, 0.1f);
    for (int i = 0; i < snake->length; ++i)
    {
        glPushMatrix();
        glTranslatef((float)snake->segments[i].x + 0.5f,
                     (float)snake->segments[i].y + 0.5f,
                     (float)snake->segments[i].z + 0.5f);
        draw_cube(0.9f);
        glPopMatrix();
    }
}

static void render_food(const Vec3i *food)
{
    glColor3f(0.9f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef((float)food->x + 0.5f, (float)food->y + 0.5f, (float)food->z + 0.5f);
    draw_cube(0.7f);
    glPopMatrix();
}

static void render_scene(const GameState *game, int width, int height, float yaw, float pitch, float distance)
{
    set_perspective(width, height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -distance);
    glRotatef(pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(yaw, 0.0f, 1.0f, 0.0f);

    const float center = (float)game->grid_size / 2.0f;
    glTranslatef(-center, -center, -center);

    render_grid(game);
    if (game->food_available)
    {
        render_food(&game->food);
    }
    render_snake(&game->snake);
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window *window = SDL_CreateWindow(
        "3D Snake",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1024,
        768,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context)
    {
        fprintf(stderr, "SDL_GL_CreateContext failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_GL_SetSwapInterval(1);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05f, 0.05f, 0.08f, 1.0f);

    GameState game;
    game_init(&game, 12);

    float camera_yaw = 45.0f;
    float camera_pitch = -35.0f;
    float camera_distance = game.grid_size * 2.5f;

    bool running = true;
    Uint64 last_counter = SDL_GetPerformanceCounter();
    const Uint64 freq = SDL_GetPerformanceFrequency();

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    running = false;
                    break;
                case SDLK_UP:
                    game_change_direction(&game, DIR_POS_Y);
                    break;
                case SDLK_DOWN:
                    game_change_direction(&game, DIR_NEG_Y);
                    break;
                case SDLK_LEFT:
                    game_change_direction(&game, DIR_NEG_X);
                    break;
                case SDLK_RIGHT:
                    game_change_direction(&game, DIR_POS_X);
                    break;
                case SDLK_PAGEUP:
                    game_change_direction(&game, DIR_POS_Z);
                    break;
                case SDLK_PAGEDOWN:
                    game_change_direction(&game, DIR_NEG_Z);
                    break;
                case SDLK_SPACE:
                    game_toggle_pause(&game);
                    break;
                case SDLK_r:
                    game_reset(&game);
                    break;
                case SDLK_RETURN:
                    if (game.status == GAME_OVER)
                    {
                        game_reset(&game);
                    }
                    break;
                case SDLK_w:
                    camera_pitch -= 5.0f;
                    break;
                case SDLK_s:
                    camera_pitch += 5.0f;
                    break;
                case SDLK_a:
                    camera_yaw -= 5.0f;
                    break;
                case SDLK_d:
                    camera_yaw += 5.0f;
                    break;
                case SDLK_q:
                    camera_distance -= 0.5f;
                    break;
                case SDLK_e:
                    camera_distance += 0.5f;
                    break;
                default:
                    break;
                }
            }
        }

        if (camera_pitch < -85.0f)
        {
            camera_pitch = -85.0f;
        }
        if (camera_pitch > -5.0f)
        {
            camera_pitch = -5.0f;
        }
        if (camera_distance < 5.0f)
        {
            camera_distance = 5.0f;
        }

        Uint64 current_counter = SDL_GetPerformanceCounter();
        float dt = (float)((double)(current_counter - last_counter) / (double)freq);
        last_counter = current_counter;

        game_update(&game, dt);

        char title[128];
        snprintf(title, sizeof(title), "3D Snake - Score: %d%s",
                 game.score,
                 game.status == GAME_OVER ? " (Game Over)" : (game.status == GAME_PAUSED ? " (Paused)" : ""));
        SDL_SetWindowTitle(window, title);

        int width, height;
        SDL_GL_GetDrawableSize(window, &width, &height);
        render_scene(&game, width, height, camera_yaw, camera_pitch, camera_distance);

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
