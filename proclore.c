#include "proclore.h"
void proclore_function(pid_t pid)
{
    pid_t pgid = getpgid(pid);
    char status;
    char statPath[1024];
    char exePath[1024];
    char vmPath[1024];
    printf("PID: %d\n", pid);
    sprintf(statPath, "/proc/%d/stat", pid);
    char total[3];
    FILE *sf = fopen(statPath, "r");
    if (sf == NULL)
    {
        perror("Error opening stat file");
    }
    else
    {
        fscanf(sf, "%*d %*s %c", &status);
        fclose(sf);
        total[0] = status;
    }
    if (pid == pgid)
    {
        strcat(total, "+");
    }
    printf("process status :%s\n", total);
    printf("Process Group: %d\n", pgid);
    sprintf(vmPath, "/proc/%d/status", pid);
    FILE *vvf = fopen(vmPath, "r");
    if (vvf == NULL)
    {
        perror("Error opening");
    }
    else
    {
        char rem[1024];
        while (fgets(rem, sizeof(rem), vvf))
        {
            if (strstr(rem, "VmSize:") != NULL)
            {
                printf("%s", rem);
                break;
            }
        }
        fclose(vvf);
    }
    sprintf(exePath, "/proc/%d/exe", pid);
    char ppt[1024];
    ssize_t lenppt = readlink(exePath, ppt, sizeof(ppt) - 1);
    if (lenppt == -1)
    {
        perror("Error reading executable path");
    }
    else
    {
        ppt[lenppt] = '\0';
        printf("executable Path: %s\n", ppt);
    }
}
