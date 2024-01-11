#include "include/line.h"

static void hit_area_init(HitArea *area, f32 y) {
    area->cap = 16;
    area->len = 0;
    area->y_pos = y;

    area->ptr = (Hit *)malloc(sizeof(Hit) * area->cap);
    assert(area->ptr);
}

static void hit_area_destroy(HitArea *area) { free(area->ptr); }

static void hit_area_push(HitArea *area, Hit hit) {
    if (area->cap == area->len) {
        area->cap *= 2;
        area->ptr = realloc(area->ptr, sizeof(Hit) * area->cap);
        assert(area->ptr);
    }
    area->ptr[area->len++] = hit;
}

static void hit_render(Hit *hit, f32 y, Context *context) {
    SDL_Rect rect = {.w = 8, .h = 5, .x = hit->x_pos, .y = y};
    SDL_SetRenderDrawColor(context->renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(context->renderer, &rect);
}

static void hit_area_render(HitArea *area, Context *context) {
    for (usize i = 0; i < area->len; ++i) {
        hit_render(&area->ptr[i], area->y_pos, context);
    }
}

void life_line_init(LifeLine *line, Window *window) {
    line->color = (SDL_Color){.r = 0xFF, .g = 0xFF, .b = 0xFF, .a = 0xFF};

    line->x_pos = 0;
    line->y_pos = window->height - window->height / 11;
    line->width = window->width;
    line->height = 5;

    hit_area_init(&(line->hit_area), line->y_pos);
}

void life_line_destroy(LifeLine *line) { hit_area_destroy(&line->hit_area); }

void life_line_render(LifeLine *line, Context *context) {

    SDL_Rect rect = {.w = line->width, .h = line->height, .x = line->x_pos, .y = line->y_pos};

    SDL_SetRenderDrawColor(context->renderer, line->color.r, line->color.g, line->color.b, 255);
    SDL_RenderFillRect(context->renderer, &rect);

    hit_area_render(&line->hit_area, context);
}

static bool bullet_hit_life_line(LifeLine *line, EnemyBullet *bullet) {
    return SDL_HasIntersection(&(SDL_Rect){.x = line->x_pos, .y = line->y_pos, .w = line->width, .h = line->height},
                               &(SDL_Rect){.x = bullet->x_pos, .y = bullet->y_pos, .w = bullet->width, .h = bullet->height});
}

void life_line_update(LifeLine *line, EnemyBulletVec *bullets) {
    for (usize i = 0; i < bullets->len; ++i) {
        if (!bullet_hit_life_line(line, &bullets->ptr[i])) {
            continue;
        }

        bullets->ptr[i].out = true;
        hit_area_push(&line->hit_area, (Hit){.x_pos = bullets->ptr[i].x_pos});
    }
}

bool game_over(LifeLine *line, EnemyArr *enemies) {
    for (usize i = 0; i < enemies->cols * enemies->rows; ++i) {
        if (enemies->ptr[i].dead) {
            continue;
        }

        Enemy *curr_enemy = &enemies->ptr[i];

        if (SDL_HasIntersection(
            &(SDL_Rect){.x = line->x_pos, .y = line->y_pos, .w = line->width, .h = line->height},
            &(SDL_Rect){
                .x = curr_enemy->x_pos, 
                .y = curr_enemy->y_pos, 
                .w = curr_enemy->animation.fames[0].width * curr_enemy->scale, 
                .h = curr_enemy->animation.fames[0].height * curr_enemy->scale}
            )
        ) {
            return true;     
        }
    }

    return false;
}
