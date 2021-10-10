# Lec7 Precompile Header

tutorial : [here](https://www.youtube.com/watch?v=UQ718BrbQ5E&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=11)

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/c9a4e3fd1fdbaca25b420f68c04f6206f3875cec/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

prerequisite : [Precompiled Headers in C++](https://www.youtube.com/watch?v=eSI4wctZUto)

## Core purpose of precompile header 

grab a bunch of header file and convert them a kind of compiled format that the compiler can use instead of read those header file over and over again (It's wasting time ， imagine you have hundreds of files)

## Precompile header file

add these file and you can delete the header files [showed below] in other files

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

