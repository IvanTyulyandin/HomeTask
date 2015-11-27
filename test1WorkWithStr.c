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
    int i, min;
    if (strlen(s1) < strlen(s2))
    {
        min = strlen(s1);
    }
    else
    {
        min = strlen(s2);
    }
    for (i = 0; i < min; i++)
    {
        if ((int*)s1[i] > (int*)s2[i])
        {
            return 1;
            break;
        }
        else if ((int*)s1[i] < (int*)s2[i])
             {
                 return -1;
                 break;
             }
    }
    if (strlen(s1) > strlen(s2))
    {
        return 1;
    }
    else if (strlen(s1) < strlen(s2))
         {
             return -1;
         }
         else
         {
             return 0;
         }
}
