/*****************************************************************************/
/**
 * @file    jdSaveData.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    20/APR/2021
 * @brief   header with estructurs to save and load files
 *
 *
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/
#pragma once
#include "jdPrerequisitesCore.h"
#include "jdComponent.h"
#include "jdCamera.h"
#include <jdQuaternion.h>

namespace jdEngineSDK {
  namespace TYPESAVED {
    enum E {
      MODEL = 0,
      TEXTURE
    };
  }

  struct FILE_BASE_INFO {
    uint32 id;
    uint32 NameSize;
    uint32 DirectionSize;
    TYPESAVED::E type;
  };

  struct NAME_INFO {
    String name;
    String dir;
  };

  struct TEXTURE_INFO {
    uint32 width;
    uint32 height;
    uint32 channels;
    uint32 textureSize;
  };

  struct MODEL_INFO {
    uint32 numMeshes;
    uint32 numAnimations;
    JDMatrix4 globalInverseTransform;
    bool animationNodes;
  };

  struct MESH_INFO {
    uint32 nameSize;
    uint32 numVertex;
    uint32 numIndex;
    uint32 numBonesTransform;
    uint32 numBones = 0;
    uint32 num_bonesMap;
    uint32 numBonesInfo;
    uint32 albedoID;
    uint32 normalID;
    uint32 specID;
    uint32 roughnessID;
  };

  struct ANIMATION_INFO {
    uint32 nameSize;
    float duration;
    float ticks_per_second;
    uint32 numChannels;
    uint32 numNodes;
  };

  struct ANIMATION_NODE_INFO {
    uint32 nameSize;
    uint32 numScalingKeys;
    uint32 numRotationKeys;
    uint32 numPositionKeys;
  };

  struct ANIMATION_MODEL_NODES {
    uint32 nameSize;
    uint32 numChildren;
    uint32 numMeshes;
    JDMatrix4 transform;
  };

  struct OBJECT_BASE_INFO {
    bool hasTransform = false;
    bool hasRenderModel = false;
    bool hasLight = false;
    bool hasCamera = false;
    uint32 numChildren;
    uint32 nameSize;
  };

  struct OBJECT_TRANSFORM_INFO {
    JDVector3 position = { 0, 0, 0 };
    JDVector3 worldPosition = { 0, 0, 0 };
    Quaternion rotation = { 0, 0, 0 };
    JDVector3 euler = { 0, 0, 0 };
    JDVector3 scale = { 1, 1, 1 };
    JDMatrix4 matTransform;
    JDMatrix4 matLocalTransform;
    JDVector3 forward = { 0, 0, 0 };
    JDVector3 right = { 0, 0, 0 };
    JDVector3 up = { 0, 0, 0 };
    JDVector3 worldForward = { 0, 0, 0 };
    JDVector3 worldRight = { 0, 0, 0 };
    JDVector3 worldUp = { 0, 0, 0 };
  };

  struct OBJECT_RENDERMODEL_INFO {
    uint32 modelID;
    uint32 currentAnimationID;
    uint32 numMeshes;
    bool m_playAnimation = true;
  };

  struct OBJECT_RENDERMODEL_TEXTURE_INFO {
    uint32 albedoID;
    uint32 normalID;
    uint32 specID;
    uint32 roughnessID;
  };

  struct OBJECT_CAMERA_INFO {
    uint32 nameSize;
    uint32 id;
    float _near;
    float _far;
    Radian fovAngle;
    float aspecRatio;
    float width;
    float height;
    JDVector3 front;
    JDVector3 up;
    JDVector3 right;
    JDVector4 position;
    CAMERA_PROJECTION_TYPE::E projType;
  };

  struct OBJECT_LIGHT_INFO {
    JDVector4 color;
    uint32 idArray;
    uint32 type;
  };
}