/*****************************************************************************/
/**
 * @file    jdRenderShaderTest.h
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
#include "jdRenderShaderTestsPrerequisites.h"

#include <jdRender.h>
#include <jdGraphicApi.h>
#include <jdCameraManager.h>
#include <jdPoint.h>
#include <SFML/Window.hpp>
#include <imgui.h>
#include <imfilebrowser.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
namespace jdEngineSDK {
  /**
   * @brief struct to vertex data
   */
  struct SimpleVertex
  {
    JDVector4 pos;
  };

  /**
   * @brief struct to contant buffer for beziel
   */
  struct CBBezier
  {
    JDMatrix4 m_world;
    JDMatrix4 m_view;
    JDMatrix4 m_proj;
  };

  class RenderShaderTestingApi : public RenderApi
  {
   public:
    /**
     * @brief Default constructor
     */
    RenderShaderTestingApi();
    /**
     * @brief Default destructor
     */
    ~RenderShaderTestingApi();

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
    /**
     * @brief shared ponter to a constant buffer never change
     */
    SPtr<ConstantBuffer> m_CBBezier = nullptr;

//////////////////////////////////////////////////////////////////////////////////////////////
// Data of Constant Buffers
//////////////////////////////////////////////////////////////////////////////////////////////
    /**
      * @brief the data to bezier buffer
      */
    CBBezier m_DBezierCB;

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

    JDMatrix4 m_world;
    float m_worldScale = 1;

    SPtr<VertexBuffer> m_figureVB;
    SPtr<VertexBuffer> m_cubeVB;
    SPtr<IndexBuffer> m_cubeIB;
  };

  extern "C" JD_PLUGIN_EXPORT RenderApi *
    createRenderAPI() {
    RenderShaderTestingApi* pRA = new RenderShaderTestingApi();
    return pRA;
  }
}