# Lec7 Precompile Header

教程地址 : [here](https://www.youtube.com/watch?v=UQ718BrbQ5E&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=11)

代码版本 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/47c30b6d3536a4f128ad70958fbbde41f3ca3076/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

先修课程 : [Precompiled Headers in C++](https://www.youtube.com/watch?v=eSI4wctZUto)

## 为什么需要预编译头文件

预编译头文件可以先将一些头文件转换格式，这样就不需要在每个文件里遍历头文件了（如果有成千上百个文件，这将极大节省编译时间）

## 预编译头文件

编写好预编译头文件后就可以删除其他文件里的相关头文件了。

要把预编译头文件包含到每个cpp文件的第一行，例如：

```c++
#include "cc3d_pch.h"
#include "Application.h"



/////// Event System //////
#include "CC3D/Events/ApplicationEvent.h"
#include "CC3D/Events/KeyEvent.h"
#include "CC3D/Log.h"
///////////////////////////
```

否则会报错：

![image-20211010204608974](https://i.loli.net/2021/10/10/8KBcHrDRTMPlQgL.png)

error link : [here](https://msdn.microsoft.com/query/dev16.query?appId=Dev16IDEF1&l=ZH-CN&k=k(C1010)&rd=true)

cc3d_pch.h in CC3D

```c++
#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#ifdef  CC3D_PLATFORM_WINDOWS
	#include <Windows.h>
#endif //  CC3D_PLATFORM_WINDOWS
```

![image-20211010204122847](https://i.loli.net/2021/10/10/ekwVHImK617frc9.png)

![image-20211010204203246](https://i.loli.net/2021/10/10/EBYd52qegmn1Pw9.png)