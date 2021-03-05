/*****************************************************************************/
/**
 * @file    jdTimer.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    20/02/2021
 * @brief   Class for querying high precision timers.
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
  
  using std::chrono::high_resolution_clock;
  using std::chrono::time_point;
  using std::chrono::duration;
  using std::chrono::duration_cast;

  class JD_UTILITY_EXPORT Timer {
   public:
    /**
     * @brief default constructor and start timing.
     */
    Timer();

    /**
     * @brief default destructor
     */
    ~Timer() {};

    /**
     * @brief Reset the timer to zero.
     */
    void
    reset();

    /**
     * @brief Returns time in milliseconds since timer was initialized or last reset.
     */
    uint64
    getMilliseconds() const;

    /**
     * @brief Returns time in microseconds since timer was initialized or last reset.
     */
    uint64
    getMicroseconds() const;

    /**
     * @brief Returns time in seconds since timer was initialized or last reset.
     */
    float
    getSeconds() const;

    /**
     * @brief Returns the time at which the timer was initialized, in milliseconds.
     * @return  Time in milliseconds.
     */
    uint64
    getStartMilliseconds() const;

		 private:
    /**
     * @brief A high resolution clock.
     */
    high_resolution_clock m_highResClock;

    /**
     * @brief the time point of the clock     
     */
    time_point<high_resolution_clock> m_startTime;
		};

}