#include "jdComponentRenderModel.h"

#include "jdGraphicApi.h"

namespace jdEngineSDK {
  void 
  CRenderModel::Update(const float& deltatime) {
    if (m_model == nullptr)
    {
      return;
    }
    if (m_model->m_currentAnimation != nullptr)
    {
      m_animatedTime += deltatime;
      for (auto mesh : m_model.get()->m_meshes)
      {
        mesh->animated(m_animatedTime);
      }
    }
  }
  
  void
  CRenderModel::draw() {
    if (m_model == nullptr)
    {
      return;
    }
    for (auto mesh : m_model.get()->m_meshes)
    {
      auto tranforms = mesh->getBonesTransform();

      g_graphicsApi().updateSubresource(g_graphicsApi().getConstantBufferBones(), &tranforms);

      g_graphicsApi().PixelShaderSetShaderResources(mesh->getAlbedoTexture(), 0);

      g_graphicsApi().setVertexBuffer(mesh.get()->getVertexBuffer());
      g_graphicsApi().setIndexBuffer(mesh.get()->getIndenBuffer());

      g_graphicsApi().DrawIndex(mesh.get()->getIndexNum());
    }
  }
}
