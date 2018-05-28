void myprint(char* msg, int len);

int choose(int a, int b)
{
    if (a >= b)
    {
        myprint("the first one", 13);
    }
    else
    {
        myprint("the second one", 13);
    }

    return 0;
}