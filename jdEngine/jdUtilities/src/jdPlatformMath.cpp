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

  const float PlatformMath::EULERS_NUMBER = (2.71828182845904523536f);
  
  float
  PlatformMath::factorial(const int32& n) {
    float result = 1;
    for (int32 i = 2; i <= n; i++)  {
      result *= i;
    }
    return result;
  }

  float
  PlatformMath::taylorSerie(const int32& n, const int32& x) {
    float t, s = 0;
    for (int32 i = 0; i < n; i++) {
      t = Pow(x, i) / factorial(i);
      s += t;
    }
    return s;
  }

  float 
  PlatformMath::eulerSerie(const int32& amount) {
    int32 i, increase, fact_num;
    float operate_serie, sum_serie, increase_serie;

    increase = 1;
    increase_serie = 0.0;

    for (i = 1; i <= amount; i++) {
      fact_num = increase * i;
      increase = fact_num;
      operate_serie = (1.0 / fact_num) + increase_serie;
      increase_serie = operate_serie;
    }

     return operate_serie;
  }
}

