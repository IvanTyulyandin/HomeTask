#include <stdlib.h>
#include <stdio.h>
#include <sys/queue.h>
#include <string.h>

int const STR_SIZE = 255;

typedef struct entry entry;

struct entry
{
    void (*funPtr)(char*);
    char* currStr;
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
        printf("%s ", buf);
        free(buf);
    }
    return;
}

void initEntry(entry *forInit, char *strToProc)
{
    (*forInit).currStr = getStr();
    strcpy((*forInit).currStr, strToProc);
    (*forInit).funPtr = printOneWord;
    return;
}

int main()
{
    TAILQ_HEAD(tailhead, entry) head;
    TAILQ_INIT(&head);
    entry *n1, *n2, *n3, *np;

    // need to change this part of code for more tasks
    n1 = malloc(sizeof(entry));
    initEntry(n1, "1 11");
    TAILQ_INSERT_TAIL(&head, n1, entries);

    n2 = malloc(sizeof(entry));
    initEntry(n2, "2 22 222");
    TAILQ_INSERT_TAIL(&head, n2, entries);

    n3 = malloc(sizeof(entry));
    initEntry(n3, "3 33");
    TAILQ_INSERT_TAIL(&head, n3, entries);
    //end of part

    while (head.tqh_first != NULL)
    {
        for (np = head.tqh_first; np != NULL; np = np -> entries.tqe_next)
            if ((*np).currStr[0] == 0)
            {
                TAILQ_REMOVE(&head, np, entries);
            }
            else
            {
                (*np).funPtr(&((*np).currStr));
            }
    }
    return 0;
}


