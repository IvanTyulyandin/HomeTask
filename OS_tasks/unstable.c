#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/stat.h>
#include <fcntl.h>

#define STR_SIZE 255
#define MAX_ARGS 32

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
        return (*str);
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
    if ((str[0] == 0) || (str[0] == NULL))
    {
        return;
    }
    int fileName = getFileName(str);
    switch (str[0])
    {
        case '<':
            {
                int input = open(fileName, O_RDONLY);
                dup2(input, STDIN_FILENO);
                close(input);
            }
        case '>':
            {
                int output = open(fileName, O_WRONLY);
                dup2(output, STDOUT_FILENO);
                close(output);
            }
        default: printf("azaza");
    }
    return;
}

int main()
{
    char* buf;
    buf = getStr();
    char* args[MAX_ARGS];
    gets(buf);
    while (strcmp(buf, "exit") != 0)
    {
        int i;
        for (i = 0; i < MAX_ARGS; i ++)
        {
            args[i] = getStr();
        }
        char* tmp;
        tmp = getStr();
        tmp = splitIfSomeSpecial(&buf); // in tmp - args, in buf - >, <, etc.
        i = 0;
        while (tmp[0] != 0)
        {
            strcpy(args[i], getNextArg(&tmp));
            tmp = deleteSpaces(tmp);
            i ++;
        }
        args[i] = NULL;
        pid_t pid = fork();
        if (pid == -1)
        {
            printf("Fork error!");
            exit(1);
        }
        if (pid == 0)
        {
            setDesc(buf);
            execvp(args[0], &args[0]);
        }
        else
        {
            wait(0);
            gets(buf);
        }
    }
    return 0;
}
