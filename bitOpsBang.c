int bang(int x)
{
    int frt = 0, scd = 0, msk = 1;
    msk = msk << 31;
    frt = (frt & msk) >> 31;
    scd = (~x + 1) >> 31;
    return ~(frt ^ scd) & 1; // for 0 or 1 doing &1
}
