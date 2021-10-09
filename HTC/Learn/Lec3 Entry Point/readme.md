# Lec3 Entry Point

tutorial : [here](https://www.youtube.com/watch?v=meARMOmTLgE&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=5)

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/1655f04900207eba1f0fef3a9b34b8a8a897ae98/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

## Directory 

![image-20211009153409928](https://i.loli.net/2021/10/09/TM3vY7j1IVWAH5Q.png)

CC3D.h would include all the need of CC3D project

It's easy to let other application use our engine's code

![image-20211009153648169](https://i.loli.net/2021/10/09/UgW5JNEtR7CfbZK.png)

```c++
//CC3D.h in CC3D
#pragma once
// For use by CC3D Application
#include "CC3D/Application.h"
```

## Define some Marcos

```c++
#pragma once
//Core.h in CC3D
#ifdef  CC3D_PLATFORM_WINDOWS
	#ifdef CC3D_BUILD_DLL
		#define CC3D_API _declspec(dllexport)
	#else
		#define CC3D_API _declspec(dllimport)
	#endif // CC3D_BUILD_DLL
#else
	#error CC3D only supports Windows!
#endif //  CC3D_PLATFORM_WINDOWS

```

![image-20211009154023202](https://i.loli.net/2021/10/09/8OgfyUKpubSXMxa.png)

![image-20211009154348045](https://i.loli.net/2021/10/09/O3hkDNxmuagW8Ci.png)

## Create Application Class

```c++
//Application.h in CC3D
#pragma once
#include "Core.h"//contain marcos
namespace CC3D {
	//class _declspec(dllexport) Application
	class CC3D_API Application //use CC3D_API as replacement of _declspec(dllexport) 
	{
	public:
		Application();
		virtual ~Application();//this class would be inherited by other class
		void run();
	};
}
```

```c++
////Application.cpp in CC3D
#include "Application.h"
namespace CC3D {
	Application::Application()
	{
	}
	Application::~Application()
	{
	}
	void Application::run()
	{
		while (true)
		{

		}
	}
}
```

## Make SandBox to use CC3D

```
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
```

Build Success and Run it :

![image-20211009154503937](https://i.loli.net/2021/10/09/jcD1pnqFOWgAm2f.png)

Paste dll to exe directory :

![image-20211009154656957](https://i.loli.net/2021/10/09/vXEQOjul169h7er.png)

Run success :

![image-20211009154618267](https://i.loli.net/2021/10/09/XUGcBqNHYDgEsmL.png)

## **Entry Point** (IMPORTANT):

in Application.h (in CC3D):

```c++
namespace CC3D {
//...other code
	//To be defined in CLIENT
	Application* CreateApplication();
}
```

in SandBox.cpp (in SandBox):

```c++
CC3D::Application* CC3D::CreateApplication()
{
	return new SandBox();
}
```

create EntryPoint.h in CC3D:

(Note , this file may get lots of errors before you finish all the necessary process. So Reopen the Visual Studio when you finish all the process and the errors would disappear automatically.)

```c++
#pragma once

#ifdef  CC3D_PLATFORM_WINDOWS

extern CC3D::Application* CC3D::CreateApplication();

int main(int argc,char** argv)
{
	auto app = CC3D::CreateApplication();
	app->Run();
	delete app;
	return 0;
}
#else
	#error CC3D only supports Windows!
#endif //  CC3D_PLATFORM_WINDOWS

```

include that file in CC3D.h:

```
//-------Entry Point--------------
#include "CC3D/EntryPoint.h"
//--------------------------------
```

Finally,build the SandBox project and Paste the dll into exe's directory

and it should run properly.