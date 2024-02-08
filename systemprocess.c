#include "systemprocess.h"

void childHandler(int signum)
{
    int status;
    pid_t completedPid;
    while ((completedPid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        Node *newNode = (Node *)malloc(sizeof(Node));
        if (newNode == NULL)
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
        newNode->pid = completedPid;
        newNode->status = status;
        strncpy(newNode->process_name, s, 100 - 1);
        newNode->process_name[100 - 1] = '\0'; // Ensure null-termination
        newNode->next = completedList;
        completedList = newNode;
    }
}

void systemproc(char *tokens[], int num_tokens, int proccessid)
{
    char *command = strdup(tokens[0]);
    char *argv[num_tokens + 1];
    for (int i = 0; i < num_tokens; i++)
    {
        argv[i] = strdup(tokens[i]);
    }
    if (argv[num_tokens - 1][strlen(argv[num_tokens - 1]) - 1] == '\n')
        argv[num_tokens - 1][strlen(argv[num_tokens - 1]) - 1] = '\0';
    argv[num_tokens] = NULL;

    double start_time, end_time;
    start_time = time(NULL);

    int rc = fork();
    processList = addProcess(processList, rc, tokens, num_tokens);
    if (!proccessid)
        foregroundProcessPid = rc;
    if (rc < 0)
        perror("creating child process failed\n");
    else if (rc == 0)
    {
        if (proccessid)
            setpgid(0, 0);
        if (execvp(command, argv) == -1)
        {
            check = 1;
            fprintf(stderr, "invalid command\n");
        }
    }
    else if (rc > 0)
    {

        if (!proccessid)
            waitpid(rc, NULL, WUNTRACED);
        else
            printf("child with pid [%d] sent to background\n", rc);
        strcpy(s, tokens[0]);
        strcpy(command_pid[rc], tokens[0]);
        signal(SIGCHLD, childHandler);
        end_time = time(NULL);
        int duration = (int)(end_time - start_time);
        if (duration > 2.0 && !proccessid)
        {
            tim = duration;
            strcpy(com, command);
        }
    }
}