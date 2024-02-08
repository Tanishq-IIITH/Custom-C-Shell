#include "headers.h"

void activity_handler(char *tokens[], int num_tokens);
void split(struct ProcessInfo *source, struct ProcessInfo **front, struct ProcessInfo **back);
struct ProcessInfo *merge(struct ProcessInfo *a, struct ProcessInfo *b);
void mergeSort(struct ProcessInfo **headRef);