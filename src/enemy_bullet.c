#include "include/enemy_bullet.h"

EnemyBullet enemy_bullet_new(i32 x, i32 y, i32 width, i32 height, SDL_Color color) { return (EnemyBullet){.x_pos = x, .y_pos = y, .width = width, .height = height, .color = color}; }

#ifdef DEBUG
static void bullet_debug(EnemyBullet *bullet) { printf("x: %f y: %f w: %d h: %d out: %s, ", bullet->x_pos, bullet->y_pos, bullet->width, bullet->height, bullet->out ? "true" : "false"); }

static void bullet_vec_debug(EnemyBulletVec *vec) {
    printf("len: %lu cap: %lu\n", vec->len, vec->cap);

    printf("[ ");

    for (usize i = 0; i < vec->len; ++i) {
        bullet_debug(&vec->ptr[i]);
    }

    printf(" ]\n");
}
#endif

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

bool enemy_bullet_hit(EnemyBullet *bullet, Ship *ship) {
    if (bullet->out) {
        return false;
    }
    return ((bullet->y_pos + bullet->height > ship->y_pos) && (bullet->y_pos <= ship->y_pos + ship->image.height * ship->scale) && (bullet->x_pos > ship->x_pos) &&
            (bullet->x_pos < ship->x_pos + ship->image.width * ship->scale));
}

void enemy_bullet_vec_update(EnemyBulletVec *vec, f64 delta_time, Ship *ship, Window *window) {
    vec->cooldown -= delta_time;

    for (usize i = 0; i < vec->len; ++i) {
        if (vec->ptr[i].y_pos + vec->ptr[i].height >= (f32)window->height) {
            vec->ptr[i].out = true;
        }
    }

    for (usize i = 0; i < vec->len; ++i) {
        if (vec->ptr[i].out) {
            delete_at_index(vec, i);
        }
    }

    for (usize i = 0; i < vec->len; ++i) {
        if (vec->ptr[i].out) {
            continue;
        }

        if (enemy_bullet_hit(&vec->ptr[i], ship)) {
            ship_shot(ship, window->width);
            vec->ptr[i].out = true;
        }

        vec->ptr[i].y_pos = vec->ptr[i].y_pos + 1000.f * delta_time;
    }
}

void enemy_bullet_vec_render(EnemyBulletVec *vec, Context *context) {
#ifdef DEBUG
    bullet_vec_debug(vec);
#endif
    for (usize i = 0; i < vec->len; ++i) {
        enemy_bullet_render(&vec->ptr[i], context);
    }
}
