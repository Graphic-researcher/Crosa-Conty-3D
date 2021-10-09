//#include "../../CC3D/src/CC3D.h"
#include "CC3D.h"

class SandBox : public CC3D::Application
{
public:
	SandBox() {}
	~SandBox() {}

};

CC3D::Application* CC3D::CreateApplication()
{
	return new SandBox();
}