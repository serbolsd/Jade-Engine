/*****************************************************************************/
/**
 * @file    jdRasterizeState.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    30/OCT/2020
 * @brief   Virtual Rasterizer State class for any Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/
#pragma once
#include "jdPrerequisitesCore.h"
#include "jdResource.h"

namespace jdEngineSDK {
  namespace RASTERIZER_FILL_MODE {
    enum E {
      D3D11_FILL_WIREFRAME = 2,
      D3D11_FILL_SOLID = 3
    };
  };

  namespace RASTERIZER_CULL_MODE {
    enum E {
      D3D11_CULL_NONE = 1,
      D3D11_CULL_FRONT = 2,
      D3D11_CULL_BACK = 3
    };
  };


		class RasterizeState	{
		 public:
    RasterizeState() = default;
				virtual ~RasterizeState() { release(); };

				virtual void
				release() {};
		 private:

		};
}