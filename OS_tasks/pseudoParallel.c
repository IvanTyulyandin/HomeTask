#include <stdio.h>
#include <string.h>
#include <time.h>

int const STR_SIZE = 255;
int const NUM_TASKS = 3;

typedef struct processInfo processInfo;

struct processInfo // task of process - print string word by word
{
    int (*funPtr)(char*);
    int priority; // priority == -1 => this task is done
    char* currStr;
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

char* printOneWord(char* str)
{
    if (str[0]!= 0)
    {
        char* buf = getStr();
        buf[0] = 0;
        int i = 0;
        while ((str[0] != ' ') && (str[0] != 0))
        {
            buf[i] = str[0];
            str ++;
            i ++;
        }
        buf[i] = 0;
        if (str[0] == ' ')
        {
            str ++;
        }
        printf("%s ", buf);
        free(buf);
    }
    return str;
}

int findMaxPriority(processInfo tasks[NUM_TASKS])
{
    int i = 0, maxVal = -1, mainTask = -1;
    for (i; i < NUM_TASKS; i ++)
    {
        if (tasks[i].priority > maxVal)
        {
            maxVal = tasks[i].priority;
            mainTask = i;
        }
    }
    return mainTask;
}

int executeOneTask(processInfo tasks[NUM_TASKS]) // return number of done process
{                                                // if -1 => all process are done
    int howMuchIsDone = 0;
    int i;
    for (i = 0; i < NUM_TASKS; i ++)
    {
        if (tasks[i].priority == -1)
        {
            howMuchIsDone ++;
        }
    }
    if (howMuchIsDone == NUM_TASKS)
    {
        return -1;
    }
    int topPriority = findMaxPriority(tasks);
    tasks[topPriority].currStr = tasks[topPriority].funPtr((tasks[topPriority].currStr)); //call function with currStr, nothing more :)
    tasks[topPriority].priority = rand() % NUM_TASKS;
    if (tasks[topPriority].currStr[0] == 0)
    {
        tasks[topPriority].priority = -1;
    }
    return topPriority;
}

int main()
{
   srand(time(NULL));
   processInfo tasks[NUM_TASKS];
   int i = 0;
   for (i; i < NUM_TASKS; i ++)
   {
       tasks[i].funPtr = printOneWord;
       tasks[i].priority = rand() % NUM_TASKS;
       tasks[i].currStr = getStr();
   }
   // need to change this part of code for more tasks
   strcpy(tasks[0].currStr, "0 0 0");
   strcpy(tasks[1].currStr, "1 1 1 1");
   strcpy(tasks[2].currStr, "2 2 2 2 2");
   // end of part
   int whatDone = -1;
   do
   {
       whatDone = executeOneTask(tasks);
   } while (whatDone != -1);
   for (i = 0; i < NUM_TASKS; i ++)
   {
       free(tasks[i].currStr);
   }
   return 0;
}
