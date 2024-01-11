#pragma once

#include "image.h"
#include "common.h"
#include "sdl.h"

typedef struct Background {
    Image image;
    f64 progress;
} Background;

void background_init(Background *background, Context *context);
void background_destroy(Background *background);

void background_update(Background *background, f64 delta_time);
void background_render(Background *background, Context *context);
