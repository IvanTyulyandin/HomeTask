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

char* splitIfNotSTDIN(char** str) //try to find < or >
{
    char* buf;
    buf = getStr();
    if (!(strstr((*str), ">")))
    {
        strcpy(buf, (*str));
        (*str)[0] = 0;
        return buf;
    }
    else
    {
        int i = 0;
        while ((*str)[0] != '>')
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
        printf("Execvp error! Program was not found!\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        if ((sumOfCom - 1) != curCom)
        {
            close(fd[1]);
        }
        wait(0);
        executeOne(sumOfCom, curCom + 1, fd[0], descOut, commands);
        close(fd[0]);
    }
    return;
}

char* findInputRedirect(char** str)
{
    char* buf = getStr();
    buf[0] = 0;
    if (strstr((*str), "<") != NULL)
    {
        int i = 0;
        while ((*str)[i] != '<')
        {
            i ++;
        }
        buf[0] = (*str)[i]; // put < in buf
        int bufCur = 1;
        i ++;
        while ((*str)[i] == ' ') // put all spaces between < and name of file
        {
            buf[bufCur] = (*str)[i];
            i ++;
            bufCur ++;
        }
        while ((*str)[i] != ' ') // put name of file to buf
        {
            buf[bufCur] = (*str)[i];
            i ++;
            bufCur ++;
        }
        buf[bufCur] = 0;
        int j;
        int sLen = strlen((*str));
        for (j = i - bufCur; j + bufCur < sLen; j ++)
        {
            (*str)[j] = (*str)[j + bufCur];
        }
        (*str)[j] = 0;
    }
    return buf;
}

void firstExecute(char* commands[MAX_COMMANDS], int numCom, char* strInput, char* strOutput)
{
    int descIn = STDIN_FILENO;
    int descOut = STDOUT_FILENO;
    if (strInput[0] != 0)
    {
        descIn = open(getFileName(strInput), O_RDONLY | O_CREAT);
    }
    if (strOutput[0] != 0)
    {
        descOut = open(getFileName(strOutput), O_WRONLY | O_CREAT);
    }
    pid_t pid = fork();
    if (pid == -1)
    {
        printf("Fork error!");
        exit(1);
    }
    if (pid == 0)
    {
        executeOne(numCom, 0, descIn, descOut, commands);
        exit(EXIT_SUCCESS);
    }
    else
    {
        wait(0);
    }
    return;
}

int main()
{
    char* buf;
    buf = getStr();
    char* commands[MAX_COMMANDS];
    fgets(buf, 80, stdin);
    buf[strlen(buf) - 1] = 0;
    int i;
    for (i = 0; i < MAX_COMMANDS; i ++)
    {
        commands[i] = getStr();
    }
    char* tmp = getStr();
    char* strOutput = getStr();
    char* strInput = getStr();
    while (strcmp(buf, "exit") != 0)
    {
        strcpy(tmp ,splitIfNotSTDIN(&buf)); // in tmp - args, in buf - > NameOfFile
        strcpy(strOutput, buf);
        strcpy(strInput,findInputRedirect(&tmp));
        i = 0;
        while (tmp[0] != 0)
        {
            strcpy(commands[i], getNewCommand(&tmp));
            tmp = deleteSpaces(tmp);
            i ++;
        }
        strcpy(commands[i], "\0");
        firstExecute(commands, i, strInput, strOutput);
        printf("Done!\n");
        fgets(buf, 80, stdin);
        buf[strlen(buf) - 1] = 0;
    }
    return 0;
}
