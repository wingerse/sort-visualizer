#pragma once
#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct Game Game;

#include "sort.h"
#include "draw.h"

#define MARKS_SIZE 3

typedef struct Game {
    SDL_Window *w;
    SDL_Renderer *r;
    int ar_len;
    int *ar;
    int marks[MARKS_SIZE];
    bool quit;
    int comparisions;
    int ar_access;
    int step_delay;
    int sort_func_index;
    int draw_func_index;
    SDL_Texture *font;
    bool run_sort;
} Game;

void sdl_error(void);
void swap(int *ar, int i, int j);
void render_text(Game *g, const char *text);
void Game_init(Game *g, SDL_Window *w, SDL_Renderer *r);
void Game_reset_marks(Game *g);
void Game_set_color(Game *g, int index);
void Game_set_color_custom(Game *g, int index, int color);
void Game_change_ar_len(Game *g, int new_len);
void Game_shuffle(Game *g);
void Game_draw(Game *g);
void Game_delay(Game *g);
void Game_delay_custom(Game *g, int delay);
void Game_run(Game *g);
void Game_destroy(Game *g);