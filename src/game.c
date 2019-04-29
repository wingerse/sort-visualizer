#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include "../obj/res/font.png.h"
#include <SDL2/SDL_image.h>
#include "text.h"
#include "audio.h"

#define DEFAULT_AR_LEN 128
#define DEFAULT_STEP_DELAY 1

void sdl_error(void) 
{
    printf(SDL_GetError());
    printf("\n");
    exit(EXIT_FAILURE);
}

static void load_font(Game *g)
{
    SDL_RWops *rw = SDL_RWFromMem(res_font_png, sizeof(res_font_png));
    SDL_Surface *s = IMG_Load_RW(rw, 1); 
    SDL_Texture *t = SDL_CreateTextureFromSurface(g->r, s);
    SDL_FreeSurface(s);
    g->font = t;
} 

void Game_init(Game *g, SDL_Window *w, SDL_Renderer *r)
{
    g->w = w;
    g->r = r;
    g->run_sort = false;
    g->ar = NULL;
    Game_change_ar_len(g, DEFAULT_AR_LEN);
    g->comparisions = 0;
    g->ar_access = 0;
    g->quit = false;
    g->step_delay = DEFAULT_STEP_DELAY;
    Game_reset_marks(g);
    g->sort_func_index = 0;
    g->draw_func_index = 0;
    load_font(g);
    start_audio(g);
}

void Game_reset_marks(Game *g)
{
    for (int i = 0; i < MARKS_SIZE; i++) {
        g->marks[i] = -1;
    }
}

void Game_set_color(Game *g, int index)
{
    Game_set_color_custom(g, index, 0xffffff);
}

void Game_set_color_custom(Game *g, int index, int color)
{
    if (index == g->marks[0]) { SDL_SetRenderDrawColor(g->r, 0xff, 0x00, 0x00, SDL_ALPHA_OPAQUE); }
    else if (index == g->marks[1]) { SDL_SetRenderDrawColor(g->r, 0x00, 0xff, 0x00, SDL_ALPHA_OPAQUE); }
    else if (index == g->marks[2]) { SDL_SetRenderDrawColor(g->r, 0x00, 0x00, 0xff, SDL_ALPHA_OPAQUE); }
    else { SDL_SetRenderDrawColor(g->r, (Uint8)((color&0xff0000) >> 16), (Uint8)((color&0x00ff00) >> 8), (Uint8)(color&0x0000ff), SDL_ALPHA_OPAQUE); }
}

void Game_change_ar_len(Game *g, int new_len)
{
    free(g->ar);
    g->ar_len = new_len;
    g->ar = malloc((size_t)new_len * sizeof(int));

    for (int i = 0; i < new_len; i++) {
        g->ar[i] = i+1; // starts at 1
    }
}

void swap(int *ar, int i, int j) 
{
    int tmp = ar[i];
    ar[i] = ar[j];
    ar[j] = tmp;
}

void Game_shuffle(Game *g)
{
    for (int i = 0; i < g->ar_len; i++) {
        int j = (int)((g->ar_len-1) * (float)rand() / RAND_MAX);
        g->marks[0] = i;
        g->marks[1] = j;
        swap(g->ar, i, j);
        Game_draw(g);
        Game_delay_custom(g, 1);
        if (g->quit) { return; }
    }
}

static char text_buffer[1000];

void Game_draw(Game *g)
{
    int window_w, window_h;
    SDL_GetWindowSize(g->w, &window_w, &window_h);

    SDL_SetRenderDrawColor(g->r, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(g->r);

    draw_funcs[g->draw_func_index].f(g, window_w, window_h);

    SortFuncInterface s = sort_funcs[g->sort_func_index];
    DrawFuncInterface d = draw_funcs[g->draw_func_index];
    sprintf(text_buffer, "%s\n%s\nn: %d\nStep Delay: %d ms\nArray access: %d\nComparisons: %d", 
            s.name, d.name, g->ar_len, g->step_delay, g->ar_access, g->comparisions);
    draw_text(text_buffer, g, (SDL_Point){5, 5});

    SDL_RenderPresent(g->r);
}

static void handle_events(Game *g, SDL_Event *e)
{
    switch (e->type) {
    case SDL_WINDOWEVENT:
        if (e->window.event == SDL_WINDOWEVENT_CLOSE) {
            g->quit = true;
        }
        break;
    case SDL_KEYDOWN:
        if (e->key.state == SDL_PRESSED) {
            switch (e->key.keysym.sym) {
            case SDLK_ESCAPE:
                g->quit = true;
                break;
            case SDLK_UP:
                g->step_delay *= 2;
                break;
            case SDLK_DOWN:
                if (g->step_delay / 2 != 0) { g->step_delay /= 2; }
                break;
            case SDLK_RIGHT:
                if (g->draw_func_index + 1 < DRAW_FUNCS_COUNT) { g->draw_func_index++; }
                break;
            case SDLK_LEFT:
                if (g->draw_func_index > 0) { g->draw_func_index--; }
                break;
            }
        }
        break;
    }
}

static void handle_events2(Game *g, SDL_Event *e)
{
    switch (e->type) {
    case SDL_KEYDOWN:
        if (e->key.state == SDL_PRESSED) {
            switch (e->key.keysym.sym) {
            case SDLK_RETURN:
                g->run_sort = true;
                break;
            case SDLK_w:
                if (g->sort_func_index + 1 < SORT_FUNCS_COUNT) { g->sort_func_index++; }
                break;
            case SDLK_s:
                if (g->sort_func_index > 0) { g->sort_func_index--; }
                break;
            case SDLK_d:
                Game_change_ar_len(g, g->ar_len * 2);
                break;
            case SDLK_a:
                if (g->ar_len / 2 != 0) {
                    Game_change_ar_len(g, g->ar_len / 2);
                }
                break;
            }
        }
        break;
    }
}

void Game_delay(Game *g)
{
    Game_delay_custom(g, g->step_delay);
}

void Game_delay_custom(Game *g, int delay)
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        handle_events(g, &e);        
    }

    SDL_Delay((Uint32)delay);
}

void Game_run(Game *g)
{
    Game_draw(g);
    SDL_Event e;
    while (!g->quit) {
        if (!SDL_WaitEvent(&e)) sdl_error();
        handle_events(g, &e);
        handle_events2(g, &e);
        
        if (g->run_sort) {
            Game_shuffle(g);
            Game_reset_marks(g);
            g->comparisions = 0;
            g->ar_access = 0;
            sort_funcs[g->sort_func_index].f(g);
            Game_reset_marks(g);
            g->run_sort = false;
            Game_draw(g);
        }
        Game_draw(g);
    }   
}

void Game_destroy(Game *g)
{
    free(g->ar);
    SDL_DestroyRenderer(g->r);
    SDL_DestroyWindow(g->w);
}