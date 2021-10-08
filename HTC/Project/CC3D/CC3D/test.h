#pragma once
#include<iostream>
//static void Log(const char* m)//Fine
inline void Log(const char* m)//Fine
{
    std::cout << m << std::endl;
}