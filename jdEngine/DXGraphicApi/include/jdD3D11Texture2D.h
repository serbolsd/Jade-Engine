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

#include <jdTexture2D.h>

using namespace jdEngineSDK;

namespace jdEngineSDK {

  class D3D11Texture2D : public Texture2D
  {
    friend class D3D11RenderTarget;
    friend class D3D11RenderTargetView;
   public:
    /**
     * @brief Default constructor
     */
    D3D11Texture2D() = default;

    /**
     * @brief Default destructor
     */
    ~D3D11Texture2D() { release(); };

    /**
     * @brief function to release
     */
    void
    release() override {
      SAFE_RELEASE(m_texture);
      SAFE_RELEASE(m_pRenderTarget);
      SAFE_RELEASE(m_textureForDepthStencil);
      for (size_t i = 0; i < m_ppSRV.size(); i++)
      {
        SAFE_RELEASE(m_ppSRV[i]);
      }
      for (size_t i = 0; i < m_ppUAV.size(); i++)
      {
        SAFE_RELEASE(m_ppUAV[i]);
      }
    };

   protected:
    friend class GraphicApi;
    friend class DirectX11Api;

    /**
     * @brief pointer to direct 11 texture2D class
     */
    ID3D11Texture2D* m_texture = nullptr;

    /**
     * @brief pointer to direct 11 texture2D class
     */
    ID3D11Texture2D* m_textureForDepthStencil = nullptr;

    /**
     * @brief pointer to direct 11 RenderTargetView class
     */
    ID3D11RenderTargetView* m_pRenderTarget=nullptr;

    /**
     * @brief vector of pointer to direct 11 ShaderResourceView class
     */
    Vector<ID3D11ShaderResourceView*> m_ppSRV;

    /**
     * @brief vector of pointer to direct 11 UnorderedAccessView class
     */
    Vector<ID3D11UnorderedAccessView*> m_ppUAV;
  };
}
