#include "include/pause.h"

void scene_pause(SDL_Renderer *renderer) {
    Image pause_background = image_new("assets/pause_scene.png", renderer);

    SDL_RenderCopy(renderer, pause_background.texture, NULL, NULL);

    image_delete(&pause_background);
}
