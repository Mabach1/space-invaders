#include "include/roadblock.h"

Block block_new(f32 x, f32 y, i32 dimension, SDL_Color color) {
    return (Block){
        .x_pos = x,
        .y_pos = y,
        .dimension = dimension,
        .color = color,
    };
}

static usize coords(usize row, usize col, usize cols) { return row * cols + col; }

void copy_matrix(u32 *dst, u32 *src, usize size) {
    for (usize i = 0; i < size; ++i) {
        dst[i] = src[i];
    }
}

void roadblock_init_from_matrix(Roadblock *roadblock, u32 *matrix, usize rows, usize cols, f32 x, f32 y, i32 dimension) {
    roadblock->matrix = malloc(sizeof(u32) * rows * cols);
    assert(roadblock->matrix);

    copy_matrix(roadblock->matrix, matrix, cols * rows);

    roadblock->rows = rows;
    roadblock->cols = cols;
    roadblock->x_pos = x;
    roadblock->y_pos = y;

    roadblock->texture = (Block *)malloc(sizeof(Block) * rows * cols);
    assert(roadblock->texture);

    for (usize i = 0; i < rows; ++i) {
        for (usize j = 0; j < cols; ++j) {
            usize index = coords(i, j, cols);

            roadblock->texture[index] = block_new(
                x + j * (f32)dimension, 
                y + i * (f32)dimension, 
                dimension, 
                matrix[index] == 0 ? (SDL_Color){.r = 0x00, .g = 0x00, .b = 0x00, .a = 0xFF} : (SDL_Color){.r = 0x00, .g = 0xFF, .b = 0x00, .a = 0xFF}
            );
        }
    }
}

void roadblock_init(Roadblock *roadblock, f32 x, f32 y) {
    const usize rows = 10;
    const usize cols = 21;

    u32 matrix[] = {
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    };

    roadblock_init_from_matrix(roadblock, matrix, rows, cols, x, y, 15);
}

void roadblock_destroy(Roadblock *roadblock) {
    free(roadblock->texture);
}

// static void roadblock_debug(Roadblock *roadblock) {
//     system("clear");
//     for (usize i = 0; i < roadblock->rows; ++i) {
//         for (usize j = 0; j < roadblock->cols; ++j) {
//             printf("%u ", roadblock->matrix[coords(i, j, roadblock->cols)]);
//         }
//         printf("\n");
//     }
// }

void roadblock_render(Roadblock *roadblock, Context *context) {
    // roadblock_debug(roadblock);

    for (usize i = 0; i < roadblock->cols * roadblock->rows; ++i) {
        SDL_Rect rect = {.w = roadblock->texture[i].dimension, .h = roadblock->texture[i].dimension, .x = roadblock->texture[i].x_pos, .y = roadblock->texture[i].y_pos};
        SDL_SetRenderDrawColor(context->renderer, roadblock->texture[i].color.r, roadblock->texture[i].color.g, roadblock->texture[i].color.b, 255);
        SDL_RenderFillRect(context->renderer, &rect);
    }
}

static bool bullet_hit_roadblock(Block *block, Bullet *bullet) {
    if (bullet->out) {
        return false;
    }

    return SDL_HasIntersection(&(SDL_Rect){.x = block->x_pos, .y = block->y_pos, .w = block->dimension, .h = block->dimension}, &(SDL_Rect){.x = bullet->x_pos, .y = bullet->y_pos, .w = bullet->width, .h = bullet->height});
}

static bool enemy_bullet_hit_roadblock(Block *block, EnemyBullet *bullet) {
    if (bullet->out) {
        return false;
    }

    return SDL_HasIntersection(&(SDL_Rect){.x = block->x_pos, .y = block->y_pos, .w = block->dimension, .h = block->dimension}, &(SDL_Rect){.x = bullet->x_pos, .y = bullet->y_pos, .w = bullet->width, .h = bullet->height});
}

static void roadblock_check_bullets(Roadblock *roadblock, BulletVec *bullets) {
    for (usize i = roadblock->cols * roadblock->rows - 1; i; --i) {
        if (roadblock->matrix[i] == 0) {
            continue;
        }
        
        for (usize j = 0; j < bullets->len; ++j) {
            if (bullet_hit_roadblock(&roadblock->texture[i], &bullets->ptr[j])) {
                roadblock->matrix[i] = 0;
                roadblock->texture[i].color.r = 0x00;
                roadblock->texture[i].color.g = 0x00;
                roadblock->texture[i].color.b = 0x00;
                bullets->ptr[j].out = true;
            }
        }
    }
}

static void roadblock_check_enemy_bullets(Roadblock *roadblock, EnemyBulletVec *bullets) {
    for (usize i = 0; i < roadblock->cols * roadblock->rows; ++i) {
        if (roadblock->matrix[i] == 0) {
            continue;
        }
        
        for (usize j = 0; j < bullets->len; ++j) {
            if (enemy_bullet_hit_roadblock(&roadblock->texture[i], &bullets->ptr[j])) {
                roadblock->matrix[i] = 0;
                roadblock->texture[i].color.r = 0x00;
                roadblock->texture[i].color.g = 0x00;
                roadblock->texture[i].color.b = 0x00;
                bullets->ptr[j].out = true;
            }
        }
    }

}

void roadblock_update(Roadblock *roadblock, BulletVec *bullets, EnemyBulletVec *enemy_bullets) {
    roadblock_check_bullets(roadblock, bullets);
    roadblock_check_enemy_bullets(roadblock, enemy_bullets);
}

void barricade_init(Barricade *barricade, Window *window) {
    barricade->number_of = 4;
    barricade->ptr = (Roadblock *)malloc(sizeof(Roadblock) * barricade->number_of);

    i32 right_offset = window->width / 20;
    
    for (usize i = 0; i < barricade->number_of; ++i) {
        roadblock_init(&barricade->ptr[i], (i + 1) *right_offset + (i * 15.f * 21), window->height - 550.f);
    }
}

void barricade_destroy(Barricade *barricade) {
    for (usize i = 0; i < barricade->number_of; ++i) {
        roadblock_destroy(&barricade->ptr[i]);
    }
}

void barricade_update(Barricade *barricade, BulletVec *bullets, EnemyBulletVec *enemy_bullets) {
    for (usize i = 0; i < barricade->number_of; ++i) {
        roadblock_update(&barricade->ptr[i], bullets, enemy_bullets);
    }
}

void barricade_render(Barricade *barricade, Context *context) {
    for (usize i = 0; i < barricade->number_of; ++i) {
        roadblock_render(&barricade->ptr[i], context);
    }
}
