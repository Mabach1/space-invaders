#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "include/background.h"
#include "include/enemy.h"
#include "include/line.h"
#include "include/roadblock.h"
#include "include/sdl.h"
#include "include/pause.h"
#include "include/ship.h"

typedef enum Scene {
    SceneGame,
    ScenePause,
} Scene;

int main(void) {
    srand(time(NULL));

    Context game_context = {0};

    sdl_context_init(&game_context, "Space Invaders", 1200, 1500);

    SDL_Event event = {0};

    Background background = {0};
    background_init(&background, &game_context);

    Ship ship = {0};
    ship_init(&ship, image_new("assets/ship.png", game_context.renderer), game_context.window.width / 2, (game_context.window.height / 12) * 10);

    ShipLivesIndicator indicator = {0};
    ship_display_lives_init(&indicator, &ship, &game_context);

    EnemyArr enemies = {0};
    enemy_arr_init(&enemies, 11, 5, &game_context);

    Barricade barricade = {0};
    barricade_init(&barricade, &game_context);

    LifeLine life_line = {0};
    life_line_init(&life_line, &game_context.window);

    TextBox text_box = {0};
    text_box_init(&text_box, (SDL_Color){.r = 0xFF, .g = 0xFF, .b = 0xFF, .a = 0xFF});

    u64 last = SDL_GetPerformanceCounter();

    struct Direction {
        bool pressed;
        ShipDir dir;
    };

    struct Direction dir = {.pressed = false, .dir = ShipDirRight};

    bool running = true;
    bool paused = false;
    bool end = false;
    bool control_pressed = false;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                end = true;
            }

            if (event.type == SDL_KEYUP && event.key.keysym.sym != SDLK_SPACE) {
                dir.pressed = false;
            }

            if (event.type == SDL_KEYUP && event.key.keysym.sym != SDLK_LCTRL) {
                control_pressed = false;
            }

            if (event.type == SDL_KEYDOWN) {
                if (paused) {
                    paused = false;
                }

                if (event.key.keysym.sym == SDLK_LCTRL ) {
                    control_pressed = true;
                }

                if (control_pressed && event.key.keysym.sym == SDLK_w) {
                    end = true;
                }

                switch (event.key.keysym.sym) {
                    case SDLK_LEFT: {
                        dir.pressed = true;
                        dir.dir = ShipDirLeft;
                        break;
                    }
                    case SDLK_RIGHT: {
                        dir.pressed = true;
                        dir.dir = ShipDirRight;
                        break;
                    }
                    case SDLK_SPACE: {
                        ship_shoot(&ship);
                        break;
                    }
                    case SDLK_ESCAPE: {
                        paused = paused ? false : true;
                        break;
                    }
                }
            }
        }

        if (end) {
            break;
        }

        u64 now = SDL_GetPerformanceCounter();
        f64 delta_time = (f64)((now - last) / (f64)SDL_GetPerformanceFrequency());
        last = now;

        if (!paused) {
            enemy_arr_update(&enemies, delta_time, &game_context, &ship.bullets, &ship);
            background_update(&background, delta_time);
            ship_update(&ship, delta_time);
            barricade_update(&barricade, &ship.bullets, &enemies.bullets);
            life_line_update(&life_line, &enemies.bullets);
        }

        SDL_RenderClear(game_context.renderer);

        running = !game_over(&life_line, &enemies);

        if (dir.pressed) {
            ship_move(&ship, dir.dir, delta_time, game_context.window.width);
        }

        if (ship.lives == 0) {
            running = false;
        }

        background_render(&background, &game_context);
        ship_display_score(&ship, &text_box, &game_context);

        barricade_render(&barricade, &game_context);

        ship_display_lives_render(&indicator, &ship, &game_context);
        ship_render(&ship, &game_context);

        enemy_arr_render(&enemies, &game_context);

        life_line_render(&life_line, &game_context);

        SDL_SetRenderDrawColor(game_context.renderer, 17, 19, 23, 255);

        if (paused) 
            scene_pause(game_context.renderer); 

        SDL_RenderPresent(game_context.renderer);
    }

    text_box_destroy(&text_box);
    background_destroy(&background);
    life_line_destroy(&life_line);
    ship_display_lives_destroy(&indicator);
    barricade_destroy(&barricade);
    enemy_arr_destroy(&enemies);
    ship_destroy(&ship);
    sdl_context_destroy(&game_context);

    return 0;
}
