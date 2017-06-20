void taskXor(int n)
{
    char s[255];
    int data[255] = {0};
    int i, j;
    for (i = 0; i < n; i++)
    {
        scanf("%s", &s);
        j = 0;
        for (; j < strlen(s); j++)
        {
            data[j] = (data[j] ^ (int)s[j]);
        }
    }
    i = 0;
    while (data[i] != 0)
    {
        printf("%c", data[i]);
        i++;
    }
}
