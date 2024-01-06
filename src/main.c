#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "include/sdl.h"
#include "include/ship.h"
#include "include/enemy.h"

int main(void) {
    Context game_context = {0};

    sdl_context_init(&game_context, "Space Invaders", 1800, 1350);

    bool running = true;

    SDL_Event event = {0};

    Ship ship = {0};
    ship_init(&ship, image_new("assets/ship.png", game_context.renderer), game_context.window.width / 2, (game_context.window.heigh / 12) * 10);

    Enemy enemy = enemy_new(image_new("assets/enemy.png", game_context.renderer), 100, 100);

    u64 last = SDL_GetPerformanceCounter();

    struct Direction {
        bool pressed;
        ShipDir dir;
    };

    struct Direction dir = {.pressed = false, .dir = ShipDirRight};

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
                    }
                }
            }
        }

        u64 now = SDL_GetPerformanceCounter();
        f64 delta_time = (f64)((now - last) / (f64)SDL_GetPerformanceFrequency());
        last = now;

        ship_update(&ship, delta_time);
        enemy_update(&enemy, delta_time, game_context.window.width, &ship.bullets);

        SDL_RenderClear(game_context.renderer);

        ship_render(&ship, &game_context);
        enemy_render(&enemy, &game_context);

        SDL_SetRenderDrawColor(game_context.renderer, 0, 0, 0, 255);

        if (dir.pressed) {
            ship_move(&ship, dir.dir, delta_time, game_context.window.width);
        }

        SDL_RenderPresent(game_context.renderer);
    }

    enemy_delete(&enemy);
    ship_destroy(&ship);
    sdl_context_destroy(&game_context);

    return 0;
}
