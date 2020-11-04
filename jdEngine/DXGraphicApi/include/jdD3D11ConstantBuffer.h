/*****************************************************************************/
/**
 * @file    jdD3D11ContantBuffer.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    8/JUL/2020
 * @brief   ContantBuffer class for Direct11 Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once

#include "jdD3D11Prerequisites.h"

#include "jdConstantBuffer.h"

namespace jdEngineSDK {
  class D3D11ConstantBuffer : public ConstantBuffer
  {
   public:
    /**
     * @brief Default constructor
     */
     D3D11ConstantBuffer() = default;

    /**
     * @brief virtual destructor
     */
    ~D3D11ConstantBuffer() { release(); };

    /**
     * @brief virtual function to release
     */
    void
    release() { 
      SAFE_RELEASE(m_pBuffer); 
    };

   protected:
    friend class GraphicApi;
    friend class DirectX11Api;

    /**
     * @brief pointer to direct 11 Buffer class
     */
    ID3D11Buffer* m_pBuffer = nullptr;
  };
}
