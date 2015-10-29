#include <stdio.h>

int main()
{
    union tFltP
    {
        float flt;
        struct
        {
            unsigned int mant : 23;
            unsigned int exp : 8;
            unsigned int sgn : 1;
        } bit;
    };

    union tFltP fltP;
    printf("Enter float\n");
    scanf("%f", &fltP.flt);
    printf("(-1)^%d * ", fltP.bit.sgn);
    int i = 22;
    for (; i >= 0; i--)
    {
        printf("%d", (fltP.bit.mant >> i) & 1);
    }
    fltP.bit.exp = fltP.bit.exp - 127;
    printf(" * 2^%d", fltP.bit.exp);
    return 0;
}
