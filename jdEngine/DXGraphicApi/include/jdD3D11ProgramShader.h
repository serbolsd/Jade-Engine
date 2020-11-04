/*****************************************************************************/
/**
 * @file    jdD3D11ProgramShader.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    26/JUN/2020
 * @brief   override class of ProgramShader for Direct11
 *
 *
 *
 * @bug     can't set resources with functions to set vertx and pixel together
 */
 /*****************************************************************************/
#pragma once

#include "jdD3D11Prerequisites.h"

#include <jdD3D11VertexShader.h>
#include <jdD3D11PixelShader.h>
#include <jdProgramShader.h>

namespace jdEngineSDK {

  class D3D11ProgramShader : public ProgramShader
  {
  public:
    /**
     * @brief Default constructor
     */
    D3D11ProgramShader() {
      D3D11PixelShader* pixel = new D3D11PixelShader();
      m_PS.reset(pixel);
      D3D11VertexShader* vertex = new D3D11VertexShader();
      m_VS.reset(vertex);
    };

    /**
     * @brief Default destructor
     */
    ~D3D11ProgramShader() { release(); };

    /**
     * @brief get shared_ptr to pixel shader
     * @return a shared_ptr to pixel shader
     */
    SPtr<PixelShader>
    getPixelShader() override { 
      return SPtr<PixelShader>(m_PS, reinterpret_cast<PixelShader*>(m_VS.get()));
    };

    /**
     * @brief get shared_ptr to Vertex shader
     * @return a shared_ptr to Vertex shader
     */
    SPtr<VertexShader>
    getVertexShader() override { 
      return SPtr<VertexShader>(m_VS,reinterpret_cast<VertexShader*>(m_VS.get()));
    };

    /**
     * @brief function to release
     */
    void
    release() override {
      m_PS->release();
      m_VS->release();
    };

  protected:
    friend class GraphicApi;
    friend class DirectX11Api;

    /**
     * @brief Pixel Shader
     */
    SPtr<D3D11PixelShader> m_PS=nullptr;

    /**
     * @brief Vertex Shader
     */
    SPtr<D3D11VertexShader> m_VS=nullptr;
    
  };
}