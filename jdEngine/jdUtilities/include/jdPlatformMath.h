#pragma once

#include "jdPlatformDefines.h"

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
      return 1.0 / std::sqrt(val);
    }

    template<typename T>
    static T Sqrt(T val) {
      return std::sqrt(val);
    }

    template<>
    static float Sqrt(float val) {
      return std::sqrtf(val);
    }

    template<typename T>
    static T Pow(T val, T pot = 2) {
      return std::pow(val, pot);
    }

    template<typename T>
    static T Cos(T val) {
      return std::cos(val);
    }

    template<>
    static float Cos(float val) {
      return std::cosf(val);
    }

    template<typename T>
    static T Sin(T val) {
      return std::sin(val);
    }

    template<>
    static float Sin(float val) {
      return std::sinf(val);
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

    static const float DEG2RAD;
    static const float RAD2DEG;

    static const float EPSILONF;

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
