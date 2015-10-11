int logicalShift(int x, int cnt)
{
    int msk = 1, inth = 1;
    msk = msk << 31;
    msk = ~msk;
    inth = inth << 31 >> 31;
    msk = msk >> (cnt + inth);
    x = x >> cnt;
    return x & msk;
}
