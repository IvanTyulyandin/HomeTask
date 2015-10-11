int bitAnd(int x, int y)
{
    int xorX = 0, xorY = 0, res = 0;
    xorX = ~x;
    xorY = ~y;
    res = ~(xorX | xorY);
    return res;
}
