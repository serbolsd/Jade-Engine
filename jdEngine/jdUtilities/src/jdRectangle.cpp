#include "jdRectangle.h"

namespace jdEngineSDK {
  Rectangle::Rectangle(const Rectangle& rectangle) 
    : m_scale(rectangle.m_scale), 
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
    if (Math::abs(getArea()-rectangle.getArea()) <= Math::EPSILONF) {
      if (Math::abs(getHeight() - rectangle.getHeight()) < Math::EPSILONF) {
        return true;
      }
    }
    return false;
  }

  bool 
  Rectangle::operator!=(const Rectangle& rectangle) const {
    if (Math::abs(getArea() - rectangle.getArea()) > Math::EPSILONF) {
      if (Math::abs(getHeight() - rectangle.getHeight()) > Math::EPSILONF) {
        return true;
      }
    }
    return false;
  }

  bool 
  Rectangle::operator<(const Rectangle& rectangle) const {
    if (getArea() < rectangle.getArea()) {
      if (getHeight() < rectangle.getHeight() || 
          getBase() < rectangle.getBase()) {
        return true;
      }
    }
    return false;
  }
  
  bool 
  Rectangle::operator<=(const Rectangle& rectangle) const {
    if (Math::abs(getArea() - rectangle.getArea()) < Math::EPSILONF &&
      getArea() < rectangle.getArea()) {
      if (Math::abs(getHeight() - rectangle.getHeight()) < Math::EPSILONF ||
          Math::abs(getBase() - rectangle.getBase()) < Math::EPSILONF ||
          getHeight() < rectangle.getHeight() ||
          getBase() < rectangle.getBase()) {
        return true;
      }
    }
    return false;
  }
  
  bool 
  Rectangle::operator>(const Rectangle& rectangle) const {
    if (getArea() > rectangle.getArea()) {
      if (getHeight() > rectangle.getHeight() ||
          getBase() > rectangle.getBase()) {
        return true;
      }
    }
    return false;
  }
  
  bool Rectangle::operator>=(const Rectangle& rectangle) const {
    if (Math::abs(getArea() - rectangle.getArea()) < Math::EPSILONF &&
        getArea() > rectangle.getArea()) {
      if (Math::abs(getHeight() - rectangle.getHeight()) < Math::EPSILONF ||
          Math::abs(getBase() - rectangle.getBase()) < Math::EPSILONF ||
          getHeight() > rectangle.getHeight() ||
          getBase() > rectangle.getBase()) {
        return true;
      }
    }
    return false;
  }
}
