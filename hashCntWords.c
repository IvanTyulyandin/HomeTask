#include <stdio.h>
#include <ctype.h>
#include <time.h>

typedef struct node node;
typedef struct list list;
typedef struct hashT hashT;

const int maxLenStr = 255;

struct node
{
    int val;
    char *key;
    node *next;
};

struct list
{
    node *head;
    node *last;
};

struct hashT
{
    int size;
    list **entry;
    int (*hashFunc)(char *, hashT*);
};

int hashConst(char *key, hashT* ht)
{
    return 19 % ht -> size;
}

int hashSum(char *key, hashT* ht)
{
    int res = 0;
    char i = 0;
    while (key[i] != 0)
    {
        res += (int)key[i];
        i++;
    }
    return res;
}

int hashRot13(char *key, hashT* ht)
{
    long i, hash = 0;
    for(i = 0; key[i] != 0; i++)
    {
        hash += (int)key[i];
        hash -= (hash << 13) | (hash >> 19);
    }
    int size = ht -> size;
    return ((hash % size) + size) % size;
}

hashT* createHashT(int(*func)(char*, hashT*), int size)
{
    list** entry ;
    entry = (list**)malloc(sizeof(list*) * size);
    if (entry == NULL)
    {
        printf("Not enough memory!");
        exit(1);
    }
    int i;
    for (i = 0; i < size; i++)
    {
        list *cur;
        cur = (list*)malloc(sizeof(list));
        if (cur == NULL)
        {
            printf("Not enough memory!");
            exit(1);
        }
        entry[i] = cur;
        entry[i] -> head = NULL;
        entry[i] -> last = NULL;
    }
    hashT* newTable = (hashT*)malloc(sizeof(hashT));
    if (newTable == NULL)
    {
        printf("Not enough memory!");
        exit(1);
    }
    newTable -> entry = entry;
    newTable -> size = size;
    newTable -> hashFunc = func;
    return newTable;
}

void handleNewElem(char *key, hashT* ht)
{
    int (*pH)(char*, hashT*) = ht -> hashFunc;
    int offset = (*pH)(key, ht);
    list *curList = ht -> entry[offset];
    node *cur = curList -> head;
    while ((cur != NULL) && (strcmp(cur -> key, key) != 0))
    {
        cur = cur -> next;
    }
    if (cur == NULL)
    {
        node *bf;
        bf = (node*)malloc(sizeof(node));
        if (bf == NULL)
        {
            printf("Not enough memory!");
            exit(1);
        }
        bf -> key = (char*)malloc(sizeof(char) * (strlen(key) + 1));
        if (bf == NULL)
        {
            printf("Not enough memory!");
            exit(1);
        }
        strcpy(bf -> key, key);
        bf -> val = 1;
        bf -> next = NULL;
        if (curList -> head == NULL)
        {
            curList -> head = bf;
            curList -> last = curList -> head;
        }
        else
        {
            curList -> last -> next = bf;
            curList -> last = bf;
        }
    }
    else
    {
        cur -> val += 1;
    }
}

void delTable(hashT** htp)
{
    hashT* ht = *htp;
    list *curList;
    int i;
    for (i = 0; i < ht -> size; i++)
    {
       curList = ht -> entry[i];
       node *cur = curList -> head;
       node *prev = NULL;
       while (cur != NULL)
       {
           prev = cur;
           cur = cur -> next;
           free(prev -> key);
           prev -> key = NULL;
           free(prev);
       }
       curList -> head = NULL;
       curList -> last = NULL;
    }
    free(ht -> entry);
    ht -> entry = NULL;
    free(ht);
    *htp = NULL;
}

int cntAllElems(hashT *ht)
{
    list *curList;
    int i, res = 0;
    for (i = 0; i < ht -> size; i++)
    {
        curList = ht -> entry[i];
        node *cur = curList -> head;
        while (cur != NULL)
        {
            res++;
            cur = cur -> next;
        }
    }
    return res;
}

int cntNotNullChains(hashT* ht)
{
    list *curList;
    int i, res = 0;
    for (i = 0; i < ht -> size; i++)
    {
        curList = ht -> entry[i];
        if (curList -> head != NULL)
        {
            res++;
        }
    }
    return res;
}

float cntMediumLen(hashT *ht)
{
    list *curList;
    int i;
    float res;
    for (i = 0; i < ht -> size; i++)
    {
        curList = ht -> entry[i];
        node *cur = curList -> head;
        while (cur != NULL)
        {
            res += 1;
            cur = cur -> next;
        }
    }
    printf("", res, ht -> size);
    res = res / (ht -> size);
    return res;
}

int cntMinNotNull(hashT *ht)
{
    list *curList;
    int i, minLen = 100000, curLen;
    node *cur;
    curList = ht -> entry[0];
    cur = curList -> head;
    for (i = 0; i < ht -> size; i++)
    {
        curList = ht -> entry[i];
        curLen = 0;
        cur = curList -> head;
        while (cur != NULL)
        {
            curLen++;
            cur = cur -> next;
        }
        if ((curLen < minLen) && (curLen != 0))
        {
            minLen = curLen;
        }
    }
    if (minLen != 0)
    {
        return minLen;
    }
    else
    {
        printf("Table is empty!\n");
        return 0;
    }
}

int cntMaxLen(hashT *ht)
{
    list *curList;
    int i, curLen, maxLen = 0;
    node *cur;
    for (i = 0; i < (ht -> size); i++)
    {
        curList = ht -> entry[i];
        cur = curList -> head;
        curLen = 0;
        while (cur != NULL)
        {
            curLen++;
            cur = cur -> next;
        }
        if (curLen > maxLen)
        {
            maxLen = curLen;
        }
    }
    return maxLen;
}

void useHt(int(*fp)(char* , hashT*), int size, char *s)
{
    FILE *f = fopen("input.txt", "r");
    if (f == NULL)
    {
        printf("File wasn't opened!");
        exit(1);
    }
    hashT *ht;
    ht = createHashT(fp, size);
    int c, i, curState = 0, amount = 0;
    while ((c = fgetc(f)) != EOF)
    {
        switch (curState)
        {
            case 0:
            {
                if (isalnum(c) != 0)
                {
                    i = 0;
                    s[i] = c;
                    curState = 1;
                }
                break;
            }
            case 1:
            {
                if (isalnum(c) != 0)
                {
                    i++;
                    s[i] = c;
                }
                else
                {
                    i++;
                    s[i] = 0;
                    handleNewElem(s, ht);
                    curState = 0;
                    s[0] = 0;
                }
                break;
            }
        }
    }
    if (s[0] != 0)
    {
        handleNewElem(s, ht);
    }
    fclose(f);
    printf("    Different words %d\n", cntAllElems(ht));
    printf("    Max length of chain %d\n", cntMaxLen(ht));
    printf("    Medium length %.5f\n", cntMediumLen(ht));
    printf("    Length of minimum not null chain %d\n", cntMinNotNull(ht));
    printf("    Used baskets %d\n", cntNotNullChains(ht));
    delTable(&ht);
}

int main()
{
    const int sizeConst = 1;
    const int sizeSum = 5000;
    const int sizeRot13 = 100000;
    int allSize[3] = {sizeConst, sizeSum, sizeRot13};
    int hashArr[3] = {hashConst, hashSum, hashRot13};
    int sizeArr = 3;
    char *s;
    s = (char*)malloc(sizeof(char) * maxLenStr);
    if (s == NULL)
    {
        printf("Not enough memory!");
        exit(1);
    }
    int i;
    clock_t start, delta;
    for (i = 0; i < sizeArr; i++)
    {
        switch (i)
        {
            case 0:
                {
                   printf("hashConst\n");
                   break;
                }
            case 1:
                {
                    printf("hashSum\n");
                    break;
                }
            case 2:
                {
                    printf("hashRot13\n");
                    break;
                }
        }
        start = clock();
        useHt(hashArr[i], allSize[i], s);
        delta = clock() - start;
        printf("    %.4f\n\n", (double)delta / CLOCKS_PER_SEC);
    }
    free(s);
    return 0;
}
