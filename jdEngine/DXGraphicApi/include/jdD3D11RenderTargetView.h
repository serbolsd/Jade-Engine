/*****************************************************************************/
/**
 * @file    jdDirect11RenderTargetView.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    02/JUN/2020
 * @brief   Virtual Rendet Target class for any Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/
#pragma once
#include "jdD3D11Prerequisites.h"

#include <jdRenderTargetView.h>

#include "jdD3D11Texture2D.h"

using namespace jdEngineSDK;

namespace jdEngineSDK {

  class D3D11RenderTargetView : public D3D11RenderTarget, RenderTargetView
  {
   public:
    /**
     * @brief defaulr constructor
     */
    D3D11RenderTargetView() = default;

    /**
     * @brief destructor
     */
    ~D3D11RenderTargetView() { release();  };

    /**
     * @brief function to release
     */
    void*
    getRenderTexture(uint32 index = 0) override {
      if (index>= m_pRT.m_ppSRV.size() || 0 > index)
      {
        return m_pRT.m_ppSRV[0];
      }
      return m_pRT.m_ppSRV[index];
    };
    void
    release() override {
      m_pRT.release();
      SAFE_RELEASE(m_pDepthStencil);
    }

   protected:
    friend class GraphicApi;
    friend class DirectX11Api;
   private:

  };
}