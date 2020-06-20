#pragma once

#include "jdPlatformDefines.h"
#include "jdPlatformTypes.h"

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
    static T sin(T val) {
      return std::sin(val);
    }

    template<>
    static float sin(float val) {
      return std::sinf(val);
    }

    template<typename T>
    static T tan(T val) {
      return std::tan(val);
    }

    template<>
    static float tan(float val) {
      return std::tanf(val);
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


    static const float DEG2RAD;
    static const float RAD2DEG;

    static const float EPSILONF;

    static const float EULERS_NUMBER;
     
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
