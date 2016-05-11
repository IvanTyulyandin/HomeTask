#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int INST_LD = 0;
const int INST_ST = 1;
const int INST_LDC = 2;  // 0 - 2 need addr or int for args
const int INST_ADD = 3;
const int INST_SUB = 4;
const int INST_CMP = 5;
const int INST_RET = 6;
const int INST_ERR = 7;
const int INST_JMP = 8; //8 - 9 need char* for args, it will be used for parsing
const int INST_BR = 9;
const int BLOCK_SIZE = 262144; // 2^18 size of data/stack
const int CODE_SIZE = 10000;
const int STR_SIZE = 255;

typedef struct instr instr;
typedef struct label label;

struct instr
{
    int type;
    union arg
    {
        int num;
        char *lbl;
    };
};

struct label
{
    char *name;
    int addr;
};

int* initBlock(int size) // for data and stack init
{
    int *block;
    block = (int*)malloc(size * sizeof(int));
    if (block == NULL)
    {
        printf("Not enough memory for data/stack!");
        exit(1);
    }
    return block;
}

char* getStr()
{
    char* buf;
    buf = (char*)malloc(sizeof(char) * STR_SIZE);
    if (buf == NULL)
    {
        printf("Not enough memory! getStr");
        exit(1);
    }
    return buf;
}

char* deleteSpaces(char* s)
{
    while (*s == ' ')
    {
        s ++;
    }
    return s;
}

int getType(char *s)
{
    char *com[10] = {"ld", "st", "ldc", "add", "sub", "cmp", "ret", "err", "jmp", "br"};
    //need "err" for saving numeration of INST_xxx
    int i;
    for (i = 0; i <= 9; i ++)
        if (strcmp(com[i], s) == 0)
        {
            return i;
        }
    return INST_ERR;
}

int searchLabel(char *lbl, label *LB, int LP) // return addr of label else -1
{
    int i, res = -1;
    for (i = 0; i < LP; i ++)
    {
        if (strcmp(LB[i].name, lbl) == 0)
        {
            res = LB[i].addr;
        }
    }
    return res;
}

instr parseStr(char *s, label *LB, int *LP, int IP)
{
    instr whatToRet;
    char *buf;
    int i = 0;
    buf = getStr();
    buf[0] = 0;
    strcat(s, ";");
    s = deleteSpaces(s);
    if (strchr(s, ':') != NULL) // searching labels
        {
            i = 0;
            while ((s[i] != ' ') && (s[i] != ':'))
            {
                buf[i] = s[i];
                i ++;
            }
            buf[i] = 0;
            if (searchLabel(buf, LB, *LP) != -1)
            {
                printf("Two or more labels %s\n", buf);
                exit(1);
            }
            LB[*LP].name = getStr();
            strcpy(LB[*LP].name, buf);
            LB[*LP].addr = IP;
            (*LP) ++;
            s = s + strlen(buf);
            s = deleteSpaces(s);
            s = s + 1; // deleting :
            s = deleteSpaces(s);
        }
    i = 0;
    while ((s[i] != ';') && (s[i] != ' '))
    {
        buf[i] = s[i];
        i ++;
    }
    buf[i] = 0; // buf  => name of instruction
    whatToRet.type = getType(buf);
    if (whatToRet.type == INST_ERR)
    {
        printf("Bad command at line %d\n", IP);
    }
    s = s + strlen(buf);
    s = deleteSpaces(s);
    buf[0] = 0;
    if (s[0] != ';') // then has arg
    {
        i = 0;
        while ((s[i] != ';') && s[i] != ' ')
        {
            buf[i] = s[i];
            i ++;
        }
        buf[i] = 0;
    }
    if (strcmp(buf, "") == 0)
    {
        if ((whatToRet.type <= 2) || (whatToRet.type >= 8))
        {
            printf("Need argument at line %d\n", IP);
            whatToRet.type = INST_ERR;
        }
    }
    else
    {
        if ((whatToRet.type > 2) && (whatToRet.type < 8))
        {
            printf("Don't need argument at line %d\n", IP);
            whatToRet.type = INST_ERR;
        }
    }
    whatToRet.lbl = getStr();
    strcpy(whatToRet.lbl, buf);
    return whatToRet;
}

void correctArgs(instr *CS, int codeAmount, label *LB, int LP) // CS[IP].lbl -> int or addr of label
{
    int IP;
    for (IP = 0; IP < codeAmount; IP ++)
    {
        if (CS[IP].type <= 2) // need int as arg
        {
            CS[IP].num = atoi(CS[IP].lbl);
        }
        else
        {
            if (CS[IP].type >= 8) // need addr of label
            {
                CS[IP].num = searchLabel(CS[IP].lbl, LB, LP);
                if (CS[IP].num == -1)
                {
                    printf("Unknown label at line %d\n", IP);
                    CS[IP].type = INST_ERR;
                }
            }
        }
    }
}

void exitIfTypeErr(instr *CS, int codeAmount)
{
    int IP;
    for (IP = 0; IP < codeAmount; IP ++)
    {
        if (CS[IP].type == INST_ERR)
        {
            exit(1);
        }
    }
}

void runInstr(instr *CS, int codeAmount, int *DS, int *SS)
{
    int IP = 0;
    int SP = -1;
    while (CS[IP].type != INST_RET)
    {
        // used 0, 1, 2..., not INST_num
        // else error case label does not reduce to integer constant
        // const int != constant expression
        switch (CS[IP].type)
        {
            case 0 :  // ld
                SP ++;
                if (SP > BLOCK_SIZE)
                {
                    printf("Error : stack overflow (line %d)\n", IP);
                    exit(1);
                }
                if (CS[IP].num > BLOCK_SIZE)
                {
                    printf("Error : range check error (line %d)\n", IP);
                    exit(1);
                }
                SS[SP] = DS[CS[IP].num];
                IP ++;
                break;
            case 1 :  //st
                if ((CS[IP].num > BLOCK_SIZE) || (CS[IP].num < 0))
                {
                    printf("Error : range check error (line %d)\n", IP);
                    exit(1);
                }
                DS[CS[IP].num] = SS[SP];
                SP --;
                if (SP < -1)
                {
                    printf("Error : stack is empty (line %d)\n", IP);
                    exit(1);
                }
                IP ++;
                break;
            case 2 :  //ldc
                SP ++;
                if (SP > BLOCK_SIZE)
                {
                    printf("Error : stack overflow (line %d)\n", IP);
                    exit(1);
                }
                SS[SP] = CS[IP].num;
                IP ++;
                break;
            case 3 : case 4 : //add or sub
                if (SP < 1)
                {
                    printf("Error : not enough arguments at stack (line %d)\n", IP);
                    exit(1);
                }
                if (CS[IP].type == INST_ADD)
                {
                    SS[SP - 1] = SS[SP] + SS[SP - 1];
                }
                else
                {
                   // printf("%d %d %d\n", SS[SP - 2], SS[SP - 1], SS[SP]);
                    SS[SP - 1] = SS[SP] - SS[SP - 1];
                }
                SP --;
                IP ++;
                break;
            case 5 :  //cmp
                if (SS[SP] > SS[SP - 1])
                {
                    SS[SP - 1] = 1;
                }
                else if (SS[SP] < SS[SP - 1])
                     {
                         SS[SP - 1]  = -1;
                     }
                     else
                     {
                         SS[SP - 1] = 0;
                     }
                SP --;
                if (SP < -1)
                {
                    printf("Error : stack is empty (line %d)\n", IP);
                    exit(1);
                }
                IP ++;
                break;
            case 8 :  //jmp
                IP = CS[IP].num;
                break;
            case 9 :  //br
                if (SS[SP] != 0)
                {
                    IP = CS[IP].num;
                }
                else
                {
                    IP ++;
                }
                break;
        }
    }
    if (SP != -1)
        printf("Answer : %d\n", SS[SP]);
    else printf("Stack is empty!");
}

int main()
{
    int *SS, *DS, IP = 0, SP = 0, LP = 0; // LP - pointer to top of a label array
    instr *CS;
    label *LB; // LB - label array
    SS = initBlock(BLOCK_SIZE);
    DS = initBlock(BLOCK_SIZE);
    CS = (instr*)malloc(sizeof(instr) * CODE_SIZE);
    LB = (label*)malloc(sizeof(label) * CODE_SIZE);
    if ((CS == NULL) || (LB == NULL))
    {
        printf("Not enough memory for code or label!");
        exit(1);
    }
    char *name = getStr();
    printf("Enter name of file\n");
    scanf("%s", name);
    FILE *f = fopen(name, "r");
    if (f == NULL)
    {
        printf("File wasn't opened!");
        exit(1);
    }
    char *s = getStr();
    while (fgets(s, STR_SIZE, f) != NULL)
    {
        s = deleteSpaces(s);
        if (s[strlen(s) - 1] == '\n')
        {
            s[strlen(s) - 1] = 0;
        }
        if ((strcmp(s, "") != 0) && (s[0] != ';'))
        {
            CS[IP] = parseStr(s, LB, &LP, IP);
            IP ++;
        }
    }
    fclose(f);
    correctArgs(CS, IP, LB, LP);
    exitIfTypeErr(CS, IP);
    runInstr(CS, IP, DS, SS);
    int i;
    free(SS);
    free(DS);
    for (i = 0; i < IP; i ++)
    {
        free(CS[IP].lbl);
    }
    free(CS);
    free(LB);
    free(s);
    free(name);
    return 0;
}
