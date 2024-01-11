#pragma once

#include <SDL2/SDL.h>

#include "common.h"
#include "enemy_bullet.h"
#include "enemy.h"
#include "sdl.h"

typedef struct Hit {
    f32 x_pos;
} Hit;

typedef struct HitArea {
    f32 y_pos;
    Hit *ptr;
    usize len;
    usize cap;
} HitArea;

typedef struct LifeLine {
    f32 x_pos;
    f32 y_pos;
    i32 width;
    i32 height;
    SDL_Color color;
    HitArea hit_area;
} LifeLine;

void life_line_init(LifeLine *line, Window *window);
void life_line_destroy(LifeLine *line);

void life_line_render(LifeLine *line, Context *context);
void life_line_update(LifeLine *line, EnemyBulletVec *enemy_bullets);

bool game_over(LifeLine *line, EnemyArr *enemies);
