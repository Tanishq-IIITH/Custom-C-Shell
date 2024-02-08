#include "activities.h"

// Function to split the linked list into two halves
void split(struct ProcessInfo *source, struct ProcessInfo **front, struct ProcessInfo **back)
{
    struct ProcessInfo *slow;
    struct ProcessInfo *fast;
    slow = source;
    fast = source->next;
    while (fast != NULL)
    {
        fast = fast->next;
        if (fast != NULL)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }
    *front = source;
    *back = slow->next;
    slow->next = NULL;
}
struct ProcessInfo *merge(struct ProcessInfo *a, struct ProcessInfo *b)
{
    struct ProcessInfo *result = NULL;

    if (a == NULL)
        return b;
    else if (b == NULL)
        return a;

    char str1[20], str2[20]; // Assuming a buffer large enough to hold the string

    sprintf(str1, "%d", a->pid);
    sprintf(str2, "%d", b->pid);

    if (strcmp(str1, str2) <= 0)
    {
        result = a;
        result->next = merge(a->next, b);
    }
    else
    {
        result = b;
        result->next = merge(a, b->next);
    }

    return result;
}

void mergeSort(struct ProcessInfo **headRef)
{
    struct ProcessInfo *head = *headRef;
    struct ProcessInfo *a;
    struct ProcessInfo *b;

    if ((head == NULL) || (head->next == NULL))
        return;

    split(head, &a, &b);
    mergeSort(&a);
    mergeSort(&b);
    *headRef = merge(a, b);
}

void activity_handler(char *tokens[], int num_tokens)
{
    mergeSort(&processList);
    struct ProcessInfo *current = processList;
    while (current != NULL)
    {
        int pid = current->pid;
        char location[1024];
        sprintf(location, "/proc/%d/cmdline", pid);
        sprintf(location, "/proc/%d/stat", pid);
        FILE *f = fopen(location, "r");
        char state = 0;
        int pd;
        char new_name[1024];
        char statee[100];
        if (f == NULL)
        {
            printf(" %d ", current->pid);
            for (int i = 0; i < current->sz; i++)
            {
                printf("%s ", current->name[i]);
            }
            printf("finished\n");
            processList = removeProcess(processList, current->pid);
        }
        else
        {
            fscanf(f, " %d ", &pd);
            fscanf(f, " %s ", new_name);
            fscanf(f, " %c ", &state);
            if (state == 'R')
                strcpy(statee, "Running");
            else if (state == 'T')
                strcpy(statee, "Stopped");
            else if (state == 'Z')
                strcpy(statee, "Zombie");
            else if (state == 'S')
                strcpy(statee, "Running");
            else
                strcpy(statee, "Unknown State");
            fclose(f);
            printf(" %d ", current->pid);
            for (int i = 0; i < current->sz; i++)
            {
                printf("%s ", current->name[i]);
            }
            printf("%s\n", statee);
        }
        current = current->next;
    }
    return;
}