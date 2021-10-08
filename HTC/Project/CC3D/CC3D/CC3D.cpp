#include <iostream>
//Learn from: https://www.youtube.com/watch?v=H4s55GgAg0I
static void Log(const char* m);
static int Muti(int a, int b)
{
    Log("Hi,I'm HTC");
    return a * b;
}
int main()
{
    std::cout << Muti(1, 2);
    system("pause");
    return 0;
}

static void Log(const char* m)
{
    std::cout << m << std::endl;
}