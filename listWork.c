#include <stdio.h>

typedef struct node node;

struct node
{
    int data;
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
            last = newNode;
            newNode -> data = val;
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

int main()
{
    head = NULL;
    last = NULL;
    char c;
    int hlp;
    while ((c = getchar()) != 'q')
    {
        if (c == 'a')
        {
            printf("Enter int to list ");
            scanf("%d", &hlp);
            add(hlp);
        }
        if (c == 'r')
        {
            printf("Enter int to delete from list ");
            scanf("%d", &hlp);
            deleteElem(hlp);
        }
        if (c == 'p')
        {
            printList();
        }
    }
    deleteList();
    return 0;
}
