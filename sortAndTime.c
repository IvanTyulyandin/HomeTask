#include <stdio.h>
#include <time.h>
#include <stdlib.h>

const maxVal = 100;
int n;
int *pData = NULL;
clock_t start = 0, fin = 0;
int dontWaitMore;

void screwUpData()
{
   int i;
   for (i = 0; i < n; i++)
    {
        pData[i] = rand()%100;
    }
}

void print()
{
    int i = 0;
    for (; i < n; i++)
    {
        printf("%d ", pData[i]);
    }
    printf("\n");
}

void bubbleSort()
{
    dontWaitMore = 0;
    int i, j;
    int hlp = 0;
    clock_t cur = 0;
    for (i = 0; i < n; i++)
    {
         cur = clock();
         if (((double)cur / CLOCKS_PER_SEC) > 15.0)
            {
                dontWaitMore = 1;
                return;
            }
        for (j = 0; j < n - i -1; j++)
        {
            if (pData[j] > pData[j + 1])
            {
                hlp = pData[j];
                pData[j] = pData[j + 1];
                pData[j + 1] = hlp;
            }
        }
    }
}

void countingSort()
{
    int hlpMas[100] = {0}; // 100 = maxVal;
    int i, j;
    for (i = 0; i < n; i++)
    {
        hlpMas[pData[i]] += 1;
    }
    int cnt = 0;
    for (i = 0; i < maxVal; i++)
    {
        for (j = 0; j < hlpMas[i]; j++)
        {
            pData[cnt] = i;
            cnt++;
        }
    }
}

void quickSort (int first, int last)
{
 int pivot, i, j, buf;
 if ( first >= last ) return;
 i = first;
 j = last;
 pivot = pData[(first + last) / 2];
 while ( i <= j )
 {
    while ( pData[i] < pivot ) i ++;
    while ( pData[j] > pivot ) j --;
    if ( i <= j )
    {
        buf = pData[i];
        pData[i] = pData[j];
        pData[j] = buf;
        i++;
        j--;
    }
 }
quickSort (first, j);
quickSort (i, last);
}

void getMem(int num)
{
    pData = (int*)malloc(num * sizeof(int));
    if (pData == NULL)
    {
        printf("Not enough memory!");
        system("pause");
    }
}

int main()
{
    srand(time(NULL));
    long numEl[9] = {5, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8};
    int i;
    printf("BubbleSort\n");
    for (i = 0; i < 9; i++)
      {
        n = numEl[i];
        printf("%9d ", numEl[i]);
        getMem(n);
        screwUpData(pData);
        start = clock();
        bubbleSort();
        fin = clock() - start;
        if (dontWaitMore == 0)
        {
            printf("%10.4f", (double)fin / CLOCKS_PER_SEC);
        }
        else printf("       n/a");
        free(pData);
        pData = NULL;
        printf("\n");
      }
    printf("\nCountingSort\n");
    for (i = 0; i < 9; i++)
    {
        n = numEl[i];
        printf("%9d ", numEl[i]);
        getMem(n);
        screwUpData(pData);
        start = clock();
        countingSort();
        fin = clock() - start;
        printf("%10.4f\n", (double)fin / CLOCKS_PER_SEC);
        free(pData);
        pData = NULL;
    }
    printf("\nQuickSort\n");
    for (i = 0; i < 9; i++)
    {
        n = numEl[i];
        printf("%9d ", numEl[i]);
        getMem(n);
        screwUpData(pData);
        start = clock();
        quickSort(0, n - 1);
        fin = clock() - start;
        printf("%10.4f\n", (double)fin / CLOCKS_PER_SEC);
        free(pData);
        pData = NULL;
    }
    return 0;
}
