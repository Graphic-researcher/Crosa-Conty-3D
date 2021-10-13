#pragma once
///Learn from:https://www.youtube.com/watch?v=p4sDgQ-jao4
//#include"common.h"
namespace funcptr {
	void Hello(const char* m)
	{
		printf(m);
		printf("\n");
	}

	void PrintValue(int v)
	{
		std::cout << "Value: " << v << std::endl;
	}

	void ForEach(const std::vector<int>& values, void(*func)(int))
	{
		for (int value : values)
		{
			func(value);
		}
	}

	void Method1()
	{
		typedef void(*HelloFunction)(const char* m);

		void(*htc)(const char* m) = nullptr;
		//auto func = &Hello;
		auto func = Hello;
		func("I'm func");
		htc = Hello;
		htc = &Hello;
		HelloFunction fp = Hello;
		htc("I'm htc");
		//HelloFunction("I'm HelloFunction"); //wrong 
		fp("I'm fp");
	}

	void Method2()
	{
		std::vector<int> values = { 1,2,3,4,5 };
		ForEach(values,PrintValue);
	}

	//lambda anonymous function
	void Method3()
	{
		std::vector<int> values = { 1,2,3,4,5 };
		ForEach(values, [](int v) {std::cout << "Value: " << v << std::endl; });
	}

	void FunctionPointerTutor()
	{
		printf(Segline);
		printf("This is FunctionPointerTutor\n");
		printf("Learn from:https://www.youtube.com/watch?v=p4sDgQ-jao4 \n");
		printf("Method1:\n");
		funcptr::Method1();
		printf("Method2:\n");
		funcptr::Method2();
		printf("Method3:\n");
		funcptr::Method3();
		printf(Segline);
	}
}