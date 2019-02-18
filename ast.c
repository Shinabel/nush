// Ideas were taken from ast.h written by Nat Tuck from lecture notes
// ccs.neu.edu/home/ntuck/courses/2019/01/cs3650/notes/09-calculator/calc/ast.h


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ast.h"

// Initialize values in ast
void
set_ast(nush_ast* ast, char* op, nush_ast* a0, nush_ast* a1, int size, char** data) 
{
    ast->op = op;
    ast->arg0 = a0;
    ast->arg1 = a1;
    ast->size = size;
    ast->data = data;
}

// Create an ast for commands
nush_ast*
make_ast_cmd(int size, char** data)
{
    nush_ast* ast = malloc(sizeof(nush_ast));
    set_ast(ast, "run", NULL, NULL, size, data);
    return ast;

}

// Create an ast for operations
nush_ast*
make_ast_op(char* op, nush_ast* a0, nush_ast* a1)
{
    nush_ast* ast = malloc(sizeof(nush_ast));
    set_ast(ast, op, a0, a1, 0, NULL);
    return ast;
}

// Create an ast for redirect operations
nush_ast*
make_ast_redir(char* op, nush_ast* a0, char** data) {
    nush_ast* ast = malloc(sizeof(nush_ast));
    if (strcmp(op, "<") == 0) {
        set_ast(ast, "<", a0, NULL, 1, data);
    }
    if (strcmp(op, ">") == 0) {
        set_ast(ast, ">", a0, NULL, 1, data);
    }
    return ast;
}

// Free an ast
void free_ast(nush_ast* ast)
{
    if (ast) {
        if (ast->arg0) {
            free_ast(ast->arg0);
        }
        if (ast->arg1) {
            free_ast(ast->arg1);
        }
        if (ast->data) {
            for (int i = 0; i < ast->size; ++i) {
                if (ast->data[i] != 0) {
                    free(ast->data[i]);
                }
            }
            free(ast->data);
        }
        free(ast);
    }
}


void print_ast(nush_ast* ast)
{
    if(ast) {
       if (ast->op) {
            printf("%s\n", ast->op);
        }
    
        if (ast->size) {
            printf("%d\n", ast->size);
        }

        if (ast->data) {       
            for (int i = 0; i < ast->size; ++i) {
                printf("%s ", ast->data[i]);
            }
            printf("\n");   
        }
        if (ast->arg0) {
            print_ast(ast->arg0);
        }
        if (ast->arg1) {
            print_ast(ast->arg1);
        }
    }
}

