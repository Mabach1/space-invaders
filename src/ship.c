#include "include/ship.h"

static const f32 SHIP_SCALE = 0.2;

void ship_display_lives_init(ShipLivesIndicator *indicator, Ship *ship, Context *context) {
    i32 ship_height = ship->image.height * ship->scale;
    i32 ship_width = ship->image.width * ship->scale;

    ship_init(&indicator->life_indicator[0], ship->image, 10.f, context->window.height - ship_height - 10.f);
    ship_init(&indicator->life_indicator[1], ship->image, 20.f + ship_width, context->window.height - ship_height - 10.f);
}

void ship_display_lives_destroy(ShipLivesIndicator *indicator) {
    ship_destroy(&indicator->life_indicator[0]);
    ship_destroy(&indicator->life_indicator[1]);
}

void ship_display_lives_render(ShipLivesIndicator *indicator, Ship *ship, Context *context) {
    if (ship->lives == 1) {
        return;
    }

    for (i8 i = 0; i < ship->lives - 1; ++i) {
        ship_render(&indicator->life_indicator[i], context);
    }
}

void ship_init(Ship *ship, Image image, f64 x, f64 y) {
    ship->image = image;
    ship->scale = SHIP_SCALE;
    ship->x_pos = x;
    ship->y_pos = y;
    ship->shoot_cooldown = 0.f;
    ship->lives = 3;
    ship->dead = false;
    ship->death_cooldown = 3.f;
    bullet_vec_init(&ship->bullets);
}

void ship_destroy(Ship *ship) {
    image_delete(&ship->image);
    bullet_vec_destroy(&ship->bullets);
}

void ship_move(Ship *ship, ShipDir dir, f64 delta_time, i32 window_width) {
    if (ship->dead) {
        return;
    }

    if (dir == ShipDirRight) {
        u32 right_limit = window_width - (window_width / 25) - (u32)(ship->image.width * ship->scale);
        if (ship->x_pos < right_limit) {
            ship->x_pos += delta_time * 850.f;
        }
    } else if (dir == ShipDirLeft) {
        if (ship->x_pos > (u32)window_width / 25) {
            ship->x_pos -= delta_time * 850.f;
        }
    }
}

void ship_render(Ship *ship, Context *context) {
    if (ship->dead) {
        return;
    }

    SDL_Rect dst = {.w = ship->image.width * ship->scale, .h = ship->image.height * ship->scale, .x = ship->x_pos, .y = ship->y_pos};

    SDL_RenderCopyEx(context->renderer, ship->image.texture, NULL, &dst, 0.f, NULL, SDL_FLIP_NONE);

    for (usize i = 0; i < ship->bullets.len; ++i) {
        bullet_render(&ship->bullets.ptr[i], context);
    }
}

void ship_shoot(Ship *ship) {
    if (ship->shoot_cooldown > 0.f || ship->dead) {
        return;
    }

    ship->shoot_cooldown = 0.3f;

    Bullet new_bullet = {
        .x_pos = ship->x_pos + (ship->image.width * ship->scale) / 2, .y_pos = ship->y_pos - 5, .width = 8, .height = 50, .color = (SDL_Color){.r = 0x0, .g = 0xFF, .b = 0x0, .a = 0xFF}, .out = false};

    bullet_vec_push(&ship->bullets, new_bullet);
}

void ship_update(Ship *ship, f64 delta_time) {
    ship->shoot_cooldown -= delta_time;
    ship->death_cooldown -= delta_time;

    if (ship->death_cooldown <= 0.f) {
        ship->dead = false;
        ship->death_cooldown = 3.f;
    }

    bullet_vec_update(&ship->bullets, delta_time);
}

void ship_shot(Ship *ship, i32 window_width) {
    ship->lives -= 1;
    ship->dead = true;
    ship->x_pos = window_width / 25;
}
