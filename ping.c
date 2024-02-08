#include "ping.h"

void pings(char *tokens[], int num_tokens)
{
    int signals = atoi(tokens[2]);
    int pid = atoi(tokens[1]);
    signals %= 32;

    if (kill(pid, signals) == 0)
    {
        printf("Signal %d sent to process with PID %d\n", signals, pid);
    }
    else
    {
        perror("kill");
        return;
    }
}