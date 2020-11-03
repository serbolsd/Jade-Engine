/*****************************************************************************/
/**
 * @file    jdRenderTarget.h
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
#include "jdPrerequisitesCore.h"

namespace jdEngineSDK {

  class RenderTarget
  {
   public:
    /**
     * @brief Default constructor
     */
    RenderTarget() : m_id(0) {};
    
    /**
     * @brief virtual destructor
     */
    virtual ~RenderTarget() {};
    
    /**
     * @brief virtual function to release the texture
     */
    virtual void
    release() {};

    virtual void*
    getRenderTexture(uint32 /*index*/ = 1) { return nullptr; };

    /**
     * @brief virtual function to get the render target ID
     * @return int32 with the render target ID
     */
    virtual int32
    getID() { return m_id; };

   protected:
     int32 m_id = 0;
     uint32 m_mipLeve=1;
  };
}
