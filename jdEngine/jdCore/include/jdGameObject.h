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

#include "jdComponent.h"

namespace jdEngineSDK {
  class JD_CORE_EXPORT GameObject
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
    friend class ResourceManager;
    friend class Component;
    friend class CTransform;
   public:
    GameObject() = default;
    ~GameObject() {};

    void
    setParent(WeakSptr<GameObject> parent){
      m_parent = parent.lock().get();
    }

    void
    addComponent(COMPONENT_TYPE::E componentType);

    /*template <typename T>
    SPtr<T> GetComponent();*/

    SPtr<Component>
    getComponent(COMPONENT_TYPE::E componentType);

    void
    removeComponent(COMPONENT_TYPE::E componentType);

    void
    setName(const String& name){
    m_name = name;
    }

    String&
    getName() {
      return m_name;
    }

    void
    setID(uint32 id){
      m_ID = id;
    }

    virtual void
    Update(const float& deltaTime);

    virtual void
    draw();

    uint32 m_layer = 0;

    bool selected = false;

    MultiSet<SPtr<GameObject>, CLayer> m_children;

    uint32 m_modelOption=0;
   protected:
    GameObject* m_parent = nullptr;
    Map<COMPONENT_TYPE::E, SPtr<Component>> m_components;

    String m_name = "";

    uint32 m_ID = 0;
  };

}
