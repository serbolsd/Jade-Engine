/*****************************************************************************/
/**
 * @file    jdRTTIPrerequisites.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    24/02/2021
 * @brief   Run-time type information utilities
 *
 * @bug	    No known bugs.
 */
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/**
 * Includes
 */
/*****************************************************************************/
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

#include "jdPlatformTypes.h"

namespace jdEngineSDK {
  namespace SERIALIZE_ID {
    /**
     * Unique id for the serializable type.
     */
    enum E {
      kID = 0
    };
  }


  namespace SERIALIZE_DYM_SIZE {
    /**
     * 0 (Object has static size less than 255 bytes, e.g. int)
     * 1 (Dynamic size with no size restriction, e.g. string)
     */
    enum E {
      kHasDynamicSize = 0
    };
  }

#define JD_ALLOW_MEMCPY_SERIALIZATION(type)                                   \
  static_assert(std::is_trivially_copyable<type>() == true,                   \
                #type " is not trivially copyable");                          \
  template<> struct RTTIPlainType<type> {                                     \
    enum {kID=0}; enum {kHasDynamicSize = 0};                                 \
    static void                                                               \
    toMemory(const type& data, char* memory) {                                \
      memcpy(memory, &data, sizeof(type));                                    \
    }                                                                         \
    static uint32                                                             \
    fromMemory(type& data, char* memory) {                                    \
      memcpy(&data, memory, sizeof(type));                                    \
      return static_cast<uint32>(sizeof(type));                               \
    }                                                                         \
    static uint32                                                             \
    getDynamicSize(const type&) {                                             \
      return static_cast<uint32>(sizeof(type));                               \
    }                                                                         \
  };

}