#include "include/ship.h"

void ship_init(Ship *ship, Image image, u32 x, u32 y) {
    ship->image = image;
    ship->scale = 0.3f; 
    ship->x_pos = x;
    ship->y_pos = y;
    ship->shoot_cooldown = 0.f;
    bullet_vec_init(&ship->bullets);
}

void ship_destroy(Ship *ship) {
    image_delete(&ship->image);
    bullet_vec_destroy(&ship->bullets);
}

void ship_move(Ship *ship, ShipDir dir, f64 delta_time, i32 window_width) {
    if (dir == ShipDirRight) {
        u32 right_limit = window_width - (window_width / 25) - (u32)(ship->image.width * ship->scale);
        if (ship->x_pos < right_limit) {
            ship->x_pos += delta_time * 1000.f;
        }
    } else if (dir == ShipDirLeft) {
        if (ship->x_pos > (u32)window_width / 25) {
            ship->x_pos -= delta_time * 1000.f;
        }
    }
}

void ship_render(Ship *ship, Context *context) {
    SDL_Rect src = {
        .w = ship->image.width,
        .h = ship->image.height,
        .x = 0,
        .y = 0
    };

    SDL_Rect dst = {
        .w  = ship->image.width * ship->scale,
        .h = ship->image.height * ship->scale,
        .x = ship->x_pos,
        .y = ship->y_pos 
    };

    SDL_RenderCopyEx(context->renderer, ship->image.texture, &src, &dst, 0.f, NULL, SDL_FLIP_NONE);

    for (usize i = 0; i < ship->bullets.len; ++i) {
        bullet_render(&ship->bullets.ptr[i], context);
    }
}

void ship_shoot(Ship *ship) {
    if (ship->shoot_cooldown > 0.f) {
        return;
    }

    ship->shoot_cooldown = 0.5f;

    Bullet new_bullet = {
        .x_pos = ship->x_pos + (ship->image.width * ship->scale) / 2,
        .y_pos = ship->y_pos - 5,
        .width = 8,
        .height  = 50,
        .color = (SDL_Color) { .r = 0x0, .g = 0xFF, .b = 0x0, .a = 0xFF },
        .out = false
    };

    bullet_vec_push(&ship->bullets, new_bullet);
}

void ship_update(Ship *ship, f64 delta_time) {
    ship->shoot_cooldown -= delta_time;
    bullet_vec_update(&ship->bullets, delta_time);
} 
