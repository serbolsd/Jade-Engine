#include "jdMesh.h"
#include "jdModel.h"
namespace jdEngineSDK {
  void 
  Mesh::animated(const float& animtaionTime) {
    if (nullptr != m_myModel->m_currentAnimation) {
      boneTransform(animtaionTime);
    }
  }

  void
  Mesh::boneTransform(const float& deltaTime) {
  		JDMatrix4 identity_matrix; // = mat4(1.0f);
  		identity_matrix.identity();
  		
  		float time_in_ticks = deltaTime * m_myModel->m_currentAnimation->ticks_per_second;
  		float animation_time = fmod(time_in_ticks, (float)m_myModel->m_scene->mAnimations[0]->mDuration);
  
  		readNodeHierarchy(animation_time, m_myModel->m_scene->mRootNode, identity_matrix);
  
    //uint32 size = m_skeletalSData->m_bonesInfo.size();
  		//for (uint32 i = 0; i < size; ++i) {
    //  m_cbBonesTranform.boneTransform[i] = m_skeletalSData->m_bonesInfo[i].Transform;
  		//}
    //for (uint32 i = 0; i < m_skeletalSData->m_numBones; ++i)
    //{
    //  m_cbBonesTranform.boneTransform[i] = m_bonesTransforms[i];
    //  //if (i < m_skeletalSData->m_numBones)
    //  //{
    //  //}
    //  //else
    //  //{
    //  //  break;
    //  //}
    //}
  }
  
  void 
  Mesh::readNodeHierarchy(const float& p_animation_time, 
                                       const aiNode* p_node, 
                                       const JDMatrix4& parent_transform) {
  
  		String node_name(p_node->mName.data);
  
  		const aiAnimation* animation = m_myModel->m_scene->mAnimations[0];
  		JDMatrix4 node_transform;
    std::memcpy(&node_transform, &p_node->mTransformation, sizeof(JDMatrix4));
  
  		const aiNodeAnim* node_anim = findNodeAnim(animation, node_name);
  
  		if (node_anim)
  		{
  
  				//scaling
  				//aiVector3D scaling_vector = node_anim->mScalingKeys[2].mValue;
  				JDVector3 scaling_vector = calcInterpolatedScaling(p_animation_time, node_anim);
  				JDMatrix4 scaling_matr;
      scaling_matr.scale(scaling_vector);
  
  				//rotation
  				//aiQuaternion rotate_quat = node_anim->mRotationKeys[2].mValue;
  				JDMatrix4 rotate_matr = calcInterpolatedRotation(p_animation_time, node_anim);
  
  				//translation
  				//aiVector3D translate_vector = node_anim->mPositionKeys[2].mValue;
  				JDVector3 translate_vector = calcInterpolatedPosition(p_animation_time, node_anim);
  				JDMatrix4 translate_matr;
      translate_matr.translate(translate_vector);
      node_transform = translate_matr * rotate_matr * scaling_matr;
  		}
  		JDMatrix4 global_transform = parent_transform * node_transform;
  
  		
  		if (m_skeletalSData->m_bonesMap.find(node_name) != m_skeletalSData->m_bonesMap.end()) {// true if node_name exist in bone_mapping	
  				uint32 bone_index = m_skeletalSData->m_bonesMap[node_name];
      //m_skeletalSData->m_bonesInfo[bone_index].Transform =
      m_cbBonesTranform.boneTransform[bone_index]=
        m_myModel->m_global_inverse_transform * 
        global_transform * 
        m_skeletalSData->m_bonesInfo[bone_index].offset;
  		}
  
  		for (uint32 i = 0; i < p_node->mNumChildren; ++i)	{
  				readNodeHierarchy(p_animation_time, p_node->mChildren[i], global_transform);
  		}
  
  }
  
  const aiNodeAnim* 
  Mesh::findNodeAnim(const aiAnimation* p_animation, const String& p_node_name) {
  		// channel in animation contains aiNodeAnim (aiNodeAnim its transformation for bones)
  		// numChannels == numBones
  		for (uint32 i = 0; i < p_animation->mNumChannels; ++i)	{
  				const aiNodeAnim* node_anim = p_animation->mChannels[i];
  				if (String(node_anim->mNodeName.data) == p_node_name)	{
  						return node_anim;
  				}
  		}
  		return nullptr;
  }
  
  JDVector3 
  Mesh::calcInterpolatedScaling(const float& p_animation_time, const aiNodeAnim* p_node_anim) {
    JDVector3 scaling;
    if (p_node_anim->mNumScalingKeys == 1) {
      std::memcpy(&scaling, &p_node_anim->mScalingKeys[0].mValue, sizeof(JDVector3));
      return scaling;
    }

    uint32 scaling_index = findScaling(p_animation_time, p_node_anim); 
    uint32 next_scaling_index = scaling_index + 1; 
    JD_ASSERT(next_scaling_index < p_node_anim->mNumScalingKeys);
    float delta_time = (float)(p_node_anim->mScalingKeys[next_scaling_index].mTime - 
                       p_node_anim->mScalingKeys[scaling_index].mTime);
    
    float factor = (p_animation_time - 
                   (float)p_node_anim->mScalingKeys[scaling_index].mTime) / 
                   delta_time;
    JD_ASSERT(factor >= 0.0f && factor <= 1.0f);
    aiVector3D start = p_node_anim->mScalingKeys[scaling_index].mValue;
    aiVector3D end = p_node_anim->mScalingKeys[next_scaling_index].mValue;
    aiVector3D delta = end - start;
    aiVector3D lastScaling = start + factor * delta;
    std::memcpy(&scaling, &lastScaling, sizeof(JDVector3));
    return scaling;
  }

  uint32 Mesh::findScaling(const float& p_animation_time, const aiNodeAnim* p_node_anim)
  {
    
    for (uint32 i = 0; i < p_node_anim->mNumScalingKeys - 1; ++i) {
      if (p_animation_time < (float)p_node_anim->mScalingKeys[i + 1].mTime) {
        return i;
      }
    }

    JD_ASSERT(0);
    return 0;
  }

  JDMatrix4 
  Mesh::calcInterpolatedRotation(const float& p_animation_time, 
                                 const aiNodeAnim* p_node_anim) {
    JDMatrix4 final;
    JDMatrix3 inter;
    if (p_node_anim->mNumRotationKeys == 1) {
      std::memcpy(&inter, 
                  &p_node_anim->mRotationKeys[0].mValue.GetMatrix(), 
                  sizeof(JDMatrix3));
      final = inter;
      return final;
    }

    uint32 rotation_index = findRotation(p_animation_time, p_node_anim);
    uint32 next_rotation_index = rotation_index + 1;
    JD_ASSERT(next_rotation_index < p_node_anim->mNumRotationKeys);

    float delta_time = (float)(p_node_anim->mRotationKeys[next_rotation_index].mTime - 
                       p_node_anim->mRotationKeys[rotation_index].mTime);
    
    float factor = (p_animation_time - 
                   (float)p_node_anim->mRotationKeys[rotation_index].mTime) / 
                   delta_time;

    JD_ASSERT(factor >= 0.0f && factor <= 1.0f);
    aiQuaternion start_quat = p_node_anim->mRotationKeys[rotation_index].mValue;
    aiQuaternion end_quat = p_node_anim->mRotationKeys[next_rotation_index].mValue;

    return nlerp(start_quat, end_quat, factor);
  }

  uint32 Mesh::findRotation(const float& p_animation_time, const aiNodeAnim* p_node_anim)
  {
   
    for (uint32 i = 0; i < p_node_anim->mNumRotationKeys - 1; ++i) {
      if (p_animation_time < (float)p_node_anim->mRotationKeys[i + 1].mTime) {
        return i;
      }
    }

    JD_ASSERT(0);
    return 0;
  }

  JDVector3 
  Mesh::calcInterpolatedPosition(const float& p_animation_time, 
                                 const aiNodeAnim* p_node_anim) {
    JDVector3 pos;
    if (p_node_anim->mNumPositionKeys == 1) {
      std::memcpy(&pos, &p_node_anim->mPositionKeys[0].mValue, sizeof(JDVector3));
      return pos;
    }

    uint32 position_index = findPosition(p_animation_time, p_node_anim);
    uint32 next_position_index = position_index + 1;
    JD_ASSERT(next_position_index < p_node_anim->mNumPositionKeys);
    float delta_time = (float)(p_node_anim->mPositionKeys[next_position_index].mTime - 
                        p_node_anim->mPositionKeys[position_index].mTime);
    float factor = (p_animation_time - 
                   (float)p_node_anim->mPositionKeys[position_index].mTime) / 
                   delta_time;
    JD_ASSERT(factor >= 0.0f && factor <= 1.0f);
    aiVector3D start = p_node_anim->mPositionKeys[position_index].mValue;
    aiVector3D end = p_node_anim->mPositionKeys[next_position_index].mValue;
    aiVector3D delta = end - start;
    aiVector3D final = start + factor * delta;
    std::memcpy(&pos, &final, sizeof(JDVector3));
    return pos;
  }

  uint32 
  Mesh::findPosition(const float& p_animation_time, const aiNodeAnim* p_node_anim) {
    for (uint32 i = 0; i < p_node_anim->mNumPositionKeys - 1; ++i) {
      if (p_animation_time < (float)p_node_anim->mPositionKeys[i + 1].mTime) {
        return i;
      }
    }

    JD_ASSERT(0);
    return 0;
  }

  JDMatrix4 
  Mesh::nlerp(aiQuaternion a, aiQuaternion b, const float& blend) {
    //cout << a.w + a.x + a.y + a.z << endl;
    a.Normalize();
    b.Normalize();

    aiQuaternion result;
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
    result.Normalize();
    JDMatrix3 temp;
    std::memcpy(&temp, &result.GetMatrix(), sizeof(JDMatrix3));
    JDMatrix4 final(temp);
    return final;
  }

}