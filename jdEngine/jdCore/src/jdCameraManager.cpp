#include "jdCameraManager.h"

namespace jdEngineSDK {
  
  void 
  CameraManager::onStartUp() {
    JDVector3 Eye(0.0f, 130, -195);
    JDVector3 At(0.0f, 130.0f, 0.0f);
    JDVector3 Up(0.0f, 1.0f, 0.0f);

    uint32 id = createHash("debug");
    Camera* newCam = new Camera("debug",
                                 id, 
                                 Eye, 
                                 Up, 
                                 At,
                                 0.01f,
                                 100000.0f,
                                 Radian(Math::HALF_PI / 2),
                                 float(800 / 600), 
                                 CAMERA_PROJECTION_TYPE::PERSPECTIVE);
    m_debugCamera.reset(newCam);
    //m_Cameras.insert(std::pair <uint32, SPtr<Camera>>(id, m_debugCamera));

    id = createHash("main");
    newCam = new Camera("main",
                         id, 
                         Eye, 
                         Up, 
                         At,
                         0.01f,
                         100000.0f,
                         Radian(Math::HALF_PI / 2),
                         float(800 / 600), 
                         CAMERA_PROJECTION_TYPE::PERSPECTIVE);
    m_mainCamera.reset(newCam);
    m_Cameras.insert(std::pair <uint32, SPtr<Camera>>(id, m_mainCamera));
    ++m_numCameras;
    m_camerasName.push_back((char*)"none");
    m_camerasName.push_back((char*)"main");

    //createCamera("debug",
    //  Eye,
    //  Up,
    //  At,
    //  0.01f,
    //  100000.0f,
    //  Radian(Math::HALF_PI / 2),
    //  float(800 / 600),
    //  CAMERA_PROJECTION_TYPE::PERSPECTIVE);
    //
    //createCamera("main",
    //  Eye,
    //  Up,
    //  At,
    //  0.01f,
    //  100000.0f,
    //  Radian(Math::HALF_PI / 2),
    //  float(800 / 600),
    //  CAMERA_PROJECTION_TYPE::PERSPECTIVE);
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
    ++m_numCameras;
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
    //char* nam = move(name.data());
    char* cname = new char[name.size() + 1];
    strcpy(cname, name.c_str());
    m_camerasName.push_back(cname);
    //char* nam = m_camerasName[m_camerasName.size()-1];
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
    uint32 numCameras = (uint32)m_Cameras.size();
    for (auto cam: m_Cameras) {
      cam.second->resize(width,height);
    }
  }

  SPtr<Camera> 
  CameraManager::interpolateCameras(WeakSptr<Camera> cam1, 
                                    WeakSptr<Camera> cam2, 
                                    const float& deltaTime, 
                                    const float& interpolateTime) {
    
    Camera* _cam1 = cam1.lock().get();
    Camera* _cam2 = cam2.lock().get();
    if (nullptr == m_cameraInterpolation)
    {
      m_cameraInterpolation.reset(new Camera);
    }
    JDVector3 pos;
    JDVector3 at;
    JDVector3 up;
    m_elapseTimeForInterpolation += deltaTime;
    if (m_elapseTimeForInterpolation>= interpolateTime)
    {
      m_interpolating = false;
      m_elapseTimeForInterpolation = interpolateTime;
    }
    float factor = m_elapseTimeForInterpolation / interpolateTime;
    JDVector3 start = { _cam1->m_position.x, _cam1->m_position.y, _cam1->m_position.z };
    JDVector3 end = { _cam2->m_position.x, _cam2->m_position.y, _cam2->m_position.z };
    JDVector3 delta = end - start;
    pos = start + factor * delta;

    start = { _cam1->m_front.x, _cam1->m_front.y, _cam1->m_front.z };
    end = { _cam2->m_front.x, _cam2->m_front.y, _cam2->m_front.z };
    delta = end - start;
    at = start + factor * delta;
    at += pos;

    start = { _cam1->m_up.x, _cam1->m_up.y, _cam1->m_up.z };
    end = { _cam2->m_up.x, _cam2->m_up.y, _cam2->m_up.z };
    delta = end - start;
    up = start + factor * delta;
    m_cameraInterpolation->m_view = createViewMatrix(pos, at, up);
    m_cameraInterpolation->m_view.transpose();
    m_cameraInterpolation->m_projection = _cam1->getMatrixProjection();
    //m_cameraInterpolation->m_projection.transpose();
    if (!m_interpolating)
    {
      m_elapseTimeForInterpolation = 0.0f;
    }
    return m_cameraInterpolation;
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
