#include <stdio.h>

typedef struct node node;
typedef struct myTree myTree;

struct node
{
    int data;
    struct node *left;
    struct node *right;
};

struct myTree
{
    struct node *root;
};

void addElem(int val, myTree* tree)
{
    node *cur = tree -> root;
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
            printf("%d was in tree!\n", val);
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
        if (tree -> root != NULL)
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
            tree -> root = put;
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

void areInTree(int val, myTree* tree)
{
    node *cur = tree -> root;
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

void delElem(int val, myTree* tree)
{
    node *cur = tree -> root;
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
        if (cur == tree -> root)
        {
            free(tree -> root);
            tree -> root = NULL;
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
        if (cur == tree -> root)
        {
            if (cur -> left == NULL)
            {
                tree -> root = tree -> root -> right;
            }
            else
            {
                tree -> root = tree -> root -> left;
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
        delElem(hlp -> data, tree);
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
    myTree *tree;
    tree = (myTree*)malloc(sizeof(myTree));
    if (tree == NULL)
    {
        printf("Not enough memory!");
        exit(0);
    }
    tree -> root = NULL;
    printf("a - add\ns - search\nd - delete\nU - print by increase\nD - print by decrease\np - simply print\nq - quit\n");
    while ((c = getchar()) != 'q')
    {
        if (c == 'a')
        {
            printf("Enter int to add ");
            scanf("%d", &val);
            addElem(val, tree);
            continue;
        }
        if (c == 's')
        {
            printf("Enter int to search ");
            scanf("%d", &val);
            areInTree(val, tree);
            continue;
        }
        if (c == 'd')
        {
            printf("Enter int to delete ");
            scanf("%d", &val);
            delElem(val, tree);
            continue;
        }
        if (c == 'U')
        {
            if (tree -> root != NULL)
            {
                printByUp(tree -> root);
                printf("\n");
            }
            else
            {
                printf("No elements!\n");
            }
            continue;
        }
        if (c == 'D')
        {
            if (tree -> root != NULL)
            {
                printByDown(tree -> root);
                printf("\n");
            }
            else
            {
                printf("No elements!\n");
            }
            continue;
        }
        if (c == 'p')
        {
            if (tree -> root != NULL)
            {
                printTree(tree -> root);
                printf("\n");
            }
            else
            {
                printf("No elements!\n");
            }
            continue;
        }
    }
    if (tree -> root != NULL)
    {
        delTree(tree -> root);
        tree -> root = NULL;
    }
    return 0;
}
