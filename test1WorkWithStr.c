#include <stdio.h>
#include <string.h>

int cntLen(char *s)
{
    int i = 0;
    while (s[i] != 0)
    {
        i++;
    }
    return i;
}

int memStrCmp(char *s1, char *s2)
{
    if ((*s1 != 0) && (*s2 != 0))
    {
        while (*s1 && *s2)
        {
            if (*s1 > *s2)
            {
                return 1;
            }
            if (*s1 < *s2)
            {
                return -1;
            }
            s1++;
            s2++;
        }
    }
    else
    {
        return *s1 - *s2;
    }
    return 0;
}

void strCopy(char *s1, char *s2)
{
    int i;
    for (i = 0; s2[i] != 0; i++)
        s1[i] = s2[i];
    s1[i] = 0;
}

void concat(char *dst, char *src)
{
    int i = 0;
    while (dst[i] != 0)
    {
        i++;
    }
    int j = 0;
    while (src[j] != 0)
    {
        dst[i + j] = src[j];
        j++;
    }
    dst[i + j] = 0;
}
