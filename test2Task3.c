void searchSlash()
{
    FILE *f = fopen("input.txt", "r");
    if (f == NULL)
    {
        printf("File wasn't opened!");
        return;
    }
    else
    {
        char *str;
        str = (char*)malloc(maxLen * sizeof(char));
        if (str != NULL)
        {
            char prev;
            int i = 0;
            while (fgets(str, maxLen, f) != NULL)
            {
                prev = str[0];
                for (i = 1; str[i] != 0; i++)
                {
                    if ((prev == '/') && (prev == str[i]))
                    {
                        printf(str);
                        break;
                    }
                    else
                    {
                        prev = str[i];
                    }
                }
            }
        }
        else
        {
            printf("Not enough memory!");
            return;
        }
        fclose(f);
    }
}
