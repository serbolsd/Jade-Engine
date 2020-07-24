/*****************************************************************************/
/**
 * @file    jdBaseApp.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    22/JUN/2020
 * @brief   Base App class for any api
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once
#include <SFML/Window.hpp>
#include "jdGraphicApi.h"
#include "jdPoint.h"

using sf::WindowBase;
using sf::Event;
using sf::WindowHandle;
using graphApi = jdEngineSDK::GraphicApi * (*)();

namespace jdEngineSDK {

  class JD_CORE_EXPORT BaseApp
  {
   public:
    BaseApp();
    virtual ~BaseApp() {};

    int32 
    run();

   protected:
    virtual void
    onResize(int32 width, int32 height);

    virtual void
    onLostFocus() {};

    virtual void
    onGainedFocus() {};

    virtual void
    onTextEnterd(UNICHAR /*unicode*/) {};

    virtual void
    onKeyPressed(int32 /*code*/, 
                 bool /*alt*/, 
                 bool /*control*/, 
                 bool /*shift*/, 
                 bool /*system*/) {};

    virtual void
    onKeyReleased(int32 /*code*/, 
                  bool /*alt*/, 
                  bool /*control*/, 
                  bool /*shift*/, 
                  bool /*system*/) {};

    virtual void
    onMouseWheelScrolled(int32 /*wheel*/, 
                         const float& /*delta*/, 
                         int32 /*x*/, 
                         int32 /*y*/) {};

    virtual void
    onMouseButtonPressed(int32 /*wheel*/,
                         int32 /*x*/, 
                         int32 /*y*/) {};

    virtual void
    onMouseButtonReleased(int32 /*wheel*/,
                         int32 /*x*/, 
                         int32 /*y*/) {};
    
    virtual void
    onMouseMoved(int32 /*x*/, 
                 int32 /*y*/) {};

    virtual void
    onMouseEntered() {};

    virtual void
    onMouseRight() {};

    virtual void
    onMouseLeft() {};

    virtual void
    onJoystickButtonPressed(uint32 /*joystickId*/, uint32 /*button*/) {};

    virtual void
    onJoystickButtonReleased(uint32 /*joystickId*/, uint32 /*button*/) {};

    virtual void
    onJoystickMoved(uint32 /*joystickId*/, uint32 /*axis*/, const float& /*position*/) {};

    virtual void
    onJoystickConnected(uint32 /*joystickId*/) {};

    virtual void
    onJoystickDisconnected(uint32 /*joystickId*/) {};

    virtual void
    onCreate() {};

    virtual void
    onDestroy() {};

    virtual void
    onUpdate(const float& /*deltaTime*/) {};

    virtual void
    onRender() {};

   private:
    void
    createWindow();
    
    void
    initSystems();

    void
    destroySystems();

    void
    handleWindowEvent(Event& wndEvent);

    void 
    update(const float& deltaTime);

    void
    render();

   protected:
    JDPoint m_wndPosition;
    JDPoint m_clientSize;
    String m_wndTitle;
    bool m_fullScreen = false;

    bool m_windowHasFocus = false;

    WindowBase m_window;
  };

}
