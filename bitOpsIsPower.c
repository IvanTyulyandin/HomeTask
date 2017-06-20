int isPower2(int x)
{
    return !((x - 1) & x) & !(x >> 31) & !!(x);
}
