#include "jdBox.h"

namespace jdEngineSDK {

  Box::Box(const float& base, const float& height, const float& _long)
    : m_width (base), 
      m_height(height), 
      m_position(0.0f),	
      m_scale(1.0f), 
      m_rotation(0.0f), 
      m_length(_long) {}
  
  Box::Box(const Box& rectangle) 
    : m_width (rectangle.m_width ), 
      m_height(rectangle.m_height), 
      m_position(rectangle.m_position), 
      m_scale(rectangle.m_scale), 
      m_rotation(rectangle.m_rotation),	
      m_length(rectangle.m_length) {}
  
  Box& 
  Box::operator=(const Box& box)	{
    m_width  = box.m_width ;
    m_height = box.m_height;
    m_length = box.m_length;
    m_position = m_position;
    m_rotation = box.m_rotation;
    m_scale = box.m_scale;
    return *this;
  }
  
  bool 
  Box::operator==(const Box& box) const	{
    if (Math::abs(getHeight() - box.getHeight()) < Math::EPSILONF &&
        Math::abs(getWidth() - box.getWidth()) < Math::EPSILONF &&
        Math::abs(getLong() - box.getLong()) < Math::EPSILONF) {
      return true;
    }
    return false;
  }
  
  bool 
  Box::operator!=(const Box& box) const	{
    if (Math::abs(getHeight() - box.getHeight()) > Math::EPSILONF ||
        Math::abs(getWidth() - box.getWidth()) > Math::EPSILONF ||
        Math::abs(getLong() - box.getLong()) > Math::EPSILONF) {
      return true;
    }
    return false;
  }
}
