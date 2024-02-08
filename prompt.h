#include<stdio.h>
#include<unistd.h>
#include<sys/utsname.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <stdbool.h>

#ifndef __PROMPT_H
#define __PROMPT_H

void prompt(char *dir);

#endif