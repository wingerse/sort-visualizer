#include "draw.h"
#include <SDL2/SDL.h>
#include <math.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

DrawFuncInterface draw_funcs[DRAW_FUNCS_COUNT] = {
    {draw_rectangles, "Rectangles"},
    {draw_dots, "Dots"},
    {draw_disparity_dots, "Disparity Dots"},
    {draw_spiral, "Spiral"},
    {draw_circle, "Circle"},
};

void draw_rectangles(Game *g, int window_w, int window_h)
{
    float rect_width = (float)window_w / g->ar_len;
    for (int i = 0; i < g->ar_len; i++) {
        float rect_height = (float)g->ar[i]/g->ar_len * window_h;

        SDL_Rect rect = {(int)(i * rect_width), (int)(window_h - rect_height), (int)ceilf(rect_width), (int)rect_height};
        Game_set_color(g, i);
        SDL_RenderFillRect(g->r, &rect);
    }
}

void draw_dots(Game *g, int window_w, int window_h)
{
    float rect_width = (float)window_w / g->ar_len;
    for (int i = 0; i < g->ar_len; i++) {
        float rect_height = (float)g->ar[i]/g->ar_len * window_h;

        SDL_Rect rect = {(int)(i * rect_width + rect_width/2), (int)(window_h - rect_height), 3, 3};
        Game_set_color(g, i);
        SDL_RenderFillRect(g->r, &rect);
    }
}

void draw_disparity_dots(Game *g, int window_w, int window_h)
{
    for (int i = 0; i < g->ar_len; i++) {
        float angle = ((float)(i+1)/g->ar_len * 360) * (float)M_PI / 180.0f;
        float dist = abs(i+1 - g->ar[i]);
        dist = MIN(dist, g->ar_len-dist);
        float disparity = 1.0f - dist/(g->ar_len/2.0f);

        float _x = 0.5f + cosf(angle) * disparity / 2.0f;
        float _y = 0.5f + sinf(angle) * disparity / 2.0f;

        int x = (int)(window_w * _x);
        int y = (int)(window_h - window_h * _y);

        int radius = 3;
        SDL_Rect rect = {x-radius/2, y-radius/2, radius, radius};
        Game_set_color(g, i);
        SDL_RenderFillRect(g->r, &rect);
    }
}

void draw_spiral(Game *g, int window_w, int window_h) 
{
    for (int i = 0; i < g->ar_len; i++) {
        float angle = ((float)(i+1)/g->ar_len * 360) * (float)M_PI / 180.0f;
        float dist = (float)g->ar[i] / g->ar_len;

        float _x = 0.5f + cosf(angle) * dist / 2.0f;
        float _y = 0.5f + sinf(angle) * dist / 2.0f;

        int x = (int)(window_w * _x);
        int y = (int)(window_h - window_h * _y);

        int radius = 3;
        SDL_Rect rect = {x-radius/2, y-radius/2, radius, radius};
        Game_set_color(g, i);
        SDL_RenderFillRect(g->r, &rect);
    }
}

static int rainbow(float f)
{
    int r = (int)((sin(f * 2*(float)M_PI + 0)/2+0.5f) * 255);
    int g = (int)((sin(f * 2*(float)M_PI + 2)/2+0.5f) * 255);
    int b = (int)((sin(f * 2*(float)M_PI + 4)/2+0.5f) * 255);
    // between blue and green
    return (r<<16) | (g<<8) | b;
}

void draw_circle(Game *g, int window_w, int window_h)
{
    for (int i = 0; i < g->ar_len; i++) {
        float angle = ((float)(i+1)/g->ar_len * 360) * (float)M_PI / 180.0f;
        float f = (float)g->ar[i] / g->ar_len;
        int rgb = rainbow((float)f);

        float _x = 0.5f + cosf(angle) / 2.0f;
        float _y = 0.5f + sinf(angle) / 2.0f;

        int x = (int)(window_w * _x);
        int y = (int)(window_h - window_h * _y);

        Game_set_color_custom(g, i, rgb);
        SDL_RenderDrawLine(g->r, window_w/2, window_h/2, x, y);
    }

}