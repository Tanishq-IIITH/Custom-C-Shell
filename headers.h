#ifndef HEADERS_H_
#define HEADERS_H_
#define clear() printf("\033[H\033[J")
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include "prompt.h"
#include "time.h"
#include "ping.h"
#include "systemprocess.h"
#include "warp.h"
#include "signal.h"
#include "peek.h"
#include "trim.h"
#include "fg.h"
#include "bg.h"
#include "seek.h"
#include "pastevents.h"
#include "unistd.h"
#include "neonate.h"
#include "function_decider.h"
#include "activities.h"
#include "proclore.h"
#include "prompt.h"
#include "fcntl.h"
#include "iman.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "sys/prctl.h"

extern char *currentworkingdir;
extern char *prevworkingdir;
extern char *homeworkingdir;
extern char *com;
extern int check;
extern int tim;
extern int f_check, d_check;
extern char *to_change;
extern int foregroundProcessPid;
struct ProcessInfo
{
    pid_t pid;
    int status;
    char **name;
    int sz;
    struct ProcessInfo *next;
};
extern int past;
struct ProcessInfo *removeProcess(struct ProcessInfo *head, pid_t pid);
typedef struct Node
{
    pid_t pid;
    int status;
    char process_name[100];
    struct Node *next;
} Node;
typedef struct myname
{
    char process_name[100];
    struct myname *next;
} myname;
extern char *command_pid[100000 + 1];
extern char *s;
extern Node *completedList;
#endif
