int addOk(int x, int y)
{
    int sgnX = 0, sgnY = 0, sgnRes = 0, sgnH = 0;
    sgnX = (x >> 31) & 1;
    sgnY = (y >> 31) & 1;
    sgnRes = ((x + y) >> 31) & 1;
    sgnH = !(sgnX) | !(sgnY);
    return ((sgnX ^ sgnY) & sgnRes) | sgnH;
}
