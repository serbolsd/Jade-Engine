/*****************************************************************************/
/**
 * @file    jdD3D11GeometryShader.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    01/FEB/2021
 * @brief   Virtual Geometry Shader class for direcrt3d 11 Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/
#pragma once

#include "jdD3D11Prerequisites.h"
#include <jdGeometryShader.h>


namespace jdEngineSDK {
  class D3D11GeometryShader : public GeometryShader {
   public:
    /**
     * @brief Default constructor
     */
    D3D11GeometryShader() = default;

    /**
     * @brief Default destructor
     */
    ~D3D11GeometryShader() { release(); };

    /**
     * @brief virtual function to release
     */
    void
    release() override {
      SAFE_RELEASE(m_pdxGS);
      SAFE_RELEASE(m_pPBlob);
    }
  protected:
    friend class GraphicApi;
    friend class DirectX11Api;

    /**
     * @brief pointer to direct 11 Geometry shader class
     */
    ID3D11GeometryShader* m_pdxGS = nullptr;

    /**
     * @brief pointer to direct 11 Blob class
     */
    ID3D10Blob* m_pPBlob = nullptr;
  };
}
