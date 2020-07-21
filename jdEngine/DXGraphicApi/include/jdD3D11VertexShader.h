/*****************************************************************************/
/**
 * @file    jdD3D11VertexShader.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    26/JUN/2020
 * @brief   override class of VertexShader for Direct11
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once

#include "jdD3D11Prerequisites.h"

#include <jdVertexShader.h>

namespace jdEngineSDK {


  class D3D11VertexShader : public VertexShader
  {
  public:
    /**
     * @brief Default constructor
     */
    D3D11VertexShader() = default;

    /**
     * @brief Default destructor
     */
    ~D3D11VertexShader() { release(); };

    /**
     * @brief virtual function to release
     */
    void
    release() override  { 
      SAFE_RELEASE(m_pdxVS);
      SAFE_RELEASE(m_pVBlob);
    };

  protected:
    friend class GraphicApi;
    friend class DirectX11Api;

    /**
     * @brief pointer to direct 11 VertexShader class
     */
    ID3D11VertexShader* m_pdxVS = nullptr;

    /**
     * @brief pointer to direct 11 Blob class
     */
    ID3D10Blob* m_pVBlob = nullptr;
  };
}
