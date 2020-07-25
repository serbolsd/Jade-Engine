/*****************************************************************************/
/**
 * @file    Device.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    26/JUN/2020
 * @brief   Virtual Device class for any Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once
#include "jdPrerequisitesCore.h"

namespace jdEngineSDK {

  /**
   * @brief Struct for the info to create Device
   */
  struct sDeviceDescritor;
  
  class  Device
  {
   public:
    /**
     * @brief Default constructor
     */
    Device() = default;

    /**
     * @brief Default destructor
     */
    virtual ~Device() {};

  };
}