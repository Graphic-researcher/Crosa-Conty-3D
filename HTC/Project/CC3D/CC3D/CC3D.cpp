#include <iostream>
//Learn from: https://www.youtube.com/watch?v=or1dAmUO8k0
#include"test.h"
//#include"GLFW/glfw3.h"

extern "C" int glfwInit();

static int Muti(int a, int b)
{
    Log("Hi,I'm HTC");
    return a * b;
}

//int glfwInit()
//{
//    return 0;
//}

int main()
{
    //std::cout << Muti(1, 2) << std::endl;
    int a = glfwInit();
    std::cout << a << std::endl;
    system("pause");
    return 0;
}

//void Log(const char* m)
//{
//    std::cout << m << std::endl;
//}