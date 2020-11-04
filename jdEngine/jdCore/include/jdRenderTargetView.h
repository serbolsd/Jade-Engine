/*****************************************************************************/
/**
 * @file    jdRenderTargetView.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    02/JUN/2020
 * @brief   Virtual Rendet Target View class for any Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once
#include "jdPrerequisitesCore.h"
#include "jdRenderTarget.h"


namespace jdEngineSDK {
  class RenderTargetView : public RenderTarget
  {
   public:
    /**
     * @brief Default constructor
     */
    RenderTargetView() = default;
    
    /**
     * @brief virtual destructor
     */
    virtual ~RenderTargetView() {};
  };
}
