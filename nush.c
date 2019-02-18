#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "vec.h"
#include "tokenize.h"
#include "parser.h"

int check_ops(nush_ast* cmds);

void
check_rv(int rv)
{
    if (rv==-1) {
        perror("fail");
        exit(1);
    }
}

// Execute the command 
// At this stage, all the operations other than redirect operation
// should've been taken care of.
int
execute(nush_ast* cmds)
{
    if (cmds->size > 0) {
       if (strcmp(cmds->data[0], "cd") == 0) {
           if (cmds->size > 1) {
               return chdir(cmds->data[1]);
           } else {
               return chdir("~");
           }
       }
       if (strcmp(cmds->data[0], "exit") == 0) {
           exit(0);
       }

       int cpid;
       if ((cpid = fork())) {
           int status;
           waitpid(cpid, &status, 0);
           return status;
       }
        else {
           // redirect operation
            if ((strcmp(cmds->op, ">") == 0) || (strcmp(cmds->op, "<") == 0)) {
                int fd;
                if (strcmp(cmds->op, "<") == 0) {
                    fd = open(cmds->data[0], O_RDONLY);
                    close(0);
                }
                else {
                    fd = open(cmds->data[0], O_CREAT | O_APPEND | O_WRONLY, 0644);
                    close(1);
                }
                dup(fd);
                close(fd);
                cmds = cmds->arg0;
            }
            execvp(cmds->data[0], cmds->data);
        }
    }    
}

// Execute background operation
int
exec_back(nush_ast* cmds)
{
    int cpid;
	if ((cpid = fork())) {
        return check_ops(cmds->arg1);
    }
    else {
	    check_ops(cmds->arg0);
        exit(0);
	}
}

// Execute pipe operation
int
exec_pipe(nush_ast* ast)
{
    int rv;
    int pipe_fds[2];
    rv = pipe(pipe_fds);
    check_rv(rv);
    
    int p_read = pipe_fds[0];
    int p_write = pipe_fds[1];
    int cpid;

    if ((cpid = fork())) {
        if (cpid == -1) {
            perror("error");
            return -1;
        }
        
        int status;
        waitpid(cpid, &status, 0);
        close(p_write);
        dup2(p_read,0);
        check_ops(ast->arg1);
        return status;
    }
    else {
        close(p_read);
        dup2(p_write,1);
        check_ops(ast->arg0);
        exit(0);
    }
}

// execute accordingly in logic junction
// according to the case, cs
int
exec_logic(nush_ast* cmds, int cs)
{
    int v = check_ops(cmds->arg0);
    // for cs 0 ("||"), execute arg1 only if arg0 is false
    // for cs 1 ("&&"), execute arg1 only if arg0 is true
    if ((cs == 0 && v != 0) || (cs == 1 && v == 0)) {
        return check_ops(cmds->arg1);
    }
}

// check the operations and run correspondingly
int
check_ops(nush_ast* cmds)
{
    if (strcmp(cmds->op, "|") == 0) {
        return exec_pipe(cmds);
    } else if (strcmp(cmds->op, "&") == 0) {
        return exec_back(cmds);
    } else if (strcmp(cmds->op, "||") == 0) {
        return exec_logic(cmds, 0);
    } else if (strcmp(cmds->op, "&&") == 0) {
        return exec_logic(cmds, 1);
    } else if (strcmp(cmds->op, ";") == 0) {
        // execute arg1 no matter what arg0 comes out to be
        check_ops(cmds->arg0);
        return check_ops(cmds->arg1);
    } else {
        // command with no operation (or only redirect operations)
        return execute(cmds);
    }
}

int
main(int argc, char* argv[])
{
    char cmd[256];
    FILE* input;

    if (argc > 1) {
        input = fopen(argv[1], "r");
    }
    
    while(1) {
        if (argc < 2) {
            printf("nush$ ");
            fflush(stdout);
            // checks if there were any stdin
            if (fgets(cmd,256,stdin) == NULL) {
                exit(1);
            } 
        } else {
            fgets(cmd,256,input);
            fflush(input);
            // checks for end of the file
            if(feof(input)) {
                fclose(input);
                exit(1);
            }
        }
            vec* toks = tokenize(cmd);
            nush_ast* cmds = parse(toks);
            free_vec(toks);
            check_ops(cmds);
//            print_ast(cmds);
            free_ast(cmds);
    }
 
    return 0;
}
