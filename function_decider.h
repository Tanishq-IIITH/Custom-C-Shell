#include "headers.h"
#include "headers.h"
#include "trim.h"
#include "warp.h"
#include "peek.h"

void fixInput(int in, int out);
void get_commands(char *line);
void pipechecker(char *command, int processid);
void function_call(char *line);
extern struct ProcessInfo* processList;
struct ProcessInfo *addProcess(struct ProcessInfo *head, pid_t pid, char *tokens[], int sz);
struct ProcessInfo *findProcess(struct ProcessInfo *head, pid_t pidToFind);
struct ProcessInfo *removeProcess(struct ProcessInfo *head, pid_t pid);
void IO_handler(char *token, char *file);
int redirection(char *line, int proccessid, int pipes[], int prev, int flag);
int processInput(char *tokens[],int num_tokens, int proccessid, int pipes[], int prev);
void function_call(char *line);
