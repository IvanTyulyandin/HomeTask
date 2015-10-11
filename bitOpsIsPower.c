int isPower(int x)
{
    int minOne = 1, msk = 1, sgn=0;
    minOne = minOne << 31 >> 31;
    sgn = (x >> 31) & msk;
    return !((x ^ (x + minOne)) & (x & (x + minOne)) | sgn) ;
}
