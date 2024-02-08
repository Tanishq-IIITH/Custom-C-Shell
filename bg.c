#include "bg.h"

void background(char **tokens, int n)
{
    if (n != 2)
    {
        fprintf(stderr, "bg: invalid format! bg <pid number>\n");
        return;
    }
    int chk = (int)strtol(tokens[1], NULL, 10);
    if (chk <= 0)
    {
        fprintf(stderr, "pid does not exist \n");
        return;
    }

    if (kill(chk, SIGCONT) == -1)
    {
        perror("bg");
    }
}