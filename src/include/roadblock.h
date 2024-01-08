#pragma once

#include <SDL2/SDL.h>

#include "common.h"
#include "sdl.h"
#include "bullet.h"
#include "enemy_bullet.h"

typedef struct Block {
    f32 x_pos;
    f32 y_pos;
    i32 dimension;
    SDL_Color color;
} Block;

Block block_new(f32 x, f32 y, i32 dimension, SDL_Color color);

typedef struct {
    u32 *matrix;
    usize rows;
    usize cols;
    f32 x_pos;
    f32 y_pos;
    Block *texture;
} Roadblock;

void roadblock_init(Roadblock *roadblock, f32 x, f32 y);
void roadblock_destroy(Roadblock *roadblock);

void roadblock_render(Roadblock *roadblock, Context *context);
void roadblock_update(Roadblock *roadblock, BulletVec *bullets, EnemyBulletVec *enemy_bullets);

typedef struct {
    Roadblock *ptr;
    usize number_of;
} Barricade;

void barricade_init(Barricade *barricade, Window *window);
void barricade_destroy(Barricade *barricade);

void barricade_update(Barricade *barricade, BulletVec *bullets, EnemyBulletVec *enemy_bullets);
void barricade_render(Barricade *barricade, Context *context);
