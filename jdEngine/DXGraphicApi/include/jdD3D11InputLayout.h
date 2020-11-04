/*****************************************************************************/
/**
 * @file    jdD3D11InputLayout.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    9/JUL/2020
 * @brief   InputLayout class for Direct 11 Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once

#include "jdD3D11Prerequisites.h"
#include <jdInputLayout.h>

namespace jdEngineSDK {
  class D3D11InputLayout : public InputLayout
  {
   public:
    /**
     * @brief Default constructor
     */
    D3D11InputLayout() = default;

    /**
     * @brief destructor
     */
    virtual ~D3D11InputLayout() { release(); };

    /**
     * @brief function to release
     */
    void
    release() override {
      SAFE_RELEASE(m_pdxInputLayout);
    };
   protected:
    friend class GraphicApi;
    friend class DirectX11Api;

    /**
     * @brief pointer to direct 11 InputLayout class
     */
    ID3D11InputLayout* m_pdxInputLayout = nullptr;
  };
}
