#include "include/enemy.h"

Enemy enemy_new(Image image, f64 x, f64 y) { return (Enemy){.image = image, .scale = 0.15, .x_pos = x, .y_pos = y, .dead = false, .move_cooldown = 0.8, .dir = 1}; }

void enemy_delete(Enemy *enemy) { image_delete(&enemy->image); }

void enemy_render(Enemy *enemy, Context *context) {
    if (enemy->dead) {
        return;
    }

    SDL_Rect src = {.w = enemy->image.width, .h = enemy->image.height, .x = 0, .y = 0};

    SDL_Rect dst = {.w = enemy->image.width * enemy->scale, .h = enemy->image.height * enemy->scale, .x = enemy->x_pos, .y = enemy->y_pos};

    SDL_RenderCopyEx(context->renderer, enemy->image.texture, &src, &dst, 0.f, NULL, SDL_FLIP_NONE);
}

void enemy_update(Enemy *enemy, f64 delta_time, i32 window_width, BulletVec *bullet_vec) {
    for (usize i = 0; i < bullet_vec->len; ++i) {
        if (enemy->dead) {
            break;
        }

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
    if (bullet->out) {
        return false;
    }

    return enemy->dead || (bullet->y_pos <= enemy->y_pos + enemy->image.height * enemy->scale && bullet->x_pos >= enemy->x_pos && bullet->x_pos <= enemy->x_pos + enemy->image.width * enemy->scale);
}

static usize coords(usize row, usize col, usize cols) { return row * cols + col; }

void enemy_arr_init(EnemyArr *arr, usize cols, usize rows, Context *context) {
    enemy_bullet_vec_init(&arr->bullets);

    arr->dir = 1;

    arr->cols = cols;
    arr->rows = rows;

    arr->ptr = malloc(sizeof(Enemy) * rows * cols);
    assert(arr->ptr);

    Image image = image_new("assets/enemy.png", context->renderer);

    for (usize i = 0; i < rows; ++i) {
        for (usize j = 0; j < cols; ++j) {
            usize index = coords(i, j, cols);

            arr->ptr[index] = enemy_new(image, 0, 0);
            arr->ptr[index].y_pos = 1.2f * (context->window.heigh / 13 + (arr->ptr[index].image.height * arr->ptr[index].scale * i));
            arr->ptr[index].x_pos = 1.1f * j * (arr->ptr[index].image.width * arr->ptr[index].scale) + 50.f;
        }
    }
}

void enemy_arr_destroy(EnemyArr *arr) {
    for (usize i = 0; i < arr->cols * arr->rows; ++i) {
        enemy_delete(&arr->ptr[i]);
    }
    free(arr->ptr);
    enemy_bullet_vec_destroy(&arr->bullets);
}

void enemy_arr_render(EnemyArr *arr, Context *context) {
    enemy_bullet_vec_render(&arr->bullets, context);

    for (usize i = 0; i < arr->cols * arr->rows; ++i) {
        enemy_render(&arr->ptr[i], context);
    }
}

void enemy_arr_update(EnemyArr *arr, f64 delta_time, i32 window_width, BulletVec *bullet_vec) {
    enemy_bullet_shoot(&arr->bullets, arr);
    enemy_bullet_vec_update(&arr->bullets, delta_time);

    for (usize i = 0; i < arr->rows * arr->cols; ++i) {
        enemy_update(&arr->ptr[i], delta_time, window_width, bullet_vec);
    }

    bool dir_changed = false;

    for (usize i = 0; i < arr->rows * arr->cols; ++i) {
        if (arr->ptr[i].dead) {
            continue;
        }

        if (arr->ptr[i].dir != arr->dir) {
            dir_changed = true;
            arr->dir *= -1;
            break;
        }
    }

    if (!dir_changed) {
        return;
    }

    for (usize i = 0; i < arr->rows * arr->cols; ++i) {
        if (arr->ptr[i].dir != arr->dir) {
            arr->ptr[i].dir *= -1; 
        }
    }
}

void enemy_bullet_shoot(EnemyBulletVec *bullet_vec, EnemyArr *enemies) {
    if (bullet_vec->cooldown > 0.f) {
        return;
    }

    printf("shooting\n");

    bullet_vec->cooldown = 1.5f;

    i32 random_index = rand() % (enemies->rows * enemies->cols);

    while (enemies->ptr[random_index].dead) {
        random_index = rand() % (enemies->rows * enemies->cols);
    }

    EnemyBullet new_bullet = {
        .x_pos = enemies->ptr[random_index].x_pos + (enemies->ptr[random_index].image.width * enemies->ptr[random_index].scale) / 2,
        .y_pos = enemies->ptr[random_index].y_pos + 5,
        .width = 8,
        .height = 50,
        .color = (SDL_Color) { .r = 0x0, .g = 0xFF, .b = 0x0, .a = 0xFF },
        .out = false,
    };

    enemy_bullet_vec_push(bullet_vec, new_bullet);
}
