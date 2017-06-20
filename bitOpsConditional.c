int conditional(int x, int frt, int scd)
{
    x=!!x;
    x = x << 31 >> 31;
    return (frt & x) + (scd & (~x));
}
