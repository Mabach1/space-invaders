#pragma once

#include <SDL2/SDL_image.h>

#include "common.h"

typedef struct Image {
    SDL_Texture *texture;
    i32 width;
    i32 height;
} Image;

Image image_new(const char *image_path, SDL_Renderer *renderer);
void image_delete(Image *image);
