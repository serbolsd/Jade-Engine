#include "jdCamera.h"
#include "jdComponentTransform.h"
#include "jdGameObject.h"

namespace jdEngineSDK {

  Camera::Camera(String name, 
                 uint32 id, 
                 const JDVector3& eye,
                 const JDVector3& up,
                 const JDVector3& at,
                 const float& _near,
                 const float& _far,
                 const Radian& fovAngle,
                 const float& aspcRatio,
                 CAMERA_PROJECTION_TYPE::E projType) {
    m_type = COMPONENT_TYPE::CAMERA;
    m_name = move(name);
    m_id = id;
    m_near = _near;
    m_far = _far;
    m_fovAngle = fovAngle;
    m_aspecRatio = aspcRatio;
    m_view = createViewMatrix(eye, at, up);
    m_view.transpose();
    m_projType = projType;
    switch (projType) 
    {
    case jdEngineSDK::CAMERA_PROJECTION_TYPE::PERSPECTIVE:
      m_projection = createProjectionPerspectiveMatrix(fovAngle.valueRadians(), aspcRatio, _near, _far);
      //m_projection.transpose();
      break;
    case jdEngineSDK::CAMERA_PROJECTION_TYPE::HORTOGRAPHIC:
      break;
    default:
      break;
    }
    m_front = { m_view.m_20, m_view.m_21, m_view.m_22 };
    m_up = { m_view.m_10, m_view.m_11, m_view.m_12};
    m_right = { m_view.m_00, m_view.m_01, m_view.m_02};
    m_position = { eye, 0.0f };
  }

  void 
  Camera::Update(const float& deltatime) {
    if (nullptr == m_myObject) {
      return;
    }
    m_myObject->getComponent(COMPONENT_TYPE::TRANSFORM);
    auto component = m_myObject->getComponent(COMPONENT_TYPE::TRANSFORM);
    if (nullptr == component) {

    }
    CTransform* trans = reinterpret_cast<CTransform*>(component.get());

    if (m_front != trans->forward || 
        m_up != trans->up || 
        m_right != trans->right || 
        m_position != trans->position) {
      m_position = trans->position;
      m_view = createViewMatrix(m_position, m_position + trans->forward, trans->up);
      m_view.transpose();
      //m_view.translate(-tras);
      //m_position = { -m_view.m_03, -m_view.m_13, -m_view.m_23, 0.0f };
      m_front = { m_view.m_20, m_view.m_21, m_view.m_22 };
      m_up = { m_view.m_10, m_view.m_11, m_view.m_12 };
      m_right = { m_view.m_00, m_view.m_01, m_view.m_02 };
    }
  }

  void 
  Camera::traslate(const float& x, const float& y, const float& z) {
    //m_view *= matrix4Translate(x,y,z);
    JDVector3 tras;
    if (x!=0) {
      if (x>0) {
        tras = m_right*x;
      }
      else {
        tras = m_right*x;
      }
    }
    if (y != 0) {
      if (y > 0) {
        tras = m_up*y;
      }
      else {
        tras = m_up*y;
      }
    }
    if (z != 0) {
      if (z > 0) {
        tras = m_front*z;
      }
      else {
        tras = m_front*z;
      }
    }
    m_position += tras;
    m_view = createViewMatrix(m_position,m_position+m_front,m_up);
    m_view.transpose();
    //m_view.translate(-tras);
    //m_position = { -m_view.m_03, -m_view.m_13, -m_view.m_23, 0.0f };
    m_front = { m_view.m_20, m_view.m_21, m_view.m_22 };
    m_up = { m_view.m_10, m_view.m_11, m_view.m_12 };
    m_right = { m_view.m_00, m_view.m_01, m_view.m_02 };
  }

  void 
  Camera::rotate(const Radian& anglexRad = 0, const Radian& angleyRad = 0)  {
    if (0 != anglexRad.valueRadians())
    {
      m_view.rotateXstayPos(anglexRad);
    }
    if (0 != angleyRad.valueRadians())
    {
      m_view.rotateYstayPos(angleyRad);
    }
    m_front = { m_view.m_20, m_view.m_21, m_view.m_22 };
    m_up = { m_view.m_10, m_view.m_11, m_view.m_12 };
    m_right = { m_view.m_00, m_view.m_01, m_view.m_02 };
  }

  void 
  Camera::resize(int32 width, int32 height) {
    m_aspecRatio = (float)width / (float)height;
    m_projection = createProjectionPerspectiveMatrix(m_fovAngle.valueRadians(), 
                                                     (float)width,
                                                     (float)height,
                                                     m_near, 
                                                     m_far);
  }
  void
  Camera::adjustNearAndFar(const float& _near, const float& _far) {
    m_near = _near;
    m_far = _far;
    m_projection = createProjectionPerspectiveMatrix(m_fovAngle.valueRadians(), 
                                                     m_aspecRatio,
                                                     m_near, 
                                                     m_far);
  }
}
