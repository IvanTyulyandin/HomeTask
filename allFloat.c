#include <stdio.h>

void printResult(int sgn, int exp, int mant)
{
    if (exp == 0xff)
    {
        if (mant == 0)
        {
            if (sgn == 0)
            {
                printf("+inf");
            }
            else
            {
                printf("-inf");
            }
        }
        else
        {
            printf("NaN");
        }
    }
    else
    {
        printf("%.0f * 1.", pow((-1), sgn));
        int i;
        for (i = 22; i > 0; i--)
        {
            printf("%d", (mant >> i) & 1);
        }
        printf(" * 2^%d", exp - 127);
    }
    printf("\n");
}

void floatToInt(float num)
{
    union fltToInt
    {
        int i;
        float f;
    };
    union fltToInt u;
    u.f = num;
    int sgn = (u.i >> 31) & 1;
    int exp = (u.i >> 23) & 0xff;
    int mant = u.i & (0x7fffff);
    printResult(sgn, exp, mant);
}

void floatToBits(float num)
{
    struct bits
    {
        unsigned int mant : 23;
        unsigned int exp : 8;
        unsigned int sgn : 1;
    };
    union fltToBits
    {
        float f;
        struct bits b;
    };
    union fltToBits u;
    u.f = num;
    int sgn = u.b.sgn;
    int exp = u.b.exp;
    int mant = u.b.mant;
    printResult(sgn, exp, mant);
}

void floatToPtr(float num)
{
    int *f = (int*)&num;
    int sgn = (*f >> 31) & 1;
    int exp = (*f >> 23) & 0xff;
    int mant = *f & 0x7fffff;
    printResult(sgn, exp, mant);
}

int main()
{
    float f1, f2;
    scanf("%f%f", &f1, &f2);
    floatToInt(f1  / f2);
    floatToBits(f1 / f2);
    floatToPtr(f1 / f2);
    return 0;
}
