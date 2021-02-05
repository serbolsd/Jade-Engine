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

#include "math.h"

namespace jdEngineSDK {

  struct Time {
    static const std::string CurrentDateTime() {
      time_t     now = time(NULL);
      struct tm  tstruct;
      char       buf[80];
      localtime_s(&tstruct, &now);
      strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
      return buf;
    }
  };
}