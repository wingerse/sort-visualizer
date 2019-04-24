#pragma once

#include "game.h"

#define DRAW_FUNCS_COUNT 5

typedef void DrawFunc(Game *g, int window_w, int window_h);

typedef struct {
    DrawFunc *f;
    const char *name;
} DrawFuncInterface;

DrawFuncInterface draw_funcs[DRAW_FUNCS_COUNT];

void draw_rectangles(Game *g, int window_w, int window_h);
void draw_disparity_dots(Game *g, int window_w, int window_h);
void draw_dots(Game *g, int window_w, int window_h);
void draw_spiral(Game *g, int window_w, int window_h);
void draw_circle(Game *g, int window_w, int window_h);