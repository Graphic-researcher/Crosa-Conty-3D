//#include "../../CC3D/src/CC3D.h"
#include "CC3D.h"
class SandBox : public CC3D::Application
{
public:
	SandBox() {}
	~SandBox() {}

};



void main()
{
	SandBox* test = new SandBox();
	test->run();
	delete test;
}