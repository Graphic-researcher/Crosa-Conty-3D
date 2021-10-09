#include<iostream>
namespace CC3D {
	_declspec(dllimport) void Print();
}
void main()
{
	CC3D::Print();
	system("pause");
}