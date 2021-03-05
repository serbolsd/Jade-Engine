/*****************************************************************************/
/**
 * @file    jdTime.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    22/01/2021
 * @brief   A basic class for Time
 *
 *
 *
 * @bug	    No known bugs.
 */
 /*****************************************************************************/
#pragma once

#include "jdPrerequisitesUtil.h"
#include "jdModulo.h"
#include "jdTimer.h"
#include "math.h"

namespace jdEngineSDK {
  using std::atomic;

  class JD_UTILITY_EXPORT Time : public Module<Time> {
   public:
    /**
     * @brief default constructor
     */
    Time();

    /**
     * @brief default constructor
     */
    ~Time() {};

    /**
     * @brief Get the time since application start. Only gets updated once per frame.
     * @return  The time since application start, in seconds.
     */
    float
    getTimeSeconds() const {
      return m_timeSinceStart;
    }

    /**
     * @brief Get the time since application start. Only gets updated once per frame.
     * @return  The time since application start, in milliseconds.
     */
    uint64 
    getTimeMilliseconds() const {
      return m_timeSinceStartMs;
    }

    /**
     * @brief Gets the time since last frame. Only gets updated once per frame.
     * @return  Time since last frame was executed, in seconds.
     */
    float
    getFrameDelta() const {
      return m_frameDelta;
    }

    /**
     * @brief Returns the sequential index of the current frame. First frame is 0.
     * @return  The current frame.
     * @note  Thread safe, but only counts sim thread frames.
     */
    uint64
    getFrameId() const {
      return m_currentFrame.load();
    }

    /**
     * @brief Called every frame. Should only be called by Application.
     */
    void
    update();

    static const std::string CurrentDateTime() {
      time_t     now = time(NULL);
      struct tm  tstruct;
      char       buf[80];
      localtime_s(&tstruct, &now);
      strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
      return buf;
    }

    static const double MICROSEC_TO_SEC;

   private:
    /**
     * @brief Frame delta in seconds
     */
    float m_frameDelta = 0.0f;
    
    /**
     * @brief Time since start in seconds
     */
    float m_timeSinceStart = 0.0f;

    /**
     * @brief Time since start in milliseconds
     */
    uint64 m_timeSinceStartMs = 0;

    /**
     * @brief Time the application started, in microseconds
     */
    uint64 m_appStartTime = 0;
    
    /**
     * @brief Time since last runOneFrame call, In microseconds
     */
    uint64 m_lastFrameTime = 0;

    /**
     * @brief Counter of frames
     */
    atomic<uint32> m_currentFrame{ 0UL };

    /**
     * @brief a timer
     */
    SPtr<Timer> m_timer;
  };

  /**
   * @brief Easier way to access the Time module.
   */
  JD_UTILITY_EXPORT Time& 
  g_time();
}