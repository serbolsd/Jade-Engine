/*****************************************************************************/
/**
 * @file    jdRandom.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    22/01/2021
 * @brief   A basic class for random
 *
 *
 *
 * @bug	    No known bugs.
 */
/*****************************************************************************/
#pragma once

#include "jdPrerequisitesUtil.h"

#include <random>


#include "math.h"

namespace jdEngineSDK {
  class Random
  {
  public:
    Random() {};
    ~Random() {};
    static void
    RandomBoxMuller(float& result_a, float& result_b, float dMean, float dStdDeviation) {
      float x1 = 0, x2 = 0, w = 0, y1 = 0, y2 = 0;

      while (w >= 1.0f) {
        x1 = 2.0f * random_unit_float() - 1.0f;
        x2 = 2.0f * random_unit_float() - 1.0f;
        w = x1 * x1 + x2 * x2;
      }

      w = Math::sqrt((-2.0f * log(w)) / w);
      y1 = x1 * w;
      y2 = x2 * w;
      result_a = (dMean + y1 * dStdDeviation);
      result_b = (dMean + y2 * dStdDeviation);
      //return(m + y1 * s);
    }
		 private:
     static float
     random_unit_float() {
       srand((unsigned int)time(NULL));
       return (float(rand()) / float((RAND_MAX)));
     }
		};
}