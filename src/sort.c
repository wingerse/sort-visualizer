#include "sort.h"
#include <stdio.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static void selection_sort(Game *g)
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
            Game_draw(g);
            Game_delay(g);
            if (g->quit) { return; }
        }

        swap(g->ar, i, min_index);
    }
}

static void insertion_sort(Game *g) 
{
    for (int i = 1; i < g->ar_len; i++) {
        g->marks[1] = i;

        g->ar_access += 2;
        g->comparisions++;
        for (int j = i; j > 0 && g->ar[j-1] > g->ar[j]; j--) {
            g->marks[0] = j;
            g->marks[2] = j-1;
            swap(g->ar, j-1, j); 
            Game_draw(g);
            Game_delay(g);
            if (g->quit) { return; }

            g->ar_access += 2;
            g->comparisions++;
        }
    }
}

static void bubble_sort(Game *g)
{
    for (int i = g->ar_len-1; i > 0; i--) {
        g->marks[1] = i;
        bool swapped = false;
        for (int j = 0; j < i; j++) {
            g->marks[2] = j;
            g->marks[0] = j+1;

            g->ar_access += 2;
            g->comparisions++;
            if (g->ar[j] > g->ar[j+1]) {
                swap(g->ar, j, j+1);
                swapped = true;
            }
            Game_draw(g);
            Game_delay(g);
            if (g->quit) { return; }
        }
        if (!swapped) { break; } // is sorted
    }
}

static void shaker_sort(Game *g)
{
    bool right = true;
    int li = 0;
    int ri = g->ar_len-1;
    while (li != ri) {
        bool swapped = false;
        if (right) {
            for (int i = li; i < ri; i++) {
                g->marks[2] = i;
                g->marks[0] = i+1;
                g->ar_access += 2;
                g->comparisions++;
                if (g->ar[i] > g->ar[i+1]) {
                    swap(g->ar, i, i+1);
                    swapped = true;
                }
                Game_draw(g);
                Game_delay(g);
                if (g->quit) { return; }
            }
            right = false;
            ri--;
            if (!swapped) { break; } // is sorted
        } else {
            for (int i = ri; i > li; i--) {
                g->marks[2] = i;
                g->marks[0] = i-1;
                g->ar_access += 2;
                g->comparisions++;
                if (g->ar[i] < g->ar[i-1]) {
                    swap(g->ar, i, i-1);
                    swapped = true;
                }
                Game_draw(g);
                Game_delay(g);
                if (g->quit) { return; }
            }
            right = true;
            li++;
            if (!swapped) { break; } // is sorted
        }
    }
}

static void bubble_sort_stupid(Game *g)
{
    for (int a = 0; a < g->ar_len-1; a++) {
        g->marks[1] = a;
        for (int i = 0; i < g->ar_len-1; i++) {
            g->marks[2] = i;
            g->marks[0] = i+1;
            int item = g->ar[i];
            int item_to_right = g->ar[i+1];
            g->ar_access += 2;
            g->comparisions++;
            if (item > item_to_right) {
                g->ar[i] = item_to_right;
                g->ar[i+1] = item;
            }
            Game_draw(g);
            Game_delay(g);
            if (g->quit) { return; }
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
                Game_draw(g);
                Game_delay(g);
                if (g->quit) { return; }

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
        if (g->quit) { return; }
    }
}

static void merge_insert_sort(Game *g)
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
        Game_draw(g);
        Game_delay(g);
        if (g->quit) { return; }
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
        if (g->quit) { return; }

        // swap back because I'm done with merging
        t = g->ar;
        g->ar = tmpar;
        tmpar = t;

        // copy back from array to tempar because I need this to be in tmpar in a lower recursion level for merging.
        memcpy(tmpar + start, g->ar + start, sizeof(int)*(size_t)len);
    }
}

static void merge_sort(Game *g)
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
            if (g->quit) { return; }
        }
        // they were merged into tmpar so we need to copy back to array
        memcpy(g->ar, tmpar, (size_t)len*sizeof(int));
    }
}

static void merge_sort_bottom_up(Game *g)
{
    int *tmpar = malloc((size_t)g->ar_len * sizeof(int));
    _merge_sort_bottom_up(g, 0, g->ar_len, tmpar);
    free(tmpar);
}

static void counting_sort(Game *g)
{
    int *tmpar = calloc((size_t)g->ar_len, sizeof(int));
    for (int i = 0; i < g->ar_len; i++) {
        g->marks[0] = i;
        tmpar[g->ar[i]-1]++;
        g->ar_access++;
        Game_draw(g);
        Game_delay(g);
        if (g->quit) { return; }
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
        if (g->quit) { return; }
    }

    free(tmpar);
}

// Note: This was written before I learned the efficient partition algorithms.
static int partition(Game *g, int start, int end, int pivot_index)
{
    int pivot = g->ar[pivot_index];

    for (int i = start; i < end; ) {
        if (i == pivot_index) {
            // skip
            i++;
            continue;
        }

        g->marks[2] = i;
        g->marks[1] = pivot_index;
        g->ar_access++;
        int e = g->ar[i];

        g->comparisions++;
        if (e < pivot && i > pivot_index) {
            for (int j = i; j > pivot_index; j--) {
                g->marks[0] = j;
                g->ar_access++;
                swap(g->ar, j-1, j);
                Game_draw(g);
                Game_delay(g);
                if (g->quit) { return pivot_index; }
            } 
            pivot_index++;
            // we increment i here because although elements have been shifted up to i, and that element is the previous
            // element, which we have already checked. So we will skip it.
            i++;
        } else if (e > pivot && i < pivot_index) {
            for (int j = i; j < pivot_index; j++) {
                g->marks[0] = j;
                g->ar_access++;
                swap(g->ar, j, j+1);
                Game_draw(g);
                Game_delay(g);
                if (g->quit) { return pivot_index; }
            } 
            pivot_index--;
            // will NOT increment i here because elements have been shifted down to i and we need to check
            // it in the next iteration again. because it is the next element, which we have not checked yet.
        } else {
            // can just skip element if it's in the right position
            i++;
        }
    }

    return pivot_index;
}

static void quick_sort_(Game *g, int start, int end)
{
    if (end - start <= 1) { return; }

    // pivot_index will be selected at random
    int pivot_index = partition(g, start, end, (int)((float)rand()/RAND_MAX*(end-start) + start));
    if (g->quit) { return; }
    quick_sort_(g, start, pivot_index);
    quick_sort_(g, pivot_index+1, end);
}

static void quick_sort(Game *g) 
{
    quick_sort_(g, 0, g->ar_len);
}

SortFuncInterface sort_funcs[SORT_FUNCS_COUNT] = {
    {selection_sort, "Selection Sort"},
    {insertion_sort, "Insertion Sort"},
    {bubble_sort, "Bubble Sort"},
    {shaker_sort, "Shaker Sort"},
    {bubble_sort_stupid, "Bubble Sort (Stupid)"},
    {merge_insert_sort, "Merge Insert Sort"},
    {merge_sort, "Merge Sort (Top-Down)"},
    {merge_sort_bottom_up, "Merge Sort (Buttom-Up)"},
    {counting_sort, "Counting Sort"},
    {quick_sort, "Quick sort"},
};

