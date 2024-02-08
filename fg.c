#include "fg.h"

void change_foreground(int cpid)
{

    foregroundProcessPid = cpid;
    int status;
    kill(cpid, SIGCONT);
    waitpid(cpid, &status, WUNTRACED);
    int pd=getpid();
    if (tcsetpgrp(STDIN_FILENO, getpgid(pd)) < 0)
    {
        perror("cant get terminal back");
    }

    if (WIFEXITED(status))
    {
        processList = removeProcess(processList, cpid);
    }
    else if (WIFSTOPPED(status))
    {
        printf("\n");
        struct ProcessInfo *x = findProcess(processList, cpid);
        printf(" %d ", x->pid);
        int sz=x->sz;
        for (int i = 0; i < x->sz; i++)
        {
            printf("%s ", x->name[i]);
        }
        printf("Stopped\n");
    }
    else if (WIFSIGNALED(status))
    {
        processList = removeProcess(processList, cpid);
    }
}
void foreground(char **tokens, int num_tokens)
{
    if (num_tokens != 2)
    {
        fprintf(stderr, "fg: invalid format! correct format is fg <pid>\n");
        return;
    }
    int t = (int)strtol(tokens[1], NULL, 10);
    if (t <= 0)
    {
        fprintf(stderr, "pid does not exist \n");
        return;
    }
    change_foreground(t);
}