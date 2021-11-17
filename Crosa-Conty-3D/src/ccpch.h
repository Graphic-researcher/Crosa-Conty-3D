#pragma once

#include "CC3D/Core/PlatformDetection.h"

#ifdef CC3D_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
		#define NOMINMAX
	#endif
#endif

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "CC3D/Core/Base.h"
#include "CC3D/Core/Log.h"

#include "CC3D/Debug/Instrumentor.h"

#ifdef CC3D_PLATFORM_WINDOWS
	#include <Windows.h>
#endif