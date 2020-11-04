/*****************************************************************************/
/**
 * @file    jdD3D11Sampler.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    8/JUN/2020
 * @brief   Sampler class for Direcet 11 Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once

#include "jdD3D11Prerequisites.h"

namespace jdEngineSDK {
  class D3D11Sampler : public Sampler
  {
   public:
    /**
     * @brief Default constructor
     */
     D3D11Sampler() = default;

    /**
     * @brief virtual destructor
     */
     virtual ~D3D11Sampler() { release(); };

    /**
     * @brief virtual function to release
     */
    virtual void
    release() {
      SAFE_RELEASE(m_pSampler);
    };
   protected:
     friend class GraphicApi;
     friend class DirectX11Api;

     /**
      * @brief pointer to direct 11 VertexShader class
      */
     ID3D11SamplerState* m_pSampler = nullptr;
  };
}
