#include "include/text.h"

void text_box_init(TextBox *text_box, SDL_Color color) {
    TTF_Init();
    text_box->font = TTF_OpenFont("assets/Roboto-Medium.ttf", 60);
    text_box->color = color;
}

void text_box_destroy(TextBox *text_box) {
    TTF_CloseFont(text_box->font);
    TTF_Quit();
}

void text_box_draw(TextBox *text_box, const char *text, f32 x, f32 y, Context *context) {
    SDL_Surface *surface = TTF_RenderText_Shaded(text_box->font, text, text_box->color, (SDL_Color){.r = 17, .g = 19, .b = 23, .a = 255});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(context->renderer, surface);    

    i32 width = 0;
    i32 height = 0;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    SDL_RenderCopy(
        context->renderer, 
        texture, 
        NULL, 
        &(SDL_Rect){
            .x = x == -1.0 ? context->window.width - width - 50.f : x, 
            .y = y == -1.0 ? context->window.height - height - 30.f : y, 
            .w = width, 
            .h = height
        }
    );

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
