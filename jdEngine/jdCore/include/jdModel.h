/*****************************************************************************/
/**
 * @file    jdModel.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    03/AGO/2020
 * @brief   class for resource model Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once
#include "jdPrerequisitesCore.h"

#include <assimp/scene.h>

#include "jdResource.h"
#include "jdMesh.h"

namespace jdEngineSDK {

  struct AnimationsData {
    String name = "Animation";
    float ticks_per_second = 0.0f;
  };

		class Model : public Resource
		{
		 public:
				Model() = default;
				~Model() {};

    void
    addMesh(WeakSptr<Mesh> mesh){
      SPtr<Mesh> toadd = mesh.lock();
      toadd->m_myModel = this;
      m_meshes.push_back(mesh.lock());
    };

    Vector<SPtr<Mesh>> m_meshes;

		 private:

    friend class Mesh;
    friend class ResourceManager;
    friend class CRenderModel;

    JDMatrix4 m_global_inverse_transform;
    Vector<SPtr<AnimationsData>> m_animations;
    SPtr<AnimationsData> m_currentAnimation = nullptr;

    const aiScene* m_scene;
		};
}