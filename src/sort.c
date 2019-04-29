#include "sort.h"
#include <stdio.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

SortFuncInterface sort_funcs[SORT_FUNCS_COUNT] = {
    {selection_sort, "Selection Sort"},
    {insertion_sort, "Insertion Sort"},
    {bubble_sort, "Bubble Sort"},
    {merge_insert_sort, "Merge Insert Sort"},
    {merge_sort, "Merge Sort (Top-Down)"},
    {merge_sort_bottom_up, "Merge Sort (Buttom-Up)"},
    {counting_sort, "Counting Sort"},
};

void selection_sort(Game *g)
{
    for (int i = 0; i < g->ar_len; i++) {
        g->marks[1] = i;
        int min_index = i;
        g->marks[2] = min_index;
        int min = g->ar[i];
        g->ar_access++;
        for (int j = i; j < g->ar_len; j++) {
            g->marks[0] = j;
            g->ar_access++;
            g->comparisions++;
            if (g->ar[j] < min) {
                min = g->ar[j];
                min_index = j;
                g->marks[2] = min_index;
            }
            if (g->quit) { return; }
            Game_draw(g);
            Game_delay(g);
        }

        swap(g->ar, i, min_index);
    }
}

void insertion_sort(Game *g) 
{
    for (int i = 1; i < g->ar_len; i++) {
        g->marks[1] = i;

        g->ar_access += 2;
        g->comparisions++;
        for (int j = i; j > 0 && g->ar[j-1] > g->ar[j]; j--) {
            g->marks[0] = j;
            g->marks[2] = j-1;
            swap(g->ar, j-1, j); 
            if (g->quit) { return; }
            Game_draw(g);
            Game_delay(g);

            g->ar_access += 2;
            g->comparisions++;
        }
    }
}

void bubble_sort(Game *g)
{
    for (int i = g->ar_len-1; i > 0; i--) {
        g->marks[1] = i;
        for (int j = 0; j < i; j++) {
            g->marks[2] = j;
            g->marks[0] = j+1;

            g->ar_access += 2;
            g->comparisions++;
            if (g->ar[j] > g->ar[j+1]) {
                swap(g->ar, j, j+1);
            }
            if (g->quit) { return; }
            Game_draw(g);
            Game_delay(g);
        }
    }
}

static void merge_insert(Game *g, int start, int mid, int end)
{
    for (int i = start; i < mid; i++) {
        g->marks[1] = i;

        g->ar_access += 2;
        g->comparisions++;
        if (g->ar[i] > g->ar[mid]) {
            swap(g->ar, i, mid);

            g->ar_access += 2;
            g->comparisions++;
            for (int j = mid; j < end-1 && g->ar[j] > g->ar[j+1]; j++) {
                g->marks[2] = j;
                g->marks[0] = j+1;

                swap(g->ar, j, j+1);
                if (g->quit) { return; }
                Game_draw(g);
                Game_delay(g);

                g->ar_access += 2;
                g->comparisions++;
            }
        }
    }
}

static void _merge_insert_sort(Game *g, int start, int end)
{
    int len = end - start;
    if (len > 1) {
        int mid = (start+end) / 2;
        _merge_insert_sort(g, start, mid);
        _merge_insert_sort(g, mid, end);
        merge_insert(g, start, mid, end);
    }
}

void merge_insert_sort(Game *g)
{
    _merge_insert_sort(g, 0, g->ar_len);
}

static void merge(Game *g, int start, int mid, int end, int *tmpar)
{
    int i = start;
    int j = mid;
    int k = i;

    // copy elements into tmp array until one side is done
    while (i < mid && j < end) {
        g->marks[0] = i;
        g->marks[1] = j;

        g->comparisions++;
        g->ar_access += 2;
        if (g->ar[i] <= g->ar[j]) {
            tmpar[k] = g->ar[i];
            i++;
        } else {
            tmpar[k] = g->ar[j];
            j++;
        }
        if (g->quit) { return; }
        Game_draw(g);
        Game_delay(g);
        k++;
    }

    // copy the rest of the other side. no comparisons needed here.
    // and note: only one of these loops will run because one side is already
    // done.
    for (int m = i; m < mid; m++) {
        g->ar_access++;
        tmpar[k++] = g->ar[m];
    }    

    for (int m = j; m < end; m++) {
        g->ar_access++;
        tmpar[k++] = g->ar[m];
    }    
}

static void _merge_sort(Game *g, int start, int end, int *tmpar)
{
    int len = end - start;
    if (len > 1) {
        int mid = (start+end) / 2;
        _merge_sort(g, start, mid, tmpar);
        _merge_sort(g, mid, end, tmpar);

        // swap array and temporary array because I need to copy from tmpar to array for merging
        int *t = g->ar;
        g->ar = tmpar;
        tmpar = t;

        merge(g, start, mid, end, tmpar);

        // swap back because I'm done with merging
        t = g->ar;
        g->ar = tmpar;
        tmpar = t;

        // copy back from array to tempar because I need this to be in tmpar in a lower recursion level for merging.
        memcpy(tmpar + start, g->ar + start, sizeof(int)*(size_t)len);
    }
}

void merge_sort(Game *g)
{
    int *tmpar = malloc((size_t)g->ar_len * sizeof(int));
    memcpy(tmpar, g->ar, sizeof(int)*(size_t)g->ar_len);

    _merge_sort(g, 0, g->ar_len, tmpar);

    free(tmpar);
}

static void _merge_sort_bottom_up(Game *g, int start, int end, int *tmpar)
{
    int len = end - start;
    // sublist length starts from 1 and doubles every iteration.
    for (int w = 1; w < len; w *= 2) {
        // merge all the sublists
        for (int i = 0; i < len; i += 2*w) {
            merge(g, i, MIN(i+w, len), MIN(i+2*w, len), tmpar);
        }
        // they were merged into tmpar so we need to copy back to array
        memcpy(g->ar, tmpar, (size_t)len*sizeof(int));
    }
}

void merge_sort_bottom_up(Game *g)
{
    int *tmpar = malloc((size_t)g->ar_len * sizeof(int));
    _merge_sort_bottom_up(g, 0, g->ar_len, tmpar);
    free(tmpar);
}

void counting_sort(Game *g)
{
    int *tmpar = calloc((size_t)g->ar_len, sizeof(int));
    for (int i = 0; i < g->ar_len; i++) {
        g->marks[0] = i;
        tmpar[g->ar[i]-1]++;
        g->ar_access++;
        Game_draw(g);
        Game_delay(g);
    }

    int k = 0;
    for (int i = 0; i < g->ar_len; i++) {
        g->marks[0] = i;

        for (int j = 0; j < tmpar[i]; j++) {
            g->ar[k] = i + 1;
            g->ar_access++;
            k++;
        }
        Game_draw(g);
        Game_delay(g);
    }

    free(tmpar);
}