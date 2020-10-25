#include "jdMesh.h"
#include "jdModel.h"
namespace jdEngineSDK {
  void 
  Mesh::animated(const float& animtaionTime, SPtr<AnimationsData> m_currentAnimation) {
    if (nullptr != m_currentAnimation) {

      boneTransform(animtaionTime, m_currentAnimation);
    }
  }

  void
  Mesh::boneTransform(const float& deltaTime, SPtr<AnimationsData> m_currentAnimation) {
  		JDMatrix4 identity_matrix; // = mat4(1.0f);
  		identity_matrix.identity();
  		
  		//float time_in_ticks = deltaTime * m_myModel->m_currentAnimation->m_ticks_per_second;
  		float time_in_ticks = deltaTime * m_currentAnimation->m_ticks_per_second;
  		//float animation_time = fmod(time_in_ticks, (float)m_myModel->m_scene->mAnimations[0]->mDuration);
  		//float animation_time = fmod(time_in_ticks, (float)m_myModel->m_currentAnimation->m_duration);
  		float animation_time = fmod(time_in_ticks, (float)m_currentAnimation->m_duration);
  
  		//readNodeHierarchy(animation_time, m_myModel->m_scene->mRootNodemRootNode, identity_matrix);
  		readNodeHierarchy(animation_time, m_myModel->m_animationNodes, identity_matrix, m_currentAnimation);
  
  }
  
  void 
  Mesh::readNodeHierarchy(const float& p_animation_time, 
                          WeakSptr<ModelNodes> p_node,
                          const JDMatrix4& parent_transform,
                          SPtr<AnimationsData> m_currentAnimation) {
    ModelNodes* animNode = p_node.lock().get();
  		String node_name(animNode->m_name);
  
  		//const aiAnimation* animation = m_myModel->m_scene->mAnimations[0];
    //m_myModel->m_currentAnimation;
  		JDMatrix4 node_transform;
    std::memcpy(&node_transform, &animNode->m_transform, sizeof(JDMatrix4));
  
  		//const AnimationNode* node_anim = findNodeAnim(m_myModel->m_currentAnimation.get(), node_name);
  		const AnimationNode* node_anim = findNodeAnim(m_currentAnimation.get(), node_name);
  
  		if (node_anim)
  		{
  
  				//scaling
  				JDVector3 scaling_vector = calcInterpolatedScaling(p_animation_time, node_anim);
  				JDMatrix4 scaling_matr;
      scaling_matr.scale(scaling_vector);
  
  				//rotation
  				JDMatrix4 rotate_matr = calcInterpolatedRotation(p_animation_time, node_anim);
  
  				//translation
  				JDVector3 translate_vector = calcInterpolatedPosition(p_animation_time, node_anim);
  				JDMatrix4 translate_matr;
      translate_matr.translate(translate_vector);
      node_transform = translate_matr * rotate_matr * scaling_matr;
  		}
  		JDMatrix4 global_transform = parent_transform * node_transform;
  
  		
  		if (m_skeletalSData->m_bonesMap.find(node_name) != m_skeletalSData->m_bonesMap.end()) {// true if node_name exist in bone_mapping	
  				uint32 bone_index = m_skeletalSData->m_bonesMap[node_name];
      //m_skeletalSData->m_bonesInfo[bone_index].Transform =
      m_cbBonesTranform->boneTransform[bone_index]=
        m_myModel->m_global_inverse_transform * 
        global_transform * 
        m_skeletalSData->m_bonesInfo[bone_index].offset;
  		}
  
  		for (uint32 i = 0; i < animNode->m_numChildren; ++i)	{
  				readNodeHierarchy(p_animation_time, animNode->m_children[i], global_transform, m_currentAnimation);
  		}
  
  }
  
  const AnimationNode*
  Mesh::findNodeAnim(AnimationsData* p_animation, const String& p_node_name) {
  		// channel in animation contains aiNodeAnim (aiNodeAnim its transformation for bones)
  		// numChannels == numBones
  		for (uint32 i = 0; i < p_animation->m_numChannels; ++i)	{
  				const AnimationNode* node_anim = p_animation->m_channels[i].get();
  				if (node_anim->m_nodeName == p_node_name)	{
  						return node_anim;
  				}
  		}
  		return nullptr;
  }
  
  JDVector3 
  Mesh::calcInterpolatedScaling(const float& p_animation_time, const AnimationNode* p_node_anim) {
    JDVector3 scaling;
    if (p_node_anim->m_numScalingKeys == 1) {
      scaling = p_node_anim->m_scalingKeys[0].m_value;
      return scaling;
    }

    uint32 scaling_index = findScaling(p_animation_time, p_node_anim); 
    uint32 next_scaling_index = scaling_index + 1; 
    JD_ASSERT(next_scaling_index < p_node_anim->m_numScalingKeys);
    float delta_time = (float)(p_node_anim->m_scalingKeys[next_scaling_index].m_time -
                       p_node_anim->m_scalingKeys[scaling_index].m_time);
    
    float factor = (p_animation_time - 
                   (float)p_node_anim->m_scalingKeys[scaling_index].m_time) /
                   delta_time;
    JD_ASSERT(factor >= 0.0f && factor <= 1.0f);
    JDVector3 start = p_node_anim->m_scalingKeys[scaling_index].m_value;
    JDVector3 end = p_node_anim->m_scalingKeys[next_scaling_index].m_value;
    JDVector3 delta = end - start;
    scaling = start + factor * delta;
    return scaling;
  }

  uint32 Mesh::findScaling(const float& p_animation_time, const AnimationNode* p_node_anim)
  {
    
    for (uint32 i = 0; i < p_node_anim->m_numScalingKeys - 1; ++i) {
      if (p_animation_time < (float)p_node_anim->m_scalingKeys[i + 1].m_time) {
        return i;
      }
    }

    JD_ASSERT(0);
    return 0;
  }

  JDMatrix4 
  Mesh::calcInterpolatedRotation(const float& p_animation_time, 
                                 const AnimationNode* p_node_anim) {
    JDMatrix4 final;
    if (p_node_anim->m_numRotationKeys == 1) {
      final = p_node_anim->m_rotationKeys[0].m_value.getMatrix();
      return final;
    }

    uint32 rotation_index = findRotation(p_animation_time, p_node_anim);
    uint32 next_rotation_index = rotation_index + 1;
    JD_ASSERT(next_rotation_index < p_node_anim->m_numRotationKeys);

    float delta_time = (float)(p_node_anim->m_rotationKeys[next_rotation_index].m_time -
                       p_node_anim->m_rotationKeys[rotation_index].m_time);
    
    float factor = (p_animation_time - 
                   (float)p_node_anim->m_rotationKeys[rotation_index].m_time) /
                   delta_time;

    JD_ASSERT(factor >= 0.0f && factor <= 1.0f);
    Quaternion start_quat = p_node_anim->m_rotationKeys[rotation_index].m_value;
    Quaternion end_quat = p_node_anim->m_rotationKeys[next_rotation_index].m_value;

    return nlerp(start_quat, end_quat, factor);
  }

  uint32 Mesh::findRotation(const float& p_animation_time, const AnimationNode* p_node_anim)
  {
   
    for (uint32 i = 0; i < p_node_anim->m_numRotationKeys - 1; ++i) {
      if (p_animation_time < (float)p_node_anim->m_rotationKeys[i + 1].m_time) {
        return i;
      }
    }

    JD_ASSERT(0);
    return 0;
  }

  JDVector3 
  Mesh::calcInterpolatedPosition(const float& p_animation_time, 
                                 const AnimationNode* p_node_anim) {
    JDVector3 pos;
    if (p_node_anim->m_numPositionKeys == 1) {
      //std::memcpy(&pos, &p_node_anim->mPositionKeys[0].mValue, sizeof(JDVector3));
      pos = p_node_anim->m_positionKeys[0].m_value;
      return pos;
    }

    uint32 position_index = findPosition(p_animation_time, p_node_anim);
    uint32 next_position_index = position_index + 1;
    JD_ASSERT(next_position_index < p_node_anim->m_numPositionKeys);
    float delta_time = (float)(p_node_anim->m_positionKeys[next_position_index].m_time - 
                        p_node_anim->m_positionKeys[position_index].m_time);
    float factor = (p_animation_time - 
                   (float)p_node_anim->m_positionKeys[position_index].m_time) /
                   delta_time;
    JD_ASSERT(factor >= 0.0f && factor <= 1.0f);
    JDVector3 start = p_node_anim->m_positionKeys[position_index].m_value;
    JDVector3 end = p_node_anim->m_positionKeys[next_position_index].m_value;
    JDVector3 delta = end - start;
    pos = start + factor * delta;
    return pos;
  }

  uint32 
  Mesh::findPosition(const float& p_animation_time, const AnimationNode* p_node_anim) {
    for (uint32 i = 0; i < p_node_anim->m_numPositionKeys - 1; ++i) {
      if (p_animation_time < (float)p_node_anim->m_positionKeys[i + 1].m_time) {
        return i;
      }
    }

    JD_ASSERT(0);
    return 0;
  }

  JDMatrix4 
  Mesh::nlerp(Quaternion a, Quaternion b, const float& blend) {
    //cout << a.w + a.x + a.y + a.z << endl;
    a.normalize();
    b.normalize();

    Quaternion result;
    float dot_product = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    float one_minus_blend = 1.0f - blend;

    if (dot_product < 0.0f) {
      result.x = a.x * one_minus_blend + blend * -b.x;
      result.y = a.y * one_minus_blend + blend * -b.y;
      result.z = a.z * one_minus_blend + blend * -b.z;
      result.w = a.w * one_minus_blend + blend * -b.w;
    }
    else {
      result.x = a.x * one_minus_blend + blend * b.x;
      result.y = a.y * one_minus_blend + blend * b.y;
      result.z = a.z * one_minus_blend + blend * b.z;
      result.w = a.w * one_minus_blend + blend * b.w;
    }
    result.normalize();
    //JDMatrix3 temp;
    //std::memcpy(&temp, &result.GetMatrix(), sizeof(JDMatrix3));
    //JDMatrix4 final(temp);
    JDMatrix4 final;
    final = result.getMatrix();
    return final;
  }

}