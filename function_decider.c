#include "function_decider.h"

char *warp = "warp";
char *peek = "peek";
char *pastevents = "pastevents";
char *seek = "seek";
char *proclore = "proclore";
char *purge = "purge";

void fixInput(int in, int out)
{
    dup2(in, 0);
    close(in);
    dup2(out, 1);
    close(out);
}

void get_commands(char *line)
{
    char *command;
    char copy1[1024], copy2[1024];
    strcpy(copy1, line);
    strcpy(copy2, line);
    command = strtok(line, "&;");
    int numberofstore = 0;
    while (command != NULL)
    {
        command = trimspace(command);
        numberofstore++, command = strtok(NULL, "&;");
    }

    char *store[numberofstore + 1];
    int i = 0;
    if (numberofstore == 0)
        return;
    char *beg = copy1;
    store[0] = strtok(copy1, "&;");

    while (store[i] != NULL && strcmp(store[i], "") != 0)
    {
        i++, store[i] = strtok(NULL, "&;");
    }
    for (int j = 0; j < numberofstore; j++)
    {
        int processid = 0;
        if (copy2[strlen(store[j]) + (store[j] - beg)] == '&')
            processid = 1;
        if (strcmp(store[j], "\n") == 0)
        {
            printf("%s", (store[j]));
            continue;
        }
        int backup_stdout = dup(STDOUT_FILENO);
        int backup_stdin = dup(STDIN_FILENO);
        pipechecker(store[j], processid);
        fixInput(backup_stdin, backup_stdout);
    }

    if (!past && !check)
    {
        FILE *fptr;
        char *file = (char *)malloc(sizeof(char) * 1024);
        strcpy(file, homeworkingdir);
        strcat(file, "/past.txt");
        fptr = fopen(file, "r+");
        if (fptr == NULL)
        {
            printf("Error!");
        }
        char buffer[1024];
        char *lastLine[16];
        for (int i = 0; i < 16; i++)
        {
            lastLine[i] = (char *)malloc(sizeof(char) * 1024);
        }
        int ct = 0;
        while (fgets(buffer, sizeof(buffer), fptr) != NULL)
        {
            strcpy(lastLine[ct++], buffer);
        }

        if (strcmp(copy2, lastLine[ct - 1]) == 0)
        {
            // return;
        }
        else
        {
            fclose(fptr);
            fptr = fopen(file, "w");
            if (ct < 15)
            {
                for (int i = 0; i < ct; i++)
                {
                    fprintf(fptr, "%s", lastLine[i]);
                }
                if (ct > 0)
                {
                    fprintf(fptr, "\n");
                }
                fprintf(fptr, "%s", line);
            }
            else
            {
                if (ftruncate(fileno(fptr), 0) != 0)
                {
                    perror("Error truncating file");
                    fclose(fptr);
                    return;
                }
                for (int i = 1; i < 15; i++)
                {
                    fprintf(fptr, "%s", lastLine[i]);
                }
                fprintf(fptr, "\n");
                fprintf(fptr, "%s", copy2);
            }
        }
        free(file);
        fclose(fptr);
        for (int i = 0; i < 16; i++)
        {
            free(lastLine[i]);
        }
    }
}

void pipechecker(char *command, int processid)
{
    int count = 0;
    for (int i = 0; i < strlen(command); i++)
    {
        if (command[i] == '|')
        {
            count++;
        }
    }
    if (count == 0)
    {
        redirection(command, processid, NULL, -1, 1);
        return;
    }
    int it = 0;
    char *commands[count + 10];
    char *x = strtok(command, "|");
    while (x != NULL)
    {
        commands[it++] = malloc(sizeof(char) * 1024);
        strcpy(commands[it - 1], x);
        x = strtok(NULL, "|");
    }
    int prev = 0;
    int output = dup(1);
    int input = dup(0);
    for (int i = 0; i <= count; i++)
    {
        commands[i] = trimspace(commands[i]);
    }
    int pipes[2];
    for (int i = 0; i < it; i++)
    {
        if (pipe(pipes) == -1)
        {
            return;
        }
        int pid = fork();
        if (pid == -1)
        {
            return;
        }
        else if (pid == 0)
        {
            dup2(prev, 0);
            if (i != it - 1)
            {
                dup2(pipes[1], 1);
            }
            close(pipes[0]);
            redirection(commands[i], processid, pipes, prev, 0);
            exit(0);
        }
        else
        {
            wait(NULL);
            close(pipes[1]);
            prev = pipes[0];
        }
    }
    dup2(output, 1);
    dup2(input, 0);
    close(output);
    close(input);
    return;
}

struct ProcessInfo *addProcess(struct ProcessInfo *head, pid_t pid, char *tokens[], int sz)
{
    struct ProcessInfo *newNode = (struct ProcessInfo *)malloc(sizeof(struct ProcessInfo));
    if (newNode == NULL)
    {
        perror("malloc");
        exit(1);
    }
    newNode->name = (char **)malloc(sizeof(char *) * (sz + 1));
    for (int i = 0; i < sz; i++)
    {
        newNode->name[i] = (char *)malloc(sizeof(char) * 1024);
        strcpy(newNode->name[i], tokens[i]);
    }
    newNode->sz = sz;
    newNode->pid = pid;
    newNode->next = head;
    return newNode;
}
struct ProcessInfo *findProcess(struct ProcessInfo *head, pid_t pidToFind)
{
    struct ProcessInfo *current = head;

    while (current != NULL)
    {
        if (current->pid == pidToFind)
        {
            // Process with the specified PID found
            return current;
        }
        current = current->next;
    }

    // Process with the specified PID not found
    return NULL;
}

struct ProcessInfo *removeProcess(struct ProcessInfo *head, pid_t pid)
{
    struct ProcessInfo *current = head;
    struct ProcessInfo *prev = NULL;

    while (current != NULL)
    {
        if (current->pid == pid)
        {
            if (prev == NULL)
            {
                head = current->next;
            }
            else
            {
                prev->next = current->next;
            }
            free(current);
            break;
        }
        prev = current;
        current = current->next;
    }

    return head;
}

Node *completedList = NULL;

void IO_handler(char *token, char *file)
{
    if (strcmp(token, ">") == 0)
    {
        int new_fd;
        if ((new_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
            perror("cannot redirect output");

        else
        {
            dup2(new_fd, STDOUT_FILENO);
            close(new_fd);
        }
    }
    else if (strcmp(token, ">>") == 0)
    {
        int new_fd;
        if ((new_fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644)) == -1)
            perror("cannot redirect output");

        else
        {
            dup2(new_fd, STDOUT_FILENO);
            close(new_fd);
        }
    }
    else if (strcmp(token, "<") == 0)
    {
        int new_fd;
        if ((new_fd = open(file, O_RDONLY)) == -1)
            perror("No such input file found!");
        else
        {
            dup2(new_fd, STDIN_FILENO);
            close(new_fd);
        }
    }
}

int redirection(char *line, int proccessid, int pipes[], int prev, int flag)
{
    char copy1[1024], copy2[1024];
    strcpy(copy1, line);
    strcpy(copy2, line);
    char *tokens[1000];
    int num_tokens = 0;
    tokens[0] = strtok(copy2, " \t\n");
    while (tokens[num_tokens] != NULL)
    {
        tokens[++num_tokens] = strtok(NULL, " \t");
    }
    char *store[num_tokens];
    for (int i = 0; i < num_tokens; i++)
    {
        store[i] = (char *)malloc(sizeof(char) * 1024);
    }
    int ct = 0;
    int f = 0;
    for (int i = 0; i < num_tokens; i++)
    {
        if (strcmp(">>", tokens[i]) == 0)
        {
            f = 1;
            IO_handler(tokens[i], tokens[i + 1]);
        }
        else if (strcmp(">", tokens[i]) == 0)
        {
            f = 1;
            IO_handler(tokens[i], tokens[i + 1]);
        }
        else if (strcmp("<", tokens[i]) == 0)
        {
            f = 1;
            IO_handler(tokens[i], tokens[i + 1]);
        }
        else if (f == 0)
        {
            strcpy(store[ct++], tokens[i]);
        }
    }
    processInput(store, ct, proccessid, pipes, prev);
    return 0;
}

int processInput(char *tokens[], int num_tokens, int proccessid, int pipes[], int prev)
{
    for (int i = 0; i < num_tokens; i++)
    {
        tokens[i] = trimspace(tokens[i]);
    }
    if (strcmp(tokens[0], warp) == 0)
    {
        warps(proccessid, num_tokens, tokens);
    }
    else if (strcmp(tokens[0], peek) == 0)
    {
        peeks(tokens, num_tokens, proccessid);
    }
    else if (strcmp(tokens[0], pastevents) == 0)
    {
        past = 1;
        pasts(tokens, num_tokens, proccessid);
    }
    else if (strcmp(tokens[0], seek) == 0)
    {
        seeks(tokens, num_tokens);
    }
    else if (strcmp(tokens[0], proclore) == 0)
    {
        if (num_tokens == 1)
        {
            proclore_function(getpid());
        }
        else
            proclore_function(atoi(tokens[1]));
    }
    else if (strcmp(tokens[0], "exit") == 0)
    {
        while (processList != NULL)
            {
                struct ProcessInfo *x = processList;
                processList = processList->next;
                
                if (kill(x->pid, 9) == 0)
                {
                    printf("Signal %d sent to process with PID %d\n", 9, x->pid);
                }
                else
                {
                    perror("kill");
                    return 1;
                }
                processList = removeProcess(processList, x->pid);
            }

            printf("\n");
            // exit(0);
        exit(1);
    }
    else if (strcmp(tokens[0], "ping") == 0)
    {
        pings(tokens, num_tokens);
    }
    else if (strcmp(tokens[0], "activities") == 0)
    {
        activity_handler(tokens, num_tokens);
    }
    else if (strcmp(tokens[0], "fg") == 0)
    {
        foreground(tokens, num_tokens);
    }
    else if (strcmp(tokens[0], "bg") == 0)
    {
        if (num_tokens != 2)
            perror("invalid command");
        else
            background(tokens, num_tokens);
    }
    else if (strcmp(tokens[0], "iMan") == 0)
    {
        if (num_tokens != 2)
            perror("invaild command");
        else
            fetchManPage(tokens[1]);
    }
    else if (strcmp(tokens[0], "neonate") == 0)
    {
        if (num_tokens != 3)
            perror("invalid command");
        else
            neonate(tokens);
    }
    else
    {
        systemproc(tokens, num_tokens, proccessid);
    }
    for (int i = 0; i < num_tokens; i++)
        free(tokens[i]);

    return 0;
}

void function_call(char *line)
{
    char *token = (char *)malloc(sizeof(char) * 1024);
    strcpy(token, line);
    {
        token = trimspace(token);
        get_commands(token);
    }
    free(token);
}