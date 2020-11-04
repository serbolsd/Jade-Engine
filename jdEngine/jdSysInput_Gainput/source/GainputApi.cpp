#include "GainputApi.h"

GainputApi::GainputApi() {
	
}

GainputApi::~GainputApi() {
  delete m_mouseButtonMap;
  delete m_mouseAxisMap;
  delete m_keyMap;
  delete m_gamePadButtonMap;
  delete m_gamePadAxisMap;
}

void 
GainputApi::init(unsigned int screenWidth, unsigned int screenHeight, HWND windowHandle) {
	// Setting up Gainput
	 const gainput::DeviceId mouseId = m_manager.CreateDevice<gainput::InputDeviceMouse>();
  gainput::DeviceId keyboardId = m_manager.CreateDevice<gainput::InputDeviceKeyboard>();
  gainput::DeviceId padId = m_manager.CreateDevice<gainput::InputDevicePad>();
	 m_manager.SetDisplaySize(screenWidth, screenHeight);
  m_mouseButtonMap = new gainput::InputMap(m_manager);
  m_mouseAxisMap = new gainput::InputMap(m_manager);
  m_keyMap = new gainput::InputMap(m_manager);
  m_gamePadButtonMap = new gainput::InputMap(m_manager);
  m_gamePadAxisMap = new gainput::InputMap(m_manager);

  //mouse buttons
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButton0, mouseId, gainput::MouseButton0);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButtonLeft, mouseId, gainput::MouseButtonLeft);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButton1, mouseId, gainput::MouseButton1);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButtonMiddle, mouseId, gainput::MouseButtonMiddle);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButton2, mouseId, gainput::MouseButton2);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButtonRight, mouseId, gainput::MouseButtonRight);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButton3, mouseId, gainput::MouseButton3);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButtonWheelUp, mouseId, gainput::MouseButtonWheelUp);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButton4, mouseId, gainput::MouseButton4);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButtonWheelDown, mouseId, gainput::MouseButtonWheelDown);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButton5, mouseId, gainput::MouseButton5);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButton6, mouseId, gainput::MouseButton6);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButton7, mouseId, gainput::MouseButton7);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButton8, mouseId, gainput::MouseButton8);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButton9, mouseId, gainput::MouseButton9);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButton10, mouseId, gainput::MouseButton10);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButton11, mouseId, gainput::MouseButton11);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButton12, mouseId, gainput::MouseButton12);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButton13, mouseId, gainput::MouseButton13);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButton14, mouseId, gainput::MouseButton14);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButton15, mouseId, gainput::MouseButton15);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButton16, mouseId, gainput::MouseButton16);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButton17, mouseId, gainput::MouseButton17);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButton18, mouseId, gainput::MouseButton18);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButton19, mouseId, gainput::MouseButton19);
  m_mouseButtonMap->MapBool(MOUSE_BUTTON::kMouseButton20, mouseId, gainput::MouseButton20);

  //mouse axis
  m_mouseAxisMap->MapFloat(MOUSE_AXIS::kMouseAxisX, mouseId, gainput::MouseAxisX);
  m_mouseAxisMap->MapFloat(MOUSE_AXIS::kMouseAxisY, mouseId, gainput::MouseAxisY);

  //keyboard
  m_keyMap->MapBool(KEYBOARD::kKeyEscape, keyboardId, gainput::KeyEscape);
  m_keyMap->MapBool(KEYBOARD::kKeyF1, keyboardId, gainput::KeyF1);
  m_keyMap->MapBool(KEYBOARD::kKeyF2, keyboardId, gainput::KeyF2);
  m_keyMap->MapBool(KEYBOARD::kKeyF3, keyboardId, gainput::KeyF3);
  m_keyMap->MapBool(KEYBOARD::kKeyF4, keyboardId, gainput::KeyF4);
  m_keyMap->MapBool(KEYBOARD::kKeyF5, keyboardId, gainput::KeyF5);
  m_keyMap->MapBool(KEYBOARD::kKeyF6, keyboardId, gainput::KeyF6);
  m_keyMap->MapBool(KEYBOARD::kKeyF7, keyboardId, gainput::KeyF7);
  m_keyMap->MapBool(KEYBOARD::kKeyF8, keyboardId, gainput::KeyF8);
  m_keyMap->MapBool(KEYBOARD::kKeyF9, keyboardId, gainput::KeyF9);
  m_keyMap->MapBool(KEYBOARD::kKeyF10, keyboardId, gainput::KeyF10);
  m_keyMap->MapBool(KEYBOARD::kKeyF11, keyboardId, gainput::KeyF11);
  m_keyMap->MapBool(KEYBOARD::kKeyF12, keyboardId, gainput::KeyF12);
  m_keyMap->MapBool(KEYBOARD::kKeyF13, keyboardId, gainput::KeyF13);
  m_keyMap->MapBool(KEYBOARD::kKeyF14, keyboardId, gainput::KeyF14);
  m_keyMap->MapBool(KEYBOARD::kKeyF15, keyboardId, gainput::KeyF15);
  m_keyMap->MapBool(KEYBOARD::kKeyF16, keyboardId, gainput::KeyF16);
  m_keyMap->MapBool(KEYBOARD::kKeyF17, keyboardId, gainput::KeyF17);
  m_keyMap->MapBool(KEYBOARD::kKeyF18, keyboardId, gainput::KeyF18);
  m_keyMap->MapBool(KEYBOARD::kKeyF19, keyboardId, gainput::KeyF19);
  m_keyMap->MapBool(KEYBOARD::kKeyScrollLock, keyboardId, gainput::KeyScrollLock);
  m_keyMap->MapBool(KEYBOARD::kKeyBreak, keyboardId, gainput::KeyBreak);

  m_keyMap->MapBool(KEYBOARD::kKeySpace, keyboardId, gainput::KeySpace);

  m_keyMap->MapBool(KEYBOARD::kKeyApostrophe, keyboardId, gainput::KeyApostrophe);
  m_keyMap->MapBool(KEYBOARD::kKeyComma, keyboardId, gainput::KeyComma);
  m_keyMap->MapBool(KEYBOARD::kKeyMinus, keyboardId, gainput::KeyMinus);
  m_keyMap->MapBool(KEYBOARD::kKeyPeriod, keyboardId, gainput::KeyPeriod);
  m_keyMap->MapBool(KEYBOARD::kKeySlash, keyboardId, gainput::KeySlash);

  m_keyMap->MapBool(KEYBOARD::kKey0, keyboardId, gainput::Key0);
  m_keyMap->MapBool(KEYBOARD::kKey1, keyboardId, gainput::Key1);
  m_keyMap->MapBool(KEYBOARD::kKey2, keyboardId, gainput::Key2);
  m_keyMap->MapBool(KEYBOARD::kKey3, keyboardId, gainput::Key3);
  m_keyMap->MapBool(KEYBOARD::kKey4, keyboardId, gainput::Key4);
  m_keyMap->MapBool(KEYBOARD::kKey5, keyboardId, gainput::Key5);
  m_keyMap->MapBool(KEYBOARD::kKey6, keyboardId, gainput::Key6);
  m_keyMap->MapBool(KEYBOARD::kKey7, keyboardId, gainput::Key7);
  m_keyMap->MapBool(KEYBOARD::kKey8, keyboardId, gainput::Key8);
  m_keyMap->MapBool(KEYBOARD::kKey9, keyboardId, gainput::Key9);

  m_keyMap->MapBool(KEYBOARD::kKeySemicolon, keyboardId, gainput::KeySemicolon);
  m_keyMap->MapBool(KEYBOARD::kKeyLess, keyboardId, gainput::KeyLess);
  m_keyMap->MapBool(KEYBOARD::kKeyEqual, keyboardId, gainput::KeyEqual);

  m_keyMap->MapBool(KEYBOARD::kKeyA, keyboardId, gainput::KeyA);
  m_keyMap->MapBool(KEYBOARD::kKeyB, keyboardId, gainput::KeyB);
  m_keyMap->MapBool(KEYBOARD::kKeyC, keyboardId, gainput::KeyC);
  m_keyMap->MapBool(KEYBOARD::kKeyD, keyboardId, gainput::KeyD);
  m_keyMap->MapBool(KEYBOARD::kKeyE, keyboardId, gainput::KeyE);
  m_keyMap->MapBool(KEYBOARD::kKeyF, keyboardId, gainput::KeyF);
  m_keyMap->MapBool(KEYBOARD::kKeyG, keyboardId, gainput::KeyG);
  m_keyMap->MapBool(KEYBOARD::kKeyH, keyboardId, gainput::KeyH);
  m_keyMap->MapBool(KEYBOARD::kKeyI, keyboardId, gainput::KeyI);
  m_keyMap->MapBool(KEYBOARD::kKeyJ, keyboardId, gainput::KeyJ);
  m_keyMap->MapBool(KEYBOARD::kKeyK, keyboardId, gainput::KeyK);
  m_keyMap->MapBool(KEYBOARD::kKeyL, keyboardId, gainput::KeyL);
  m_keyMap->MapBool(KEYBOARD::kKeyM, keyboardId, gainput::KeyM);
  m_keyMap->MapBool(KEYBOARD::kKeyN, keyboardId, gainput::KeyN);
  m_keyMap->MapBool(KEYBOARD::kKeyO, keyboardId, gainput::KeyO);
  m_keyMap->MapBool(KEYBOARD::kKeyP, keyboardId, gainput::KeyP);
  m_keyMap->MapBool(KEYBOARD::kKeyQ, keyboardId, gainput::KeyQ);
  m_keyMap->MapBool(KEYBOARD::kKeyR, keyboardId, gainput::KeyR);
  m_keyMap->MapBool(KEYBOARD::kKeyS, keyboardId, gainput::KeyS);
  m_keyMap->MapBool(KEYBOARD::kKeyT, keyboardId, gainput::KeyT);
  m_keyMap->MapBool(KEYBOARD::kKeyU, keyboardId, gainput::KeyU);
  m_keyMap->MapBool(KEYBOARD::kKeyV, keyboardId, gainput::KeyV);
  m_keyMap->MapBool(KEYBOARD::kKeyW, keyboardId, gainput::KeyW);
  m_keyMap->MapBool(KEYBOARD::kKeyX, keyboardId, gainput::KeyX);
  m_keyMap->MapBool(KEYBOARD::kKeyY, keyboardId, gainput::KeyY);
  m_keyMap->MapBool(KEYBOARD::kKeyZ, keyboardId, gainput::KeyZ);

  m_keyMap->MapBool(KEYBOARD::kKeyBracketLeft, keyboardId, gainput::KeyBracketLeft);
  m_keyMap->MapBool(KEYBOARD::kKeyBackslash, keyboardId, gainput::KeyBackslash);
  m_keyMap->MapBool(KEYBOARD::kKeyBracketRight, keyboardId, gainput::KeyBracketRight);

  m_keyMap->MapBool(KEYBOARD::kKeyGrave, keyboardId, gainput::KeyGrave);

  m_keyMap->MapBool(KEYBOARD::kKeyLeft, keyboardId, gainput::KeyLeft);
  m_keyMap->MapBool(KEYBOARD::kKeyRight, keyboardId, gainput::KeyRight);
  m_keyMap->MapBool(KEYBOARD::kKeyUp, keyboardId, gainput::KeyUp);
  m_keyMap->MapBool(KEYBOARD::kKeyDown, keyboardId, gainput::KeyDown);
  m_keyMap->MapBool(KEYBOARD::kKeyInsert, keyboardId, gainput::KeyInsert);
  m_keyMap->MapBool(KEYBOARD::kKeyHome, keyboardId, gainput::KeyHome);
  m_keyMap->MapBool(KEYBOARD::kKeyDelete, keyboardId, gainput::KeyDelete);
  m_keyMap->MapBool(KEYBOARD::kKeyEnd, keyboardId, gainput::KeyEnd);
  m_keyMap->MapBool(KEYBOARD::kKeyPageUp, keyboardId, gainput::KeyPageUp);
  m_keyMap->MapBool(KEYBOARD::kKeyPageDown, keyboardId, gainput::KeyPageDown);

  m_keyMap->MapBool(KEYBOARD::kKeyNumLock, keyboardId, gainput::KeyNumLock);
  m_keyMap->MapBool(KEYBOARD::kKeyKpEqual, keyboardId, gainput::KeyKpEqual);
  m_keyMap->MapBool(KEYBOARD::kKeyKpDivide, keyboardId, gainput::KeyKpDivide);
  m_keyMap->MapBool(KEYBOARD::kKeyKpMultiply, keyboardId, gainput::KeyKpMultiply);
  m_keyMap->MapBool(KEYBOARD::kKeyKpSubtract, keyboardId, gainput::KeyKpSubtract);
  m_keyMap->MapBool(KEYBOARD::kKeyKpAdd, keyboardId, gainput::KeyKpAdd);
  m_keyMap->MapBool(KEYBOARD::kKeyKpEnter, keyboardId, gainput::KeyKpEnter);
  m_keyMap->MapBool(KEYBOARD::kKeyKpInsert, keyboardId, gainput::KeyKpInsert);
  m_keyMap->MapBool(KEYBOARD::kKeyKpEnd, keyboardId, gainput::KeyKpEnd);
  m_keyMap->MapBool(KEYBOARD::kKeyKpDown, keyboardId, gainput::KeyKpDown);
  m_keyMap->MapBool(KEYBOARD::kKeyKpPageDown, keyboardId, gainput::KeyKpPageDown);
  m_keyMap->MapBool(KEYBOARD::kKeyKpLeft, keyboardId, gainput::KeyKpLeft);
  m_keyMap->MapBool(KEYBOARD::kKeyKpBegin, keyboardId, gainput::KeyKpBegin);
  m_keyMap->MapBool(KEYBOARD::kKeyKpRight, keyboardId, gainput::KeyKpRight);
  m_keyMap->MapBool(KEYBOARD::kKeyKpHome, keyboardId, gainput::KeyKpHome);
  m_keyMap->MapBool(KEYBOARD::kKeyKpUp, keyboardId, gainput::KeyKpUp);
  m_keyMap->MapBool(KEYBOARD::kKeyKpPageUp, keyboardId, gainput::KeyKpPageUp);
  m_keyMap->MapBool(KEYBOARD::kKeyKpDelete, keyboardId, gainput::KeyKpDelete);

  m_keyMap->MapBool(KEYBOARD::kKeyBackSpace, keyboardId, gainput::KeyBackSpace);
  m_keyMap->MapBool(KEYBOARD::kKeyTab, keyboardId, gainput::KeyTab);
  m_keyMap->MapBool(KEYBOARD::kKeyReturn, keyboardId, gainput::KeyReturn);
  m_keyMap->MapBool(KEYBOARD::kKeyCapsLock, keyboardId, gainput::KeyCapsLock);
  m_keyMap->MapBool(KEYBOARD::kKeyShiftL, keyboardId, gainput::KeyShiftL);
  m_keyMap->MapBool(KEYBOARD::kKeyCtrlL, keyboardId, gainput::KeyCtrlL);
  m_keyMap->MapBool(KEYBOARD::kKeySuperL, keyboardId, gainput::KeySuperL);
  m_keyMap->MapBool(KEYBOARD::kKeyAltL, keyboardId, gainput::KeyAltL);
  m_keyMap->MapBool(KEYBOARD::kKeyAltR, keyboardId, gainput::KeyAltR);
  m_keyMap->MapBool(KEYBOARD::kKeySuperR, keyboardId, gainput::KeySuperR);
  m_keyMap->MapBool(KEYBOARD::kKeyMenu, keyboardId, gainput::KeyMenu);
  m_keyMap->MapBool(KEYBOARD::kKeyCtrlR, keyboardId, gainput::KeyCtrlR);
  m_keyMap->MapBool(KEYBOARD::kKeyShiftR, keyboardId, gainput::KeyShiftR);

  m_keyMap->MapBool(KEYBOARD::kKeyBack, keyboardId, gainput::KeyBack);
  m_keyMap->MapBool(KEYBOARD::kKeySoftLeft, keyboardId, gainput::KeySoftLeft);
  m_keyMap->MapBool(KEYBOARD::kKeySoftRight, keyboardId, gainput::KeySoftRight);
  m_keyMap->MapBool(KEYBOARD::kKeyCall, keyboardId, gainput::KeyCall);
  m_keyMap->MapBool(KEYBOARD::kKeyEndcall, keyboardId, gainput::KeyEndcall);
  m_keyMap->MapBool(KEYBOARD::kKeyStar, keyboardId, gainput::KeyStar);
  m_keyMap->MapBool(KEYBOARD::kKeyPound, keyboardId, gainput::KeyPound);
  m_keyMap->MapBool(KEYBOARD::kKeyDpadCenter, keyboardId, gainput::KeyDpadCenter);
  m_keyMap->MapBool(KEYBOARD::kKeyVolumeUp, keyboardId, gainput::KeyVolumeUp);
  m_keyMap->MapBool(KEYBOARD::kKeyVolumeDown, keyboardId, gainput::KeyVolumeDown);
  m_keyMap->MapBool(KEYBOARD::kKeyPower, keyboardId, gainput::KeyPower);
  m_keyMap->MapBool(KEYBOARD::kKeyCamera, keyboardId, gainput::KeyCamera);
  m_keyMap->MapBool(KEYBOARD::kKeyClear, keyboardId, gainput::KeyClear);
  m_keyMap->MapBool(KEYBOARD::kKeySymbol, keyboardId, gainput::KeySymbol);
  m_keyMap->MapBool(KEYBOARD::kKeyExplorer, keyboardId, gainput::KeyExplorer);
  m_keyMap->MapBool(KEYBOARD::kKeyEnvelope, keyboardId, gainput::KeyEnvelope);
  m_keyMap->MapBool(KEYBOARD::kKeyEquals, keyboardId, gainput::KeyEquals);
  m_keyMap->MapBool(KEYBOARD::kKeyAt, keyboardId, gainput::KeyAt);
  m_keyMap->MapBool(KEYBOARD::kKeyHeadsethook, keyboardId, gainput::KeyHeadsethook);
  m_keyMap->MapBool(KEYBOARD::kKeyFocus, keyboardId, gainput::KeyFocus);
  m_keyMap->MapBool(KEYBOARD::kKeyPlus, keyboardId, gainput::KeyPlus);
  m_keyMap->MapBool(KEYBOARD::kKeyNotification, keyboardId, gainput::KeyNotification);
  m_keyMap->MapBool(KEYBOARD::kKeySearch, keyboardId, gainput::KeySearch);
  m_keyMap->MapBool(KEYBOARD::kKeyMediaPlayPause, keyboardId, gainput::KeyMediaPlayPause);
  m_keyMap->MapBool(KEYBOARD::kKeyMediaStop, keyboardId, gainput::KeyMediaStop);
  m_keyMap->MapBool(KEYBOARD::kKeyMediaNext, keyboardId, gainput::KeyMediaNext);
  m_keyMap->MapBool(KEYBOARD::kKeyMediaPrevious, keyboardId, gainput::KeyMediaPrevious);
  m_keyMap->MapBool(KEYBOARD::kKeyMediaRewind, keyboardId, gainput::KeyMediaRewind);
  m_keyMap->MapBool(KEYBOARD::kKeyMediaFastForward, keyboardId, gainput::KeyMediaFastForward);
  m_keyMap->MapBool(KEYBOARD::kKeyMute, keyboardId, gainput::KeyMute);
  m_keyMap->MapBool(KEYBOARD::kKeyPictsymbols, keyboardId, gainput::KeyPictsymbols);
  m_keyMap->MapBool(KEYBOARD::kKeySwitchCharset, keyboardId, gainput::KeySwitchCharset);

  m_keyMap->MapBool(KEYBOARD::kKeyForward, keyboardId, gainput::KeyForward);
  m_keyMap->MapBool(KEYBOARD::kKeyExtra1, keyboardId, gainput::KeyExtra1);
  m_keyMap->MapBool(KEYBOARD::kKeyExtra2, keyboardId, gainput::KeyExtra2);
  m_keyMap->MapBool(KEYBOARD::kKeyExtra3, keyboardId, gainput::KeyExtra3);
  m_keyMap->MapBool(KEYBOARD::kKeyExtra4, keyboardId, gainput::KeyExtra4);
  m_keyMap->MapBool(KEYBOARD::kKeyExtra5, keyboardId, gainput::KeyExtra5);
  m_keyMap->MapBool(KEYBOARD::kKeyExtra6, keyboardId, gainput::KeyExtra6);
  m_keyMap->MapBool(KEYBOARD::kKeyFn, keyboardId, gainput::KeyFn);

  m_keyMap->MapBool(KEYBOARD::kKeyCircumflex, keyboardId, gainput::KeyCircumflex);
  m_keyMap->MapBool(KEYBOARD::kKeySsharp, keyboardId, gainput::KeySsharp);
  m_keyMap->MapBool(KEYBOARD::kKeyAcute, keyboardId, gainput::KeyAcute);
  m_keyMap->MapBool(KEYBOARD::kKeyAltGr, keyboardId, gainput::KeyAltGr);
  m_keyMap->MapBool(KEYBOARD::kKeyNumbersign, keyboardId, gainput::KeyNumbersign);
  m_keyMap->MapBool(KEYBOARD::kKeyUdiaeresis, keyboardId, gainput::KeyUdiaeresis);
  m_keyMap->MapBool(KEYBOARD::kKeyAdiaeresis, keyboardId, gainput::KeyAdiaeresis);
  m_keyMap->MapBool(KEYBOARD::kKeyOdiaeresis, keyboardId, gainput::KeyOdiaeresis);
  m_keyMap->MapBool(KEYBOARD::kKeySection, keyboardId, gainput::KeySection);
  m_keyMap->MapBool(KEYBOARD::kKeyAring, keyboardId, gainput::KeyAring);
  m_keyMap->MapBool(KEYBOARD::kKeyDiaeresis, keyboardId, gainput::KeyDiaeresis);
  m_keyMap->MapBool(KEYBOARD::kKeyTwosuperior, keyboardId, gainput::KeyTwosuperior);
  m_keyMap->MapBool(KEYBOARD::kKeyRightParenthesis, keyboardId, gainput::KeyRightParenthesis);
  m_keyMap->MapBool(KEYBOARD::kKeyDollar, keyboardId, gainput::KeyDollar);
  m_keyMap->MapBool(KEYBOARD::kKeyUgrave, keyboardId, gainput::KeyUgrave);
  m_keyMap->MapBool(KEYBOARD::kKeyAsterisk, keyboardId, gainput::KeyAsterisk);
  m_keyMap->MapBool(KEYBOARD::kKeyColon, keyboardId, gainput::KeyColon);
  m_keyMap->MapBool(KEYBOARD::kKeyExclam, keyboardId, gainput::KeyExclam);

  m_keyMap->MapBool(KEYBOARD::kKeyBraceLeft, keyboardId, gainput::KeyBraceLeft);
  m_keyMap->MapBool(KEYBOARD::kKeyBraceRight, keyboardId, gainput::KeyBraceRight);
  m_keyMap->MapBool(KEYBOARD::kKeySysRq, keyboardId, gainput::KeySysRq);


  //gamepad buttons
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButtonStart, padId, gainput::PadButtonStart);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButtonSelect, padId, gainput::PadButtonSelect);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButtonLeft, padId, gainput::PadButtonLeft);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButtonRight, padId, gainput::PadButtonRight);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButtonUp, padId, gainput::PadButtonUp);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButtonDown, padId, gainput::PadButtonDown);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButtonA, padId, gainput::PadButtonA);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButtonB, padId, gainput::PadButtonB);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButtonX, padId, gainput::PadButtonX);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButtonY, padId, gainput::PadButtonY);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButtonL1, padId, gainput::PadButtonL1);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButtonR1, padId, gainput::PadButtonR1);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButtonL2, padId, gainput::PadButtonL2);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButtonR2, padId, gainput::PadButtonR2);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButtonL3, padId, gainput::PadButtonL3);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButtonR3, padId, gainput::PadButtonR3);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButtonHome, padId, gainput::PadButtonHome);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButton17, padId, gainput::PadButton17);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButton18, padId, gainput::PadButton18);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButton19, padId, gainput::PadButton19);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButton20, padId, gainput::PadButton20);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButton21, padId, gainput::PadButton21);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButton22, padId, gainput::PadButton22);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButton23, padId, gainput::PadButton23);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButton24, padId, gainput::PadButton24);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButton25, padId, gainput::PadButton25);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButton26, padId, gainput::PadButton26);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButton27, padId, gainput::PadButton27);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButton28, padId, gainput::PadButton28);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButton29, padId, gainput::PadButton29);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButton30, padId, gainput::PadButton30);
  m_gamePadButtonMap->MapBool(GAMEPAD_BUTTON::kPadButton31, padId, gainput::PadButton31);


  //gamepad axis
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonLeftStickX, padId, gainput::PadButtonLeftStickX);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonLeftStickY, padId, gainput::PadButtonLeftStickY);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonRightStickX, padId, gainput::PadButtonRightStickX);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonRightStickY, padId, gainput::PadButtonRightStickY);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis4, padId, gainput::PadButtonAxis4);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis5, padId, gainput::PadButtonAxis5);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis6, padId, gainput::PadButtonAxis6);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis7, padId, gainput::PadButtonAxis7);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis8, padId, gainput::PadButtonAxis8);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis9, padId, gainput::PadButtonAxis9);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis10, padId, gainput::PadButtonAxis10);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis11, padId, gainput::PadButtonAxis11);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis12, padId, gainput::PadButtonAxis12);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis13, padId, gainput::PadButtonAxis13);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis14, padId, gainput::PadButtonAxis14);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis15, padId, gainput::PadButtonAxis15);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis16, padId, gainput::PadButtonAxis16);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis17, padId, gainput::PadButtonAxis17);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis18, padId, gainput::PadButtonAxis18);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis19, padId, gainput::PadButtonAxis19);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis20, padId, gainput::PadButtonAxis20);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis21, padId, gainput::PadButtonAxis21);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis22, padId, gainput::PadButtonAxis22);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis23, padId, gainput::PadButtonAxis23);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis24, padId, gainput::PadButtonAxis24);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis25, padId, gainput::PadButtonAxis25);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis26, padId, gainput::PadButtonAxis26);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis27, padId, gainput::PadButtonAxis27);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis28, padId, gainput::PadButtonAxis28);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis29, padId, gainput::PadButtonAxis29);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis30, padId, gainput::PadButtonAxis30);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAxis31, padId, gainput::PadButtonAxis31);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAccelerationX, padId, gainput::PadButtonAccelerationX);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAccelerationY, padId, gainput::PadButtonAccelerationY);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonAccelerationZ, padId, gainput::PadButtonAccelerationZ);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonGravityX, padId, gainput::PadButtonGravityX);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonGravityY, padId, gainput::PadButtonGravityY);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonGravityZ, padId, gainput::PadButtonGravityZ);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonGyroscopeX, padId, gainput::PadButtonGyroscopeX);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonGyroscopeY, padId, gainput::PadButtonGyroscopeY);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonGyroscopeZ, padId, gainput::PadButtonGyroscopeZ);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonMagneticFieldX, padId, gainput::PadButtonMagneticFieldX);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonMagneticFieldY, padId, gainput::PadButtonMagneticFieldY);
  m_gamePadAxisMap->MapFloat(GAMEPAD_AXIS::kPadButtonMagneticFieldZ, padId, gainput::PadButtonMagneticFieldZ);
}

void 
GainputApi::update() {
  // Call every frame
  m_manager.Update();
}

#if defined ( __WIN32__ ) || defined(_WIN32)
void 
GainputApi::handleMessage(MSG msg) {
  MSG message = tagMSG(msg);
  m_manager.HandleMessage(message);
}
#elif defined(__linux) || defined(__linux__) || defined(linux) || defined(LINUX)
void 
GainputApi::handleMessage(XEvent event) {
  m_manager.HandleEvent(event);
}
#endif

void 
GainputApi::resize(unsigned int screenWidth, unsigned int screenHeight) {
  m_manager.SetDisplaySize(screenWidth, screenHeight);
}

bool 
GainputApi::getKeyDown(KEYBOARD::E key) {
  return m_keyMap->GetBoolIsNew(key);
}

bool 
GainputApi::getKey(KEYBOARD::E key) {
  return m_keyMap->GetBool(key);
}

bool 
GainputApi::getKeyUp(KEYBOARD::E key) {
  return m_keyMap->GetBoolWasDown(key);
}

bool 
GainputApi::getMouseButtonDown(MOUSE_BUTTON::E mouseButton) {
  return m_mouseButtonMap->GetBoolIsNew(mouseButton);
}

bool 
GainputApi::getMouseButton(MOUSE_BUTTON::E mouseButton) {
  return m_mouseButtonMap->GetBool(mouseButton);
}

bool 
GainputApi::getMouseButtonUp(MOUSE_BUTTON::E mouseButton) {
  return m_mouseButtonMap->GetBoolWasDown(mouseButton);
}

float 
GainputApi::getMouseAxis(MOUSE_AXIS::E axis) {
  return m_mouseAxisMap->GetFloat(axis);
}

unsigned int 
GainputApi::getCursorPositionX() {
  int aber = m_manager.GetDisplayWidth();
  return m_mouseAxisMap->GetFloat(MOUSE_AXIS::kMouseAxisX) * m_manager.GetDisplayWidth();
}

unsigned int 
GainputApi::getCursorPositionY() {
  return m_mouseAxisMap->GetFloat(MOUSE_AXIS::kMouseAxisY) * m_manager.GetDisplayHeight();
}

bool 
GainputApi::getGamepadButtonDown(GAMEPAD_BUTTON::E button) {
  return m_gamePadButtonMap->GetBoolIsNew(button);
}

bool 
GainputApi::getGamepadButton(GAMEPAD_BUTTON::E button) {
  return m_gamePadButtonMap->GetBool(button);
}

bool 
GainputApi::getGamepadButtonUp(GAMEPAD_BUTTON::E button) {
  return m_gamePadButtonMap->GetBoolWasDown(button);
}

float 
GainputApi::getGamepadAxis(GAMEPAD_AXIS::E axis) {
  return m_gamePadAxisMap->GetFloat(axis);
}
