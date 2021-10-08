#pragma once
#include<iostream>
//static void Log(const char* m)//Fine
//inline void Log(const char* m)//Fine
//{
//    std::cout << m << std::endl;
//}
//void Log(const char* m);

namespace CC3D {
	void Log(const char* m);
	void Log();
}

static void Log()
{
	std::cout << "HelloWorld\n";
}