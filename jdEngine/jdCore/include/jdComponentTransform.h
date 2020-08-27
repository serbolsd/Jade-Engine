/*****************************************************************************/
/**
 * @file    jdComponentTransform.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    07/AGO/2020
 * @brief   Class for Transform component
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once
#include "jdPrerequisitesCore.h"

#include <jdVector4.h>
#include <jdMatrix4.h>

#include "jdComponent.h"

namespace jdEngineSDK {
  class GameObject;
		class JD_CORE_EXPORT CTransform : public Component
		{
    friend class GameObject;
		 public:
				CTransform() { m_type = COMPONENT_TYPE::TRANSFORM; };
    ~CTransform() {};
   
    void
    Update(const float& deltatime) override;

    JDMatrix4
    getMatrixTransform() const;

    JDVector3 position = { 0, 0, 0 };
    JDVector3 rotation = { 0, 0, 0 };
    JDVector3 scale = { 1, 1, 1 };
    JDMatrix4 matTransform;

    
   private:
		};
}
