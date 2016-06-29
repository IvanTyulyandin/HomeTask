int fitsBits(int x, int n)
{
 int msk = 0, mskh = 1, minOne = 1;
 minOne = minOne << 31 >> 31;
 mskh = mskh << 31 >> 31;
 msk = mskh << (n + minOne);
 return !((x & ~msk) ^ x) | !((~x & ~msk) ^ ~x);
}
