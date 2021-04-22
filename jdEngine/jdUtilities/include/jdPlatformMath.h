#pragma once

#include "jdPlatformDefines.h"
#include "jdPlatformTypes.h"
#include "jdStdHeaders.h"


#include <cmath>

namespace jdEngineSDK {

  struct JD_UTILITY_EXPORT PlatformMath {
    template<typename T>
    static T abs(T val) {
      return ::abs(val);// :: abs std::abs
    }

    template<>                                //especializacion del template anterior con float
    static float abs(float val) {             // abs(1.0) llama a la de arriba abs(1.0f) llama a la especializada
      return ::fabsf(val);// :: abs std::abs
    }

    template<typename T>
    static T invSqrt(T val) {
      return 1.0f / std::sqrt(val);
    }

    template<>
    static float invSqrt(float val) {
      return 1.0f / std::sqrt(val);
    }

    template<typename T>
    static T sqrt(T val) {
      return std::sqrt(val);
    }

    template<>
    static float sqrt(float val) {
      return std::sqrtf(val);
    }

    template<typename T>
    static T pow(T val, uint32 pot) {
      return static_cast<T>(std::pow(val, pot));
    }

    template<typename T>
    static T cos(T val) {
      return std::cos(val);
    }

    template<>
    static float cos(float val) {
      return std::cosf(val);
    }

    template<typename T>
    static T acos(T val) {
      return std::acos(val);
    }

    template<>
    static float acos(float val) {
      return std::acosf(val);
    }

    template<typename T>
    static T sin(T val) {
      return std::sin(val);
    }

    template<>
    static float sin(float val) {
      return std::sinf(val);
    }

    template<typename T>
    static T asin(T val) {
      return std::asin(val);
    }

    template<>
    static float asin(float val) {
      return std::asinf(val);
    }

    template<typename T>
    static T tan(T val) {
      return std::tan(val);
    }

    template<typename T>
    static T atan(T val) {
      return std::atan(val);
    }

    template<typename T>
    static T atan(T val,T val2) {
      return std::atan2(val,val2);
    }

    template<>
    static float tan(float val) {
      return std::tanf(val);
    }

    template<>
    static float atan(float val) {
      return std::atanf(val);
    }

    template<>
    static float atan(float val, float val2) {
      return std::atan2f(val, val2);
    }

    static float Min(float maxValue, float value) {
      return std::min(maxValue, value);
    }

    static const float PI;
    static const float TWO_PI;
    static const float INV_PI;
    static const float HALF_PI;

    /**
     * @brief Utility to ensure angle is between +/- 180 degrees by unwinding.
     */
    static float
    unwindDegrees(float A) {
      while (180.f < A) {
        A -= 360.f;
      }

      while (-180.f > A) {
        A += 360.f;
      }

      return A;
    }

    /**
     * @brief Given a heading which may be outside the +/- PI range, 'unwind'
     *        it back into that range.
     */
    static float
    unwindRadians(float A) {
      while (PI < A) {
        A -= TWO_PI;
      }

      while (-PI > A) {
        A += TWO_PI;
      }

      return A;
    }

    /**
     * @brief Given a factorial of number N
     * @param n is the number to calculate it´s factorial
     */
    static float
    factorial(const int32& n);

    /**
     * @brief Given summation of taylor serie of n function with x value
     * @param n is the number of functions of the serie
     * @param x is the number value
     * @return summation of taylor serie
     */
    static float
    taylorSerie(const int32& n, const int32& x);

    /**
     * @brief Given number of euler serie 
     * @param amount is the number of numbers in the series
     * @return euler number
     */
    static float
    eulerSerie(const int32& amount);

    /**
     * @brief Function to get the Sin more fast used taylor series
     * @param x is the values in redians
     * @return Aproximation of Sin
     */
    static float
    taylorSin(const float& x);

    /**
     * @brief Function to get the Cos more fast used taylor series
     * @param x is the values in redians
     * @return Aproximation of Cos
     */
    static float
    taylorCos(const float& x);

    /**
     * @brief Function to get the Tan more fast used taylor series
     * @param x is the values in redians
     * @return Aproximation of Tan
     */
    static float
    taylorTan(const float& x);

    /**
     * @brief Function to get the Tan more fast used taylor series
     * @param number to get the inverse
     * @return the inverse
     */
    static float
    Q_rsqrt(float number) {
      long i;
      float x2, y;
      const float threehalfs = 1.5f;

      x2 = number * 0.5f;
      y = number;
      i = *(long*)&y;                       //evil floatin point bit level hacking
      i = 0x5f3759df - (i >> 1);            //what the fuck?
      y = *(float*)&i;
      y = y * (threehalfs - (x2 * y * y));  //1st iteration

      return y;
    }

    static const float DEG2RAD;
    static const float RAD2DEG;

    static const float EPSILONF;

    static const float EULERS_NUMBER;

    static const float MAXFLOAT;
     
  };
  //jdWindowsMath.h osea va en otro header, solo se activa cuando el systema sea seleccionado
  //specifict math for window

  //struct WindowsMath : public PlatformMath {
  //  
  //  template<>
  //  static float invSqrt(float val) {
  //    return carmackReversse(val);
  //  }
  //};

  //jdMath.h Toda la matematica que puedes usar en el sistema
  //All usable Math
  //Se pueden haer .cpp, el generico para todos los sistams y si es necesesario uno especifico para el sistema
}
