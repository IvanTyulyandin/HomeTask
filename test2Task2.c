void entReverseStr(char *num)
{
    char c;
    int i = 0;
    while ((c = getchar()) != '\n')
    {
        num[maxLen - i] = c;
        i++;
    }
    num[maxLen] = 0;
}

int cmp(char *num1, char *num2)
{
    int i;
    for (i = maxLen; i > 0; i--)
    {
        if (num1[i] != num2[i])
        {
            return (int*)(num1[i] - num2[i]);
        }
    }
    return 0;
}
