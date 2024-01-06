#include "include/enemy.h"

Enemy enemy_new(Image image, f64 x, f64 y) { return (Enemy){.image = image, .scale = 0.2f, .x_pos = x, .y_pos = y, .dead = false, .move_cooldown = 0.8, .dir = 1}; }

void enemy_delete(Enemy *enemy) { image_delete(&enemy->image); }

void enemy_render(Enemy *enemy, Context *context) {
    if (enemy->dead) {
        return;
    }

    SDL_Rect src = {
        .w = enemy->image.width,
        .h = enemy->image.height,
        .x = 0,
        .y = 0
    };

    SDL_Rect dst = {
        .w  = enemy->image.width * enemy->scale,
        .h = enemy->image.height * enemy->scale,
        .x = enemy->x_pos,
        .y = enemy->y_pos 
    };

    SDL_RenderCopyEx(context->renderer, enemy->image.texture, &src, &dst, 0.f, NULL, SDL_FLIP_NONE);
}

void enemy_update(Enemy *enemy, f64 delta_time, i32 window_width, BulletVec *bullet_vec) {
    for (usize i = 0; i < bullet_vec->len; ++i) {
        enemy->dead = enemy_shot(enemy, &bullet_vec->ptr[i]);
        if (enemy->dead) {
            bullet_vec->ptr[i].out = true;
            return;
        }
    }

    enemy->move_cooldown -= delta_time;

    if (enemy->move_cooldown > 0.f) {
        return;
    }

    enemy->move_cooldown = 0.8;

    f64 speed = enemy->dir * (enemy->image.width * enemy->scale / 2);

    enemy->x_pos += speed; 
    
    if (enemy->x_pos + enemy->image.width * enemy->scale + speed >= (f64)window_width || enemy->x_pos <= 0.f - speed) {
        enemy->dir *= -1; 
    }
}

bool enemy_shot(Enemy *enemy, Bullet *bullet) {
    return enemy->dead || (bullet->y_pos <= enemy->y_pos + enemy->image.height * enemy->scale && bullet->x_pos >= enemy->x_pos && bullet->x_pos <= enemy->x_pos + enemy->image.width * enemy->scale);
}

// void enemy_vec_init(EnemyVec *vec) { 
//     vec->cap = 16;
//     vec->len = 16;
//     vec->ptr = malloc(sizeof(Enemy) * vec->cap);
// }

// void enemy_vec_destroy(EnemyVec *vec) {}
