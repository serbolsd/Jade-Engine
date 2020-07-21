/*****************************************************************************/
/**
 * @file    jdD3D11PixelShader.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    26/JUN/2020
 * @brief   override class of PixelShader for Direct11
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once

#include "jdD3D11Prerequisites.h"

#include <jdPixelShader.h>

namespace jdEngineSDK {


  class D3D11PixelShader : public PixelShader
  {
   public:
    /**
     * @brief Default constructor
     */
    D3D11PixelShader() = default;

    /**
     * @brief Default destructor
     */
    ~D3D11PixelShader() { release(); };

    /**
     * @brief virtual function to release
     */
    void
    release() override  { 
      SAFE_RELEASE(m_pdxPS);
      SAFE_RELEASE(m_pPBlob); 
    };

   protected:
    friend class GraphicApi;
    friend class DirectX11Api;

    /**
     * @brief pointer to direct 11 pixel shader class
     */
    ID3D11PixelShader* m_pdxPS = nullptr;

    /**
     * @brief pointer to direct 11 Blob class
     */
    ID3D10Blob* m_pPBlob = nullptr;
  };
}
