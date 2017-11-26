int main()
{
    unsigned long l;
    read(0, (void*)&l, 4);
    printf("0x%08x\n", l);
    return 0;
}

