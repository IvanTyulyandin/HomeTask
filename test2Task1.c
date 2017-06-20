char* delSymb(char *str, char del)
{
    char *bf;
    bf = (char*)malloc(1000 * sizeof(char));
    if (bf != NULL)
    {
        int i = 0;
        while (*str != 0)
        {
            if (*str != del)
            {
                bf[i] = *str;
                i++;
            }
            str++;
        }
        bf[i] = 0;
    }
    return bf;
}
