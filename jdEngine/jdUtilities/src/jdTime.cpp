#include "jdTime.h"

namespace jdEngineSDK {

  const double Time::MICROSEC_TO_SEC = 1.0 / 1000000.0;

  Time::Time() {
    m_timer.reset(new Timer);
    m_appStartTime = m_timer->getStartMilliseconds();
    m_lastFrameTime = m_timer->getMicroseconds();
  }

  void 
  Time::update() {
    uint64 currentFrameTime = m_timer->getMicroseconds();
    m_frameDelta = static_cast<float>((currentFrameTime - m_lastFrameTime) * MICROSEC_TO_SEC);
    m_timeSinceStartMs = static_cast<uint64>(currentFrameTime / 1000);
    m_timeSinceStart = m_timeSinceStartMs / 1000.0f;
    m_lastFrameTime = currentFrameTime;
    m_currentFrame.fetch_add(1, std::memory_order_relaxed);
  }

  JD_UTILITY_EXPORT Time& g_time() {
    return Time::instance();
  }
}