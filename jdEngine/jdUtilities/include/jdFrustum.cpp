#include "jdFrustum.h"

namespace jdEngineSDK
{
  Frustum::Frustum(const JDMatrix4& viewProjMat) : m_left(Plane(0,0,0,0)), 
                                                   m_right(Plane(0, 0, 0, 0)), 
                                                   m_bottom(Plane(0, 0, 0, 0)), 
                                                   m_top(Plane(0, 0, 0, 0)), 
                                                   m_back(Plane(0, 0, 0, 0)), 
                                                   m_front(Plane(0, 0, 0, 0)) {
    // Left clipping plane
    m_left.m_xyz.x = viewProjMat.m_30 + viewProjMat.m_00;
    m_left.m_xyz.x = viewProjMat.m_31 + viewProjMat.m_01;
    m_left.m_xyz.x = viewProjMat.m_32 + viewProjMat.m_02;
    m_left.w = viewProjMat.m_33 + viewProjMat.m_03;
    // Right clipping plane
    m_right.m_xyz.x = viewProjMat.m_30 - viewProjMat.m_00;
    m_right.m_xyz.x = viewProjMat.m_31 - viewProjMat.m_01;
    m_right.m_xyz.x = viewProjMat.m_32 - viewProjMat.m_02;
    m_right.w = viewProjMat.m_33 - viewProjMat.m_03;
    // Top clipping plane
    m_top.m_xyz.x = viewProjMat.m_30 - viewProjMat.m_10;
    m_top.m_xyz.x = viewProjMat.m_31 - viewProjMat.m_11;
    m_top.m_xyz.x = viewProjMat.m_32 - viewProjMat.m_12;
    m_top.w = viewProjMat.m_33 - viewProjMat.m_13;
    // Bottom clipping plane
    m_bottom.m_xyz.x = viewProjMat.m_30 + viewProjMat.m_10;
    m_bottom.m_xyz.x = viewProjMat.m_31 + viewProjMat.m_11;
    m_bottom.m_xyz.x = viewProjMat.m_32 + viewProjMat.m_12;
    m_bottom.w = viewProjMat.m_33 + viewProjMat.m_13;
    // Back clipping plane
    m_back.m_xyz.x = viewProjMat.m_30 + viewProjMat.m_20;
    m_back.m_xyz.x = viewProjMat.m_31 + viewProjMat.m_21;
    m_back.m_xyz.x = viewProjMat.m_32 + viewProjMat.m_22;
    m_back.w = viewProjMat.m_33 + viewProjMat.m_23;
    // Front clipping plane
    m_front.m_xyz.x = viewProjMat.m_30 - viewProjMat.m_20;
    m_front.m_xyz.x = viewProjMat.m_31 - viewProjMat.m_21;
    m_front.m_xyz.x = viewProjMat.m_32 - viewProjMat.m_22;
    m_front.w = viewProjMat.m_33 - viewProjMat.m_23; 
  }

  Frustum::Frustum(const Frustum& other) : m_left(other.m_left),
                                                 m_right(other.m_right),
                                                 m_bottom(other.m_bottom),
                                                 m_top(other.m_top),
                                                 m_back(other.m_back),
                                                 m_front(other.m_front) {
  }

  void 
  Frustum::setPlanes(const JDMatrix4& viewProjMat) {
    // Left clipping plane
    m_left.m_xyz.x = viewProjMat.m_30 + viewProjMat.m_00;
    m_left.m_xyz.x = viewProjMat.m_31 + viewProjMat.m_01;
    m_left.m_xyz.x = viewProjMat.m_32 + viewProjMat.m_02;
    m_left.w = viewProjMat.m_33 + viewProjMat.m_03;
    // Right clipping plane
    m_right.m_xyz.x = viewProjMat.m_30 - viewProjMat.m_00;
    m_right.m_xyz.x = viewProjMat.m_31 - viewProjMat.m_01;
    m_right.m_xyz.x = viewProjMat.m_32 - viewProjMat.m_02;
    m_right.w = viewProjMat.m_33 - viewProjMat.m_03;
    // Top clipping plane
    m_top.m_xyz.x = viewProjMat.m_30 - viewProjMat.m_10;
    m_top.m_xyz.x = viewProjMat.m_31 - viewProjMat.m_11;
    m_top.m_xyz.x = viewProjMat.m_32 - viewProjMat.m_12;
    m_top.w = viewProjMat.m_33 - viewProjMat.m_13;
    // Bottom clipping plane
    m_bottom.m_xyz.x = viewProjMat.m_30 + viewProjMat.m_10;
    m_bottom.m_xyz.x = viewProjMat.m_31 + viewProjMat.m_11;
    m_bottom.m_xyz.x = viewProjMat.m_32 + viewProjMat.m_12;
    m_bottom.w = viewProjMat.m_33 + viewProjMat.m_13;
    // Back clipping plane
    m_back.m_xyz.x = viewProjMat.m_30 + viewProjMat.m_20;
    m_back.m_xyz.x = viewProjMat.m_31 + viewProjMat.m_21;
    m_back.m_xyz.x = viewProjMat.m_32 + viewProjMat.m_22;
    m_back.w = viewProjMat.m_33 + viewProjMat.m_23;
    // Front clipping plane
    m_front.m_xyz.x = viewProjMat.m_30 - viewProjMat.m_20;
    m_front.m_xyz.x = viewProjMat.m_31 - viewProjMat.m_21;
    m_front.m_xyz.x = viewProjMat.m_32 - viewProjMat.m_22;
    m_front.w = viewProjMat.m_33 - viewProjMat.m_23;
  }
}
