typedef struct node node;
typedef struct list list;
typedef struct hashT hashT;

const int maxLenStr = 255;

int hashConst(char*);

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
    int (*hashFunc)(char *);
};

int hashConst(char *key)
{
    return 19;
}

int hashSum(char *key)
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

hashT* createHashT(int(*func)(char*), int size)
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
    int (*pH)(char*) = ht -> hashFunc;
    int offset = (*pH)(key);
    offset = ((offset % ht -> size) + ht -> size) % ht -> size; // in case of hashResult = -int
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

void set(char *key, int val, hashT* ht)
{
    int (*pH)(char*) = ht -> hashFunc;
    int offset = (*pH)(key);
    offset = ((offset % ht -> size) + ht -> size) % ht -> size;
    list *curList = ht -> entry[offset];
    node *cur = curList -> head;
    while ((cur != NULL) && (strcmp(cur -> key, key) != 0))
    {
        cur = cur -> next;
    }
    if (cur != NULL)
    {
        cur -> val = val;
    }
    else
    {
        printf("No element %s!\n", key);
    }
}

int get(char *key, hashT *ht)
{
    int (*pH)(char*) = ht -> hashFunc;
    int offset = (*pH)(key);
    offset = ((offset % ht -> size) + ht -> size) % ht -> size;
    list *curList = ht -> entry[offset];
    node *cur = curList -> head;
    while ((cur != NULL) && (strcmp(cur -> key, key) != 0))
    {
        cur = cur -> next;
    }
    if (cur != NULL)
    {
        return cur -> val;
    }
    else
    {
        printf("No element %s!\n", key);
        return 0;
    }
}

void delElem(char *key, hashT *ht)
{
    int (*pH)(char*) = ht -> hashFunc;
    int offset = (*pH)(key);
    offset = ((offset % ht -> size) + ht -> size) % ht -> size;
    list *curList = ht -> entry[offset];
    node *cur = curList -> head;
    node *prev = NULL;
    while ((cur != NULL) && (strcmp(cur -> key, key) != 0))
    {
        prev = cur;
        cur = cur -> next;
    }
    if (cur != NULL)
    {
       if (cur != curList -> head)
       {
           prev -> next = cur -> next;
           free(cur);
       }
       else
       {
           prev = curList -> head;
           curList -> head = curList -> head -> next;
           free(prev);
       }
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

int cntNotNullCell(hashT* ht)
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
    return res / ht -> size;
}

int cntMinNotNull(hashT *ht)
{
    list *curList;
    int i, minLen = -1, curLen;
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
        if ((curLen < minLen) && (minLen != -1))
        {
            minLen = curLen;
        }
        else
        {
            if ((minLen == -1) && (minLen != 0))
            {
                minLen = curLen;
            }
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
    int i, curLen, maxLen;
    node *cur;
    curList = ht -> entry[0];
    maxLen = 0;
    cur = curList -> head;
    while (cur != NULL)
    {
        maxLen++;
        cur = cur -> next;
    }
    for (i = 1; i < ht -> size; i++)
    {
        curList = ht -> entry[i];
        cur = curList -> head;
        curLen = 0;
        while (cur != NULL)
        {
            curLen++;
        }
        if (curLen > maxLen)
        {
            maxLen = curLen;
        }
    }
    return maxLen;

}

void printHt(hashT* ht)
{
    list *curList;
    if (ht != NULL)
    {
        int i;
        for (i = 0; i < ht -> size; i++)
        {
            curList = ht -> entry[i];
            node *cur = curList -> head;
            while (cur != NULL)
            {
                printf("%s %d\n", cur -> key, cur -> val);
                cur = cur -> next;
            }
        }
    }
    else
    {
        printf("HashTable is empty!");
    }
}
