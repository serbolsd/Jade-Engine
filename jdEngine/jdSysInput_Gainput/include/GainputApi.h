#pragma once
/*****************************************************************************/
/**
 * @file    GainputApi.h
 * @author  Juan Miguel Cerda Ordaz
 * @date    02/11/2020
 * @brief   Input system Gainput implementation class.
 */
/*****************************************************************************/


/*****************************************************************************/
/**
 * Includes
 */
/*****************************************************************************/
#include "InputAPI.h"
#include <gainput/gainput.h>

//DLL export for plug ins
#if defined ( __WIN32__ ) || defined(_WIN32)
#    if defined(_MSC_VER)
#    define PLUGIN_EXPORT __declspec(dllexport)
#  else
#    define PLUGIN_EXPORT __attribute__ ((dllexport))
#  endif
#else //Linux/Mac settings
#  define PLUGIN_EXPORT __attribute__ ((visibility ("default")))
#endif

class GainputApi : public InputAPI
{
 public:
  GainputApi();

  ~GainputApi();

  /**
   * @brief Initializes input system's devices, maps buttons and axis and informs window size.
   * @param screenWidth width of the window.
   * @param screenHeight height of the window.
   * @param windowHandle handle of the window.
   * @return void.
   */
  void 
  init(unsigned int screenWidth, 
       unsigned int screenHeight, 
       HWND windowHandle = nullptr) override;

  /**
   * @brief Updates input system.
   * @return void.
   */
  void 
  update() override;

  /**
   * @brief Handle messages(windows) or events(Linux).
   * @param msg message.
   * @return void.
   */
#if defined ( __WIN32__ ) || defined(_WIN32)
  void
  handleMessage(MSG msg) override;
#elif defined(__linux) || defined(__linux__) || defined(linux) || defined(LINUX)
  void
  handleMessage(XEvent event) override;
#endif

  /**
   * @brief Informs to the input system about the new window size.
   * @param screenWidth width of the window.
   * @param screenHeight height of the window.
   * @return void.
   */
  void 
  resize(unsigned int screenWidth, unsigned int screenHeight) override;

  /**
   * @brief Check if given key has been pressed one time.
   * @param key key to check.
   * @return true if is the first time the key has been pressed since last release.
   */
  bool
  getKeyDown(KEYBOARD::E key) override;

  /**
   * @brief Check if given key is being pressed.
   * @param key key to check.
   * @return true if the key is currently pressed.
   */
  bool
  getKey(KEYBOARD::E key) override;

  /**
   * @brief Check if given key has been released.
   * @param key key to check.
   * @return true if the key has been released.
   */
  bool
  getKeyUp(KEYBOARD::E key) override;

  /**
   * @brief Check if given mouse button has been pressed one time.
   * @param mouseButton button to check.
   * @return true if is the first time the button has been pressed since last release.
   */
  bool
  getMouseButtonDown(MOUSE_BUTTON::E mouseButton) override;

  /**
   * @brief Check if given mouse button is being pressed.
   * @param mouseButton mouse button to check.
   * @return true if the mouse button is currently pressed.
   */
  bool
  getMouseButton(MOUSE_BUTTON::E mouseButton) override;

  /**
   * @brief Check if given mouse button has been released.
   * @param mouseButton mouse button to check.
   * @return true if the mouse button has been released.
   */
  bool
  getMouseButtonUp(MOUSE_BUTTON::E mouseButton) override;

  /**
   * @brief gets normalize mouse position in window on given axis.
   * @param axis axis to get.
   * @return current float normalized position on given axis.
   */
  float
	 getMouseAxis(MOUSE_AXIS::E axis) override;

  /**
   * @brief gets cursor's position on X axis in the window.
   * @return position of cursor in window pixels.
   */
  unsigned int
	 getCursorPositionX() override;

  /**
   * @brief gets cursor's position on Y axis in the window.
   * @return position of cursor in window pixels.
   */
  unsigned int
	 getCursorPositionY() override;

  /**
   * @brief Check if given gamepad button has been pressed one time.
   * @param button button to check.
   * @return true if is the first time the gamepad button has been pressed since last release.
   */
  bool
  getGamepadButtonDown(GAMEPAD_BUTTON::E button) override;

  /**
   * @brief Check if given gamepad button is being pressed.
   * @param button gamepad button to check.
   * @return true if the gamepad button is currently pressed.
   */
  bool
  getGamepadButton(GAMEPAD_BUTTON::E button) override;

  /**
   * @brief Check if given gamepad button has been released.
   * @param button gamepad button to check.
   * @return true if the gamepad button has been released.
   */
  bool
  getGamepadButtonUp(GAMEPAD_BUTTON::E button) override;

  /**
   * @brief get axis value of given axis of the gamepad.
   * @param axis axis to get value.
   * @return current float value of the axis.
   */
	 float
	 getGamepadAxis(GAMEPAD_AXIS::E axis) override;

 protected:

 private:
  gainput::InputManager m_manager;

  gainput::InputMap* m_mouseButtonMap;
  gainput::InputMap* m_mouseAxisMap;
  gainput::InputMap* m_keyMap;
  gainput::InputMap* m_gamePadButtonMap;
  gainput::InputMap* m_gamePadAxisMap;
};

extern "C" PLUGIN_EXPORT InputAPI *
createInputApi() {
  InputAPI* pGainInput = new GainputApi();
  return pGainInput;
}
