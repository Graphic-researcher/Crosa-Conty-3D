# C++

## visual studio error

LNK + number: link error

C + number: compile error

.exe file has to have entry point(like main() func)

linking stage is to find the function have been used

## Difference between const and constexpr

**const** doesn't distinguish between Compilation-Period constants and run-time constants

**constexpr** is limited to compilation-period constants

```c++
template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
	/*constexpr std::unique_ptr<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}*/
```



## #define usage 

''##'' mains link former and later

''#'' mains variable 'type' change to **string**

```c++
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType(){return EventType::##type;}\ 
								virtual EventType GetEventType() const override {return GetStaticType();}\
								virtual const char* GetName() const override{return #type;}
```



## unclear git commit 

  670e86fe：Change bind target inside OpenGLIndexBuffer constructor

 e8b2ad93：Made Input a true singleton

60cf553b：Basic ref-counting system to terminate glfw
