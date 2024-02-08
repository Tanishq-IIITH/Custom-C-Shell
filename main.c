#include "headers.h"
#include "prompt.h"
#include "trim.h"
#include "function_decider.h"

char *currentworkingdir;
char *prevworkingdir;
char *homeworkingdir;
char *com;
int check;
int tim;
int f_check, d_check;
char *to_change;
char *s;
char *command_pid[100000 + 1];
int past = 0;
void custom_sigint_handler(int signum)
{
    int targetPID = foregroundProcessPid;
    if ((foregroundProcessPid == -1))
        return;
    if (kill(targetPID, 9) == -1)
    {
        perror("kill");
    }
    foregroundProcessPid = -1;
}
int foregroundProcessPid = -1;

void sigtstp_handler(int signum)
{
    int targetPID = foregroundProcessPid;
    if ((foregroundProcessPid == -1))
        return;

    if (kill(targetPID, SIGSTOP) == -1)
    {

        perror("kill");
    }
    foregroundProcessPid = -1;
}
struct ProcessInfo *processList = NULL;
int main()
{
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    if (signal(SIGINT, custom_sigint_handler) == SIG_ERR)
    {
        perror("signal");
    }
    if (signal(SIGTSTP, sigtstp_handler) == SIG_ERR)
    {
        perror("signal");
    }
    printf("YES");

    for (int i = 0; i < 100000 + 1; i++)
    {
        command_pid[i] = (char *)malloc(sizeof(char) * 20);
    }
    s = (char *)malloc(sizeof(char) * 100);
    clear();
    currentworkingdir = (char *)malloc(sizeof(char) * 1024);
    homeworkingdir = (char *)malloc(sizeof(char) * 1024);
    prevworkingdir = (char *)malloc(sizeof(char) * 1024);
    com = (char *)malloc(sizeof(char) * 1024);
    to_change = (char *)malloc(sizeof(char) * 1024);

    getcwd(currentworkingdir, 1024);
    strcpy(prevworkingdir, currentworkingdir);
    strcpy(homeworkingdir, currentworkingdir);
    char *input = (char *)malloc(sizeof(char) * 1024);

    while (1)
    {
        prompt(homeworkingdir);
        past=0;
        if (fgets(input, 1024, stdin) == NULL)
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
            exit(0);
        }
        Node *currentNode = completedList;
        while (currentNode != NULL)
        {
            if (WIFEXITED(currentNode->status))
            {
                printf("%s process with PID %d exited normally with status %d\n", command_pid[currentNode->pid], currentNode->pid, WEXITSTATUS(currentNode->status));
            }
            else if (WIFSIGNALED(currentNode->status))
            {
                printf("%s process with PID %d exited abnormally due to signal %d\n", command_pid[currentNode->pid], currentNode->pid, WTERMSIG(currentNode->status));
            }
            Node *temp = currentNode;
            currentNode = currentNode->next;
            free(temp);
        }
        completedList = NULL;
        input = trimspace(input);
        function_call(input);
    }
}
