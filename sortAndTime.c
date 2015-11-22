#include <stdio.h>
#include <time.h>
#include <stdlib.h>

const maxVal = 100;
int n;
int *pData = NULL;

void screwUpData(int *pData)
{
   int i;
   for (i = 0; i < n; i++)
    {
        pData[i] = rand()%100;
    }
}

void print(int *pData)
{
    int i = 0;
    for (; i < n; i++)
    {
        printf("%d ", pData[i]);
    }
    printf("\n");
}

void bubbleSort(int *pData)
{
    int i, j;
    int hlp = 0;
    for (i = 0; i < n; i++)
    {
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

void countingSort(int *pData)
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

void quickSort ( int *pData, int first, int last)
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
quickSort (pData, first, j);
quickSort (pData, i, last);
}

void getMem(int num, int *pData)
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
    long numEl[9] = {5, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};
    long i;
  //  clock_t clkBub = 0, clkQck = 0, clkCnt = 0;
    clock_t start, finish;
    for (i = 0; i < 9; i++)
    {
        n = numEl[i];
        getMem(n, pData);

        screwUpData(pData);
        /*countingSort(pData);
        start = clock();
        finish = clock();
*/

        free(pData);
    }
    return 0;
}
