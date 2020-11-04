#pragma once
#include "jdPlatformMath.h"

#include <Windows.h>

namespace jdEngineSDK{

  struct WindowsMath : PlatformMath {

    /**
     * @brief Function to get the inverse sart in Windows
     * @param number to get the inverse
     * @return the inverse
     */
    static float
    WinInvSqrt(float number) {
      return 1.0f / sqrt(number);
    }
  };
}
