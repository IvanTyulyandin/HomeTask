 #include <stdio.h>

typedef struct node node;
typedef struct myList myList;

struct node
{
    int data;
    struct node *next;
};

struct myList
{
    struct node *head;
    struct node *last;
};

void add(int val, myList* list)
{
    node *newNode;
    newNode = (node*)malloc(sizeof(node));
    if (newNode != NULL)
    {
        if (list -> head != NULL)
        {
            list -> last -> next = newNode;
            list -> last = newNode;
            newNode -> data = val;
            list -> last -> next = NULL;
        }
        else
        {
            list -> head = newNode;
            list -> head -> data = val;
            list -> head -> next = NULL;
            list -> last = list -> head;
        }
    }
    else
    {
        printf("Not enough memory!");
        exit(0);
    }
}

void printList(myList* list)
{
    node *cur;
    cur = list -> head;
    while (cur != NULL)
    {
        printf("%d ", cur -> data);
        cur = cur -> next;
    }
    printf("\nList was printed, dear user!\n");
}

void deleteList(myList* list)
{
    node *cur;
    node *after;
    cur = list -> head;
    while (cur != NULL)
    {
        after = cur -> next;
        free(cur);
        cur = after;
    }
    list -> head = NULL;
    list -> last = NULL;
}

void deleteElem(int val, myList* list)
{
   node *cur, *befCur;
   cur = list -> head;
   if (list -> head != NULL)
   {
      if (list -> head != list -> last)
      {
          befCur = list -> head;
          cur = list -> head -> next;
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
              if (list -> head -> data == val)
              {
                  cur = list -> head;
                  list -> head = list -> head -> next;
                  free(cur);
              }
          }
      }
      else
      {
          if (list -> head -> data == val)
          {
              free(list -> head);
              list -> head = NULL;
              list -> last = NULL;
          }
      }
   }
}

int main()
{
    myList* list;
    list = (myList*)malloc(sizeof(myList));
    if (list == NULL)
    {
        printf("Not enough memory!");
        exit(0);
    }
    list -> head = NULL;
    list -> last = NULL;
    char c;
    int hlp;
    while ((c = getchar()) != 'q')
    {
        if (c == 'a')
        {
            printf("Enter int to list ");
            scanf("%d", &hlp);
            add(hlp, list);
            continue;
        }
        if (c == 'r')
        {
            printf("Enter int to delete from list ");
            scanf("%d", &hlp);
            deleteElem(hlp, list);
            continue;
        }
        if (c == 'p')
        {
            printList(list);
            continue;
        }
    }
    deleteList(list);
    return 0;
}
