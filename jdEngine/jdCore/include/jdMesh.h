/*****************************************************************************/
/**
 * @file    jdMesh.h
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

#include <jdVector4.h>
#include <jdMatrix4.h>
#include <assimp/scene.h>

#include "jdComponent.h"
#include "jdResource.h"
#include "jdVertexBuffer.h"
#include "jdIndexBuffer.h"

namespace jdEngineSDK {

  class Model;

  struct cbBonesTranform
  {
    JDMatrix4 boneTransform[100];
  };

  struct DefaultVertexData {
    JDVector4 Pos = {0, 0, 0, 1};
    JDVector4 Norm = { 0, 0, 0, 0 };
    JDVector4 Tang = { 0, 0, 0, 0 };
    JDVector2 TexCoord = { 0, 0};
    JDVector4 Color = { 0, 0, 0, 0 };
    JDVector4 boneIDs = { 0,0,0,0 };
    JDVector4 boneWeights = {  0, 0, 0, 0 };
  };

  struct Bone {
    JDMatrix4 offset;
    JDMatrix4 Transform;
  };

  struct SkeletalMesh {
    uint32 m_numBones = 0;
    Map<String, int32> m_bonesMap;
    Vector<Bone> m_bonesInfo;
  };

  struct BoneMatrix {
    JDMatrix4 offset_matrix;
    JDMatrix4 final_world_transform;
  };

  class Texture2D;
		class Mesh
		{
		 public:
				Mesh() = default;
    ~Mesh() {};

    void
    setIndex(WeakSptr<uint32> index) {
      m_index = index.lock();
    }

    void
    setIndexNum(uint32 num) {
      m_numIndex = num;
    };

    uint32
    getIndexNum() {
      return m_numIndex;
    };

    void
    setVertex(WeakSptr<void> vertex) {
      m_vertex = vertex.lock();
    }

    void
    setNormals(WeakSptr<void> normals) {
      m_normals = normals.lock();
    }

    void
    setTangets(WeakSptr<void> tangents) {
      m_tangets = tangents.lock();
    }

    void
    setBiTangets(WeakSptr<void> bitangets) { 
      m_biTangets = bitangets.lock();
    }

    void
    setVertexBuffer(WeakSptr<VertexBuffer> vertexBuffer) {
      m_vertexB = vertexBuffer.lock();
    }

    void
    setIndexBuffer(WeakSptr<IndexBuffer> indexBuffer) {
      m_indexB = indexBuffer.lock();
    }

    void
    setVertexData(WeakSptr<DefaultVertexData> vertexData){
      m_vertexData = vertexData.lock();
    };

    SPtr<VertexBuffer>
    getVertexBuffer() {
      return m_vertexB;
    }
      
    SPtr<IndexBuffer> 
    getIndenBuffer(){
      return m_indexB;
    }

    void
    setAlbedoTexture(WeakSptr<Texture2D> albedo) {
      m_bAlbedo = true;
      m_txAlbedo = albedo.lock();
    };

    SPtr<Texture2D>
    getAlbedoTexture(){
      return m_txAlbedo;
    }
    
    void
    setNormalTexture(WeakSptr<Texture2D> normal) {
      m_bNormal = true;
      m_txNormal = normal.lock();
    };

    SPtr<Texture2D>
    getNormalTexture(){
      return m_txNormal;
    }

    void
    setSpecularTexture(WeakSptr<Texture2D> spc) {
      m_bSpecular = true;
      m_txSpecular = spc.lock();
    };

    SPtr<Texture2D>
    getSpecularTexture(){
      return m_txSpecular;
    }

    void
    setMetalnessTexture(WeakSptr<Texture2D> metal) {
      m_bMetalness = true;
      m_txMetalness = metal.lock();
    };

    SPtr<Texture2D>
    getMetalnessTexture(){
      return m_txMetalness;
    }

    void
    setRoughnessTexture(WeakSptr<Texture2D> Rough) {
      m_bRoughness = true;
      m_txRoughness = Rough.lock();
    };

    SPtr<Texture2D>
    getRoughnessTexture(){
      return m_txRoughness;
    }

    bool
    hasAlbedo() const {
      return m_bAlbedo;
    }
    
    bool
    hasNormal() const {
      return m_bNormal;
    }

    bool
    hasSpecular() const {
      return m_bSpecular;
    }

    bool
    hasMetalness() const {
      return m_bMetalness;
    }

    bool
    hasRoughness() const {
      return m_bRoughness;
    }

    void 
    setName(const String& name) {
      m_name = name;
    }

    String
    getName() const{
      return m_name;
    };

    uint32 m_albedoOption = 0;
    uint32 m_NormalOption = 0;
    uint32 m_specularOption = 0;
    uint32 m_MetalOption = 0;
    uint32 m_roughOption = 0;

    void
    animated(const float& animtaionTime);

    cbBonesTranform&
    getBonesTransform()
    {
      return m_cbBonesTranform;
    }

		 private:

    void 
    boneTransform(const float& deltaTime);

    void
    readNodeHierarchy(const float& p_animation_time, 
                      const aiNode* p_node, 
                      const JDMatrix4& parent_transform);

    const aiNodeAnim* 
    findNodeAnim(const aiAnimation* p_animation, const String& p_node_name);

    JDVector3
    calcInterpolatedScaling(const float& p_animation_time, const aiNodeAnim* p_node_anim);

    uint32
    findScaling(const float& p_animation_time, const aiNodeAnim* p_node_anim);

    JDMatrix4
    calcInterpolatedRotation(const float& p_animation_time, const aiNodeAnim* p_node_anim);

    uint32
    findRotation(const float& p_animation_time, const aiNodeAnim* p_node_anim);

    JDVector3
    calcInterpolatedPosition(const float& p_animation_time, const aiNodeAnim* p_node_anim);

    uint32
    findPosition(const float& p_animation_time, const aiNodeAnim* p_node_anim);

    JDMatrix4
    nlerp(aiQuaternion a, aiQuaternion b, const float& blend);

    SPtr<uint32> m_index = nullptr;
    uint32 m_numIndex = 0;
    SPtr<void> m_vertex = nullptr;
    SPtr<void> m_normals = nullptr;
    SPtr<void> m_tangets = nullptr;
    SPtr<void> m_biTangets = nullptr;

    SPtr<DefaultVertexData> m_vertexData = nullptr;
    SPtr<SkeletalMesh> m_skeletalSData = nullptr;

    SPtr<VertexBuffer> m_vertexB = nullptr;
    SPtr<IndexBuffer> m_indexB = nullptr;

    bool m_bAlbedo = true;
    bool m_bNormal = true;
    bool m_bSpecular = true;
    bool m_bMetalness = true;
    bool m_bRoughness = true;

    SPtr<Texture2D> m_txAlbedo = nullptr;
    SPtr<Texture2D> m_txNormal = nullptr;
    SPtr<Texture2D> m_txSpecular = nullptr;
    SPtr<Texture2D> m_txMetalness = nullptr;
    SPtr<Texture2D> m_txRoughness = nullptr;

    String m_name = "Mesh";

    Model* m_myModel;
    Vector<JDMatrix4> m_bonesTransforms;
    Vector<BoneMatrix> m_bone_matrices;
    cbBonesTranform m_cbBonesTranform;

    friend class ResourceManager;
    friend class CRenderModel;
    friend class Model;
		};
}
