#include "warp.h"

void swap1(char **str1_ptr, char **str2_ptr)
{
    char *temp = *str1_ptr;
    *str1_ptr = *str2_ptr;
    *str2_ptr = temp;
}

void warps(int proccessid, int num_tokens, char *tokens[])
{
    if (proccessid)
    {
        printf("[%d]\n", getpid());
    }
    if (num_tokens == 1)
    {
        chdir(homeworkingdir);
        strcpy(currentworkingdir, homeworkingdir);
        printf("%s\n", currentworkingdir);
    }
    else
    {
        for (int i = 1; i < num_tokens; i++)
        {
            tokens[i] = trimspace(tokens[i]);

            if (strcmp(tokens[i], "-") == 0)
            {
                chdir(prevworkingdir);
                swap1(&currentworkingdir, &prevworkingdir);
                printf("%s\n", currentworkingdir);
            }
            else if (strcmp(tokens[i], "~") == 0)
            {
                strcpy(prevworkingdir, currentworkingdir);
                strcpy(currentworkingdir, homeworkingdir);
                chdir(currentworkingdir);
                printf("%s\n", currentworkingdir);
            }
            else if (strcmp(tokens[i], ".") == 0)
            {
                printf("%s\n", currentworkingdir);
            }
            else if (strcmp(tokens[i], "") == 0)
            {
                chdir(homeworkingdir);
                strcpy(prevworkingdir, currentworkingdir);
                strcpy(currentworkingdir, homeworkingdir);
                printf("%s\n", currentworkingdir);
            }
            else if (strcmp(tokens[i], " ") == 0)
            {
                chdir(homeworkingdir);
                strcpy(prevworkingdir, currentworkingdir);
                strcpy(currentworkingdir, homeworkingdir);
                printf("%s\n", currentworkingdir);
            }
            else if (tokens[i][0] == '~')
            {
                strcpy(prevworkingdir, currentworkingdir);
                chdir(homeworkingdir);
                char *s1 = (char *)malloc(sizeof(char) * 1024);
                for (int j = 2; j < strlen(tokens[i]); j++)
                {
                    s1[j - 2] = tokens[i][j];
                }
                s1[strlen(tokens[i]) - 2] = '\0';
                if (chdir(s1) == -1)
                    printf("error");

                getcwd(currentworkingdir, 1024);
                printf("%s\n", currentworkingdir);
            }
            else
            {
                strcpy(prevworkingdir, currentworkingdir);
                if (chdir(tokens[i]) == -1)
                    printf("error");

                getcwd(currentworkingdir, 1024);
                printf("%s\n", currentworkingdir);
            }
        }
    }
}