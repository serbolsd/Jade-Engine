#include "jdSceneGraph.h"
#include "jdComponentTransform.h"

namespace jdEngineSDK {
  void 
  SceneGraph::onStartUp()
  {
    GameObject* root = new GameObject;
    root->m_layer = 0;
    root->setName("Root");
    root->addComponent(COMPONENT_TYPE::TRANSFORM);
    root->selected = true;
    SPtr<GameObject> ROOT(root);
    m_root = ROOT;
    m_GObjects.insert(ROOT);
    selectedObjet = m_root;
  }

  void
  SceneGraph::onUpdate(const float& deltaTime) {
    selectedObjet = nullptr;
    for (auto obj : m_GObjects)
    {
      if (obj->selected)
      {
        selectedObjet = obj;
      }
      obj->Update(deltaTime);
    }
  }

  SPtr<GameObject>
  SceneGraph::createGameObject() {
    GameObject* newObject = new GameObject;
    newObject->m_layer = 0;
    newObject->setName("GameObject##");
    newObject->addComponent(COMPONENT_TYPE::TRANSFORM);
    SPtr<GameObject> NObj(newObject);
    if (nullptr == selectedObjet)
    {
      newObject->setParent(m_root);
      m_root->m_children.insert(NObj);
    }
    else
    {
      newObject->setParent(selectedObjet);
      selectedObjet->m_children.insert(NObj);
    }
    m_GObjects.insert(NObj);
    return NObj;
  }
}