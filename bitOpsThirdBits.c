int thirdBits()
{
    const msk = 36;
    int fh = msk, sh = 0, sv = 0;
    sh = (fh << 6) + msk;
    sv = sh; //12
    fh = (sv << 12) + sh; //24
    sh = (fh << 6) + msk;
    return sh;
}
