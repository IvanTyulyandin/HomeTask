int logicalShift(int x, int cnt)
{
    int msk = 1;
    msk = msk << 31;
    msk = ~msk;
    msk = msk >> cnt;
    msk = (msk << 1) + 1;
    x = x >> cnt;
    return x & msk;
}
