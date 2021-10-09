# Lec4 Logging

tutorial : [here](https://www.youtube.com/watch?v=meARMOmTLgE&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=5)

code version:

Vendor：[spdlog](https://github.com/gabime/spdlog)

## Add Vendor Directory to CC3D and SandBox

![image-20211009213241273](https://i.loli.net/2021/10/09/ylqN7AwQxcDftUM.png)

## Custom 3rd party api to our api

create log.h  log.cpp in CC3D

```c++
//log.h in CC3D
#pragma once
#include<memory>
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
namespace CC3D {
	class CC3D_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

//Core log macros
#define CC3D_CORE_TRACE(...) ::CC3D::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CC3D_CORE_INFO(...)  ::CC3D::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CC3D_CORE_WARN(...)  ::CC3D::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CC3D_CORE_ERROR(...) ::CC3D::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CC3D_CORE_FATAL(...) ::CC3D::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client log macros
#define CC3D_TRACE(...) ::CC3D::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CC3D_INFO(...)  ::CC3D::Log::GetClientLogger()->info(__VA_ARGS__)
#define CC3D_WARN(...)  ::CC3D::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CC3D_ERROR(...) ::CC3D::Log::GetClientLogger()->error(__VA_ARGS__)
#define CC3D_FATAL(...) ::CC3D::Log::GetClientLogger()->fatal(__VA_ARGS__)
```

```c++
//log.h in CC3D
#include "Log.h"
namespace CC3D {
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		//[Time Stamp] [name of logger] [actual message]
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("HAZEL");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stderr_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}
```

add log code in EntryPoint.h and don't forget to add the header file in CC3D.h otherwise you won't build correctly

```c++
//CC3D.h in CC3D
#pragma once
// For use by CC3D Application
#include "CC3D/Application.h"
#include "CC3D/Log.h"
//-------Entry Point--------------
#include "CC3D/EntryPoint.h"
//--------------------------------
```

```c++
//EntryPoint.h in CC3D
#pragma once

#ifdef  CC3D_PLATFORM_WINDOWS

extern CC3D::Application* CC3D::CreateApplication();

int main(int argc,char** argv)
{
	//Log message
	
	CC3D::Log::Init();
	CC3D_CORE_WARN("Initializing...\n");
	int a = 5;
	CC3D_INFO("hello! Var={0}",a);

	//...other code
}
#else
	#error CC3D only supports Windows!
#endif //  CC3D_PLATFORM_WINDOWS

```

![image-20211009230136422](https://i.loli.net/2021/10/09/gao3MkPAe6bSLlU.png)