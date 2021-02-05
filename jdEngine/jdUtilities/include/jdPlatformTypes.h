#pragma once

//includes

#include <cstdint> //inclusiones externas con<>
#include "jdPlatformDefines.h" //inclusiones de esta libreria ""

namespace jdEngineSDK {
  using std::uint8_t;
  using std::uint16_t;
  using std::uint32_t;
  using std::uint64_t;
  using std::int8_t;
  using std::int16_t;
  using std::int32_t;
  using std::int64_t;

/*****************************************************************************/
/**
 * Bsic unsigned tyes
 */
/*****************************************************************************/
  using uint8 = uint8_t;      //8-bit unsigned.
  using uint16 = uint16_t;    //16-bit unsigned.
  using uint32 = uint32_t;    //32-bit unsigned.
  using uint64 = uint64_t;    //64-bit unsigned.

/*****************************************************************************/
/**
 * Bsic signed tyes
 */
/*****************************************************************************/
  using int8 = int8_t;      //8-bit signed.
  using int16 = int16_t;    //16-bit signed.
  using int32 = int32_t;    //32-bit signed.
  using int64 = int64_t;    //64-bit signed.

/*****************************************************************************/
/**
 * Character Types
 */
/*****************************************************************************/
#if JD_COMPILER == JD_COMPILER_MSVC
  using WCHAR = wchar_t;  //wide character used by visual studio
#else 
  using WCHAR = unsigned short;
#endif
  using ANSICHAR = char;  //ANSI character type
  using UNICHAR = WCHAR;  //UNICODE character type

  using BYTE = char;
  using UCHAR = unsigned char;
/*****************************************************************************/
/**
 * NULL data type
 */
/*****************************************************************************/
  using TYPE_OF_NULL = int32;
  
/*****************************************************************************/
/**
 * NULL data type
 */
/*****************************************************************************/
  using SIZE_T = size_t;
}
