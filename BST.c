#include <stdio.h>

typedef struct node node;

struct node
{
    int data;
    struct node *left;
    struct node *right;
};

node *root;

void addElem(int val)
{
    node *cur = root;
    node *prev = NULL;
    while (cur != NULL)
    {
        if (cur -> data != val)
        {
            prev = cur;
            if (cur -> data > val)
            {
                cur = cur -> left;
            }
            else
            {
                cur = cur -> right;
            }
        }
        else
        {
            printf("%d was in tree\n!", val);
            return;
        }
    }
    node *put;
    put = (node*)malloc(sizeof(node));
    if (put != NULL)
    {
        put -> data = val;
        put -> right = NULL;
        put -> left = NULL;
        if (root != NULL)
        {
            if (prev -> data > val)
            {
               prev -> left = put;
            }
            else
            {
                prev -> right = put;
            }
        }
        else
        {
            root = put;
        }
    }
}

void printTree(node *cur)
{
    printf("(%d ", cur -> data);
    if (cur -> left != NULL)
    {
        printTree(cur -> left);
    }
    else
    {
        printf("null ");
    }
    if (cur -> right != NULL)
    {
        printTree(cur -> right);
    }
    else
    {
        printf("null ");
    }
    printf(")");
}

void areInTree(int val)
{
    node *cur = root;
    while ((cur != NULL) && (cur -> data != val))
    {
        if (cur -> data > val)
        {
            cur = cur -> left;
        }
        else
        {
            cur = cur -> right;
        }
    }
    if (cur != NULL)
    {
        printf("%d is in tree\n", val);
    }
    else
    {
        printf("There is no %d in tree\n", val);
    }
}

void delElem(int val)
{
    node *cur = root;
    node *prev = NULL;
    while ((cur != NULL) && (cur -> data != val))
    {
        prev = cur;
        if (cur -> data > val)
        {
            cur = cur -> left;
        }
        else
        {
            cur = cur -> right;
        }
    }
    if (cur == NULL)
    {
        printf("Tree doesn't have %d\n", val);
        return;
    }
    if ((cur -> left == NULL) && (cur -> right == NULL))
    {
        if (cur == root)
        {
            free(root);
            root = NULL;
            return;
        }
        if (prev -> data > val)
        {
            prev -> left = NULL;
        }
        else
        {
            prev -> right = NULL;
        }
        free(cur);
        return;
    }
    if ((cur -> left == NULL) ^ (cur -> right == NULL))
    {
        if (cur == root)
        {
            if (cur -> left == NULL)
            {
                root = root -> right;
            }
            else
            {
                root = root -> left;
            }
            return;
        }
        if (cur -> left == NULL)
        {
            if (prev -> data > val)
            {
                prev -> left = cur -> right;
            }
            else
            {
                prev -> right = cur -> right;
            }
        }
        else
        {
            if (prev -> data > val)
            {
                prev -> left = cur -> left;
            }
            else
            {
                prev -> right = cur -> left;
            }
        }
        free(cur);
        return;
    }
    if ((cur -> left != NULL) && (cur -> right != NULL))
    {
        node *hlp = cur -> right;
        while (hlp -> left != NULL)
        {
            hlp = hlp -> left;
        }
        int bf = hlp -> data;
        delElem(hlp -> data);
        cur -> data = bf;
        return;
    }
}

void printByUp(node *cur)
{
    if (cur -> left != NULL)
    {
        printByUp(cur -> left);
    }
    printf("%d ", cur -> data);
    if (cur -> right != NULL)
    {
        printByUp(cur -> right);
    }
}

void printByDown(node *cur)
{
    if (cur -> right != NULL)
    {
        printByDown(cur -> right);
    }
    printf("%d ", cur -> data);
    if (cur -> left != NULL)
    {
        printByDown(cur -> left);
    }
}

void delTree(node *cur)
{
    if (cur -> left != NULL)
    {
        delTree(cur -> left);
    }
    if (cur -> right != NULL)
    {
        delTree(cur -> right);
    }
    free(cur);
}

int main()
{
    char c;
    int val;
    printf("a - add\ns - search\nd - delete\nU - print by increase\nD - print by decrease\np - simply print\nq - quit\n");
    while ((c = getchar()) != 'q')
    {
        if (c == 'a')
        {
            printf("Enter int to add ");
            scanf("%d", &val);
            addElem(val);
        }
        if (c == 's')
        {
            printf("Enter int to search ");
            scanf("%d", &val);
            areInTree(val);
        }
        if (c == 'd')
        {
            printf("Enter int to delete ");
            scanf("%d", &val);
            delElem(val);
        }
        if (c == 'U')
        {
            if (root != NULL)
            {
                printByUp(root);
                printf("\n");
            }
        }
        if (c == 'D')
        {
            if (root != NULL)
            {
                printByDown(root);
                printf("\n");
            }
        }
        if (c == 'p')
        {
            if (root != NULL)
            {
                printTree(root);
                printf("\n");
            }
        }
    }
    if (root != NULL)
    {
        delTree(root);
        root = NULL;
    }
    return 0;
}
