#include "jdCameraManager.h"

namespace jdEngineSDK {
  
  void 
  CameraManager::onStartUp() {

  }

  SPtr<Camera>
  CameraManager::createCamera(String name, 
                              const JDVector3& eye, 
                              const JDVector3& up, 
                              const JDVector3& at, 
                              const float& _near, 
                              const float& _far, 
                              const Radian& fovAngle, 
                              const float& aspcRatio, 
                              CAMERA_PROJECTION_TYPE::E projType) {
    SPtr<Camera> cam = getCamera(name.c_str());
    if (nullptr != cam)
    {
      return cam;
    }
    uint32 id = createHash(name);
    Camera* newCam = new Camera(name, 
                                id, 
                                eye, 
                                up, 
                                at,
                                _near,
                                _far,
                                fovAngle,
                                aspcRatio, 
                                projType);
    SPtr<Camera> spNewCam(newCam);
    m_Cameras.insert(std::pair <uint32, SPtr<Camera>>(id, spNewCam));
    return spNewCam;
  }

  SPtr<Camera>
  CameraManager::getCamera(const String& name) {
    return getCamera(createHash(name));
  }

  SPtr<Camera> 
  CameraManager::getCamera(uint32 id)
  {
    auto found = m_Cameras.find(id);
    if (found != m_Cameras.end())
    {
      return m_Cameras[id];
    }
    return nullptr;
  }

  void 
  CameraManager::changeCamera(const String& name) {
    m_currentCamera = getCamera(name);
  }

  void 
  CameraManager::changeCamera(uint32 id) {
    m_currentCamera = getCamera(id);
  }

  void 
  CameraManager::resizeCameraProjection(int32 width, int32 height) {
    uint32 numCameras = m_Cameras.size();
    for (uint32 i = 0; i < numCameras; ++i) {
      m_Cameras[i]->resize(width,height);
    }
  }
  
  uint32
  CameraManager::createHash(const String& name) {
    uint32 iterator = 0;
    uint32 hash = 0;
    while (name[iterator] != '\0')
    {
      hash = hash * 101 + name[iterator];
      ++iterator;
    }

    return hash;
  }

  CameraManager& 
  g_CameraMan() {
    return CameraManager::instance();
  }
}
