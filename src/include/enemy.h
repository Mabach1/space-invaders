#pragma once

#include "image.h"
#include "sdl.h"
#include "bullet.h"

typedef struct Enemy {
    Image image;
    f32 scale;
    f64 x_pos;
    f64 y_pos;
    bool dead;
} Enemy;

Enemy enemy_new(Image image, f64 x, f64 y);
void enemy_delete(Enemy *enemy);

void enemy_render(Enemy *enemy, Context *context);
void enemy_update(Enemy *enemy, f64 delta_time, i32 window_width, BulletVec *vec);

bool enemy_shot(Enemy *enemy, Bullet *bullet);

// typedef struct EnemyVec {
//     Enemy *ptr;
//     usize len;
//     usize cap;
// } EnemyVec;

// void enemy_vec_init(EnemyVec *vec);
// void enemy_vec_destroy(EnemyVec *vec);
