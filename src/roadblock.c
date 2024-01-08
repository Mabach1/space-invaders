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

void roadblock_init_from_matrix(Roadblock *roadblock, u8 *matrix, usize rows, usize cols, f32 x, f32 y, i32 dimension) {
    roadblock->matrix = matrix;
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

    u8 matrix[] = {
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

void roadblock_render(Roadblock *roadblock, Context *context) {
    for (usize i = 0; i < roadblock->cols * roadblock->rows; ++i) {
        SDL_Rect rect = {.w = roadblock->texture[i].dimension, .h = roadblock->texture[i].dimension, .x = roadblock->texture[i].x_pos, .y = roadblock->texture[i].y_pos};
        SDL_SetRenderDrawColor(context->renderer, roadblock->texture[i].color.r, roadblock->texture[i].color.g, roadblock->texture[i].color.b, 255);
        SDL_RenderFillRect(context->renderer, &rect);
    }
}
