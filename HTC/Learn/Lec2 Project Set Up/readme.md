# Lec2 Project Set Up

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/a8896b16e1ae112cb5c7c94298b4e997569188f2/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

## Core Idea

Solution Project would contain more than the engine(CC3D is the name I call it...)

Build the engine to a dll(dynamic link library)

Create some kind of external executable application project(.exe)

Link the engine(dll) to exe

## Prerequisite Tutorial

dynamic vs static link : [address1](https://www.youtube.com/watch?v=or1dAmUO8k0) [address2](https://www.youtube.com/watch?v=pLy69V2F_8M)

 C++ Linker：[here](https://www.youtube.com/watch?v=H4s55GgAg0I)

make Make & Work with Library ：[here](https://www.youtube.com/watch?v=Wt4dxDNmDA8)

## Project Configure

![image-20211009133552522](https://i.loli.net/2021/10/09/xNGmUPAnSr6fKhV.png)

![image-20211009134214094](https://i.loli.net/2021/10/09/fernlLdvmS6Iaw1.png)

Set the reference (right click sandbox and add reference)

![image-20211009135509608](https://i.loli.net/2021/10/09/BGFRXVJ8gIoPpni.png)

Close visual studio,open .sln with txt ,translate the follow code:

```shell
Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "CC3D", "CC3D\CC3D.vcxproj", "{6A94323D-E0FC-4887-90A6-B275C413AE06}"
EndProject
Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "SandBox", "SandBox\SandBox.vcxproj", "{59C8CFC0-9A85-43C9-ACE4-8D7F3E64A3B4}"
EndProject
```

to:

```shell
Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "SandBox", "SandBox\SandBox.vcxproj", "{59C8CFC0-9A85-43C9-ACE4-8D7F3E64A3B4}"
EndProject
Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "CC3D", "CC3D\CC3D.vcxproj", "{6A94323D-E0FC-4887-90A6-B275C413AE06}"
EndProject
```

Thus the SandBox(.exe) would be the first project to run if others clone your project from github or somewhere else.

## Build the dll

```c++
//test.h in CC3D:
namespace CC3D {
	_declspec(dllexport) void Print();
}
//test.cpp in CC3D:
#include"test.h"
#include<stdio.h>
namespace CC3D {
	void Print() {
		printf("HelloWorld!\n");
	}
}
```

![image-20211009141216743](https://i.loli.net/2021/10/09/TV6ZuiqbLgwOxe7.png)

## Link dll to exe

```cpp
//App.cpp in SandBox
#include<iostream>
namespace CC3D {
	_declspec(dllimport) void Print();
}
void main()
{
	CC3D::Print();
	system("pause");
}
```

![image-20211009142558926](https://i.loli.net/2021/10/09/QDF6ENJuZiPVWg5.png)

![image-20211009142425561](https://i.loli.net/2021/10/09/s61ai8oUuz59Y4k.png)

## Tips

### Add relative directory folder

use Marco **$(SolutionDir)**

![](https://i.loli.net/2021/10/08/DG5dYLAOXTKNfPg.png)

### Static Linking

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/43ef369549fd59bc0643af47de6bc2f7130615dd/HTC/Project/CC3D)

![image-20211008221144977](https://i.loli.net/2021/10/08/xECiXTfuAba6zBp.png)

![image-20211008221212060](https://i.loli.net/2021/10/08/pjyt9cxPh3l5fDb.png)

![image-20211008221323358](https://i.loli.net/2021/10/08/wcXeimHf4Dx8g1E.png)

### Dynamic Linking

code version :[here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/461d56a91c014d53425d9a779e4652fedf549e8e/HTC/Project/CC3D)

![image-20211008224942093](https://i.loli.net/2021/10/08/hN2GngBi6lYZ4IJ.png)

![image-20211008225026226](https://i.loli.net/2021/10/08/kqx2Cev96uGSEHs.png)

### Make & Work with Library

tutorial : [here](https://www.youtube.com/watch?v=Wt4dxDNmDA8)

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/a1179996c82a1ed9a2d206817a9f15f2076d468e/HTC/Project/CC3D)

![image-20211009001811096](https://i.loli.net/2021/10/09/9DcAgrZO5SGfh2V.png)

![image-20211009001944128](https://i.loli.net/2021/10/09/fZIgcFL42VvnwQu.png)

### Project OutPut Direction

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/d264d0ab682f16c152118fff939360f8b054ed07/HTC/Project/CC3D)

![image-20211009130408707](https://i.loli.net/2021/10/09/lA6G9HOW7PfedUD.png)

![image-20211009130440952](https://i.loli.net/2021/10/09/WBhC8V2IpZcY3Kr.png)

