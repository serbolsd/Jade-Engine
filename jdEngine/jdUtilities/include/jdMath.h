#pragma once
#include "jdPlatformMath.h"

#if  JD_PLATFORM == JD_PLATFORM_WIN32
# include "jdWindowsMath.h"
#elif JD_PLATFORM == JD_PLATFORM_LINUX //LINUX
//# include "jdLinuxMath.h"
//  using Math = LinuxMath;
#else
//  using Math = PlatformMath;
#endif //  JD_PLATFORM ==JD_PLATFORM_WIN32

#if  JD_PLATFORM == JD_PLATFORM_WIN32
using Math = jdEngineSDK::WindowsMath;
#elif JD_PLATFORM == JD_PLATFORM_LINUX //LINUX
//# include "jdLinuxMath.h"
//  using Math = LinuxMath;
#else
//  using Math = PlatformMath;
#endif //  JD_PLATFORM ==JD_PLATFORM_WIN32
