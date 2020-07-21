/*****************************************************************************/
/**
 * @file    Direct11DeviceContext.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    26/JUN/2020
 * @brief   override class of device context
 *
 *
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/
#pragma once
#include "jdD3D11Prerequisites.h"

#include <jdModulo.h>
#include <jdGraphicApi.h>


using namespace jdEngineSDK;

namespace jdEngineSDK {

  /**
   * @brief Struct for the info to create Device
   */
  struct sDeviceDescritor;

  class D3D11DeviceContext : public DeviceContext
  {
   public:
    /**
     * @brief Default constructor
     */
    D3D11DeviceContext() = default;

    /**
     * @brief Default destructor
     */
    ~D3D11DeviceContext() { SAFE_RELEASE(m_pd3dDeviceContext); };

   protected:
    friend class GraphicApi;
    friend class DirectX11Api;

    /**
     * @brief pointer to interface direct 11 device context class
     */
    ID3D11DeviceContext* m_pd3dDeviceContext = nullptr;
  };
}