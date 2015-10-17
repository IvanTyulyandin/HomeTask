#include <stdio.h>

int main()
{
    union chkEndian
    {
        int x;
        char unData [sizeof(int)];
    };
    union chkEndian chk;
    chk.x = 1;
    if (chk.unData[1] == 0)
    {
        printf("Little Endian");
    }
    else printf("Big Endian");
    return 0;
}
