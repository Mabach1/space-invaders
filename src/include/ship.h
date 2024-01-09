#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "bullet.h"
#include "common.h"
#include "image.h"
#include "sdl.h"

typedef struct Ship {
    Image image;
    f32 scale;  // the image has some dimensions, we will adjust these with this constant
    f64 x_pos;
    f64 y_pos;
    BulletVec bullets;
    f64 shoot_cooldown;
    i8 lives;
    bool dead;
    f64 death_cooldown;
} Ship;

typedef enum ShipDir {
    ShipDirLeft,
    ShipDirRight,
} ShipDir;

void ship_init(Ship *ship, Image image, f64 x, f64 y);
void ship_destroy(Ship *ship);

void ship_render(Ship *ship, Context *context);

void ship_move(Ship *ship, ShipDir dir, f64 delta_time, i32 window_width);
void ship_shoot(Ship *ship);
void ship_update(Ship *ship, f64 delta_time);

void ship_shot(Ship *ship, i32 window_width);

typedef struct ShipLivesIndicator {
    Ship life_indicator[2];
} ShipLivesIndicator;

void ship_display_lives_init(ShipLivesIndicator *indicator, Ship *ship, Context *context);
void ship_display_lives_destroy(ShipLivesIndicator *indicator);
void ship_display_lives_render(ShipLivesIndicator *indicator, Ship *ship, Context *context);
