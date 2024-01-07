#include "include/enemy_bullet.h"

EnemyBullet enemy_bullet_new(i32 x, i32 y, i32 width, i32 height, SDL_Color color) { return (EnemyBullet){.x_pos = x, .y_pos = y, .width = width, .height = height, .color = color}; }

void enemy_bullet_render(EnemyBullet *bullet, Context *context) {
    if (bullet->out) {
        return;
    }

    SDL_Rect rect = {.w = bullet->width, .h = bullet->height, .x = bullet->x_pos, .y = bullet->y_pos};

    SDL_SetRenderDrawColor(context->renderer, bullet->color.r, bullet->color.g, bullet->color.b, 255);
    SDL_RenderFillRect(context->renderer, &rect);
}

void enemy_bullet_vec_init(EnemyBulletVec *vec) {
    vec->cap = 4;
    vec->len = 0;

    vec->ptr = malloc(sizeof(EnemyBullet) * vec->cap);
    assert(vec->ptr);

    vec->cooldown = 1.5f;
}

void enemy_bullet_vec_destroy(EnemyBulletVec *vec) { free(vec->ptr); }

void enemy_bullet_vec_push(EnemyBulletVec *vec, EnemyBullet bullet) {
    if (vec->len == vec->cap) {
        vec->cap *= 2;
        vec->ptr = realloc(vec->ptr, sizeof(EnemyBullet) * vec->cap);
    }
    vec->ptr[vec->len++] = bullet;
}

static void delete_at_index(EnemyBulletVec *vec, usize index) {
    if (index >= vec->len) {
        return;
    }

    usize bytes_to_move = (vec->len - index - 1) * sizeof(EnemyBullet);
    memmove(&vec->ptr[index], &vec->ptr[index + 1], bytes_to_move);

    vec->len--;
}

void enemy_bullet_vec_update(EnemyBulletVec *vec, f64 delta_time) {
    vec->cooldown -= delta_time;

    for (usize i = 0; i < vec->len; ++i) {
        if (vec->ptr[i].y_pos <= 0.f) {
            delete_at_index(vec, i);
            continue;
        }
        vec->ptr[i].y_pos = vec->ptr[i].y_pos + 1000.f * delta_time;
    }
}

void enemy_bullet_vec_render(EnemyBulletVec *vec, Context *context) {
    for (usize i = 0; i < vec->len; ++i) {
        enemy_bullet_render(&vec->ptr[i], context);
    }
}
