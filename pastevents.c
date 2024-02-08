#include "pastevents.h"

void get_commands2(char *line)
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
    if (numberofstore <= 0)
        return;
    char *beg = copy1;
    store[0] = strtok(copy1, "&;");

    while (store[i] != NULL && strcmp(store[i], "") != 0)
    {
        i++, store[i] = strtok(NULL, "&;");
    }
    // int past = 0;
    int backup_stdout = dup(STDOUT_FILENO);
    int backup_stdin = dup(STDIN_FILENO);
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
        pipechecker(store[j], processid);
        fixInput(backup_stdin, backup_stdout);
    }
}

int pasts(char *tokens[], int num_tokens, int proccessid)
{
    if (proccessid)
    {
        printf("[%d]\n", getpid());
    }
    FILE *fptr;
    char *file = (char *)malloc(sizeof(char) * 1024);
    strcpy(file, homeworkingdir);
    strcat(file, "/past.txt");
    fptr = fopen(file, "r+");
    if (fptr == NULL)
    {
        printf("Error!");
    }
    if (num_tokens == 3)
    {
        if (strcmp(tokens[1], "execute") == 0)
        {

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
            get_commands2(lastLine[ct - (atoi(tokens[2]))]);
        }
        else
        {
            printf("Invalid Command\n");
            return 1;
        }
    }
    else if (num_tokens == 2)
    {
        if (strcmp(tokens[1], "purge") == 0)
        {
            if (ftruncate(fileno(fptr), 0) != 0)
            {
                perror("Error truncating file");
                fclose(fptr);
                return 1;
            }
        }
        else
        {
            printf("Invalid Command\n");
            return 1;
        }
    }
    else if (num_tokens == 1)
    {
        char buffer[1024];
        char lastLine[1024] = "";

        while (fgets(buffer, sizeof(buffer), fptr) != NULL)
        {
            strcpy(lastLine, buffer);
            printf("%s", lastLine);
        }
        printf("\n");
    }
    else
    {
        fclose(fptr);
        printf("Invalid Command\n");
        return 0;
    }
    fclose(fptr);
    return 1;
}