/*****************************************************************************/
/**
 * @file    jdD3D11IndexBuffer.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    8/JUL/2020
 * @brief   IndexBuffer class for Direcr11 Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/
#pragma once

#include "jdD3D11Prerequisites.h"

#include <jdIndexBuffer.h>

namespace jdEngineSDK {
  class D3D11IndexBuffer : public IndexBuffer
  {
   public:
    /**
     * @brief Default constructor
     */
    D3D11IndexBuffer() = default;

    /**
     * @brief virtual destructor
     */
    ~D3D11IndexBuffer() { release(); };

    /**
     * @brief virtual function to release
     */
    void
    release() override {
      SAFE_RELEASE(m_pIBuf);
    };

   protected:
    friend class GraphicApi;
    friend class DirectX11Api;

    /**
     * @brief pointer to direct 11 Buffer class
     */
    ID3D11Buffer* m_pIBuf = nullptr;

    /**
     * @brief size of the struc was made created the buffer
     */
    UINT sizeOfStruct;
  };
}
