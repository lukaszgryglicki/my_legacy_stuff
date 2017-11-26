int main(int lb, char** par)
{
    unsigned long l;
    if (lb < 2) return 1;

    l = atoi(par[1]);
    write(1, (void*)&l, 4);
    return 0;
}

