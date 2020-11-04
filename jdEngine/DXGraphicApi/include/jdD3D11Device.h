/*****************************************************************************/
/**
 * @file    Direct11Device.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    26/JUN/2020
 * @brief   override class of device
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once
#include "jdD3D11Prerequisites.h"

#include <jdDevice.h>

using namespace jdEngineSDK;

namespace jdEngineSDK {

  /**
   * @brief Struct for the info to create Device
   */
  struct sDeviceDescritor {

  };

  class D3D11Device : public Device
  {
   public:
    /**
     * @brief Default constructor
     */
    D3D11Device() = default;

    /**
     * @brief Default destructor
     */
    ~D3D11Device() { SAFE_RELEASE(m_pd3dDevice); };

   protected:
    friend class GraphicApi;
    friend class DirectX11Api;

    /**
     * @brief pointer to interface direct 11 device class
     */
    ID3D11Device* m_pd3dDevice = nullptr;
  };
}
