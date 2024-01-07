#pragma once

#include <time.h>

#include "bullet.h"
#include "image.h"
#include "sdl.h"
#include "enemy_bullet.h"

typedef struct Enemy {
    Image image;  // this will probably be union of different enemies
    f32 scale;
    f64 x_pos;
    f64 y_pos;
    bool dead;
    f64 move_cooldown;
    i32 dir;
} Enemy;

Enemy enemy_new(Image image, f64 x, f64 y);
void enemy_delete(Enemy *enemy);

void enemy_render(Enemy *enemy, Context *context);
void enemy_update(Enemy *enemy, f64 delta_time, i32 window_width, BulletVec *vec);

bool enemy_shot(Enemy *enemy, Bullet *bullet);

// enemies will be saved in row major order
typedef struct EnemyArr {
    Enemy *ptr;
    usize rows;
    usize cols;
    i32 dir;
    EnemyBulletVec bullets;
} EnemyArr;

void enemy_arr_init(EnemyArr *arr, usize cols, usize rows, Context *context);
void enemy_arr_destroy(EnemyArr *arr);

void enemy_arr_render(EnemyArr *arr, Context *context);
void enemy_arr_update(EnemyArr *arr, f64 delta_time, i32 window_width, BulletVec *bullet_vec);

void enemy_bullet_shoot(EnemyBulletVec *bullet_vec, EnemyArr *enemies);
