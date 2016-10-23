#include <stdlib.h>
#include <stdio.h>
#include <sys/queue.h>
#include <string.h>
#include <time.h>

#define STR_SIZE 255
#define PR_DONE -1
#define MAX_TASKS 3

typedef struct entry entry;

 TAILQ_HEAD(tailhead, entry) sched;

struct entry
{
    void (*funPtr)(char**);
    char* currStr;
    int priority; //-1 done, else it's priority
    TAILQ_ENTRY(entry) entries;
};

char* getStr()
{
    char* res = (char*)malloc(STR_SIZE * sizeof(char));
    if (! res)
    {
        printf("Error: not enough memory!");
        exit(1);
    }
    return res;
}

void printOneWord(char** str)
{
    if ((*str)[0]!= 0)
    {
        char* buf = getStr();
        buf[0] = 0;
        int i = 0;
        while (((*str)[0] != ' ') && ((*str)[0] != 0))
        {
            buf[i] = (*str)[0];
            (*str) ++;
            i ++;
        }
        buf[i] = 0;
        if ((*str)[0] == ' ')
        {
            (*str) ++;
        }
        printf("%s\n", buf);
        free(buf);
    }
    return;
}

void initEntry(entry* forInit, char* strForProc)
{
    (*forInit).currStr = getStr();
    strcpy((*forInit).currStr, strForProc);
    (*forInit).funPtr = printOneWord;
    (*forInit).priority = rand() % MAX_TASKS;
    return;
}

void addToScheduler(entry* newTask)
{
    if (sched.tqh_first == NULL)
    {
        TAILQ_INSERT_HEAD(&sched, newTask, entries);
    }
    else
    {
        entry* tmp = sched.tqh_first;
        entry* hlp = tmp;
        while ((tmp != NULL) && ((*tmp).priority > (*newTask).priority))
        {
            hlp = tmp;
            tmp = tmp -> entries.tqe_next;
        }
        if (tmp == sched.tqh_first)
        {
            printf("\nPUT HEAD!!!\n\n");
            TAILQ_INSERT_HEAD(&sched, newTask, entries);
        }
        else if (tmp == NULL)
             {
                printf("\nPUT TAIL!!!\n\n");
                TAILQ_INSERT_TAIL(&sched, newTask, entries);
             }
             else
             {
                 printf("\nPUT BEFORE!!!\n\n");
                 TAILQ_INSERT_AFTER(&sched, hlp, newTask, entries);
             }
    }
    return;
}

void printQueue()
{
    entry* np;
    printf("----QUEUE----\n");
    for (np = sched.tqh_first; np != NULL; np = np -> entries.tqe_next)
    {
        printf("%s pr %d\n", np->currStr, np->priority);
        sleep(1);
    }
    printf("----END OF QUEUE----\n");
    return;
}

entry getTask()
{
    entry newTask;
    if (sched.tqh_first == NULL)
    {
        printf("Schedule is emtpy!");
        exit(1);
    }
    newTask = (*(sched.tqh_first));
    TAILQ_REMOVE(&sched, sched.tqh_first, entries);
    return newTask;
}

void exeTask(entry newTask)
{
    if (newTask.priority == -1)
    {
        return;
    }
    if (newTask.currStr[0] != 0)
    {
        newTask.funPtr(&(newTask.currStr));
        if (newTask.currStr[0] != 0)
        {
            printf("try to add\n");
            sleep(4);
            addToScheduler(&newTask);
        }
    }
}


int main()
{
    srand(time(NULL));
    TAILQ_INIT(&sched);
    entry *n1, *n2, *n3, *np;

    // need to change this part of code for more tasks
    n1 = malloc(sizeof(entry));
    initEntry(n1, "1 11");
    addToScheduler(n1);

    n2 = malloc(sizeof(entry));
    initEntry(n2, "2 22 222");
    addToScheduler(n2);

    n3 = malloc(sizeof(entry));
    initEntry(n3, "3 33");
    addToScheduler(n3);
    //end of part
    entry newTask;
    printQueue();
    while (sched.tqh_first != NULL)
    {
        newTask = getTask();
        newTask.priority = rand() % 3;
        printf("\nADDING %s pr %d\n", newTask.currStr, newTask.priority);
        addToScheduler(&newTask);
        printQueue();
        sleep(1);
    }
    return 0;
}


