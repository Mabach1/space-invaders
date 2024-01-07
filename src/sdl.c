#include "include/sdl.h"

void sdl_context_init(Context *context, const char *game_name, const i32 width, const i32 height) {
    SDL_Init(SDL_INIT_VIDEO);

    context->window.width = width;
    context->window.height = height;
    
    context->window.ptr = SDL_CreateWindow(game_name, 0, 0, width, height, SDL_WINDOW_SHOWN);
    assert(context->window.ptr);

    context->renderer = SDL_CreateRenderer(context->window.ptr, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    assert(context->renderer);

    SDL_SetWindowPosition(context->window.ptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void sdl_context_destroy(Context *context) {
    SDL_DestroyRenderer(context->renderer);
    SDL_DestroyWindow(context->window.ptr);
    SDL_Quit();
}
