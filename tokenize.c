
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

#include "vec.h"
#include "tokenize.h"

int
is_word(char chr)
{
    // ispunct
    char ext[] = {'.', '\'', ',',  '/', '-', '_', ':', '{', '}', '(', ')',
    '?', '#', '$','@', '!', '`', '~', '+', '=', '"', '*', '%', '^'};
    int exist = 0; // indicator whether the given chr was in ext

    for (int i = 0; i < sizeof(ext); i++) {
        if (chr == ext[i]) {
            exist = 1;
        }
    }

    return isalnum(chr) || exist;
}

int
is_op(char c)
{
    return c == '<' || c == '>' || c == ';'
          || c == '&' || c == '|';
}

// read the line
char*
read_line(char* text, int ii)
{
    int nn = 0;
    while (is_word(text[ii + nn])) {
        ++nn;
    }

    char* toks = malloc(nn + 1);
    memcpy(toks, text + ii, nn);
    toks[nn] = 0;
    return toks;
}


// tokenize the input line into a vec
vec*
tokenize(char* text)
{
    vec* xs = make_vec();
    int size = strlen(text);
    int i = 0;
    while (i < size) {
        if (isspace(text[i])) {
            i++;
        } else if (text[i] == text[i + 1] 
            && (text[i] == '&' || text[i] == '|')) { // checks for && and ||
            char* operations = malloc(3 * sizeof(char));
            operations[0] = text[i];
            operations[1] = text[i];
            operations[2] = 0;
            push_vec(xs, operations);
            i += 2;
        } else if (is_op(text[i])) {
            char* operation = malloc(2 * sizeof(char));
            operation[0] = text[i];
            operation[1] = 0;
            push_vec(xs, operation);
            i++;
        } else {
            char* line = read_line(text, i);
            push_vec(xs, line);
            i += strlen(line);
            free(line);
        }
    }
    return xs;
}

