#include "include/enemy.h"

static const f64 ENEMY_MOVE_COOLDOWN = 0.6f;

EnemyAnimation enemy_animation_init(Image *image, Context *context) {
    EnemyAnimation animation = {0};

    (void)context;

    animation.fames[0] = image[0];
    animation.fames[1] = image[1];
    animation.fames[2] = image[2];

    animation.state = STATE_1;
    animation.death_animation_played = false;
    animation.move_down_animation = false;

    return animation;
}

void enemy_animation_destroy(EnemyAnimation *enemy_animation) {
    image_delete(&enemy_animation->fames[0]);
    image_delete(&enemy_animation->fames[1]);
    image_delete(&enemy_animation->fames[2]);
}

Enemy enemy_new(f64 x, f64 y, Context *context, Image *animation_frames) {
    return (Enemy){
        .animation = enemy_animation_init(animation_frames, context),
        .scale = 0.1,
        .x_pos = x,
        .y_pos = y,
        .dead = false,
        .move_cooldown = ENEMY_MOVE_COOLDOWN,
        .current_move_cooldown = ENEMY_MOVE_COOLDOWN,
        .dir = 1,
    };
}

void enemy_delete(Enemy *enemy) { enemy_animation_destroy(&enemy->animation); }

void enemy_render(Enemy *enemy, Context *context) {
    if (enemy->dead && enemy->animation.death_animation_played) {
        return;
    }

    SDL_Rect src = {.w = enemy->animation.fames[enemy->animation.state].width, .h = enemy->animation.fames[enemy->animation.state].height, .x = 0, .y = 0};
    SDL_Rect dst = {
        .w = enemy->animation.fames[enemy->animation.state].width * enemy->scale, .h = enemy->animation.fames[enemy->animation.state].height * enemy->scale, .x = enemy->x_pos, .y = enemy->y_pos};

    SDL_RenderCopyEx(context->renderer, enemy->animation.fames[enemy->animation.state].texture, &src, &dst, 0.f, NULL, SDL_FLIP_NONE);
}

static void enemy_move_down(Enemy *enemy) { enemy->y_pos += (enemy->animation.fames[0].height * enemy->scale) / 2; }

void enemy_update(Enemy *enemy, f64 delta_time, i32 window_width, BulletVec *bullet_vec, u32 *death_count) {
    if (enemy->animation.death_animation_played) {
        return;
    }

    for (usize i = 0; i < bullet_vec->len; ++i) {
        if (enemy->dead) {
            break;
        }

        enemy->dead = enemy_shot(enemy, &bullet_vec->ptr[i]);

        if (enemy->dead) {
            *death_count += 1;
            bullet_vec->ptr[i].out = true;
            enemy->animation.state = STATE_2;
            return;
        }
    }

    enemy->current_move_cooldown -= delta_time;

    if (enemy->current_move_cooldown > 0.f) {
        return;
    }

    if (enemy->animation.state == STATE_2) {
        enemy->animation.death_animation_played = true;
    }

    if (enemy->animation.state != STATE_2) {
        enemy->animation.state = (enemy->animation.state + 1) % 2;
    }

    enemy->current_move_cooldown = enemy->move_cooldown;

    f64 speed = enemy->dir * (enemy->animation.fames[0].width * enemy->scale / 2);

    if (enemy->animation.move_down_animation) {
        enemy_move_down(enemy);
        enemy->animation.move_down_animation = false;
        return;
    }

    enemy->x_pos += speed;

    if (enemy->x_pos + enemy->animation.fames[0].width * enemy->scale + speed >= (f64)window_width || enemy->x_pos <= 0.f - speed) {
        enemy->dir *= -1;
        enemy->animation.move_down_animation = true;
    }
}

bool enemy_shot(Enemy *enemy, Bullet *bullet) {
    if (bullet->out) {
        return false;
    }

    return enemy->dead || (bullet->y_pos <= enemy->y_pos + enemy->animation.fames[0].height * enemy->scale && bullet->x_pos >= enemy->x_pos &&
                           bullet->x_pos <= enemy->x_pos + enemy->animation.fames[0].width * enemy->scale);
}

static usize coords(usize row, usize col, usize cols) { return row * cols + col; }

void enemy_arr_init(EnemyArr *arr, usize cols, usize rows, Context *context) {
    enemy_bullet_vec_init(&arr->bullets);

    Image animation_images[] = {
        image_new("assets/enemy_frame_0.png", context->renderer),
        image_new("assets/enemy_frame_1.png", context->renderer),
        image_new("assets/enemy_frame_2.png", context->renderer),
    };

    arr->dir = 1;
    arr->cols = cols;
    arr->rows = rows;
    arr->number_of_deaths = 0;

    arr->ptr = malloc(sizeof(Enemy) * rows * cols);
    assert(arr->ptr);

    for (usize i = 0; i < rows; ++i) {
        for (usize j = 0; j < cols; ++j) {
            usize index = coords(i, j, cols);

            arr->ptr[index] = enemy_new(0, 0, context, animation_images);
            arr->ptr[index].y_pos = 1.9f * (context->window.height / 25 + (arr->ptr[index].animation.fames[0].height * arr->ptr[index].scale * i));
            arr->ptr[index].x_pos = 1.2f * j * (arr->ptr[index].animation.fames[0].width * arr->ptr[index].scale) + 50.f;
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

static void change_cooldown_based_on_deaths(EnemyArr *arr) {
    f64 factor = (f32)arr->number_of_deaths / (arr->cols * arr->rows);
    f64 new_cooldown = (1.0 - factor) * ENEMY_MOVE_COOLDOWN;

    if (new_cooldown <= ENEMY_MOVE_COOLDOWN / 6) {
        return;
    }

    for (usize i = 0; i < arr->cols * arr->rows; ++i) {
        arr->ptr[i].move_cooldown = new_cooldown;
    }
}

void enemy_arr_update(EnemyArr *arr, f64 delta_time, Window *window, BulletVec *bullet_vec, Ship *ship) {
    change_cooldown_based_on_deaths(arr);
    enemy_bullet_shoot(&arr->bullets, arr);
    enemy_bullet_vec_update(&arr->bullets, delta_time, ship, window);

    for (usize i = 0; i < arr->rows * arr->cols; ++i) {
        enemy_update(&arr->ptr[i], delta_time, window->width, bullet_vec, &arr->number_of_deaths);
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
            arr->ptr[i].animation.move_down_animation = true;
            // enemy_move_down(&arr->ptr[i]);
        }
    }
}

static f64 calculate_new_shoot_cooldown(EnemyArr *arr) {
    f64 factor = (f32)arr->number_of_deaths / (arr->cols * arr->rows);
    return (1.0 - factor);
}

void enemy_bullet_shoot(EnemyBulletVec *bullet_vec, EnemyArr *enemies) {
    if (bullet_vec->cooldown > 0.f) {
        return;
    }

    bullet_vec->cooldown = calculate_new_shoot_cooldown(enemies);

    i32 random_index = rand() % (enemies->rows * enemies->cols);

    while (enemies->ptr[random_index].dead) {
        random_index = rand() % (enemies->rows * enemies->cols);
    }

    EnemyBullet new_bullet = {
        .x_pos = enemies->ptr[random_index].x_pos + (enemies->ptr[random_index].animation.fames[0].width * enemies->ptr[random_index].scale) / 2,
        .y_pos = enemies->ptr[random_index].y_pos + 5,
        .width = 8,
        .height = 50,
        .color = (SDL_Color){.r = 0xFF, .g = 0xFF, .b = 0xDD, .a = 0xFF},
        .out = false,
    };

    enemy_bullet_vec_push(bullet_vec, new_bullet);
}
