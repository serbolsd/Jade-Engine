#include "jdComponentRenderModel.h"

#include "jdGraphicApi.h"

namespace jdEngineSDK {
  void 
  CRenderModel::Update(const float& deltatime) {
    if (m_model == nullptr)
    {
      m_animatedTime = 0.0f;
      return;
    }
    if (m_currentAnimation != nullptr)
    {
      if (m_playAnimation)
      {
        m_animatedTime += deltatime;
      }
      if (m_animatedTime >= m_currentAnimation->m_duration)
      {
        m_animatedTime = 0;
      }
      uint32 meshNum = 0;
      for (auto mesh : m_model.get()->m_meshes)
      {
        mesh->m_cbBonesTranform = &m_meshBones[meshNum];
        mesh->animated(m_animatedTime, m_currentAnimation);
        ++meshNum;
      }
    }
  }
  
  void
  CRenderModel::draw() {
    if (m_model == nullptr)
    {
      return;
    }
    uint32 meshNum = 0;
    for (auto mesh : m_model.get()->m_meshes)
    {
      //auto tranforms = mesh->getBonesTransform();

      g_graphicsApi().updateSubresource(g_graphicsApi().getConstantBufferBones(), 
                                        &m_meshBones[meshNum]);

      g_graphicsApi().PixelShaderSetShaderResources(mesh->getAlbedoTexture(), 0);
      g_graphicsApi().PixelShaderSetShaderResources(mesh->getNormalTexture(), 1);
      g_graphicsApi().PixelShaderSetShaderResources(mesh->getSpecularTexture(), 2);
      g_graphicsApi().PixelShaderSetShaderResources(mesh->getRoughnessTexture(), 3);

      g_graphicsApi().setVertexBuffer(mesh.get()->getVertexBuffer());
      g_graphicsApi().setIndexBuffer(mesh.get()->getIndenBuffer());

      g_graphicsApi().DrawIndex(mesh.get()->getIndexNum());
      ++meshNum;
    }
  }
}
