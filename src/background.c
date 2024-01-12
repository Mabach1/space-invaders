#include "include/background.h"

void background_init(Background *background, Context *context) {
    background->image = image_new("assets/background.png", context->renderer);
    background->progress = 0.0f;
}

void background_destroy(Background *background) { image_delete(&background->image); }

void background_update(Background *background, f64 delta_time) {
    background->progress += delta_time * 0.03;
    while (background->progress > 1.0) {
        background->progress -= 1.0;
    }
}

void background_render(Background *background, Context *context) {
    f64 progress = background->progress;

    SDL_Rect top = {
        .x = 0,
        .y = -(1.0 - progress) * context->window.height,
        .w = background->image.width,
        .h = background->image.height,
    };

    SDL_RenderCopy(context->renderer, background->image.texture, NULL, &top);

    SDL_Rect bottom = {
        .x = 0,
        .y = progress * context->window.height,
        .w = background->image.width,
        .h = background->image.height,
    };

    SDL_RenderCopy(context->renderer, background->image.texture, NULL, &bottom);
}
