# Lec29 Ref-Scope-Smart Pointer

课程地址 : [here](https://www.youtube.com/watch?v=HkGZ378nArE&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=41)

代码版本 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/9c8b3c848c0762b91006a6d87780788334f3eba7/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

先修课 : [smart pointer](https://www.youtube.com/watch?v=UOB7-B2MfwA&list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb&index=44)

这次教程，Cherno 讨论了自己对智能指针的看法，如果希望深入了解智能指针可以查看相关课程

## Core 作用域

```c++
//core.h
#include<memory>
namespace CC3D {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

}
```

