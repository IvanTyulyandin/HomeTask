int bitXor(int x, int y)
{
    int res = 0;
    res = ~(~(~x & y) & ~(x & ~y));
    return res;
}
