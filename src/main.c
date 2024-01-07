#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "include/enemy.h"
#include "include/sdl.h"
#include "include/ship.h"

int main(void) {
    srand(time(NULL));

    Context game_context = {0};

    sdl_context_init(&game_context, "Space Invaders", 1800, 1350);


    SDL_Event event = {0};

    Ship ship = {0};
    ship_init(&ship, image_new("assets/ship.png", game_context.renderer), game_context.window.width / 2, (game_context.window.height / 12) * 10);

    EnemyArr enemies = {0};
    enemy_arr_init(&enemies, 8, 4, &game_context);

    u64 last = SDL_GetPerformanceCounter();

    struct Direction {
        bool pressed;
        ShipDir dir;
    };

    struct Direction dir = {.pressed = false, .dir = ShipDirRight};

    bool running = true;
    bool paused = false;

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
                        // this will be changing scene to menu
                        running = false;
                        // paused = paused ? false : true;
                        break;
                    }
                }
            }
        }

        u64 now = SDL_GetPerformanceCounter();
        f64 delta_time = (f64)((now - last) / (f64)SDL_GetPerformanceFrequency());
        last = now;

        if (paused) {
            continue;
        }

        ship_update(&ship, delta_time);
        enemy_arr_update(&enemies, delta_time, &game_context.window, &ship.bullets, &ship);

        if (ship.lives == 0) {
            running = false;
        }

        SDL_RenderClear(game_context.renderer);

        ship_display_lives(&ship, &game_context);
        ship_render(&ship, &game_context);

        enemy_arr_render(&enemies, &game_context);

        SDL_SetRenderDrawColor(game_context.renderer, 0, 0, 0, 255);

        if (dir.pressed) {
            ship_move(&ship, dir.dir, delta_time, game_context.window.width);
        }

        SDL_RenderPresent(game_context.renderer);
    }

    enemy_arr_destroy(&enemies);
    ship_destroy(&ship);
    sdl_context_destroy(&game_context);

    return 0;
}
