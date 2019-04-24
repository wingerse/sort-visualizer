#include "text.h"
#include <ctype.h>

static SDL_Rect font_rects[75] = {
    {0, 0, 12, 16}, {12, 0, 10, 16}, {22, 0, 10, 16}, {32, 0, 13, 16}, {45, 0, 9, 16}, {54, 0, 11, 16}, 
    {65, 0, 11, 16}, {76, 0, 12, 16}, {88, 0, 6, 16}, {94, 0, 6, 16}, {100, 0, 11, 16}, {111, 0, 8, 16}, 
    {119, 0, 13, 16}, {132, 0, 11, 16}, {143, 0, 12, 16}, {155, 0, 11, 16}, {0, 16, 12, 16}, {12, 16, 13, 16}, 
    {25, 16, 10, 16}, {35, 16, 13, 16}, {48, 16, 11, 16}, {59, 16, 16, 16}, {75, 16, 18, 16}, {93, 16, 13, 16}, 
    {106, 16, 13, 16}, {119, 16, 12, 16}, {131, 16, 6, 16}, {140, 16, 7, 16}, {147, 16, 5, 16}, {1, 32, 8, 16}, 
    {11, 32, 7, 16}, {20, 32, 7, 16}, {28, 32, 10, 16}, {39, 32, 8, 16}, {48, 32, 8, 16}, {57, 32, 8, 16}, 
    {67, 32, 8, 16}, {77, 32, 4, 16}, {82, 32, 4, 16}, {88, 32, 8, 16}, {97, 32, 6, 16}, {105, 32, 9, 16}, 
    {116, 32, 8, 16}, {125, 32, 9, 16}, {135, 32, 9, 16}, {146, 32, 9, 16}, {156, 32, 8, 16}, {0, 48, 8, 16}, 
    {8, 48, 10, 16}, {19, 48, 8, 16}, {29, 48, 10, 16}, {42, 48, 14, 16}, {57, 48, 10, 16}, {69, 48, 9, 16}, 
    {80, 48, 9, 16}, {91, 48, 4, 16}, {97, 48, 3, 16}, {102, 48, 5, 16}, {109, 48, 8, 16}, {119, 48, 6, 16}, 
    {127, 48, 4, 16}, {131, 48, 8, 16}, {0, 64, 9, 16}, {12, 64, 5, 16}, {18, 64, 10, 16}, {29, 64, 9, 16}, 
    {39, 64, 10, 16}, {52, 64, 8, 16}, {61, 64, 9, 16}, {70, 64, 9, 16}, {79, 64, 9, 16}, {89, 64, 11, 16}, 
    {117, 64, 5, 16}, {122, 64, 7, 16}, {157, 64, 4, 16}
};

static SDL_Rect *get_char_rect(const char c) 
{
    SDL_Rect *r;
    if (isupper(c)) {
        r = &font_rects[c - 65];
    } else if (islower(c)) {
        r = &font_rects[c - 97 + 29];
    } else if (isdigit(c)) {
        r = &font_rects[c - 48 + 62];
    } else {
        switch (c) {
        case '(': r = &font_rects[26]; break;
        case ')': r = &font_rects[27]; break;
        case ':': r = &font_rects[28]; break;
        case '.': r = &font_rects[55]; break;
        case ',': r = &font_rects[56]; break;
        case ';': r = &font_rects[57]; break;
        case '?': r = &font_rects[58]; break;
        case '"': r = &font_rects[59]; break;
        case '\'': r = &font_rects[60]; break;
        case '-': r = &font_rects[61]; break;
        case '<': r = &font_rects[72]; break;
        case '>': r = &font_rects[73]; break;
        default : r = &font_rects[74]; break;
        }
    }

    return r;
}

void draw_text(const char *text, Game *g, SDL_Point pos)
{
    int x = 0;
    int y = 0;
    const char *p = text;
    for (;*p != '\0'; p++) {
        SDL_Rect* r = get_char_rect(*p);
        if (*p == '\n') {
            y += r->h + 5;
            x = 0;
            continue;
        }
        float scale = 1;
        SDL_Rect dst = { pos.x + x, pos.y + y, (int)(r->w * scale), (int)(r->h * scale)};

        SDL_RenderCopy(g->r, g->font, r, &dst);
        x += (int)(r->w * scale);
    }
}