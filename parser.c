#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ast.h"
#include "parser.h"
#include "vec.h"

// split vec
vec*
split_vec(vec* xs, int s, int e)
{
    vec* ys = make_vec();
    for (int i = s; i < e; i++) {
        push_vec(ys, vec_get(xs, i));
    }
    return ys;
}


// Convert vec to ast
nush_ast*
parse(vec* toks)
{
    char* ops[] = {"|", "||", "&", "&&", ";", ">", "<"};
    for (int ii = toks->size - 1; ii >= 0; ii--)
    {
        char* in = vec_get(toks, ii);
        // checks and split the regular operations
        for (int jj = 0; jj < 5; jj++) {
            char* op = ops[jj];
            if (strcmp(in, op) == 0) {
                vec* t0 = split_vec(toks, 0, ii);
                vec* t1 = split_vec(toks, ii+1, toks->size);                
                nush_ast* ast = make_ast_op(op, parse(t0), parse(t1));
                free_vec(t0);
                free_vec(t1);
                return ast;
            }
        }
    }

    for (int ii = 0; ii < toks-> size; ii++)
    {   
        // after all the regular operation is separated accordingly
        // checks and split the redirect operations
        // let's just assume it is always in cmd a > b form
        char* in = vec_get(toks, ii);
        for (int jj = 5; jj < 7; jj++) {
            char* op = ops[jj];
            if (strcmp(in, op) == 0) {
                char** ri = malloc((ii+1) * sizeof(char*));

                for (int i = 0; i < ii; i++) {
                    char* c = vec_get(toks, i);
                    int l = strlen(c);
                    ri[i] = malloc((l+1) * sizeof(char));
                    memcpy(ri[i], c, l);
                    ri[i][l] = 0;
                }
                ri[ii] = 0;
                
                nush_ast* ast = make_ast_cmd(ii, ri);
                char* c = vec_get(toks, ii+1);
                
                char** ro = malloc(3 * sizeof(char*));
                ro[0] = malloc((strlen(c)+1) * sizeof(char));
                memcpy(ro[0], c, strlen(c));
                ro[1] = 0;

                nush_ast* astr = make_ast_redir(in, ast, ro);
                return astr;
            }
        }
    }

    // at here, it would be "cmd arg"
    char** args = malloc((toks->size+1) * sizeof(char*));
    for (int ii = 0; ii < toks->size; ++ii) {
        char* c = vec_get(toks, ii);
        int l = strlen(c);
        
        args[ii] = malloc((l+1) * sizeof(char));
        memcpy(args[ii], c, l);
        args[ii][l] = 0;
    }
    args[toks->size] = 0;
    nush_ast* ast = make_ast_cmd(toks->size, args);
    return ast; 
}
