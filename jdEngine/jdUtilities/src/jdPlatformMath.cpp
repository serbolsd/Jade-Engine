#include "jdPlatformMath.h"
#include <float.h>

namespace jdEngineSDK {

  /*!< PI constant */
  const float PlatformMath::PI = (4.0f * std::atan(1.0f));

  /*!< Inverse of PI */
  const float PlatformMath::INV_PI = (1.0f / (4.0f * std::atan(1.0f)));

  /*!< Half PI  */
  const float PlatformMath::HALF_PI = (0.5f * (4.0f * std::atan(1.0f)));

  /*!< 2 Times PI */
  const float PlatformMath::TWO_PI = (2.0f * (4.0f * std::atan(1.0f)));

  const float PlatformMath::DEG2RAD = static_cast<float>((4.0f * std::atan(1.0f)) / 180.0f);
  const float PlatformMath::RAD2DEG = static_cast<float>(180.0f / (4.0f * std::atan(1.0f)));
  const float PlatformMath::EPSILONF = FLT_EPSILON;

  const float PlatformMath::EULERS_NUMBER = (2.71828182845904523536f);
  const float PlatformMath::MAXFLOAT = 3.402823466e+38F;
  
  float
  PlatformMath::factorial(const int32& n) {
    float result = 1;
    for (int32 i = 2; i <= n; ++i)  {
      result *= i;
    }
    return result;
  }

  float
  PlatformMath::taylorSerie(const int32& n, const int32& x) {
    float t, s = 0;
    for (int32 i = 0; i < n; ++i) {
      t = pow(x, i) / factorial(i);
      s += t;
    }
    return s;
  }

  float 
  PlatformMath::eulerSerie(const int32& amount) {
    int32 increase = 1, fact_num;
    float operate_serie = 0.0f;
    float sum_serie = 0.0f; 
    float increase_serie = 0.0f;

    for (int32 i = 1; i <= amount; ++i) {
      fact_num = increase * i;
      increase = fact_num;
      operate_serie = (1.0f / fact_num) + increase_serie;
      increase_serie = operate_serie;
      sum_serie += increase_serie;
    }

     return operate_serie;
  }

  float PlatformMath::taylorSin(const float& x) {
    static uint32 fac3 = 6;
    static uint32 fac5 = 120;
    static uint32 fac7 = 5040;
    static uint32 fac9 = 362880;
    static uint32 fac11 = 39916800;

    float sum = x;
    sum -= pow(x, 3) / fac3;
    sum += pow(x, 5) / fac5;
    sum -= pow(x, 7) / fac7;
    sum += pow(x, 9) / fac9;
    sum -= pow(x, 11) / fac11;
    return sum;
  }
  
  float 
  PlatformMath::taylorCos(const float& x) {
    float sum = 1;
    static uint32 fac2 = 2;
    static uint32 fac4 = 24;
    static uint32 fac6 = 720;
    static uint32 fac8 = 40320;
    static uint32 fac10 = 3628800;
    static uint32 fac12 = 479001600;

    sum -= pow(x, 2) / fac2;
    sum += pow(x, 4) / fac4;
    sum -= pow(x, 6) / fac6;
    sum += pow(x, 8) / fac8;
    sum -= pow(x, 10) / fac10;
    sum -= pow(x, 12) / fac12;
    return sum;
  }

  float 
  PlatformMath::taylorTan(const float& x) {

    static uint32 nominator[7] = { 1,
                                    2, 
                                    17, 
                                    62, 
                                    1382, 
                                    21844, 
                                    929569  };

    static uint32 denominator[7] = { 3,
                                      15,
                                      315,
                                      2835,
                                      155925,
                                      6081075,
                                      638512875 };
    float sum = x;
    sum += pow(x,3)* nominator[0] / denominator[0];
    sum += pow(x,5)* nominator[1] / denominator[1];
    sum += pow(x,7)* nominator[2] / denominator[2];
    sum += pow(x,9)* nominator[3] / denominator[3];
    sum += pow(x,11)* nominator[4] / denominator[4];
    sum += pow(x,13)* nominator[5] / denominator[5];
    sum += pow(x,15)* nominator[6] / denominator[6];

    return sum;
  }
}

