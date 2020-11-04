#include "jdGameObject.h"
#include "jdComponent.h"
#include "jdComponentRenderModel.h"
#include "jdComponentTransform.h"
#include "jdComponentLight.h"
#include "jdCameraManager.h"

namespace jdEngineSDK {
  SPtr<Component>
  GameObject::addComponent(COMPONENT_TYPE::E componentType, SPtr<Component> component) {
    //auto newComp = component.lock();
    //auto compT = newComp.get()->getType();
    auto comp = m_components.find(componentType);
    if (comp != m_components.end())
    {
      //Component alredy exist in this object
      return nullptr;
    }
    SPtr<Component> compo = component;
    if (nullptr != compo)
    {
      compo->setGameObject(this);
      m_components.insert(std::pair <COMPONENT_TYPE::E, SPtr<Component>>(componentType, 
                                                                         compo));
      return compo;
    }
    Component* addComponent = nullptr;
    switch (componentType)
    {
     case jdEngineSDK::COMPONENT_TYPE::TRANSFORM:
      addComponent = new CTransform;
      break;
     case jdEngineSDK::COMPONENT_TYPE::RENDERMODEL:
       addComponent = new CRenderModel;
      break;
     case jdEngineSDK::COMPONENT_TYPE::TEXTURE:
      break;
     case jdEngineSDK::COMPONENT_TYPE::BOXCOLLIDER:
      break;
     case jdEngineSDK::COMPONENT_TYPE::SPHERECOLLIDRE:
      break;
     case jdEngineSDK::COMPONENT_TYPE::SOUND:
      break;
     case jdEngineSDK::COMPONENT_TYPE::SPRITE:
      break;
     case jdEngineSDK::COMPONENT_TYPE::CAMERA:
       addComponent = new Camera();
       break;
     case jdEngineSDK::COMPONENT_TYPE::LIGHT:
       addComponent = new CLight;
       break;
     case jdEngineSDK::COMPONENT_TYPE::KNUMCOMPONENTS:
      break;
     default:
       return nullptr;
      break;
    }
    if (nullptr == addComponent)
    {
      return nullptr;
    }
    addComponent->setGameObject(this);
    SPtr<Component> newComp(addComponent);
    m_components.insert(std::pair <COMPONENT_TYPE::E, SPtr<Component>>(componentType, 
                                                                       newComp));
    return newComp;
  }
  //template<typename T>
  //inline SPtr<T> GameObject::GetComponent()
  //{
  //  for (auto component : m_components)
  //  {
  //    if (dynamic_cast<SPtr<T>>(component.second))
  //    {
  //      return component.second
  //    }
  //  }
  //  //for Component* c in m_components)
  //  //if (dynamic_cast<T>(*c))
  //  //  return (T*)c;
  //  //return nullptr;
  //}

  SPtr<Component> 
  GameObject::getComponent(COMPONENT_TYPE::E componentType) {
    auto component = m_components.find(componentType);
      if (component != m_components.end())
      {
        return component->second;
      }
    return nullptr;
  }

  void
  jdEngineSDK::GameObject::removeComponent(COMPONENT_TYPE::E componentType) {
    auto it = m_components.find(componentType);
    if (it != m_components.end())
    {
      m_components.erase(componentType);
    }
  }

  void
  GameObject::Update(const float& deltaTime) {
    for (auto compo : m_components)
    {
      compo.second.get()->Update(deltaTime);
    }
    //auto transform = m_components.find(COMPONENT_TYPE::TRANSFORM);
    //if (transform != m_components.end())
    //{
    //  transform->second.get()->Update(deltaTime);
    //}
    //auto renderModel = m_components.find(COMPONENT_TYPE::RENDERMODEL);
    //if (renderModel != m_components.end())
    //{
    //  renderModel->second.get()->Update(deltaTime);
    //}
  }

  void
  GameObject::draw() {
    auto render = m_components.find(COMPONENT_TYPE::RENDERMODEL);
    if (render != m_components.end())
    {
      auto model = reinterpret_cast<CRenderModel*>(render->second.get());
      model->draw();
    }
  }
}