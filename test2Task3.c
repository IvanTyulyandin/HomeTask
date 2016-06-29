#include <stdio.h>
#include <string.h>

const int maxLen = 255;

void searchDoubleSlash(FILE *f)
{
    char *str;
    str = (char*)malloc(maxLen * sizeof(char));
    if (str != NULL)
    {
        char prev;
        int i = 0;
        int wereTwoSlash;
        while (fgets(str, maxLen, f) != NULL)
        {
            prev = str[0];
            if (prev != '\n')
            {
                wereTwoSlash = 0;
                for (i = 1; str[i] != 0; i++)
                {
                    if (wereTwoSlash == 1)
                    {
                        printf("%c", str[i]);
                    }
                    else
                    {
                        if ((str[i] == '/') && (prev == '/'))
                        {
                            wereTwoSlash = 1;
                            printf("//");
                        }
                    }
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
}

int main()
{

    FILE *f = fopen("input.txt", "r");
    if (f == NULL)
    {
        printf("File wasn't opened!");
        exit(1);
    }
    searchDoubleSlash(f);
    fclose(f);
    return 0;
}
