#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/stat.h>
#include <fcntl.h>

#define STR_SIZE 255
#define MAX_ARGS 32
#define MAX_COMMANDS 16

char* getStr()
{
    char* buf;
    buf = (char*)malloc(STR_SIZE * sizeof(char));
    if (!buf)
    {
        printf("Malloc error!");
        exit(1);
    }
    return buf;
}

char* deleteSpaces(char* str)
{
    char* buf = str;
    while (buf[0] == ' ')
    {
        buf ++;
    }
    return buf;
}

char* getNextArg(char** str)
{
    char* res;
    res = getStr();
    if ((*str)[0] == 0)
    {
        return NULL;
    }
    int i = 0;
    while (((*str)[0] != 0) && ((*str)[0] != ' '))
    {
        res[i] = (*str)[0];
        (*str) ++;
        i ++;
    }
    res[i] = 0;
    return res;
}

char* splitIfSomeSpecial(char** str) //try to find < or >
{
    char* buf;
    buf = getStr();
    if (!(strstr((*str), ">")) && !(strstr((*str), "<")))
    {
        strcpy(buf, (*str));
        (*str)[0] = 0;
        return buf;
    }
    else
    {
        int i = 0;
        while (((*str)[0] != '<') && ((*str)[0] != '>'))
        {
            buf[i] = (*str)[0];
            i ++;
            (*str) ++;
        }
        while (buf[i] == ' ')
        {
            i --;
        }
        i ++;
        buf[i] = 0;
    }
    return buf;
}

char* getFileName(char* str) // str looks like < file.txt
{
    str ++; // skip < or >
    str = deleteSpaces(str);
    return str;
}

void setDesc(char* str) //str looks like < (or) > file.txt
{
    if (str[0] == 0)
    {
        return;
    }
    char* fileName = getFileName(str);
    switch (str[0])
    {
        case '<':
            {
                int input = open(fileName, O_RDONLY);
                if (input == -1)
                {
                    printf("error open!");
                    exit(1);
                }
                dup2(input, STDIN_FILENO);
                close(input);
                break;
            }
        case '>':
            {
                int output = open(fileName, O_WRONLY);
                if (output == -1)
                {
                    printf("error open!");
                    exit(1);
                }
                dup2(output, STDOUT_FILENO);
                close(output);
                break;
            }
    }
    return;
}

int countPipe(char* str)
{
    int i = 0;
    int count = 0;
    while (str[i] != 0)
    {
        if (str[i] == '|')
        {
            count ++;
        }
    }
    return count;
}

char* getNewCommand(char** str) // str looks like com1 | com2 | com3 etc.
{
    if ((*str)[0] == 0)
    {
        return NULL;
    }
    while (((*str)[0] == ' ') || ((*str)[0] == '|'))
    {
        (*str) ++;
    }
    int i = 0;
    char* buf = getStr();
    while (((*str)[0] != 0) && ((*str)[0] != '|')) //taking new command and it's args
    {
        buf[i] = (*str)[0];
        i ++;
        (*str) ++;
    }
    i --;
    while (buf[i] == ' ')
    {
        i --;
    }
    i ++;
    buf[i] = 0;
    return buf;
}

void executeOne(int sumOfCom, int curCom, int descIn, int descOut, char* commands[MAX_COMMANDS])
{
    int fd[2];
    fd[0] = -1; // will be used for checking pipe (if it was done)
    //printf("--%d %d--\n", curCom, sumOfCom);
    if (sumOfCom == curCom)
    {
        return;
    }
    if ((sumOfCom - 1) == curCom)
    {
        fd[1] = descOut;
    }
    else
    {
        if (pipe(fd) == -1)
        {
            printf("pipe error!");
            exit(1);
        }
    }
    //printf("in execute1\n");
    pid_t pid = fork();
    if (pid == -1)
    {
        printf("Fork error!");
        exit(1);
    }
    if (pid == 0)
    {
        if (fd[0] != -1)
        {
            close(fd[0]);// pipe was done
        }
        char* args[MAX_ARGS];
        int i;
        for (i = 0; i < MAX_ARGS; i ++)
        {
            args[i] = getStr();
        }
        char* tmp = getStr();
        strcpy(tmp, commands[curCom]);
        i = 0;
        while (tmp[0] != 0)
        {
            strcpy(args[i], getNextArg(&tmp));
            tmp = deleteSpaces(tmp);
            i ++;
        }
        args[i] = NULL;
        dup2(descIn, 0);
        dup2(fd[1], 1);
        execvp(args[0], &args[0]);
        // if here - exec error
    }
    else
    {
        //close(fd[1]);
        printf("in parent 1\n");
        wait(0);
        executeOne(sumOfCom, curCom + 1, fd[0], descOut, commands);
        close(fd[0]);
    }
    return;
}

void firstExecute(char* commands[MAX_COMMANDS], int numCom)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        printf("Fork error!");
        exit(1);
    }
    if (pid == 0)
    {
        executeOne(numCom, 0, STDIN_FILENO, STDOUT_FILENO, commands);
    }
    else
    {
        wait(0);
        printf("all was done\n");
    }
    return;
}

int main()
{
    char* buf;
    buf = getStr();
    char* commands[MAX_COMMANDS];
    gets(buf);
    int i;
    for (i = 0; i < MAX_COMMANDS; i ++)
    {
        commands[i] = getStr();
    }
    while (strcmp(buf, "exit") != 0)
    {
        char* tmp;
        tmp = getStr();
        tmp = splitIfSomeSpecial(&buf); // in tmp - args, in buf - >, <, etc.
        i = 0;
        while (tmp[0] != 0)
        {
            strcpy(commands[i], getNewCommand(&tmp));
            tmp = deleteSpaces(tmp);
            i ++;
        }
        commands[i] = NULL;
        firstExecute(commands, i);
        printf("Done!!\n");
        gets(buf);
    }
    return 0;
}
