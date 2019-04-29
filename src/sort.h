#pragma once

#include "game.h"

#define SORT_FUNCS_COUNT 7

typedef void SortFunc(Game *g);

typedef struct {
    SortFunc *f;
    const char *name;
} SortFuncInterface;

SortFuncInterface sort_funcs[SORT_FUNCS_COUNT];

void selection_sort(Game *g);
void insertion_sort(Game *g);
void bubble_sort(Game *g);
void merge_insert_sort(Game *g);
void merge_sort(Game *g);
void merge_sort_bottom_up(Game *g);
void counting_sort(Game *g);