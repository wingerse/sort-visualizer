#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include "game.h"
#include "sort.h"

int main(void) 
{
    srand((unsigned int)time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) sdl_error();
    if (!IMG_Init(IMG_INIT_PNG)) sdl_error();
    
    SDL_Window *w = SDL_CreateWindow("Sort Visualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 1000, 
                     SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!w) sdl_error();
    SDL_Renderer *r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
    if (!r) sdl_error();
    SDL_GL_SetSwapInterval(0);
    
    Game g;
    Game_init(&g, w, r);

    Game_run(&g);

    Game_destroy(&g);

    IMG_Quit();
    SDL_Quit();
    return 0;
}