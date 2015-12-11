#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node node;

struct node
{
    int data;
    int num;
    struct node *next;
};

node *head;
node *last;

void add(int val)
{
    node *newNode;
    newNode = (node*)malloc(sizeof(node));
    if (newNode != NULL)
    {
        if (head != NULL)
        {
            last -> next = newNode;
            newNode -> data = val;
            last = newNode;
            last -> next = NULL;
        }
        else
        {
            head = newNode;
            head -> data = val;
            head -> next = NULL;
            last = head;
        }
    }
    else
    {
        printf("Not enough memory!");
        system("pause");
    }
}

void printList()
{
    node *cur;
    cur = head;
    while (cur != NULL)
    {
        printf("%d ", cur -> data);
        cur = cur -> next;
    }
    printf("\nList was printed, dear user!\n");
}

void deleteList()
{
    node *cur;
    node *after;
    cur = head;
    while (cur != NULL)
    {
        after = cur -> next;
        free(cur);
        cur = after;
    }
    head = NULL;
    last = NULL;
}

void deleteElem(int val)
{
   node *cur, *befCur;
   cur = head;
   if (head != NULL)
   {
      if (head != last)
      {
          befCur = head;
          cur = head -> next;
          while ((cur != NULL) && (cur -> data != val))
          {
              befCur = cur;
              cur = cur -> next;
          }
          if (cur != NULL)
          {
              befCur -> next = cur -> next;
              free(cur);
          }
          else
          {
              if (head -> data == val)
              {
                  cur = head;
                  head = head -> next;
                  free(cur);
              }
          }
      }
      else
      {
          if (head -> data == val)
          {
              free(head);
              head = NULL;
              last = NULL;
          }
      }
   }
}

int cntElems()
{
    node *cur;
    int i = 0;
    cur = head;
    while (cur != NULL)
    {
        cur -> num = i;
        cur = cur -> next;
        i++;
    }
    return i;
}

void makeLoop()
{
   int i;
   i = cntElems();
   int srcLoop = rand()%i;
   node *cur;
   cur = head;
   for (i = 0; i < srcLoop; i++)
   {
       cur = cur -> next;
   }
   last -> next = cur;
}

void chkLoop()
{
    node *oneStep, *twoStep;
    if (head != NULL)
    {
        oneStep = head;
        twoStep = head -> next;
        while ((twoStep != NULL) && (twoStep != oneStep))
        {
            if (twoStep -> next -> next != NULL)
            {
                twoStep = twoStep -> next -> next;
                oneStep = oneStep -> next;
            }
            else
            {
                printf("It's a loop\n");
                return;
            }
        }
        if (twoStep != NULL)
        {
            printf("It's a loop\n");
        }
        else
        {
            printf("It's not a loop\n");
        }
    }
}

void makeNorm()
{
    last -> next = NULL;
}
int main()
{
    head = NULL;
    last = NULL;
    char c;
    int hlp;
    srand(time(NULL));
    printf("a - add int, r - delete int, p - print list, l - make loop, c - check loop, q - quit, n - delete loop\n");
    printf("Enter command\n");
    while ((c = getchar()) != 'q')
    {
        if (c == 'a')
        {
            printf("Enter int to list ");
            scanf("%d", &hlp);
            add(hlp);
            continue;
        }
        if (c == 'r')
        {
            printf("Enter int to delete from list ");
            scanf("%d", &hlp);
            deleteElem(hlp);
            continue;
        }
        if (c == 'p')
        {
            printList();
            continue;
        }
        if (c == 'l')
        {
            makeLoop();
            continue;
        }
        if (c == 'c')
        {
            chkLoop();
            continue;
        }
        if (c == 'n')
        {
            makeNorm();
            continue;
        }
    }
    deleteList();
    return 0;
}
