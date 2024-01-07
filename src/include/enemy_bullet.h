#pragma once

#include <SDL2/SDL.h>

#include "common.h"
#include "sdl.h"

typedef struct {
    f32 x_pos;
    f32 y_pos;
    i32 width;
    i32 height;
    SDL_Color color;
    bool out;   // either it flew out the window or it hit
} EnemyBullet;

EnemyBullet enemy_bullet_new(i32 x, i32 y, i32 width, i32 height, SDL_Color color);
void enemy_bullet_render(EnemyBullet *bullet, Context *context);

typedef struct {
    EnemyBullet *ptr;
    usize len;
    usize cap;
    f64 cooldown; 
} EnemyBulletVec;

void enemy_bullet_vec_init(EnemyBulletVec *vec);
void enemy_bullet_vec_destroy(EnemyBulletVec *vec);

void enemy_bullet_vec_push(EnemyBulletVec *vec, EnemyBullet bullet);
void enemy_bullet_vec_update(EnemyBulletVec *vec, f64 delta_time);
void enemy_bullet_vec_render(EnemyBulletVec *vec, Context *context);
