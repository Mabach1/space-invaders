#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "include/enemy.h"
#include "include/line.h"
#include "include/roadblock.h"
#include "include/sdl.h"
#include "include/ship.h"

int main(void) {
    srand(time(NULL));

    Context game_context = {0};

    sdl_context_init(&game_context, "Space Invaders", 1200, 1500);

    SDL_Event event = {0};

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

    u64 last = SDL_GetPerformanceCounter();

    struct Direction {
        bool pressed;
        ShipDir dir;
    };

    struct Direction dir = {.pressed = false, .dir = ShipDirRight};

    bool running = true;
    bool paused = false;
    bool end = false;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (event.type == SDL_KEYUP && event.key.keysym.sym != SDLK_SPACE) {
                dir.pressed = false;
            }

            if (event.type == SDL_KEYDOWN) {
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
                        // TODO: this will be changing scene to menu
                        end = true;
                        // paused = paused ? false : true;
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

        if (paused) {
            continue;
        }

        ship_update(&ship, delta_time);
        enemy_arr_update(&enemies, delta_time, &game_context.window, &ship.bullets, &ship);
        barricade_update(&barricade, &ship.bullets, &enemies.bullets);
        life_line_update(&life_line, &enemies.bullets);

        running = !game_over(&life_line, &enemies);

        if (dir.pressed) {
            ship_move(&ship, dir.dir, delta_time, game_context.window.width);
        }

        if (ship.lives == 0) {
            running = false;
        }

        SDL_RenderClear(game_context.renderer);
        barricade_render(&barricade, &game_context);

        ship_display_lives_render(&indicator, &ship, &game_context);
        ship_render(&ship, &game_context);

        enemy_arr_render(&enemies, &game_context);

        life_line_render(&life_line, &game_context);

        SDL_SetRenderDrawColor(game_context.renderer, 0, 0, 0, 255);

        SDL_RenderPresent(game_context.renderer);
    }

    life_line_destroy(&life_line);
    ship_display_lives_destroy(&indicator);
    barricade_destroy(&barricade);
    enemy_arr_destroy(&enemies);
    ship_destroy(&ship);
    sdl_context_destroy(&game_context);

    return 0;
}
