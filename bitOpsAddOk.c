int addOk (int x, int y)
{
	return !(~(x >> 31 ^ y >> 31) & (x >> 31 ^ ((x + y) >> 31)));
}
