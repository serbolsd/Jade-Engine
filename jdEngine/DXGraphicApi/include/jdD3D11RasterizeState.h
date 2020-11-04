/*****************************************************************************/
/**
 * @file    jdD3D11RasterizeState.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    30/OCT/2020
 * @brief   Rasterizer State class for DirectX11 Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/
#pragma once
#include "jdD3D11Prerequisites.h"
#include <jdRasterizeState.h>

namespace jdEngineSDK {
  class JD_PLUGIN_EXPORT D3D11RasterizeState : public RasterizeState
  {
   public:
    D3D11RasterizeState() = default;
    virtual ~D3D11RasterizeState() { release(); };

    void
    release() override {
      SAFE_RELEASE(m_pRasterizeState);
    };
   private:
    ID3D11RasterizerState* m_pRasterizeState;

    friend class DirectX11Api;
  };
}