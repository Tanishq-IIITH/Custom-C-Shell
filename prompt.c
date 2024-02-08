#include "headers.h"
#define clear() printf("\033[H\033[J")

void prompt(char *dir)
{
    char *username = getenv("USER");
    char cwd[1024];
    char hostname[1023 + 1];
    gethostname(hostname, 1023 + 1);
    getcwd(cwd, 1024);

    printf("\033[1;32m%s@\033[0m", username);
    printf("\033[1;32m%s\033[0m", hostname);
    if (strlen(cwd) >= strlen(homeworkingdir))
    {
        printf(":");
        printf("\033[1;34m~\033[0m");
        int x = strlen(homeworkingdir);
        for (int i = x; i < strlen(cwd); i++)
        {
            printf("\033[1;34m%c\033[0m", cwd[i]);
        }
        if (tim > 2)
        {
            printf("%s ", com);
            printf(": %ds", tim);
        }
        tim = 0;
    }
    else
    {
        printf(":\033[1;34m%s\033[0m", cwd);
        if (tim > 2)
        {
            printf(" %s : %ds ", com, tim);
        }
        tim = 0;
    }
    printf(">");

    return;
}
