#include "OISInputApi.h"
#include <sstream>
#include <iostream>
using namespace GAMEPAD_BUTTON;

IOSApi::IOSApi() {

}

IOSApi::~IOSApi() {
  onShutDown();
}

void 
IOSApi::onShutDown() {
  if (nullptr != m_inputManager) {
    if (nullptr != m_mouse) {
      m_inputManager->destroyInputObject(m_mouse);
      m_mouse = nullptr;
    }
  
    if (nullptr != m_keyboard) {
      m_inputManager->destroyInputObject(m_keyboard);
      m_keyboard = nullptr;
    }
  
    if (nullptr != m_joystick) {
      m_inputManager->destroyInputObject(m_joystick);
      m_joystick = nullptr;
    }
  
    // and comment the line below it
    m_inputManager->destroyInputSystem(m_inputManager);
    //mInputSystem->destroyInputSystem();
    m_inputManager = nullptr;
  }
}

void 
IOSApi::init(unsigned int screenWidth, unsigned int screenHeight, HWND windowHandle) {
  std::ostringstream windowHndStr;
  windowHndStr << windowHandle;

  //m_pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
  m_inputManager = InputManager::createInputSystem((size_t)windowHandle);
  m_mouse = static_cast<OIS::Mouse*>(m_inputManager->createInputObject(OIS::OISMouse, false));
  m_keyboard = static_cast<OIS::Keyboard*>(m_inputManager->createInputObject(OIS::OISKeyboard,
    false));
  m_mouse->getMouseState().width = screenWidth;
  m_mouse->getMouseState().height = screenHeight;
  m_joystick = static_cast<OIS::JoyStick*>(m_inputManager->createInputObject(OIS::OISJoyStick, 
                                                                             false));
  //mapInput input;
  //input.key = OIS::KC_A;
  //input.m_pressed = OIS::KC_A;
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyEscape,
    mapInput(KC_ESCAPE)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKey1, mapInput(KC_1)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKey2, mapInput(KC_2)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKey3, mapInput(KC_3)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKey4, mapInput(KC_4)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKey5, mapInput(KC_5)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKey6, mapInput(KC_6)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKey7, mapInput(KC_7)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKey8, mapInput(KC_8)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKey9, mapInput(KC_9)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKey0, mapInput(KC_0)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyMinus,
                                                         mapInput(KC_MINUS)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyEquals, 
                                                         mapInput(KC_EQUALS)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyBack, 
                                                         mapInput(KC_BACK)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyTab, 
                                                         mapInput(KC_TAB)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyQ, mapInput(KC_Q)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyW, mapInput(KC_W)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyE, mapInput(KC_E)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyR, mapInput(KC_R)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyT, mapInput(KC_T)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyY, mapInput(KC_Y)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyU, mapInput(KC_U)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyI, mapInput(KC_I)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyO, mapInput(KC_O)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyP, mapInput(KC_P)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyBracketLeft, 
                                                         mapInput(KC_LBRACKET)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyBracketRight, 
                                                         mapInput(KC_RBRACKET)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyReturn, 
                                                         mapInput(KC_RETURN)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyCtrlL, 
                                                         mapInput(KC_LCONTROL)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyA, mapInput(KC_A)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyS, mapInput(KC_S)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyD, mapInput(KC_D)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyF, mapInput(KC_F)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyG, mapInput(KC_G)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyH, mapInput(KC_H)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyJ, mapInput(KC_J)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyK, mapInput(KC_K)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyL, mapInput(KC_L)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeySemicolon, 
                                                         mapInput(KC_SEMICOLON)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyApostrophe, 
                                                         mapInput(KC_APOSTROPHE)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyGrave, 
                                                         mapInput(KC_GRAVE)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyShiftL, 
                                                         mapInput(KC_LSHIFT)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyBackslash, 
                                                         mapInput(KC_BACKSLASH)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyZ, mapInput(KC_Z)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyX, mapInput(KC_X)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyC, mapInput(KC_C)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyV, mapInput(KC_V)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyB, mapInput(KC_B)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyN, mapInput(KC_N)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyM, mapInput(KC_M)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyComma, 
                                                         mapInput(KC_COMMA)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyPeriod, 
                                                         mapInput(KC_PERIOD)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeySlash, 
                                                         mapInput(KC_SLASH)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyShiftR, 
                                                         mapInput(KC_RSHIFT)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyKpMultiply, 
                                                         mapInput(KC_MULTIPLY)));
  //THE BASE APP DON'T HAVE LEFT MENU ENUM
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyLMenu, 
                                                         mapInput(KC_LMENU)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeySpace, 
                                                         mapInput(KC_SPACE)));
  //I THINK CAPSLOCK AND CAPITAL ARE THE SAME 
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyCapsLock, mapInput(KC_CAPITAL)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyF1, mapInput(KC_F1)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyF2, mapInput(KC_F2)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyF3, mapInput(KC_F3)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyF4, mapInput(KC_F4)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyF5, mapInput(KC_F5)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyF6, mapInput(KC_F6)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyF7, mapInput(KC_F7)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyF8, mapInput(KC_F8)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyF9, mapInput(KC_F9)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyF10, 
                                                         mapInput(KC_F10)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyNumLock, 
                                                         mapInput(KC_NUMLOCK)));
  //I THINK kKeyScrollLock AND KC_SCROLL ARE THE SAME
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyScrollLock, 
                                                         mapInput(KC_SCROLL)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyNumPad7, 
                                                         mapInput(KC_NUMPAD7)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyNumPad8, 
                                                         mapInput(KC_NUMPAD8)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyNumPad9, 
                                                         mapInput(KC_NUMPAD9)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyKpSubtract, 
                                                         mapInput(KC_SUBTRACT)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyNumPad4, 
                                                         mapInput(KC_NUMPAD4)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyNumPad5, 
                                                         mapInput(KC_NUMPAD5)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyNumPad6, 
                                                         mapInput(KC_NUMPAD6)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyKpAdd, 
                                                         mapInput(KC_ADD)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyNumPad1, 
                                                         mapInput(KC_NUMPAD1)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyNumPad2, 
                                                         mapInput(KC_NUMPAD2)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyNumPad3, 
                                                         mapInput(KC_NUMPAD3)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyNumPad0, 
                                                         mapInput(KC_NUMPAD0)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyKpDecimal, mapInput(KC_DECIMAL)));
  //here will go  KC_OEM_102
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyF11,
                                                         mapInput(KC_F11)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyF12,
                                                         mapInput(KC_F12)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyF13,
                                                         mapInput(KC_F13)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyF14,
                                                         mapInput(KC_F14)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyF15,
                                                         mapInput(KC_F15)));
  //here will go KC_KANA
  //here will go KC_ABNT_C1
  //here will go KC_CONVERT
  //here will go KC_NOCONVERT
  //here will go KC_YEN
  //here will go KC_ABNT_C2
  //here will go KC_NUMPADEQUALS
  //here will go KC_PREVTRACK
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyAt, mapInput(KC_AT)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyColon, 
                                                         mapInput(KC_COLON)));
  //here will go KC_UNDERLINE
  //here will go KC_KANJI
  //m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyMediaStop, other stop
  //                                                       mapInput(KC_STOP)));
  //here will go KC_AX
  //here will go KC_UNLABELED
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyMediaNext,
                                                         mapInput(KC_NEXTTRACK)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyNumPadEnter, 
                                                         mapInput(KC_NUMPADENTER)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyCtrlR, 
                                                         mapInput(KC_RCONTROL)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyMute, 
                                                         mapInput(KC_MUTE)));
  //here will go KC_CALCULATOR
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyMediaPlayPause, 
                                                         mapInput(KC_PLAYPAUSE)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyMediaStop, 
                                                         mapInput(KC_MEDIASTOP)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyTwosuperior, 
                                                         mapInput(KC_TWOSUPERIOR)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyVolumeDown, 
                                                         mapInput(KC_VOLUMEDOWN)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyVolumeUp, 
                                                         mapInput(KC_VOLUMEUP)));
  //here will go KC_WEBHOME
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyNumPadComma, 
                                                         mapInput(KC_NUMPADCOMMA)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyKpDivide, 
                                                         mapInput(KC_DIVIDE)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeySysRq, 
                                                         mapInput(KC_SYSRQ)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyRMenu, 
                                                         mapInput(KC_RMENU)));
  //here will go KC_PAUSE
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyHome, 
                                                         mapInput(KC_HOME)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyUp, 
                                                         mapInput(KC_UP)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyPageUp, 
                                                         mapInput(KC_PGUP)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyLeft, 
                                                         mapInput(KC_LEFT)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyRight, 
                                                         mapInput(KC_RIGHT)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyEnd, 
                                                         mapInput(KC_END)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyDown, 
                                                         mapInput(KC_DOWN)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyKpPageDown, 
                                                         mapInput(KC_PGDOWN)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyInsert, 
                                                         mapInput(KC_INSERT)));
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyDelete, 
                                                         mapInput(KC_DELETE)));
  //here will go KC_LWIN
  //here will go KC_RWIN
  //here will go KC_APPS
  m_keyboardMap.insert(std::pair <KEYBOARD::E, mapInput>(KEYBOARD::kKeyPower, 
                                                         mapInput(KC_POWER)));
  //here will go KC_SLEEP
  //here will go KC_WAKE
  //here will go KC_WEBSEARCH
  //here will go KC_WEBFAVORITES
  //here will go KC_WEBREFRESH
  //here will go KC_WEBSTOP
  //here will go KC_WEBFORWARD
  //here will go KC_WEBBACK
  //here will go KC_MYCOMPUTER
  //here will go KC_MAIL
  //here will go KC_MEDIASELECT

  m_mouseMap.insert(std::pair <MOUSE_BUTTON::E, mapInputMouse>(MOUSE_BUTTON::kMouseButton0,
                                                               mapInputMouse(MB_Left)));
  m_mouseMap.insert(std::pair <MOUSE_BUTTON::E, mapInputMouse>(MOUSE_BUTTON::kMouseButton1,
                                                               mapInputMouse(MB_Right)));
  m_mouseMap.insert(std::pair <MOUSE_BUTTON::E, mapInputMouse>(MOUSE_BUTTON::kMouseButton2,
                                                               mapInputMouse(MB_Middle)));
  m_mouseMap.insert(std::pair <MOUSE_BUTTON::E, mapInputMouse>(MOUSE_BUTTON::kMouseButton3, 
                                                               mapInputMouse(MB_Button3)));
  m_mouseMap.insert(std::pair <MOUSE_BUTTON::E, mapInputMouse>(MOUSE_BUTTON::kMouseButton4, 
                                                               mapInputMouse(MB_Button4)));
  m_mouseMap.insert(std::pair <MOUSE_BUTTON::E, mapInputMouse>(MOUSE_BUTTON::kMouseButton5, 
                                                               mapInputMouse(MB_Button5)));
  m_mouseMap.insert(std::pair <MOUSE_BUTTON::E, mapInputMouse>(MOUSE_BUTTON::kMouseButton6, 
                                                               mapInputMouse(MB_Button6)));
  m_mouseMap.insert(std::pair <MOUSE_BUTTON::E, mapInputMouse>(MOUSE_BUTTON::kMouseButton7, 
                                                               mapInputMouse(MB_Button7)));

  //JoyStick Map for Xbox One controller
  m_joyStickdMap.insert(std::pair <GAMEPAD_BUTTON::E, mapJoystick>(kPadButtonStart,
                                                                   mapJoystick(0)));
  m_joyStickdMap.insert(std::pair <GAMEPAD_BUTTON::E, mapJoystick>(kPadButtonSelect,
                                                                   mapJoystick(1)));
  m_joyStickdMap.insert(std::pair <GAMEPAD_BUTTON::E, mapJoystick>(kPadButtonL3,
                                                                   mapJoystick(2)));
  m_joyStickdMap.insert(std::pair <GAMEPAD_BUTTON::E, mapJoystick>(kPadButtonR3,
                                                                   mapJoystick(3)));
  m_joyStickdMap.insert(std::pair <GAMEPAD_BUTTON::E, mapJoystick>(kPadButtonL1,
                                                                   mapJoystick(4)));
  m_joyStickdMap.insert(std::pair <GAMEPAD_BUTTON::E, mapJoystick>(kPadButtonR1,
                                                                   mapJoystick(5))); 
  m_joyStickdMap.insert(std::pair <GAMEPAD_BUTTON::E, mapJoystick>(kPadButtonA,
                                                                   mapJoystick(8)));
  m_joyStickdMap.insert(std::pair <GAMEPAD_BUTTON::E, mapJoystick>(kPadButtonB,
                                                                   mapJoystick(9)));
  m_joyStickdMap.insert(std::pair <GAMEPAD_BUTTON::E, mapJoystick>(kPadButtonX,
                                                                   mapJoystick(10)));
  m_joyStickdMap.insert(std::pair <GAMEPAD_BUTTON::E, mapJoystick>(kPadButtonY,
                                                                   mapJoystick(10)));
  m_joyStickdMap.insert(std::pair <GAMEPAD_BUTTON::E, mapJoystick>(kPadButtonUp,
                                                                   mapJoystick(-1)));
  m_joyStickdMap.insert(std::pair <GAMEPAD_BUTTON::E, mapJoystick>(kPadButtonDown,
                                                                   mapJoystick(-1)));
  m_joyStickdMap.insert(std::pair <GAMEPAD_BUTTON::E, mapJoystick>(kPadButtonLeft,
                                                                   mapJoystick(-1)));
  m_joyStickdMap.insert(std::pair <GAMEPAD_BUTTON::E, mapJoystick>(kPadButtonRight,
                                                                   mapJoystick(-1)));

}

void 
IOSApi::update() {
  m_keyboard->capture();
  m_mouse->capture();
  m_joystick->capture();
}

void 
IOSApi::resize(unsigned int screenWidth, unsigned int screenHeight) {
  if (nullptr != m_mouse)
  {
    m_mouse->getMouseState().height = screenHeight;
    m_mouse->getMouseState().height = screenWidth;
  }
}

bool
IOSApi::getKeyDown(KEYBOARD::E key) {
  //if (m_keyboard->isKeyDown(OIS::KC_ESCAPE) && key == KEYBOARD::kKeyW)
  //  return true;
  auto found = m_keyboardMap.find(key);
  if (found != m_keyboardMap.end())
  {
    if (!found->second.m_pressed && m_keyboard->isKeyDown(found->second.m_key))
    {
      found->second.m_pressed = true;
      return found->second.m_pressed;
    }
  }
  return false;
}

bool
IOSApi::getKey(KEYBOARD::E key) {
  getKeyDown(key);
  auto found = m_keyboardMap.find(key);
  if (found != m_keyboardMap.end())
  { 
    if (!m_keyboard->isKeyDown(found->second.m_key))
    {
      return false;
    }
    return found->second.m_pressed;
  }
  return false;
}

bool 
IOSApi::getKeyUp(KEYBOARD::E key) {
  auto found = m_keyboardMap.find(key);
  if (found != m_keyboardMap.end())
  {
    if (found->second.m_pressed && !m_keyboard->isKeyDown(found->second.m_key))
    {
      found->second.m_pressed = false;
      return true;
    }
  }
  return false;
}

bool 
IOSApi::getMouseButtonDown(MOUSE_BUTTON::E mouseButton) {
  //OIS::MouseState& s =
  auto found = m_mouseMap.find(mouseButton);
  if (found != m_mouseMap.end())
  {
    if (!found->second.m_pressed && m_mouse->getMouseState().buttonDown(found->second.m_key))
    {
      found->second.m_pressed = true;
      return found->second.m_pressed;
    }
  }
  return false;
}

bool 
IOSApi::getMouseButton(MOUSE_BUTTON::E mouseButton) {
  getMouseButtonDown(mouseButton);
  auto found = m_mouseMap.find(mouseButton);
  if (found != m_mouseMap.end())
  {
    if (!m_mouse->getMouseState().buttonDown(found->second.m_key))
    {
      return false;
    }
    return found->second.m_pressed;
  }
  return false;
}

bool 
IOSApi::getMouseButtonUp(MOUSE_BUTTON::E mouseButton) {
  auto found = m_mouseMap.find(mouseButton);
  if (found != m_mouseMap.end())
  {
    if (found->second.m_pressed && !m_mouse->getMouseState().buttonDown(found->second.m_key))
    {
      found->second.m_pressed = false;
      return true;
    }
  }
  return false;
}

float 
IOSApi::getMouseAxis(MOUSE_AXIS::E axis) {
  switch (axis)
  {
  case MOUSE_AXIS::kMouseAxisX:
    return (float)m_mouse->getMouseState().X.abs;
    break;
  case MOUSE_AXIS::kMouseAxisY:
    return (float)m_mouse->getMouseState().Y.abs;
    break;
  default:
    break;
  }
  return 0.0f;
}

unsigned int 
IOSApi::getCursorPositionX() {
  return (float)m_mouse->getMouseState().X.abs;
  return 0;
}

unsigned int 
IOSApi::getCursorPositionY() {
  return (float)m_mouse->getMouseState().Y.abs;
  return 0;
}

bool 
IOSApi::getGamepadButtonDown(GAMEPAD_BUTTON::E button) {
  
  //auto state = m_joystick->getJoyStickState();
  //m_joystick.
  //int buttonsSize = state.mButtons.size();
  //for (int i = 0; i < buttonsSize; i++)
  //{
  //  if (state.mButtons[i])
  //  {
  //    std::cout << i<<std::endl;
  //  }
  //}

  auto found = m_joyStickdMap.find(button);
  if (found == m_joyStickdMap.end()) {
    return false;
  }
  if (button == kPadButtonLeft)
  {
    int dir = m_joystick->getJoyStickState().mPOV->direction;
    if (!found->second.m_pressed &&
       (dir == m_joystick->getJoyStickState().mPOV->NorthWest ||
        dir == m_joystick->getJoyStickState().mPOV->West ||
        dir == m_joystick->getJoyStickState().mPOV->SouthWest)) {
      found->second.m_pressed = true;
      return true;
    }
    
  }
  if (button == kPadButtonRight)
  {
    int dir = m_joystick->getJoyStickState().mPOV->direction;
    if (!found->second.m_pressed &&
       (dir == m_joystick->getJoyStickState().mPOV->East ||
        dir == m_joystick->getJoyStickState().mPOV->NorthEast ||
        dir == m_joystick->getJoyStickState().mPOV->SouthEast)) {
      found->second.m_pressed = true;
      return true;
    }
  }
  if (button == kPadButtonUp)
  {
    int dir = m_joystick->getJoyStickState().mPOV->direction;
    if (!found->second.m_pressed &&
       (dir == m_joystick->getJoyStickState().mPOV->North ||
        dir == m_joystick->getJoyStickState().mPOV->NorthEast ||
        dir == m_joystick->getJoyStickState().mPOV->NorthWest)) {
      found->second.m_pressed = true;
      return true;
    }
  }
  if (button == kPadButtonDown)
  {
    int dir = m_joystick->getJoyStickState().mPOV->direction;
    if (!found->second.m_pressed &&
       (dir == m_joystick->getJoyStickState().mPOV->South ||
        dir == m_joystick->getJoyStickState().mPOV->SouthEast ||
        dir == m_joystick->getJoyStickState().mPOV->SouthWest)) {
      found->second.m_pressed = true;
      return true;
    }
  }
 
  if (found->second.m_button <0)
  {
    return false;
  }
  if (!found->second.m_pressed && 
      m_joystick->getJoyStickState().mButtons[found->second.m_button]) {
    found->second.m_pressed = true;
    return true;
  }
  return false;
}

bool 
IOSApi::getGamepadButton(GAMEPAD_BUTTON::E button) {

  auto found = m_joyStickdMap.find(button);
  if (found == m_joyStickdMap.end()) {
    return false;
  }
  if (found->second.m_button < 0)
  {
    return false;
  }
  if (found->second.m_pressed) {
    return true;
  }
  return false;
}

bool 
IOSApi::getGamepadButtonUp(GAMEPAD_BUTTON::E button) {
  auto found = m_joyStickdMap.find(button);
  if (found == m_joyStickdMap.end()) {
    return false;
  }
  if (button == kPadButtonLeft)
  {
    int dir = m_joystick->getJoyStickState().mPOV->direction;
    if (found->second.m_pressed &&
       (dir != m_joystick->getJoyStickState().mPOV->NorthWest ||
        dir != m_joystick->getJoyStickState().mPOV->West ||
        dir != m_joystick->getJoyStickState().mPOV->SouthWest)) {
      found->second.m_pressed = false;
      return true;
    }

  }
  if (button == kPadButtonRight)
  {
    int dir = m_joystick->getJoyStickState().mPOV->direction;
    if (found->second.m_pressed &&
       (dir != m_joystick->getJoyStickState().mPOV->East ||
        dir != m_joystick->getJoyStickState().mPOV->NorthEast ||
        dir != m_joystick->getJoyStickState().mPOV->SouthEast)) {
      found->second.m_pressed = false;
      return true;
    }
  }
  if (button == kPadButtonUp)
  {
    int dir = m_joystick->getJoyStickState().mPOV->direction;
    if (found->second.m_pressed &&
       (dir != m_joystick->getJoyStickState().mPOV->North ||
        dir != m_joystick->getJoyStickState().mPOV->NorthEast ||
        dir != m_joystick->getJoyStickState().mPOV->NorthWest)) {
      found->second.m_pressed = false;
      return true;
    }
  }
  if (button == kPadButtonDown)
  {
    int dir = m_joystick->getJoyStickState().mPOV->direction;
    if (found->second.m_pressed &&
       (dir != m_joystick->getJoyStickState().mPOV->South ||
        dir != m_joystick->getJoyStickState().mPOV->SouthEast ||
        dir != m_joystick->getJoyStickState().mPOV->SouthWest)) {
      found->second.m_pressed = false;
      return true;
    }
  }
  if (found->second.m_button < 0)
  {
    return false;
  }
  if (found->second.m_pressed &&
    !m_joystick->getJoyStickState().mButtons[found->second.m_button]) {
    found->second.m_pressed = false;
    return true;
  }
  return false;
}

float 
IOSApi::getGamepadAxis(GAMEPAD_AXIS::E axis) {
  //switch (axis)
  //{
  //case GAMEPAD_AXIS::kPadButtonLeftStickX:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonLeftStickY:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonRightStickX:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonRightStickY:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis4:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis5:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis6:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis7:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis8:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis9:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis10:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis11:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis12:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis13:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis14:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis15:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis16:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis17:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis18:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis19:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis20:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis21:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis22:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis23:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis24:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis25:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis26:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis27:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis28:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis29:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis30:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAxis31:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAccelerationX:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAccelerationY:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonAccelerationZ:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonGravityX:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonGravityY:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonGravityZ:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonGyroscopeX:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonGyroscopeY:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonGyroscopeZ:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonMagneticFieldX:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonMagneticFieldY:
  //  break;
  //case GAMEPAD_AXIS::kPadButtonMagneticFieldZ:
  //  break;
  //case GAMEPAD_AXIS::kCount:
  //  break;
  //default:
  //  break;
  //}
  //m_joystick->getJoyStickState().mAxes.
  auto state = m_joystick->getJoyStickState();
  //m_joystick.
  int axesSize = state.mAxes.size();
  for (int i = 0; i < axesSize; i++)
  {
    if (state.mAxes[i].abs != 0)
    {
      std::cout << i << "--"<< state.mAxes[i].abs <<std::endl;
    }
  }
  return 0.0f;
}