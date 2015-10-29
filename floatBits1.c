#include <stdio.h>

int main()
{
    union tFlt
    {
        int intF;
        float fltF;
    };

    union tFlt flt;
    printf("Enter float\n");
    scanf("%f", &flt.fltF);
    int sgn = 0, exp = 0, mant = 0;
    sgn = (flt.intF >> 31) & 1;
    exp = (flt.intF >> 23) & 0xFF;
    exp = exp - 127;
    mant = flt.intF & 0x7FFFFF;
    printf("(-1)^%d * ", sgn);
    int i = 22;
    for (; i >= 0; i--)
    {
        printf("%d", (mant >> i) & 1);
    }
    printf(" * 2^%d", exp);
    return 0;
}
