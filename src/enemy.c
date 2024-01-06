#include "include/enemy.h"

Enemy enemy_new(Image image, f64 x, f64 y) { return (Enemy){.image = image, .scale = 0.2f, .x_pos = x, .y_pos = y, .dead = false}; }

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
            printf("dead!\n");
            return;
        }
    }

    enemy->x_pos += 120.f * delta_time;
    
    if (enemy->x_pos + enemy->image.width * enemy->scale >= (f64)window_width) {
        enemy->x_pos = window_width; 
    }

    if (enemy->x_pos < 0) {
        enemy->x_pos = 0; 
    }
}

bool enemy_shot(Enemy *enemy, Bullet *bullet) {
    if (enemy->dead) {
        return true;
    }
#ifdef DEBUG
    system("clear");
    printf("enemy: %f %f %f %f\n", enemy->image.height * enemy->scale, enemy->image.width * enemy->scale, enemy->x_pos, enemy->y_pos);
    printf("bullet: %d %d %f %f\n", bullet->height, bullet->width, bullet->x_pos, bullet->y_pos);
#endif

    return bullet->y_pos <= enemy->y_pos + enemy->image.height * enemy->scale && bullet->x_pos >= enemy->x_pos && bullet->x_pos <= enemy->x_pos + enemy->image.width * enemy->scale;
}

// void enemy_vec_init(EnemyVec *vec) { 
//     vec->cap = 16;
//     vec->len = 16;
//     vec->ptr = malloc(sizeof(Enemy) * vec->cap);
// }

// void enemy_vec_destroy(EnemyVec *vec) {}
