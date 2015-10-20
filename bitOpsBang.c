int bang(int x)
{
    int frt = 0, scd = 0, msk = 1;
    msk = msk << 31;
    frt = x >> 31; 
    scd = (~x + 1) >> 31;
    return ~(frt ^ scd) & 1; 
}
