/*****************************************************************************/
/**
 * @file    jdDirect11RenderTarget.h
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

#include <jdRenderTarget.h>
#include "jdD3D11Texture2D.h"
using namespace jdEngineSDK;

namespace jdEngineSDK {

  class D3D11RenderTarget : public RenderTarget
  {
   public:
    /**
     * @brief Default constructor
     */
    D3D11RenderTarget() = default;

    /**
     * @brief destructor
     */
    ~D3D11RenderTarget() { release(); };

    /**
     * @brief function to release
     */
    void
    release() override {
      m_pRT.release();
      SAFE_RELEASE(m_pDepthStencil);
    }

    void*
    getRenderTexture(uint32 index = 0) override {
      if (index >= m_pRT.m_ppSRV.size() || 0 > index)
      {
        return m_pRT.m_ppSRV[0];
      }
      return m_pRT.m_ppSRV[index];
    };


   protected:
    friend class GraphicApi;
    friend class DirectX11Api;

    /**
     * @brief member direct 11 texture 2D
     */
    D3D11Texture2D m_pRT;

    /**
     * @brief pointer to direct 11 depth stencil class
     */
    ID3D11DepthStencilView* m_pDepthStencil = nullptr;
  };
}