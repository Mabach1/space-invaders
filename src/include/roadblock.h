#pragma once

#include <SDL2/SDL.h>

#include "bullet.h"
#include "common.h"
#include "enemy_bullet.h"
#include "sdl.h"

#if PIXEL_ROADBLOCKS
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
#else

typedef enum BlockState {
    BlockState1 = 0,  // brand new
    BlockState2 = 1,  // here's where it starts to crumble
    BlockState3 = 2,
    BlockState4 = 3,
    BlockState5 = 4,
    BlockState6 = 5,  // here is nothing
} BlockState;

typedef struct Block {
    Image *images;
    usize number_of;
    BlockState state;
    f32 x_pos;
    f32 y_pos;
    f32 scale;
} Block;

Block block_new(const char *prefix_path, SDL_Renderer *renderer, f32 x, f32 y);
void block_delete(Block *block);

typedef struct Roadblock {
    Block blocks[12];
    usize cols;
    usize rows;
    f32 x_pos;
    f32 y_pos;
} Roadblock;

void roadblock_init(Roadblock *roadblock, f32 x, f32 y, Context *context);
void roadblock_destroy(Roadblock *roadblock);

void roadblock_update(Roadblock *roadblock, BulletVec *bullets, EnemyBulletVec *enemy_bullets);
void roadblock_render(Roadblock *roadblock, Context *context);

typedef struct {
    Roadblock *ptr;
    usize number_of;
} Barricade;

void barricade_init(Barricade *barricade, Context *context);
void barricade_destroy(Barricade *barricade);

void barricade_update(Barricade *barricade, BulletVec *bullets, EnemyBulletVec *enemy_bullets);
void barricade_render(Barricade *barricade, Context *context);

#endif
