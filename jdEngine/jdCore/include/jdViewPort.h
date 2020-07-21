/*****************************************************************************/
/**
 * @file    jdViewPort.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    26/JUN/2020
 * @brief   Virtual Device class for any Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once
#include "jdPrerequisitesCore.h"

namespace jdEngineSDK {
  struct ViewPort
  {
    float Width;
    float Height;
    float MinDepth;
    float MaxDepth;
    float m_topLeftX=0.0f;
    float m_topLeftY=0.0f;
  };
}
