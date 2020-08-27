/*****************************************************************************/
/**
 * @file    jdGameObject.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    03/AGO/2020
 * @brief   Virtual class for any Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/
#pragma once
#include "jdPrerequisitesCore.h"

#include <jdModulo.h>

#include "jdGameObject.h"

namespace jdEngineSDK {

		class JD_CORE_EXPORT SceneGraph : public Module<SceneGraph>
		{
   private:
    class CLayer 
    {
     public:
      bool 
      operator()(SPtr<GameObject> gA, SPtr<GameObject> gB) const {
        return (gA.get()->m_layer < gB.get()->m_layer);
      };
    };
		 public:
    SceneGraph() = default;
    ~SceneGraph() { release(); };

    void
    onStartUp();

    void
    onUpdate(const float& deltaTime);

    SPtr<GameObject>
    createGameObject();

    void 
    release() {};

    MultiSet<SPtr<GameObject>, CLayer> m_GObjects;

    SPtr<GameObject> selectedObjet = nullptr;
    SPtr<GameObject> m_root = nullptr;
	 	private:

		};
}