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
		class JD_CORE_EXPORT CRenderModel : public Component
		{
    friend class SceneGraph;
    friend class GameObject;
		 public:
				CRenderModel () = default;
    ~CRenderModel() {};

    void
    setModel(WeakSptr<Model> model){
      m_model = model.lock();
      changeModel((uint32)m_model->m_meshes.size());
      m_animationOption = 0;
      m_currentAnimation = nullptr;
      m_animatedTime = 0.0f;
    }

    void
    Update(const float& deltatime);

    void
    draw();

    void
    changeModel(uint32 numMeshNewMode) {
      m_meshBones.clear();
      m_meshBones.resize(numMeshNewMode);
    };

    void
    noneAnimation() {
      m_meshBones.clear();
      if (nullptr != m_model)
      {
        m_meshBones.resize(m_model->m_meshes.size());
      }
    }

    SPtr<Model> m_model = nullptr;
    SPtr<AnimationsData> m_currentAnimation = nullptr;
    float m_animatedTime = 0.0f;
    uint32 m_animationOption = 0;
    bool m_playAnimation = true;
		 private:
    Vector<cbBonesTranform> m_meshBones;
		};
}
