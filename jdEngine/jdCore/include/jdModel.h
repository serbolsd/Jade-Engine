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
    String m_name;

    /** 
     * @briefThe transformation relative to the node's parent. 
     */
    JDMatrix4 m_transform;

    /** 
     *@brief Parent node. NULL if this node is the root node. 
     */
    SPtr<ModelNodes> m_parent = nullptr;

    /** 
     * @brief The number of child nodes of this node. 
     */
    uint32 m_numChildren;

    /** 
     * @briefThe child nodes of this node. NULL if mNumChildren is 0. 
     */
    Vector<SPtr<ModelNodes>>  m_children;

    /** 
     * @briefThe number of meshes of this node. 
     */
    uint32 m_numMeshes;
  };

  struct ScalingKey {
    JDVector3 m_value;
    float m_time;
  };

  struct RotationKey {
    /** The value of this key */
    Quaternion m_value;

    /** The time of this key */
    float m_time;
  };

  struct PositionKey {
    /** The time of this key */
    float m_time;

    /** The value of this key */
    JDVector3 m_value;
  };

  struct AnimationNode {
    /** The name of the node affected by this animation. The node
   *  must exist and it must be unique.*/
    String m_nodeName = "";

    /** The number of scaling keys */
    uint32 m_numScalingKeys = 0;
    /** The scaling keys of this animation channel. Scalings are
     *  specified as 3D vector. The array is mNumScalingKeys in size.
     *
     * If there are scaling keys, there will also be at least one
     * position and one rotation key.*/
    Vector<ScalingKey> m_scalingKeys;

    /** The number of rotation keys */
    uint32 m_numRotationKeys = 0;

    /** The rotation keys of this animation channel. Rotations are
     *  given as quaternions,  which are 4D vectors. The array is
     *  mNumRotationKeys in size.
     *
     * If there are rotation keys, there will also be at least one
     * scaling and one position key. */
    Vector<RotationKey> m_rotationKeys;

    /** The number of position keys */
    uint32 m_numPositionKeys;

    /** The position keys of this animation channel. Positions are
     * specified as 3D vector. The array is mNumPositionKeys in size.
     *
     * If there are position keys, there will also be at least one
     * scaling and one rotation key.*/
    Vector<PositionKey> m_positionKeys;
  };

  struct AnimationsData {
    String m_name = "Animation";
    float m_duration = 0.0f;
    float m_ticks_per_second = 0.0f;
    /** The number of bone animation channels. Each channel affects
     *  a single node. */
    uint32 m_numChannels = 0;
    /** The node animation channels. Each channel affects a single node.
     *  The array is mNumChannels in size. */
    Vector<SPtr<AnimationNode>> m_channels;

  };

		class Model : public Resource
		{
		 public:
				Model() = default;
				~Model() {

      //checar el node model
    };

    void
    addMesh(WeakSptr<Mesh> mesh){
      SPtr<Mesh> toadd = mesh.lock();
      toadd->m_myModel = this;
      m_meshes.push_back(mesh.lock());
    };

    Vector<SPtr<Mesh>> m_meshes;
    Vector<char*> m_AnimationsList;
    Vector<SPtr<AnimationsData>> m_animations;
		 private:

    friend class Mesh;
    friend class ResourceManager;
    friend class CRenderModel;

    JDMatrix4 m_global_inverse_transform;
    SPtr<AnimationsData> m_currentAnimation = nullptr;
    SPtr<ModelNodes> m_animationNodes = nullptr;

    uint32 m_numAnimations = 0;
		};
}