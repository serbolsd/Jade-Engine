#include "jdPlatformMath.h"
#include <float.h>

namespace jdEngineSDK {

  /*!< PI constant */
  const float PlatformMath::PI = static_cast<float>(4.0f * std::atan(1.0f));

  /*!< Inverse of PI */
  const float PlatformMath::INV_PI = static_cast<float>(1.0f / PI);

  /*!< Half PI  */
  const float PlatformMath::HALF_PI = static_cast<float>(0.5f * PI);

  /*!< 2 Times PI */
  const float PlatformMath::TWO_PI = static_cast<float>(2.0f * PI);

  const float PlatformMath::DEG2RAD = static_cast<float>(PI / 180.0f);
  const float PlatformMath::RAD2DEG = static_cast<float>(180.0f / PI);
  const float PlatformMath::EPSILONF = FLT_EPSILON;
}

