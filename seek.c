#include "seek.h"

void removeExtension(char *filename)
{
    char *dot = strrchr(filename, '.');
    if (dot != NULL)
    {
        *dot = '\0';
    }
}

void seeks(char *tokens[], int num_tokens)
{
    int e_flag = 0, f_flag = 0, d_flag = 0;
    if (num_tokens == 2)
    {
        if (!rec(currentworkingdir, 1, 0, 1, tokens[1]))
        {
            printf("No match found!\n");
        }

        d_check = 0, f_check = 0;
        return;
    }
    else if (num_tokens == 3)
    {
        // printf("OK");
        if (rec(tokens[num_tokens - 1], 1, 0, 1, tokens[num_tokens - 2]))
        {
        }
        else
        {
            printf("No match found!\n");
        }
        d_check = 0, f_check = 0;
        return;
    }
    else if (num_tokens == 4)
    {
        if (strcmp("-e", tokens[1]) == 0)
        {
            e_flag = 1;
        }
        else if (strcmp("-d", tokens[1]) == 0)
        {
            d_flag = 1;
        }
        else if (strcmp("-f", tokens[1]) == 0)
        {
            f_flag = 1;
        }
        else
        {
            printf("Invalid Command!\n");
            return;
        }
    }
    else if (num_tokens == 5)
    {
        if (strcmp("-e", tokens[1]) == 0)
        {
            e_flag = 1;
        }
        else if (strcmp("-d", tokens[1]) == 0)
        {
            d_flag = 1;
        }
        else if (strcmp("-f", tokens[1]) == 0)
        {
            f_flag = 1;
        }
        else
        {
            printf("Invalid Command!\n");
            return;
        }
        if (strcmp("-e", tokens[2]) == 0)
        {
            e_flag = 1;
        }
        else if (strcmp("-d", tokens[2]) == 0)
        {
            d_flag = 1;
        }
        else if (strcmp("-f", tokens[2]) == 0)
        {
            f_flag = 1;
        }
        else
        {
            printf("Invalid Command!\n");
            return;
        }
        if (d_flag && f_flag)
        {
            d_check = 0, f_check = 0;
            printf("Invalid flags !\n");
            return;
        }
    }
    else
    {
        printf("Invalid Command !\n");
        return;
    }
    if (d_flag == 0 && f_flag == 0)
    {
        d_flag = 1, f_flag = 1;
    }
    int c = rec(tokens[num_tokens - 1], d_flag, e_flag, f_flag, tokens[num_tokens - 2]);
    if (c == 0)
    {
        printf("No match found!\n");
    }
    else if (c == 1 && d_check && !f_check && e_flag)
    {
        chdir(to_change);
    }
    else if (c == 1 && f_check && !d_check && e_flag)
    {
        printf("%s\n", to_change);
        FILE *fptr = fopen(to_change, "r");
        if (fptr == NULL)
        {
            if (errno == EACCES)
            {
                printf("Missing permissions for task!\n");
            }
            printf("Cannot open file \n");
            exit(0);
        }
        c = fgetc(fptr);
        while (c != EOF)
        {
            printf("%c", c);
            c = fgetc(fptr);
        }
        printf("\n");
        fclose(fptr);
    }

    d_check = 0, f_check = 0;
}

int rec(char *dir, int d_flag, int e_flag, int f_flag, char *file)
{
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
        return 0;
    }
    struct dirent *d;
    int sum = 0;
    while ((d = readdir(dh)) != NULL)
    {
        char *temp = (char *)malloc(sizeof(char) * 1024);
        if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
        {
            continue;
        }
        strcpy(temp, d->d_name);
        removeExtension(temp);
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dir, d->d_name);
        struct stat statbuf;
        if (stat(path, &statbuf) == -1)
        {
            perror("Error getting file information");
            continue;
        }
        if (S_ISDIR(statbuf.st_mode))
        {
            if (strcmp(temp, file) == 0 || strcmp(d->d_name, file) == 0)
            {
                sum++;
                d_check = 1;
                if (d_flag)
                {
                    printf("\033[1;32m%s\033[0m", path);
                    printf("\n");
                }
                strcpy(to_change, path);
            }
            sum += rec(path, d_flag, e_flag, f_flag, file);
        }
        else if (S_ISREG(statbuf.st_mode) && (strcmp(temp, file) == 0 || strcmp(d->d_name, file) == 0))
        {
            sum++;
            f_check = 1;
            if (f_flag)
            {
                printf("\033[1;34m%s\033[0m", path);
                printf("\n");
                strcpy(to_change, path);
            }
        }
    }
    closedir(dh);
    return sum;
}