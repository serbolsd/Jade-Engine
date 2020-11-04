/*****************************************************************************/
/**
 * @file    jdSampler.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    26/JUN/2020
 * @brief   Virtual Sampler class for any Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once

#include "jdPrerequisitesCore.h"

namespace jdEngineSDK {
  class Sampler
  {
   public:
    /**
     * @brief Default constructor
     */
    Sampler() = default;

    /**
     * @brief virtual destructor
     */
    virtual ~Sampler() {};

    /**
     * @brief virtual function to release
     */
    virtual void
    release() {};
  };
}
