const int maxLen = 255;

void entRevStr(char *n)
{
    int i = 0;
    char c;
    while ((c = getchar()) != '\n')
    {
        n[maxLen - i] = c;
        i++;
    }
    n[maxLen - i] = 0;
}

int cmp(char *n1, char *n2)
{
    int i = 0;
    while (n1[maxLen - i] == n2[maxLen - i])
    {
        if ((n1[maxLen - i] == 0) && (n2[maxLen - i] == 0))
        {
            return 0;
        }
        i++;
    }
    return (int)(n1[maxLen - i] - n2[maxLen - i]);
}
