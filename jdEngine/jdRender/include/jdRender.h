/*****************************************************************************/
/**
 * @file    jdRender.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    06/DIC/2020
 * @brief   class virtual class of Render
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once
#include "jdRenderPrerequisites.h"

#include <jdModulo.h>
#include <jdGraphicApi.h>
#include <jdSceneGraph.h>
#include <jdResourceManager.h>
#include <jdPoint.h>
#include <jdLogger.h>
#include <jdRandom.h>


namespace jdEngineSDK {
  class JD_RENDER_EXPORT RenderApi : public Module<RenderApi> {
   public:
    /**
     * @brief Default constructor
     */
    RenderApi() {};

    /**
     * @brief Default destructor
     */
    virtual ~RenderApi() {};

    /**
     * @brief to set the api objetc selected
     */
    void
      setObject(RenderApi* api) {
      RenderApi::_instance() = api;
    }

    /**
     * @brief function to call to init the render
     * @param wndHandle is the windowHandle
     * @param clientSize is size of the window's client
     */
    virtual void
    init(void* /*wndHandle*/, const JDPoint& /*clientSize*/) {};

    /**
     * @brief function to call on update
     * @param deltaTime is de elapse time with the last frame
     */
    virtual void
    onUpdate(const float& /*deltaTime*/) {};

    /**
     * @brief function to call on render
     */
    virtual void
    onRender() {};

    /**
     * @brief function to call when shut down
     */
    virtual void
    onDestroy() {};

    /**
     * @brief function to call on resize
     * @param width is the new width
     * @param height is the new height
     */
    virtual void
    onResize(int32 width, int32 height) {};

    /**
     * @brief funtion to call back, check the inputs of window
     */
    virtual void
    handleWindownput(const float& deltaTime) {};

    /**
     * @brief function to call when a mouse button pressed
     * @param button is the button code
     * @param x is the position of the mouse pointer, relative to the left of the window.
     * @param y is the position of the mouse pointer, relative to the top of the window.
     */
    virtual void
    onMouseButtonPressed(int32 button, int32 /*x*/, int32 /*y*/) {};

    /**
     * @brief function to call when text entered
     * @param unicode is the text that entered
     */
    virtual void
    onTextEnterd(UNICHAR /*unicode*/) {};

    /**
     * @brief function to call when a key is pressed
     * @param code is the key code pressed
     * @param control is if the key control is pressing
     * @param shift is if the key shift is pressing
     * @param system is if the key system is pressing
     */
    virtual void
    onKeyPressed(int32 /*code*/,
                 bool /*alt*/,
                 bool /*control*/,
                 bool /*shift*/,
                 bool /*system*/) {};

    /**
     * @brief function to call when a key is released
     * @param code is the key code pressed
     * @param control is if the key control is pressing
     * @param shift is if the key shift is pressing
     * @param system is if the key system is pressing
     */
    virtual void
    onKeyReleased(int32 code, 
                  bool alt, 
                  bool control, 
                  bool shift, 
                  bool system) {};

    /**
     * @brief function to call when a mouse button released
     * @param button is the button code
     * @param x is the position of the mouse pointer, relative to the left of the window.
     * @param y is the position of the mouse pointer, relative to the top of the window.
     */
    virtual void
    onMouseButtonReleased(int32 button,
                          int32 x, 
                          int32 y) {};
    
    /**
     * @brief function to set Handle if needed
     * @param wndHandle is the windowHandle
     */
    virtual void
    setWindowHandle(void* handle) { m_wndHandle = handle; };

    bool*
    m_windowHasFocus = nullptr;

   protected:
    /**
     * @brief handle to window
     */
    void* m_wndHandle = nullptr;

    /**
     * @brief the window's client size
     */
    JDPoint m_clientSize = { 800,800 };

  };

  JD_RENDER_EXPORT RenderApi&
  g_Render();

}