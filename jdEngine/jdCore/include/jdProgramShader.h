/*****************************************************************************/
/**
 * @file    jdProgramShader.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    26/JUN/2020
 * @brief   Virtual ProgramShader class for any 
 *          Graphic Api with a pixel and vertex shader 
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once

#include "jdPrerequisitesCore.h"

#include "jdPixelShader.h"
#include "jdVertexShader.h"

namespace jdEngineSDK {
  class ProgramShader
  {
   public:
   /**
    * @brief Default constructor
    */
				ProgramShader() = default;
    /**
     * @brief virtual destructor
     */
    virtual ~ProgramShader() {};

    /**
     * @brief virtual fnction to get shared_ptr to Pixel shader
     * @return a shared_ptr to Pixel shader
     */
    virtual SPtr<PixelShader>
    getPixelShader() { return nullptr; };

    /**
     * @brief virtual fnction to get shared_ptr to Vertex shader
     * @return a shared_ptr to Vertex shader
     */
    virtual SPtr<VertexShader>
    getVertexShader() { return nullptr; };

    /**
     * @brief virtual function to release
     */
    virtual void
    release() {};
		
  };

}
