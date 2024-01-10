#pragma once

#include <time.h>

#include "bullet.h"
#include "enemy_bullet.h"
#include "image.h"
#include "sdl.h"

typedef enum AnimationState {
    STATE_0 = 0,
    STATE_1 = 1,
    STATE_2 = 2,
} AnimationState;

typedef struct EnemyAnimation {
    Image fames[3];
    AnimationState state;
    bool death_animation_played;
    bool move_down_animation;
} EnemyAnimation;

typedef struct Enemy {
    EnemyAnimation animation;
    f32 scale;
    f64 x_pos;
    f64 y_pos;
    bool dead;
    f64 move_cooldown;
    f64 current_move_cooldown;
    i32 dir;
} Enemy;

Enemy enemy_new(f64 x, f64 y, Context *context, Image *animation_frames);
void enemy_delete(Enemy *enemy);

void enemy_render(Enemy *enemy, Context *context);
void enemy_update(Enemy *enemy, f64 delta_time, i32 window_width, BulletVec *vec, u32 *death_count);

bool enemy_shot(Enemy *enemy, Bullet *bullet);

// enemies will be saved in row major order
typedef struct EnemyArr {
    Enemy *ptr;
    usize rows;
    usize cols;
    i32 dir;
    u32 number_of_deaths;
    EnemyBulletVec bullets;
} EnemyArr;

void enemy_arr_init(EnemyArr *arr, usize cols, usize rows, Context *context);
void enemy_arr_destroy(EnemyArr *arr);

void enemy_arr_render(EnemyArr *arr, Context *context);
void enemy_arr_update(EnemyArr *arr, f64 delta_time, Window *window, BulletVec *bullet_vec, Ship *ship);

void enemy_bullet_shoot(EnemyBulletVec *bullet_vec, EnemyArr *enemies);
