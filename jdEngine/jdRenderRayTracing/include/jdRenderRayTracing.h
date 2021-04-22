/*****************************************************************************/
/**
 * @file    jdRenderRayTracing.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    14/MAR/2021
 * @brief   class for render
 *
 * @note only to test shaders
 *
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/
#pragma once
#include "jdRenderRayTracingPrerequisites.h"

#include "jdRTSphere.h"

#include <jdRender.h>
#include <jdGraphicApi.h>
#include <jdCameraManager.h>
#include <jdPoint.h>
#include <SFML/Window.hpp>
#include <imgui.h>
#include <imfilebrowser.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#include "PerlinNoise.h"
namespace jdEngineSDK {

#define M_PI 3.141592653589793 
#define INFINITY 1e8
#define MAX_RAY_DEPTH 5 

  class RenderRayTracingApi : public RenderApi
  {
   public:
    /**
     * @brief Default constructor
     */
     RenderRayTracingApi();
    /**
     * @brief Default destructor
     */
    ~RenderRayTracingApi();

    /**
     * @brief function to call to init the render
     * @param wndHandle is the windowHandle
     * @param clientSize is size of the window's client
     */
    void
    init(void* wndHandle, const JDPoint& clientSize) override;

    /**
     * @brief function to call on update
     * @param deltaTime is de elapse time with the last frame
     */
    void
    onUpdate(const float& deltaTime) override;

    /**
     * @brief function to call on render
     */
    void
    onRender() override;

    /**
     * @brief function to call when shut down
     */
    void
    onDestroy() override;

    /**
     * @brief function to call on resize
     * @param width is the new width
     * @param height is the new height
     */
    void
    onResize(int32 width, int32 height) override;

    /**
     * @brief function to call when a mouse button pressed
     * @param button is the button code
     * @param x is the position of the mouse pointer, relative to the left of the window.
     * @param y is the position of the mouse pointer, relative to the top of the window.
     */
    void
    onMouseButtonPressed(int32 button, int32 x, int32 y) override;

    /**
     * @brief function to call when text entered
     * @param unicode is the text that entered
     */
    void
    onTextEnterd(UNICHAR unicode) override;

    /**
     * @brief function to call when a key is pressed
     * @param code is the key code pressed
     * @param control is if the key control is pressing
     * @param shift is if the key shift is pressing
     * @param system is if the key system is pressing
     */
    void
    onKeyPressed(int32 code,
                 bool alt,
                 bool control,
                 bool shift,
                 bool system) override;

    /**
     * @brief function to call when a key is released
     * @param code is the key code pressed
     * @param control is if the key control is pressing
     * @param shift is if the key shift is pressing
     * @param system is if the key system is pressing
     */
    void
    onKeyReleased(int32 code,
                  bool alt,
                  bool control,
                  bool shift,
                  bool system) override;

    /**
     * @brief function to call when a mouse button released
     * @param button is the button code
     * @param x is the position of the mouse pointer, relative to the left of the window.
     * @param y is the position of the mouse pointer, relative to the top of the window.
     */
    void
    onMouseButtonReleased(int32 button,
                          int32 x,
                          int32 y) override;


  private:
    /**
     * @brief function to call on crete
     */
    void
    onCreate();

    /**
     * @brief function to Init imgui
     */
    void
    initImGui();

    /**
     * @brief function to set dockers of imgui and set the menuBar
     */
    void
    imguiDockScreen();

    float 
    mix(const float& a, const float& b, const float& mix) {
      return b * mix + a * (1 - mix);
    }
    
    JDVector3 Trace(const JDVector3& rayorig,
                    const JDVector3& raydir,
                    const Vector<jdRTSphere>& spheres,
                    const int& depth);

    void
    updateImagen();

    void
    ResetNoise();

//////////////////////////////////////////////////////////////////////////////////////////////
// Render Targets
//////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief shared pointer to render target view
     */
    SPtr<RenderTargetView> m_rtv = nullptr;

//////////////////////////////////////////////////////////////////////////////////////////////
// Shaders
//////////////////////////////////////////////////////////////////////////////////////////////
    /**
       * @brief shared pointer to a program shader for only draw a geometry
       */
    SPtr<ProgramShader> m_PSBasic = nullptr;

//////////////////////////////////////////////////////////////////////////////////////////////
// Constant Buffers
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
// Data of Constant Buffers
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
// View Ports
//////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief the viewPort of the window
     */
    ViewPort m_viewPort;
//////////////////////////////////////////////////////////////////////////////////////////////
// Input Layout
//////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief shared ponter to a input layout
     */
    SPtr<InputLayout> m_inLayOut = nullptr;

//////////////////////////////////////////////////////////////////////////////////////////////
// Cameras
//////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief share pointer to debug camera
     * @note the debug camera is the camera how you can see the scene all time
     */
    SPtr<Camera> m_debugCam = nullptr;

    Vector<JDPoint> m_pixelOrder;
    int32 m_pixelCount = 0;
    int32 m_currentPixelCount = 0;
    int32 m_maxPixelCount = 0;
    int32 m_pixelPerFrame=1000;
    

    unsigned char* m_image;
    JDVector3* m_pixels;
    uint32 m_imgWidth = 640;
    float m_invImgWidth = 0;
    uint32 m_imgHeight = 480;
    float m_invImgHeight = 0;

    float m_fov = 30;
    float m_aspectratio = 0;
    float m_angle = 0;

    Vector<jdRTSphere> m_spheres;

    SPtr<Texture2D> m_testRT;

    float m_elapseTime=0;

    int m_depthOfRay = 5;


    PerlinNoise m_pn;
    unsigned char* m_imgNoise;
    SPtr<Texture2D> m_testNoise;
    int m_NoiseScale = 10;
    int m_NoiseMultiply = 20;
  };

  extern "C" JD_PLUGIN_EXPORT RenderApi *
  createRenderAPI() {
    RenderRayTracingApi* pRA = new RenderRayTracingApi();
    return pRA;
  }
}