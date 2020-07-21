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

  class D3D11SwapChain : public SwapChain
  {
   public:
    /**
     * @brief Default constructor
     */
    D3D11SwapChain() = default;

    /**
     * @brief Default destructor
     */
    ~D3D11SwapChain() { SAFE_RELEASE(m_pdxgSwapChain); };

   protected:
    friend class GraphicApi;
    friend class DirectX11Api;

    /**
     * @brief pointer to direct 11 swap chian class
     */
    IDXGISwapChain* m_pdxgSwapChain = nullptr;
  };
}