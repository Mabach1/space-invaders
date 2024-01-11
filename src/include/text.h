#pragma once

#include <SDL2/SDL_ttf.h>

#include "common.h"
#include "sdl.h"

typedef struct {
    TTF_Font *font;
    SDL_Color color;
} TextBox;

void text_box_init(TextBox *text_box, SDL_Color color);
void text_box_destroy(TextBox *text_box);

void text_box_draw(TextBox *text_box, const char *text, f32 x, f32 y, Context *context);
