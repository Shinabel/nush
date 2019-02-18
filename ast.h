// Ideas were taken from ast.h written by Nat Tuck from lecture notes
// ccs.neu.edu/home/ntuck/courses/2019/01/cs3650/notes/09-calculator/calc/ast.h

#ifndef NUSH_AST_H
#define NUSH_AST_H

#include <stdlib.h>

typedef struct nush_ast
{
    char* op;
    // Op is either:
    // one of: '<', '>', '|', '&', ';', '&&', '||'
    //     or: "run" for indicating command (data is the command)
    struct nush_ast* arg0;
    struct nush_ast* arg1;
    int size;
    char** data; 
} nush_ast;

nush_ast* make_ast_cmd(int size, char** data);
nush_ast* make_ast_op(char* op, nush_ast* a0, nush_ast* a1);
nush_ast* make_ast_redir(char* op, nush_ast* a0, char** data);

void print_ast(nush_ast* ast);
void free_ast(nush_ast* ast);
#endif
