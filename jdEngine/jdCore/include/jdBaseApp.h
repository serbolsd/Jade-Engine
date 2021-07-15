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
#include <jdPoint.h>
#include <imgui.h>

#include "jdGraphicApi.h""
#include "jdSceneGraph.h"
#include "jdResourceManager.h"
#include "jdCameraManager.h"
#include "InputAPI.h"
#include "jdRender.h"
#include "jdLogger.h"
#include "jdProfiler.h"
#include "jdPhysical.h"

using sf::WindowBase;
using sf::Event;
using sf::WindowHandle;
using graphApi = jdEngineSDK::GraphicApi * (*)();
using RenApi = jdEngineSDK::RenderApi* (*)();

namespace jdEngineSDK {

  class JD_CORE_EXPORT BaseApp
  {
   public:
    /**
     * @brief Default constructor
     */
    BaseApp();

    /**
     * @brief virtual constructor
     */
    virtual ~BaseApp() {};

    /**
     * @brief run the app: init systems loop and destroy system
     */
    int32 
    run();

   protected:
    /**
     * @brief function to call when the window resized
     * @param width is the new width of client
     * @param height is the new width of height
     */
    virtual void
    onResize(int32 width, int32 height);

    /**
     * @brief function to call when the window lost focus
     */
    virtual void
    onLostFocus() {};

    /**
     * @brief function to call when the window gained focus
     */
    virtual void
    onGainedFocus() {};

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
    onKeyReleased(int32 /*code*/, 
                  bool /*alt*/, 
                  bool /*control*/, 
                  bool /*shift*/, 
                  bool /*system*/) {};

    /**
     * @brief function to call when the mouse wheel scrolled
     * @param wheel is the whell code
     * @param delta is Number of ticks the wheel has moved (positive is up, negative is down)
     * @param x is the position of the mouse pointer, relative to the left of the window.
     * @param y is the position of the mouse pointer, relative to the top of the window.
     */
    virtual void
    onMouseWheelScrolled(int32 /*wheel*/, 
                         const float& /*delta*/, 
                         int32 /*x*/, 
                         int32 /*y*/) {};

    /**
     * @brief function to call when a mouse button pressed
     * @param button is the button code
     * @param x is the position of the mouse pointer, relative to the left of the window.
     * @param y is the position of the mouse pointer, relative to the top of the window.
     */
    virtual void
    onMouseButtonPressed(int32 /*button*/,
                         int32 /*x*/, 
                         int32 /*y*/) {};

    /**
     * @brief function to call when a mouse button released
     * @param button is the button code
     * @param x is the position of the mouse pointer, relative to the left of the window.
     * @param y is the position of the mouse pointer, relative to the top of the window.
     */
    virtual void
    onMouseButtonReleased(int32 /*button*/,
                          int32 /*x*/, 
                          int32 /*y*/) {};
    
    /**
     * @brief function to call when the mouse moved
     * @param x is the position of the mouse pointer, relative to the left of the window.
     * @param y is the position of the mouse pointer, relative to the top of the window.
     */
    virtual void
    onMouseMoved(int32 /*x*/, 
                 int32 /*y*/) {};

    /**
     * @brief function to call when the mouse entered to window
     */
    virtual void
    onMouseEntered() {};

    /**
     * @brief function to call when the mouse right button pressed
     */
    virtual void
    onMouseRight() {};

    /**
     * @brief function to call when the mouse left to window
     */
    virtual void
    onMouseLeft() {};

    /**
     * @brief function to call when a joystick's button pressed
     * @param joystickId is the control who pressed the button
     * @param button is the button pressed
     */
    virtual void
    onJoystickButtonPressed(uint32 /*joystickId*/, uint32 /*button*/) {};

    /**
     * @brief function to call when a joystick's button released
     * @param joystickId is the control who released the button
     * @param button is the button released
     */
    virtual void
    onJoystickButtonReleased(uint32 /*joystickId*/, uint32 /*button*/) {};

    /**
     * @brief function to call when a joystick moved
     * @param joystickId is the control who moved
     * @param axis is the axis when moved
     * @param position is postion of axis
     */
    virtual void
    onJoystickMoved(uint32 /*joystickId*/, uint32 /*axis*/, const float& /*position*/) {};

    /**
     * @brief function to call when a joystick connected
     * @param joystickId is the control who connected
     */
    virtual void
    onJoystickConnected(uint32 /*joystickId*/) {};

    /**
     * @brief function to call when a joystick connected
     * @param joystickId is the control who connected
     */
    virtual void
    onJoystickDisconnected(uint32 /*joystickId*/) {};

    /**
     * @brief function to call when the app is being created, before the loop starts
     */
    virtual void
    onCreate() {};

    /**
     * @brief function to call when the app is being detroyed, after the loop ends
     */
    virtual void
    onDestroy() {};

    /**
     * @brief function to call to update the data
     */
    virtual void
    onUpdate(const float& /*deltaTime*/) {};

    /**
     * @brief function to call to render
     */
    virtual void
    onRender() {};

   private:
    /**
     * @brief create the window
     */
    void
    createWindow();
    
    /**
     * @brief init all system of engine
     */
    void
    initSystems();

    /**
     * @brief destroy all system of engine
     */
    void
    destroySystems();

    /**
     * @brief funtion to call back, check the events of window
     */
    void
    handleWindowEvent(Event& wndEvent);


    /**
     * @brief funtion to call back, check the inputs of window
     */
    virtual void
    handleWindownput(const float& deltaTime) {};

    /**
     * @brief update
     */
    void 
    update(const float& deltaTime);

    /**
     * @brief render
     */
    void
    render();

   protected:
    /**
     * @brief the window's position
     */
    JDPoint m_wndPosition;

    /**
     * @brief the window's client size
     */
    JDPoint m_clientSize;

    /**
     * @brief the window's title
     */
    String m_wndTitle;

    /**
     * @brief if the window is full screen
     */
    bool m_fullScreen = false;

    /**
     * @brief if the window has focus
     */
    bool m_windowHasFocus = false;

    /**
     * @brief if simulate physics
     */
    bool m_simulate = false;

    /**
     * @brief if simulate physics
     */
    bool m_simulatePause = false;

    /**
     * @brief the window
     */
    WindowBase m_window;

    InputAPI* m_inputAPI;
  };

}
