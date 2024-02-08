#include "peek.h"

int cmpstringp(const void *p1, const void *p2)
{
    return strcmp(*(char *const *)p1, *(char *const *)p2);
}

void peeks(char *tokens[], int num_tokens, int proccessid)
{
    if (proccessid)
    {
        printf("[%d]\n", getpid());
    }
    int hidden_flag = 0, details_flag = 0;
    if (num_tokens == 1)
    {
        LS(".", 0, 0);
    }
    else if (num_tokens == 2)
    {
        if (strcmp(tokens[1], "-a") == 0)
        {
            LS(".", 1, 0);
        }

        else if (strcmp(tokens[1], "-l") == 0)
        {
            LS(".", 0, 1);
        }
        else if (strcmp(tokens[1], ".") == 0)
        {
            LS(tokens[1], hidden_flag, details_flag);
        }
        else if (strcmp(tokens[1], "..") == 0)
        {
            LS(tokens[1], hidden_flag, details_flag);
        }
        else if (strcmp(tokens[1], "~") == 0)
        {
            LS(homeworkingdir, hidden_flag, details_flag);
        }
        else if (strcmp(tokens[1], "-") == 0)
        {
            LS(prevworkingdir, hidden_flag, details_flag);
        }
        else if (strcmp(tokens[1], "-la") == 0)
        {
            LS(prevworkingdir, 1, 1);
        }
        else
        {
            LS(tokens[1], hidden_flag, details_flag);
        }
    }
    else if (num_tokens == 3)
    {
        if (strlen(tokens[1]) == 2)
        {
            if (strcmp(tokens[1], "-a") == 0)
            {
                hidden_flag = 1;
            }
            else if (strcmp(tokens[1], "-l") == 0)
            {
                details_flag = 1;
            }
            else
            {
                printf("Invalid Command\n");
                return;
            }
        }
        else if (strlen(tokens[1]) == 3)
        {
            if (strcmp(tokens[1], "-al") == 0)
                hidden_flag = 1, details_flag = 1;
            else if (strcmp(tokens[1], "-la") == 0)
                details_flag = 1, hidden_flag = 1;
            else
            {
                printf("Invalid Command\n");
                return;
            }
        }
        else
        {
            printf("Invalid Command\n");
            return;
        }
        LS(tokens[2], hidden_flag, details_flag);
    }
    else if (num_tokens == 4)
    {
        int f1 = 0, f2 = 0;
        if (strcmp(tokens[1], "-l") == 0 || strcmp(tokens[1], "-a") == 0)
        {
            f1 = 1;
            if (strcmp(tokens[1], "-l") == 0)
            {
                details_flag = 1;
            }
            else
            {
                hidden_flag = 1;
            }
        }
        if (strcmp(tokens[2], "-l") == 0 || strcmp(tokens[2], "-a") == 0)
        {
            f2 = 1;
            if (strcmp(tokens[2], "-l") == 0)
            {
                details_flag = 1;
            }
            else
            {
                hidden_flag = 1;
            }
        }
        if (f1 && f2)
        {
            LS(tokens[3], hidden_flag, details_flag);
        }
        else
        {
            printf("Invalid Command\n");
            return;
        }
    }
    else
    {
        printf("Invalid command\n");
        return;
    }
}

char *months(int no, char *name)
{
    if (no == 0)
        strcpy(name, "Jan");

    else if (no == 1)
        strcpy(name, "Feb");

    else if (no == 2)
        strcpy(name, "Mar");
    else if (no == 3)
        strcpy(name, "Apr");

    else if (no == 4)
        strcpy(name, "May");

    else if (no == 5)
        strcpy(name, "Jun");

    else if (no == 6)
        strcpy(name, "Jul");

    else if (no == 7)
        strcpy(name, "Aug");

    else if (no == 8)
        strcpy(name, "Sep");

    else if (no == 9)
        strcpy(name, "Oct");

    else if (no == 10)
        strcpy(name, "Nov");

    else if (no == 11)
        strcpy(name, "Dec");

    return name;
}

void LS(char *dir, int op_a, int op_l)
{

    if (strcmp(dir, "~") == 0)
    {
        strcpy(dir, homeworkingdir);
    }
    else if (strcmp(dir, "-") == 0)
    {
        strcpy(dir, prevworkingdir);
    }

    DIR *dh = opendir(dir);
    if (!dh)
    {
        if (errno = ENOENT)
        {
            perror("Directory doesn't exist");
        }
        else
        {
            perror("Unable to read directory");
        }
        return;
    }
    int numberofiles = 0;

    while (readdir(dh) != NULL)
    {
        numberofiles++;
    }

    closedir(dh);
    dh = opendir(dir);
    char **store = (char **)malloc(sizeof(char *) * numberofiles);

    for (int i = 0; i < numberofiles; i++)
        store[i] = (char *)malloc(sizeof(char) * 1024);

    int ct = 0;
    struct dirent *d;
    int totalsize = 0;
    while ((d = readdir(dh)) != NULL)
    {
        if (!op_a && d->d_name[0] == '.')
            continue;
        struct stat data;
        char *newdir = (char *)malloc(sizeof(char) * 1024);
        strcpy(newdir, dir);
        strcat(newdir, "/");
        strcat(newdir, d->d_name);
        if (lstat(newdir, &data) == -1)
        {
            printf("%s\n", d->d_name);
            perror("Error getting stat struct");
            return;
        }
        strcpy(store[ct], d->d_name);
        ct++;
        totalsize += data.st_blocks;
    }
    if (op_l)
        printf("total %d\n", totalsize / 2);
    qsort(store, ct, sizeof(char *), cmpstringp);
    for (int i = 0; i < ct; i++)
    {
        struct stat data;
        char *newdir = (char *)malloc(sizeof(char) * 1024);
        strcpy(newdir, dir);
        strcat(newdir, "/");
        strcat(newdir, store[i]);
        if (lstat(newdir, &data) == -1)
        {
            printf("%s\n", store[i]);
            perror("Error getting stat struct");
            return;
        }

        int c1 = 0, c2 = 0, c3 = 0;
        if (data.st_mode & __S_IFDIR)
        {
            c1 = 1;
        }
        else if (data.st_mode & S_IXUSR)
        {
            c2 = 1;
        }
        else if (S_ISLNK(data.st_mode))
        {
            c3 = 1;
        }
        if (op_l)
        {
            if (c1)
            {
                printf("%s", "\033[1m\033[34m");
            }
            else if (c2)
            {
                printf("%s", "\033[1m\033[32m");
            }
            else if (c3)
            {
                printf("%s", "\033[1m\033[33m");
            }
            else
            {
                printf("%s", "\033[1m\033[0m");
            }
            struct passwd *pws = getpwuid(data.st_uid);
            char *user_name = pws->pw_name;
            struct group *grp = getgrgid(data.st_gid);
            char *group_name = grp->gr_name;
            char modeStr[11]; // 9 permissions + 1 file type + 1 null terminator
            mode_t mode = data.st_mode;

            snprintf(modeStr, sizeof(modeStr), "%c%c%c%c%c%c%c%c%c%c",
                     (S_ISDIR(mode)) ? 'd' : '-',
                     (mode & S_IRUSR) ? 'r' : '-',
                     (mode & S_IWUSR) ? 'w' : '-',
                     (mode & S_IXUSR) ? 'x' : '-',
                     (mode & S_IRGRP) ? 'r' : '-',
                     (mode & S_IWGRP) ? 'w' : '-',
                     (mode & S_IXGRP) ? 'x' : '-',
                     (mode & S_IROTH) ? 'r' : '-',
                     (mode & S_IWOTH) ? 'w' : '-',
                     (mode & S_IXOTH) ? 'x' : '-');

            printf("%s", modeStr);
            time_t l_m = data.st_mtime;
            struct tm last_mod;
            localtime_r(&l_m, &last_mod);
            int month = last_mod.tm_mon;
            int day = last_mod.tm_mday;
            int min = last_mod.tm_min;
            int hour = last_mod.tm_hour;
            char *monthName = (char *)malloc(sizeof(char) * 5);
            months(month, monthName);
            printf("%2ld ", data.st_nlink);
            printf("%s  %s ", user_name, group_name);
            printf("%5ld ", data.st_size);
            printf("%s %02d %02d:%02d ", monthName, day, hour, min);
            printf("%s ", store[i]);
            printf("%s\n", "\033[1m\033[0m");
        }
        else
        {
            if (c1)
            {
                printf("%s", "\033[1m\033[34m");
            }
            else if (c2)
            {
                printf("%s", "\033[1m\033[32m");
            }
            else if (c3)
            {
                printf("%s", "\033[1m\033[33m");
            }
            else
            {
                printf("%s", "\033[1m\033[0m");
            }
            printf("%s\n", store[i]);
            printf("%s", "\033[1m\033[0m");
        }
        free(store[i]);
    }
    closedir(dh);
    free(d);
}
