#include "include/bullet.h"

Bullet bullet_new(i32 x, i32 y, i32 width, i32 height, SDL_Color color) { return (Bullet){.x_pos = x, .y_pos = y, .width = width, .height = height, .color = color}; }

void bullet_render(Bullet *bullet, Context *context) {
    if (bullet->out) {
        return;
    }

    SDL_Rect rect = {.w = bullet->width, .h = bullet->height, .x = bullet->x_pos, .y = bullet->y_pos};

    SDL_SetRenderDrawColor(context->renderer, bullet->color.r, bullet->color.g, bullet->color.b, 255);
    SDL_RenderFillRect(context->renderer, &rect);
}

void bullet_vec_init(BulletVec *vec) {
    vec->cap = 4;
    vec->len = 0;

    vec->ptr = malloc(sizeof(Bullet) * vec->cap);
    assert(vec->ptr);
}

void bullet_vec_destroy(BulletVec *vec) { free(vec->ptr); }

void bullet_vec_push(BulletVec *vec, Bullet bullet) {
    if (vec->len == vec->cap) {
        vec->cap *= 2;
        vec->ptr = realloc(vec->ptr, sizeof(Bullet) * vec->cap);
    }
    vec->ptr[vec->len++] = bullet;
}

static void delete_at_index(BulletVec *vec, usize index) {
   if (index >= vec->len) {
        return;
    }

    usize bytes_to_move = (vec->len - index - 1) * sizeof(Bullet);
    memmove(&vec->ptr[index], &vec->ptr[index + 1], bytes_to_move);

    vec->len--;
}

void bullet_vec_update(BulletVec *vec, f64 delta_time) {
    for (usize i = 0; i < vec->len; ++i) {
        if (vec->ptr[i].y_pos <= 0.f || vec->ptr[i].out) {
            delete_at_index(vec, i);
            continue;
        }
        vec->ptr[i].y_pos = vec->ptr[i].y_pos - 1000.f * delta_time;
    }
}
