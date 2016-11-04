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
    (*forInit).entries.tqe_next = NULL;
    (*forInit).entries.tqe_prev = NULL;
    return;
}

void addToScheduler(entry* newTask)
{
    entry* tmp;
    entry* hlp = NULL;
    TAILQ_FOREACH(tmp, &sched, entries)
    {
        if ((*tmp).priority > (*newTask).priority)
        {
            hlp = tmp;
        }
    }
    if (hlp == NULL)
    {
        TAILQ_INSERT_HEAD(&sched, newTask, entries);
    }
    else
    {
        if (hlp == TAILQ_LAST(&sched, tailhead))
        {
            TAILQ_INSERT_TAIL(&sched, newTask, entries);
        }
        else
        {
            TAILQ_INSERT_AFTER(&sched, hlp, newTask, entries);
        }
    }
    return;
}

entry getTask()
{
    entry newTask;
    if (TAILQ_EMPTY(&sched))
    {
        printf("Schedule is emtpy!");
        exit(1);
    }
    newTask = (*TAILQ_FIRST(&sched));
    newTask.entries.tqe_next = NULL;
    newTask.entries.tqe_prev = NULL;
    TAILQ_REMOVE(&sched, TAILQ_FIRST(&sched), entries);
    return newTask;
}

void exeTask(entry* newTask)
{
    if ((*newTask).currStr[0] != 0)
    {
        (*newTask).funPtr(&((*newTask).currStr));
        if ((*newTask).currStr[0] != 0)
        {
            (*newTask).priority = rand() % MAX_TASKS;
        }
        else
        {
            (*newTask).priority = -1;
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
    while (! TAILQ_EMPTY(&sched))
    {
        entry gettedTask = getTask();
        entry* newTask = malloc(sizeof(entry));
        (*newTask) = gettedTask;
        (*newTask).priority = rand() % MAX_TASKS;
        exeTask(newTask);
        if ((*newTask).priority != -1)
        {
            addToScheduler(newTask);
        }
    }
    return 0;
}


