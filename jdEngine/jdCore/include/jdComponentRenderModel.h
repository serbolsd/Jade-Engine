/*****************************************************************************/
/**
 * @file    jdComponentRenderModel.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    07/AGO/2020
 * @brief   Class for render model component
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once
#include "jdPrerequisitesCore.h"

#include "jdComponent.h"
#include "jdModel.h"

namespace jdEngineSDK {
		class CRenderModel : public Component
		{
    friend class SceneGraph;
    friend class GameObject;
		 public:
				CRenderModel () = default;
    ~CRenderModel() {};

    void
    setModel(WeakSptr<Model> model){
      m_model = model.lock();
    }

    void
    Update(const float& deltatime);

    void
    draw();

    SPtr<Model> m_model = nullptr;
		 private:
     float m_animatedTime = 0.0f;
		};
}
