#pragma once

#define INPUT_EXPORTS //remove if needed

#if defined ( __WIN32__ ) || defined(_WIN32)
#  if defined(_MSC_VER)
#    if defined( STATIC_LIB )
#      define INPUT_EXPORT
#    else
#      if defined( INPUT_EXPORTS )
#        define INPUT_EXPORT __declspec( dllexport )
#      else
#        define INPUT_EXPORT __declspec( dllimport )
#      endif
#    endif
#  else //Any other compiler
#    if defined( STATIC_LIB )
#      define INPUT_EXPORT
#    else
#      if defined( INPUT_EXPORTS )
#        define INPUT_EXPORT __attribute__ ( dllexport )
#      else
#        define INPUT_EXPORT __attribute__ ( dllimport )
#      endif
#    endif
#  endif
#  define INPUT_HIDDEN
#else //Linux/Mac settings
# define INPUT_EXPORT __attribute__ ((visibility ("default")))
# define INPUT_HIDDEN __attribute__ ((visibility ("hidden")))
#endif

/*****************************************************************************/
/**
	* @file    InputAPI.h
	* @author  Juan Miguel Cerda Ordaz
	* @date    02/11/2020
	* @brief   Input system base class.
	*/
	/*****************************************************************************/


	/*****************************************************************************/
	/**
		* Includes
		*/
		/*****************************************************************************/
#if defined ( __WIN32__ ) || defined(_WIN32)
#include <windows.h>
#elif defined(__linux) || defined(__linux__) || defined(linux) || defined(LINUX)
#include <X11/Xlib.h>
#endif

namespace KEYBOARD
{
		enum E
		{
				kKeyEscape,
				kKeyF1,
				kKeyF2,
				kKeyF3,
				kKeyF4,
				kKeyF5,
				kKeyF6,
				kKeyF7,
				kKeyF8,
				kKeyF9,
				kKeyF10,
				kKeyF11,
				kKeyF12,
				kKeyF13,
				kKeyF14,
				kKeyF15,
				kKeyF16,
				kKeyF17,
				kKeyF18,
				kKeyF19,
				kKeyPrint,
				kKeyScrollLock,
				kKeyBreak,

				kKeySpace,

				kKeyApostrophe,
				kKeyComma,
				kKeyMinus,
				kKeyPeriod,
				kKeySlash,

				kKey0,
				kKey1,
				kKey2,
				kKey3,
				kKey4,
				kKey5,
				kKey6,
				kKey7,
				kKey8,
				kKey9,

				kKeyNumPad0,
				kKeyNumPad1,
				kKeyNumPad2,
				kKeyNumPad3,
				kKeyNumPad4,
				kKeyNumPad5,
				kKeyNumPad6,
				kKeyNumPad7,
				kKeyNumPad8,
				kKeyNumPad9,
				kKeyNumPadEnter,
				kKeyNumPadComma,

				kKeySemicolon,
				kKeyLess,
				kKeyEqual,

				kKeyA,
				kKeyB,
				kKeyC,
				kKeyD,
				kKeyE,
				kKeyF,
				kKeyG,
				kKeyH,
				kKeyI,
				kKeyJ,
				kKeyK,
				kKeyL,
				kKeyM,
				kKeyN,
				kKeyO,
				kKeyP,
				kKeyQ,
				kKeyR,
				kKeyS,
				kKeyT,
				kKeyU,
				kKeyV,
				kKeyW,
				kKeyX,
				kKeyY,
				kKeyZ,

				kKeyBracketLeft,
				kKeyBackslash,
				kKeyBracketRight,

				kKeyGrave,

				kKeyLeft,
				kKeyRight,
				kKeyUp,
				kKeyDown,
				kKeyInsert,
				kKeyHome,
				kKeyDelete,
				kKeyEnd,
				kKeyPageUp,
				kKeyPageDown,

				kKeyNumLock,
				kKeyKpEqual,
				kKeyKpDivide,
				kKeyKpMultiply,
				kKeyKpSubtract,
				kKeyKpAdd,
				kKeyKpEnter,
				kKeyKpInsert, // 0
				kKeyKpEnd, // 1
				kKeyKpDown, // 2
				kKeyKpPageDown, // 3
				kKeyKpLeft, // 4
				kKeyKpBegin, // 5
				kKeyKpRight, // 6
				kKeyKpHome, // 7
				kKeyKpUp, // 8
				kKeyKpPageUp, // 9
				kKeyKpDelete, // ,
				kKeyKpDecimal, // ,

				kKeyBackSpace,
				kKeyTab,
				kKeyReturn,
				kKeyCapsLock,
				kKeyShiftL,
				kKeyCtrlL,
				kKeySuperL,
				kKeyAltL,
				kKeyAltR,
				kKeySuperR,
				kKeyMenu,	//Can use this for both menu in some input api
				kKeyLMenu,
				kKeyRMenu,
				kKeyCtrlR,
				kKeyShiftR,

				kKeyBack,
				kKeySoftLeft,
				kKeySoftRight,
				kKeyCall,
				kKeyEndcall,
				kKeyStar,
				kKeyPound,
				kKeyDpadCenter,
				kKeyVolumeUp,
				kKeyVolumeDown,
				kKeyPower,
				kKeyCamera,
				kKeyClear,
				kKeySymbol,
				kKeyExplorer,
				kKeyEnvelope,
				kKeyEquals,
				kKeyAt,
				kKeyHeadsethook,
				kKeyFocus,
				kKeyPlus,
				kKeyNotification,
				kKeySearch,
				kKeyMediaPlayPause,
				kKeyMediaStop,
				kKeyMediaNext,
				kKeyMediaPrevious,
				kKeyMediaRewind,
				kKeyMediaFastForward,
				kKeyMute,
				kKeyPictsymbols,
				kKeySwitchCharset,

				kKeyForward,
				kKeyExtra1,
				kKeyExtra2,
				kKeyExtra3,
				kKeyExtra4,
				kKeyExtra5,
				kKeyExtra6,
				kKeyFn,

				kKeyCircumflex,
				kKeySsharp,
				kKeyAcute,
				kKeyAltGr,
				kKeyNumbersign,
				kKeyUdiaeresis,
				kKeyAdiaeresis,
				kKeyOdiaeresis,
				kKeySection,
				kKeyAring,
				kKeyDiaeresis,
				kKeyTwosuperior,
				kKeyRightParenthesis,
				kKeyDollar,
				kKeyUgrave,
				kKeyAsterisk,
				kKeyColon,
				kKeyExclam,

				kKeyBraceLeft,
				kKeyBraceRight,
				kKeySysRq,

				kCount
		};
}

namespace MOUSE_BUTTON
{
		enum E
		{
				kMouseButton0 = 0,
				kMouseButtonLeft = kMouseButton0,
				kMouseButton1,
				kMouseButtonMiddle = kMouseButton1,
				kMouseButton2,
				kMouseButtonRight = kMouseButton2,
				kMouseButton3,
				kMouseButtonWheelUp = kMouseButton3,
				kMouseButton4,
				kMouseButtonWheelDown = kMouseButton4,
				kMouseButton5,
				kMouseButton6,
				kMouseButton7,
				kMouseButton8,
				kMouseButton9,
				kMouseButton10,
				kMouseButton11,
				kMouseButton12,
				kMouseButton13,
				kMouseButton14,
				kMouseButton15,
				kMouseButton16,
				kMouseButton17,
				kMouseButton18,
				kMouseButton19,
				kMouseButton20,
				kCount
		};
}

namespace MOUSE_AXIS
{
		enum E
		{
				kMouseAxisX,
				kMouseAxisY,
				kCount
		};
}

namespace GAMEPAD_BUTTON
{
		enum E
		{
				kPadButtonStart,
				kPadButtonSelect,
				kPadButtonLeft,
				kPadButtonRight,
				kPadButtonUp,
				kPadButtonDown,
				kPadButtonA, // Cross
				kPadButtonB, // Circle
				kPadButtonX, // Square
				kPadButtonY, // Triangle
				kPadButtonL1,
				kPadButtonR1,
				kPadButtonL2,
				kPadButtonR2,
				kPadButtonL3, // Left thumb
				kPadButtonR3, // Right thumb
				kPadButtonHome, // PS button
				kPadButton17,
				kPadButton18,
				kPadButton19,
				kPadButton20,
				kPadButton21,
				kPadButton22,
				kPadButton23,
				kPadButton24,
				kPadButton25,
				kPadButton26,
				kPadButton27,
				kPadButton28,
				kPadButton29,
				kPadButton30,
				kPadButton31,
				kCount
		};
}

namespace GAMEPAD_AXIS
{
		enum E
		{
				kPadButtonLeftStickX,
				kPadButtonLeftStickY,
				kPadButtonRightStickX,
				kPadButtonRightStickY,
				kPadButtonAxis4, // L2/Left trigger
				kPadButtonAxis5, // R2/Right trigger
				kPadButtonAxis6,
				kPadButtonAxis7,
				kPadButtonAxis8,
				kPadButtonAxis9,
				kPadButtonAxis10,
				kPadButtonAxis11,
				kPadButtonAxis12,
				kPadButtonAxis13,
				kPadButtonAxis14,
				kPadButtonAxis15,
				kPadButtonAxis16,
				kPadButtonAxis17,
				kPadButtonAxis18,
				kPadButtonAxis19,
				kPadButtonAxis20,
				kPadButtonAxis21,
				kPadButtonAxis22,
				kPadButtonAxis23,
				kPadButtonAxis24,
				kPadButtonAxis25,
				kPadButtonAxis26,
				kPadButtonAxis27,
				kPadButtonAxis28,
				kPadButtonAxis29,
				kPadButtonAxis30,
				kPadButtonAxis31,
				kPadButtonAccelerationX,
				kPadButtonAccelerationY,
				kPadButtonAccelerationZ,
				kPadButtonGravityX,
				kPadButtonGravityY,
				kPadButtonGravityZ,
				kPadButtonGyroscopeX,
				kPadButtonGyroscopeY,
				kPadButtonGyroscopeZ,
				kPadButtonMagneticFieldX,
				kPadButtonMagneticFieldY,
				kPadButtonMagneticFieldZ,
				kCount,
		};
}


class INPUT_EXPORT InputAPI
{
 public:
	 InputAPI() {};

		virtual
		~InputAPI() {};

		/**
			* @brief Initializes input system's devices, maps buttons and axis and informs window size.
			* @param screenWidth width of the window.
			* @param screenHeight height of the window.
   * @param windowHandle handle of the window.
			* @return void.
			*/
		virtual void
		init(unsigned int /*screenWidth*/, unsigned int /*screenHeight*/, HWND /*windowHandle*/ = nullptr) {};

		/**
			* @brief Updates input system.
			* @return void.
			*/
		virtual void
		update() {};

		/**
			* @brief Handle messages(windows) or events(Linux).
			* @param msg message.
			* @return void.
			*/
#if defined ( __WIN32__ ) || defined(_WIN32)
		virtual void
		handleMessage(MSG msg) {};
#elif defined(__linux) || defined(__linux__) || defined(linux) || defined(LINUX)
		virtual void
				handleMessage(XEvent event) {};
#endif

		/**
			* @brief Informs to the input system about the new window size.
			* @param screenWidth width of the window.
			* @param screenHeight height of the window.
			* @return void.
			*/
		virtual void
		resize(unsigned int /*screenWidth*/, unsigned int /*screenHeight*/) {};

		/**
			* @brief Check if given key has been pressed one time.
			* @param key key to check.
			* @return true if is the first time the key has been pressed since last release.
			*/
		virtual bool
		getKeyDown(KEYBOARD::E /*key*/) { return false; };

		/**
			* @brief Check if given key is being pressed.
			* @param key key to check.
			* @return true if the key is currently pressed.
			*/
		virtual bool
		getKey(KEYBOARD::E /*key*/) { return false; };

		/**
			* @brief Check if given key has been released.
			* @param key key to check.
			* @return true if the key has been released.
			*/
		virtual bool
		getKeyUp(KEYBOARD::E /*key*/) { return false; };

		/**
			* @brief Check if given mouse button has been pressed one time.
			* @param mouseButton button to check.
			* @return true if is the first time the button has been pressed since last release.
			*/
		virtual bool
		getMouseButtonDown(MOUSE_BUTTON::E /*mouseButton*/) { return false; };

		/**
			* @brief Check if given mouse button is being pressed.
			* @param mouseButton mouse button to check.
			* @return true if the mouse button is currently pressed.
			*/
		virtual bool
		getMouseButton(MOUSE_BUTTON::E /*mouseButton*/) { return false; };

		/**
			* @brief Check if given mouse button has been released.
			* @param mouseButton mouse button to check.
			* @return true if the mouse button has been released.
			*/
		virtual bool
		getMouseButtonUp(MOUSE_BUTTON::E /*mouseButton*/) { return false; };

		/**
			* @brief gets normalize mouse position in window on given axis.
			* @param axis axis to get.
			* @return current float normalized position on given axis.
			*/
		virtual float
		getMouseAxis(MOUSE_AXIS::E /*axis*/) { return 0.0f; };

		/**
			* @brief gets cursor's position on X axis in the window.
			* @return position of cursor in window pixels.
			*/
		virtual unsigned int
		getCursorPositionX() { return 0; };

		/**
			* @brief gets cursor's position on Y axis in the window.
			* @return position of cursor in window pixels.
			*/
		virtual unsigned int
		getCursorPositionY() { return 0; };

		/**
			* @brief Check if given gamepad button has been pressed one time.
			* @param button button to check.
			* @return true if is the first time the gamepad button has been pressed since last release.
			*/
		virtual bool
		getGamepadButtonDown(GAMEPAD_BUTTON::E /*button*/) { return false; };

		/**
			* @brief Check if given gamepad button is being pressed.
			* @param button gamepad button to check.
			* @return true if the gamepad button is currently pressed.
			*/
		virtual bool
		getGamepadButton(GAMEPAD_BUTTON::E /*button*/) { return false; };

		/**
			* @brief Check if given gamepad button has been released.
			* @param button gamepad button to check.
			* @return true if the gamepad button has been released.
			*/
		virtual bool
		getGamepadButtonUp(GAMEPAD_BUTTON::E /*button*/) { return false; };

		/**
			* @brief get axis value of given axis of the gamepad.
			* @param axis axis to get value.
			* @return current float value of the axis.
			*/
		virtual float
		getGamepadAxis(GAMEPAD_AXIS::E /*axis*/) { return 0.0f; };

 protected:

 
 private:

};
