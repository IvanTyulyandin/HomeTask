int main()
{
    HashTable_t hashTable = createHashTable();
    int i = 0, n;
    char s[255];
    printf(" ");
    scanf("%d", &n);
    for(; i < n; i++)
    {
        scanf("%s", &s);
        putHash(hashTable, s);
    }
    cntNotEqualStr(hashTable);
}
