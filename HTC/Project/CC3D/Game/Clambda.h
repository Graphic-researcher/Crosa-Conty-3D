#pragma once
//#include "common.h"
///Learn from:https://www.youtube.com/watch?v=mWgmBBz0y8c
///see more about lambda in cppreference.com
///https://en.cppreference.com/w/cpp/language/lambda
namespace Clambda {
	void ForEach(const std::vector<int>& values, void(*func)(int))
	{
		for (int value : values)
		{
			func(value);
		}
	}

	//std form,same as above
	void ForEach1(const std::vector<int>& values, const std::function<void(int)>&func)
	{
		for (int value : values)
		{
			func(value);
		}
	}

	//lambda anonymous function
	void Method1()
	{
		std::vector<int> values = { 1,2,3,4,5 };
		auto lambda = [](int v) {std::cout << "Value: " << v << std::endl; };
		ForEach(values, lambda);
	}

	void Method2()
	{
		std::vector<int> values = { 1,2,3,4,5 };
		//passing everything in
		int a = 5;
		auto lambda = [=](int v) {
			std::cout << "Value: " << v << std::endl; 
			std::cout << "a: " << a << std::endl;	
		};
		ForEach1(values, lambda);
		std::string b = "Wall\n";
		auto lambda2 = [b](int v) {
			std::cout << "Value: " << v << std::endl;
			std::cout << b;
		};
		printf("Another Method:\n");
		ForEach1(values, lambda2);
		auto lambda3 = [&b](int v) {
			std::cout << "Value: " << v << std::endl;
			std::cout << b;
		};
		printf("Another Method:\n");
		ForEach1(values, lambda3);
	}

	void Method3()
	{
		std::vector<int> values = { 1,2,3,4,5 };
		auto it = std::find_if(values.begin(), values.end(), [](int value) {return value > 3; });
		std::cout << *it << std::endl;//return the first satisfied number
		auto lambda = [](int v) {std::cout << "Value: " << v << std::endl; };
		ForEach1(values, lambda);
	}

	void LambdaTutor()
	{
		printf(Segline);
		printf("This is LambdaTutor\n");
		printf("Learn from:https://www.youtube.com/watch?v=mWgmBBz0y8c \n");
		printf("Method1:\n");
		Method1();
		printf("Method2:\n");
		Method2();
		printf("Method3:\n");
		Method3();
		printf(Segline);
	}
}