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

#include "jdResource.h"
#include "jdMesh.h"
#include "jdQuaternion.h"

namespace jdEngineSDK {

  struct ModelNodes {
    /** 
     * @brief The name of the node.
     */
    String mName;

    /** 
     * @briefThe transformation relative to the node's parent. 
     */
    JDMatrix4 transformation;

    /** 
     *@brief Parent node. NULL if this node is the root node. 
     */
    SPtr<ModelNodes> parent = nullptr;

    /** 
     * @brief The number of child nodes of this node. 
     */
    uint32 numChildren;

    /** 
     * @briefThe child nodes of this node. NULL if mNumChildren is 0. 
     */
    Vector<SPtr<ModelNodes>>  children;

    /** 
     * @briefThe number of meshes of this node. 
     */
    uint32 numMeshes;
  };

  struct ScalingKey {
    JDVector3 value;
    float time;
  };

  struct RotationKey {
    /** The value of this key */
    Quaternion Value;

    /** The time of this key */
    float mTime;
  };

  struct PositionKey {
    /** The time of this key */
    float mTime;

    /** The value of this key */
    JDVector3 mValue;
  };

  struct AnimationNode {
    /** The name of the node affected by this animation. The node
   *  must exist and it must be unique.*/
    String mNodeName = "";

    /** The number of scaling keys */
    uint32 mNumScalingKeys = 0;
    /** The scaling keys of this animation channel. Scalings are
     *  specified as 3D vector. The array is mNumScalingKeys in size.
     *
     * If there are scaling keys, there will also be at least one
     * position and one rotation key.*/
    Vector<ScalingKey> m_ScalingKeys;

    /** The number of rotation keys */
    uint32 m_NumRotationKeys = 0;

    /** The rotation keys of this animation channel. Rotations are
     *  given as quaternions,  which are 4D vectors. The array is
     *  mNumRotationKeys in size.
     *
     * If there are rotation keys, there will also be at least one
     * scaling and one position key. */
    Vector<RotationKey> m_RotationKeys;

    /** The number of position keys */
    uint32 m_NumPositionKeys;

    /** The position keys of this animation channel. Positions are
     * specified as 3D vector. The array is mNumPositionKeys in size.
     *
     * If there are position keys, there will also be at least one
     * scaling and one rotation key.*/
    Vector<PositionKey> m_PositionKeys;
  };

  struct AnimationsData {
    String name = "Animation";
    float duration = 0.0f;
    float ticks_per_second = 0.0f;
    /** The number of bone animation channels. Each channel affects
     *  a single node. */
    uint32 numChannels = 0;
    /** The node animation channels. Each channel affects a single node.
     *  The array is mNumChannels in size. */
    Vector<SPtr<AnimationNode>> channels;

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
    SPtr<ModelNodes> m_animationNodes = nullptr;
		};
}