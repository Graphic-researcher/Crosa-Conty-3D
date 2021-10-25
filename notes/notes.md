# C++

## visual studio error

LNK + number: link error

C + number: compile error

.exe file has to have entry point(like main() func)

linking stage is to find the function have been used

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

