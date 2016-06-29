int getByte(int x, int wtw)
{
    int cnt = 0;
    cnt = (wtw << 3);
	x = (x >> cnt) & 0xFF;
	return x;
}
