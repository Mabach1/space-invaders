#pragma once

#include <SDL2/SDL_ttf.h>

#include "common.h"

typedef struct {
    SDL_Window *ptr;
    i32 width;
    i32 heigh;
} Window;

typedef struct {
    Window window;
    SDL_Renderer *renderer;
} Context;

void sdl_context_init(Context *context, const char *game_name, const i32 width, const i32 height);
void sdl_context_destroy(Context *context);
