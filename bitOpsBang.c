int bang(int x)
{
	return (((~x + 1) | x) >> 31) + 1;
}
