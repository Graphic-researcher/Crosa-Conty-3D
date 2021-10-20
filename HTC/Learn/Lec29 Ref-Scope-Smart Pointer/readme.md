# Lec29 Ref-Scope-Smart Pointer

tutorial : [here](https://www.youtube.com/watch?v=HkGZ378nArE&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=41)

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/9c8b3c848c0762b91006a6d87780788334f3eba7/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

prerequisite : [smart pointer](https://www.youtube.com/watch?v=UOB7-B2MfwA&list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb&index=44)

In This vedio,Cherno discuss sth about smart pointers(unique pointer,shared pointer).Check out the related tutorials if you want to go deeply about that.

## Core Scope

Code will be more readable.

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

