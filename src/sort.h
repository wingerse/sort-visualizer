#pragma once

#include "game.h"

#define SORT_FUNCS_COUNT 8

typedef void SortFunc(Game *g);

typedef struct {
    SortFunc *f;
    const char *name;
} SortFuncInterface;

SortFuncInterface sort_funcs[SORT_FUNCS_COUNT];