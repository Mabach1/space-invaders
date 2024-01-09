#include "include/image.h"

Image image_new(const char *image_path, SDL_Renderer *renderer) {
    Image new_image = {NULL, 0, 0};

    new_image.texture = IMG_LoadTexture(renderer, image_path);
    assert(new_image.texture);

    SDL_QueryTexture(new_image.texture, NULL, NULL, &new_image.width, &new_image.height);

    return new_image;
}

void image_delete(Image *image) {
    if (image) {
        SDL_DestroyTexture(image->texture);
    }
}
