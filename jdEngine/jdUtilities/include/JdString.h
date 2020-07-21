#pragma once

/*****************************************************************************/
/**
 * Includes
 */
/*****************************************************************************/
#include "jdPlatformDefines.h"
#include <string>

namespace jdEngineSDK {
  using std::char_traits;
  using std::basic_string;
  using std::basic_stringstream;

  template<typename T>
  using BasicString = basic_string<T, char_traits<T>, std::allocator<T>>;

  template<typename T>
  using BasicStringStream = basic_stringstream<T, char_traits<T>, std::allocator<T>>;

  /**
   * @brief Wide string used primarily for handling ASCII text
   * Cuando estemos hablando de algo que venga del systema operativo, 
   * por que no sabemos cual idima es
   * Cualquier funcion que necesite un WString es const WString&
   */

  using WString = BasicString<UNICHAR>;

  /**
   * @brief Narrow string used primarily for handling ASCII text
   * Cualquier cosa que sea texto dentro del motor
   * Cualquier funcion que necesite un string es const String&
   * Tambien con cualquier cosa con mayor a 4 bytes
   */
  using String = BasicString<ANSICHAR>;

  using WStringStream = BasicStringStream<UNICHAR>;

  using StringStream = BasicStringStream<ANSICHAR>;
}