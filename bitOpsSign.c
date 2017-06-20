int sign(int x)
{
    const msk = 0;
    int hlp = x;
    x = (hlp >> 31) | (!!x);
    return x;
}
