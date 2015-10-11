int addOk(int x, int y)
{
    int sgnX = 0, sgnY = 0, sgnRes = 0, msk = 1;
    sgnX = (x >> 31) & msk;
    sgnY = (y >> 31) & msk;
    sgnRes = ((x + y) >> 31) & 1;
    return (sgnX ^ sgnY) & sgnRes;
}
