#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/io.h>
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

void setDescriptors(char* str) // str looks like < file.txt
{
    char* fileName;
    fileName = getStr();
    fileName = getFileName(str);
    switch (str[0])
    {
        case '<':
        {
            int input;
            input = open(fileName, O_RDONLY);
            if (input == -1)
            {
                printf("Can't open file %s!", fileName);
                exit(1);
            }
            dup2(input, 0); // replace standard input with input file
            close(input);
        };
        case '>':
        {
            int output;
            output = open(fileName, O_WRONLY);
            if (output == -1)
            {
                printf("Can't open file %s!", fileName);
                exit(1);
            }
            dup2(output, 1); // replace standard output with output file
            close(output);
        };
    }
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
        if (pid == 0)
        {
            setDescriptors(buf);
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
