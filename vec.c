// CS3650 HW04 Starter Code
// Author: Nat Tuck
// Some changes were taken from svec.c written by Nat Tuck from lecture notes
// ccs.neu.edu/home/ntuck/courses/2019/01/cs3650/notes/09-calculator/calc/svec.c

#include <string.h>
#include <stdlib.h>
#include <assert.h>


#include "vec.h"

/*
typedef struct vec {
    char** data;
    long size;
    long cap;
} vec;
*/

vec*
make_vec()
{
    vec* xs = malloc(sizeof(vec));
    xs->size = 0;
    xs->cap  = 2;
    xs->data = malloc(2 * sizeof(char*));
    memset(xs->data, 0, 2 * sizeof(char*));
    return xs;
}

void
free_vec(vec* xs)
{
    for (int i = 0; i < xs->size; i++) {
        if (xs->data[i] != 0) {
            free(xs->data[i]);
        }
    }
    free(xs->data);
    free(xs);
}

char*
vec_get(vec* xs, long xx)
{
    assert(xx >= 0 && xx < xs->size);
    return xs->data[xx];
}

void
push_vec(vec* xs, char* xx)
{
    if (xs->size >= xs->cap) {
        xs->cap *= 2;
        xs->data = (char**) realloc(xs->data, xs->cap * sizeof(char*));
    }

    xs->data[xs->size] = strdup(xx);
    xs->size++;
}

