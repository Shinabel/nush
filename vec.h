// CS3650 HW04 Starter Code
// Author: Nat Tuck

#ifndef VEC_H
#define VEC_H

typedef struct vec {
    char** data;
    long size;
    long cap;
} vec;

vec* make_vec();
char* vec_get(vec* xs, long i);
void free_vec(vec* xs);
void push_vec(vec* xs, char* xx);
#endif
