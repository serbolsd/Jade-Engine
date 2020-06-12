#include "jdRectangle.h"

namespace jdEngineSDK {
  Rectangle::Rectangle(const Rectangle& rectangle) : m_scale(rectangle.m_scale), 
                                                     m_rotation(rectangle.m_rotation),
                                                     m_position(rectangle.m_position),
                                                     m_base(rectangle.m_base),
                                                     m_height(rectangle.m_height) {}
  
  Rectangle& 
  Rectangle::operator=(const Rectangle& rectangle) {
    m_scale = rectangle.m_scale;
    m_rotation = rectangle.m_rotation;
    m_position = rectangle.m_position;
    m_base = rectangle.m_base;
    m_height = rectangle.m_height;
    return *this;
  }

  bool 
  Rectangle::operator==(const Rectangle& rectangle) const {
    if (Math::abs(getArea()-rectangle.getArea()) < FLT_EPSILON)
    {
      return true;
    }
    return false;
  }
}
