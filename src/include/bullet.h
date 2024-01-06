#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "common.h"
#include "sdl.h"

typedef struct Bullet {
    f32 x_pos;
    f32 y_pos;
    i32 width;
    i32 height;
    SDL_Color color;
    bool out;  // has meaning when used in vector
} Bullet;

Bullet bullet_new(i32 x, i32 y, i32 width, i32 height, SDL_Color color);
void bullet_render(Bullet *bullet, Context *context);

typedef struct {
    Bullet *ptr;
    usize len;
    usize cap;
} BulletVec;

void bullet_vec_init(BulletVec *vec);
void bullet_vec_destroy(BulletVec *vec);

void bullet_vec_push(BulletVec *vec, Bullet bullet);
void bullet_vec_update(BulletVec *vec, f64 delta_time);
