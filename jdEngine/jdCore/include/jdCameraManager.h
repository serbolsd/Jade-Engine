/*****************************************************************************/
/**
 * @file    jdCameraManager.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    10/OCT/2020
 * @brief   Module of resource manager
 *
 *
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/
#pragma once
#include "jdPrerequisitesCore.h"

#include <jdModulo.h>

#include "jdCamera.h"

namespace jdEngineSDK {
		class JD_CORE_EXPORT CameraManager : public Module<CameraManager>
		{
		public:
    /**
     * @brief default constructor
     */
				CameraManager() = default;

    /**
     * @brief default destructor
     * @note call de release function
     */
    ~CameraManager() { release();  };

    void
    onStartUp() override;

    /**
     * @brief function to release the cameras
     */
    void
    release() {};

    /**
     * @brief function to create a new camera
     * @param name is the name to call the camera, this name generate its id
     * @param eye is the vector with the position of the camera
     * @param up is the vector with the direction of up of the camera
     * @param at is the vector with the direction where the camera's loocking 
     * @param projType is projection type of the camera
     */
    SPtr<Camera>
    createCamera(String name,
                 const JDVector3& eye = JDVector3(0.0f, 0.0f, 0.0f),
                 const JDVector3& up = JDVector3(0.0f, 1.0f, 0.0f),
                 const JDVector3& at = JDVector3(0.0f, 0.0f, 1.0f),
                 const float& _near = 0.01f,
                 const float& _far = 1000.0f,
                 const Radian& fovAngle = Radian(Math::PI / 4),
                 const float& aspcRatio = 1.777777777777778f,
                 CAMERA_PROJECTION_TYPE::E projType = CAMERA_PROJECTION_TYPE::PERSPECTIVE);

    /**
     * @brief get the current camera
     */
    SPtr<Camera>
    getCurrentCamera(){
      return m_currentCamera;
    };

    /**
     * @brief function get the main camera
     */
    SPtr<Camera>
    getMainCamera() {
      return m_mainCamera;
    };

    /**
     * @brief get the debug camera
     */
    SPtr<Camera>
    getDebugCamera(){
      return m_debugCamera;
    };

    /**
     * @brief get a camera with its names
     * @param name is the name of camera to search
     */
    SPtr<Camera>
    getCamera(const String& name);

    /**
     * @brief get a camera with its id
     * @param id is the id of camera to search
     */
    SPtr<Camera>
    getCamera(uint32 id);
    
    /**
     * @brief change the current camera to the camera with the name
     * @param name is the name of camera to change
     */
    void
    changeCamera(const String& name);

    /**
     * @brief change the current camera to the camera with the id
     * @param id is the id of camera to change
     */
    void
    changeCamera(uint32 id);

    void
    resizeCameraProjection(int32 width, int32 height);

    uint32 
    getNumberOfCameras() {
      return m_numCameras;
    }

    SPtr<Camera>
    interpolateCameras(WeakSptr<Camera> cam1, 
                       WeakSptr<Camera> cam2, 
                       const float& deltaTime, 
                       const float& interpolateTime);


    bool m_interpolating = false;

    Vector<char*> m_camerasName;
		private:
    /**
     * @brief the current camera
     */
    SPtr<Camera> m_currentCamera = nullptr;

    /**
     * @brief the main camera
     */
    SPtr<Camera> m_mainCamera = nullptr;

    /**
     * @brief the debug camera
     */
    SPtr<Camera> m_debugCamera = nullptr;

    /**
     * @brief a map with all cameras created
     * @note the id is created with the name of the camera
     */
    Map<uint32, SPtr<Camera>> m_Cameras;

    /**
     * @brief fuction to creat a id with a name
     */
    uint32
    createHash(const String& name);

    uint32 m_numCameras = 0;

    float m_elapseTimeForInterpolation = 0.0f;
    SPtr<Camera> m_cameraInterpolation = nullptr;
		};

  JD_CORE_EXPORT CameraManager&
  g_CameraMan();
}
