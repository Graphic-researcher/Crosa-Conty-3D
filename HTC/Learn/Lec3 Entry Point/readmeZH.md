# Lec3 Entry Point

课程地址: [here](https://www.youtube.com/watch?v=meARMOmTLgE&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=5)

代码版本 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/1655f04900207eba1f0fef3a9b34b8a8a897ae98/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

## 目录结构 

![image-20211009153409928](https://i.loli.net/2021/10/09/TM3vY7j1IVWAH5Q.png)

CC3D.h 将包含CC3D所需要的代码

这样其他应用就可以容易地使用CC3D代码

![image-20211009153648169](https://i.loli.net/2021/10/09/UgW5JNEtR7CfbZK.png)

```c++
//CC3D.h in CC3D
#pragma once
// For use by CC3D Application
#include "CC3D/Application.h"
```

## 定义一些宏

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

## 创建应用类

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

## 让SandBox使用应用类

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

构建并运行 :

![image-20211009154503937](https://i.loli.net/2021/10/09/jcD1pnqFOWgAm2f.png)

将dll放入exe同级目录 :

![image-20211009154656957](https://i.loli.net/2021/10/09/vXEQOjul169h7er.png)

运行成功 :

![image-20211009154618267](https://i.loli.net/2021/10/09/XUGcBqNHYDgEsmL.png)

**程序入口点** (核心):

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

创建 EntryPoint.h in CC3D:

(注意，在没有完成所有代码编写前，vs可能会报错，这是正常现象，完成必要代码编写后，关闭vs然后重新打开，错误就没啦(*^_^*))

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

最后构建SandBox项目然后把dll放入exe同级目录，程序应该运行正常。