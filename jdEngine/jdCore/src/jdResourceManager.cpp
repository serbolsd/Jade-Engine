#include "jdResource.h"
#include "jdResourceManager.h"
#include "jdGraphicApi.h"
namespace jdEngineSDK {
  ResourceManager&
  g_ResourceMan() {
    return ResourceManager::instance();
  }

  SPtr<Resource> ResourceManager::loadResourceFromFile(const char* path, const RESOURCE_TYPE::E& type)
  {
    uint32 newId = createHash(path);
    if (isLoadedResources(path))
    {
      return m_resources.find(newId)->second;
    }
    SPtr<Resource> newResource;
    switch (type)
    {
     case RESOURCE_TYPE::MODEL:
       newResource = loadModel(path);
      break;
     case RESOURCE_TYPE::TEXTURE:
       newResource = loadTexture(path);
       break;
     case RESOURCE_TYPE::AUDIO:
       break;
     case RESOURCE_TYPE::VIDEO:
       break;
     case RESOURCE_TYPE::FILE:
       break;
    default:
      break;
    }
    if (nullptr == newResource)
    {
      return nullptr;
    }
    newResource->setName(getFileName(path));
    newResource.get()->setID(newId);
    m_resources.insert(std::pair <uint32, SPtr<Resource>>(newId, newResource));
    return newResource;
  }

  bool 
  ResourceManager::isLoadedResources(const char* path) {
    uint32 id = createHash(path);
    auto found = m_resources.find(id);
    if (found != m_resources.end())
    {
      return true;
    }
    return false;
  }

  void 
  ResourceManager::onStartUp() {
    DEFAULT_TEXTURE_ERROR = g_graphicsApi().getDefaultTextureError();
    DEFAULT_TEXTURE_TRANSPARENT = g_graphicsApi().getDefaultTextureTransparent();
    DEFAULT_TEXTURE_BLACK = g_graphicsApi().getDefaultTextureBlack();
    DEFAULT_TEXTURE_WHITE = g_graphicsApi().getDefaultTextureWhite();
    DEFAULT_TEXTURE_NORMAL = g_graphicsApi().getDefaultTextureNormal();
    DEFAULT_TEXTURE_CHESS = g_graphicsApi().getDefaultTextureChess();
    m_modelsNames.push_back((char*)"None");
    m_texturesNames.push_back((char*)"None");
  }

  SPtr<Resource> ResourceManager::loadModel(const char* path)
  {
    m_aiScene = m_aiImporter.ReadFile(path,
                              aiProcessPreset_TargetRealtime_MaxQuality |
                              aiProcess_ConvertToLeftHanded | 
                              aiProcess_Triangulate);
    if (nullptr == m_aiScene)
    {
      String advice = "could't load model with de address: ";
      advice += path;
      std::cout << advice << std::endl;
      return nullptr;
    }

    Model* newModel = new Model;
    int32 sameName = 1;
    for (uint32 i = 0; i < m_aiScene->mNumMeshes; ++i)
    {
      auto mesh = m_aiScene->mMeshes[i];
      Mesh* newMesh = new Mesh;
      std::vector <std::uint32_t> indis;
      uint32 numIndex;
      uint32 numVertex = mesh->mNumVertices;
      //indis.reserve(numIndex);
      String meshName = mesh->mName.C_Str();
      if (i< m_aiScene->mNumMeshes-1)
      {
        for (uint32 j = 0; j < m_aiScene->mNumMeshes; j++)
        {
          if (meshName == m_aiScene->mMeshes[j]->mName.C_Str())
          {
            meshName += "##";
            meshName += static_cast<char>(sameName);
            ++sameName;
          }
        }
      }
      newMesh->setName(meshName);
      //_mesh->nameMesh = _device->model->mMeshes[meshIndx]->mName.C_Str();
      /////////////////////////////////////////////////////////////////////////
      // Load Vertex, index and bones information
      /////////////////////////////////////////////////////////////////////////
      if (!mesh->HasFaces())
      {
        delete newMesh;
        String advice = "could't load model because don't have faces";
        std::cout << advice << std::endl;
        return nullptr;
      }
      for (std::uint32_t faceIdx = 0u; faceIdx < mesh->mNumFaces; faceIdx++)
      {
        //assert(m_device.model->mMeshes[faceIdx]->mFaces->mNumIndices == 3u);
        for (uint32 in = 0u; in < mesh->mFaces[faceIdx].mNumIndices; in++)
        {
          indis.push_back(mesh->mFaces[faceIdx].mIndices[in]);
        }
      }
      numIndex = (uint32)indis.size();
      DefaultVertexData* meshVertex = new DefaultVertexData[numVertex];
      uint32* meshIndex = new uint32[numIndex];
      //Vertex Info
      for (uint32 v = 0; v < numVertex; ++v)
      {
        if (mesh->HasPositions())
        {
          if (NULL != mesh->mVertices)
          {
            meshVertex[v].Pos.x = mesh->mVertices[v].x;
            meshVertex[v].Pos.y = mesh->mVertices[v].y;
            meshVertex[v].Pos.z = mesh->mVertices[v].z;
            if (meshVertex[v].Pos == JDVector4(0, 0, 0, 1))
            {
              std::cout << "algo anda mal\n";
            }
            meshVertex[v].Pos.w = 1.0f;
          }
        }
        if (mesh->HasNormals())
        {
          if (NULL != mesh->mNormals)
          {
            meshVertex[v].Norm.x = mesh->mNormals[v].x;
            meshVertex[v].Norm.y = mesh->mNormals[v].y;
            meshVertex[v].Norm.z = mesh->mNormals[v].z;
            meshVertex[v].Norm.w = 0.0f;
          }
        }
        if (mesh->HasTangentsAndBitangents())
        {
          if (NULL != mesh->mTangents)
          {
            meshVertex[v].Tang.x = mesh->mTangents[v].x;
            meshVertex[v].Tang.y = mesh->mTangents[v].y;
            meshVertex[v].Tang.z = mesh->mTangents[v].z;
            meshVertex[v].Tang.w = 0.0f;
          }
        }
        if (NULL != mesh->mTextureCoords[0])
        {
          meshVertex[v].TexCoord.x = mesh->mTextureCoords[0][v].x;
          meshVertex[v].TexCoord.y = mesh->mTextureCoords[0][v].y;
        }
        //if (NULL != mesh->mColors[v])
        //{
        //  meshVertex[v].Color.x = mesh->mColors[v]->r;
        //  meshVertex[v].Color.y = mesh->mColors[v]->g;
        //  meshVertex[v].Color.z = mesh->mColors[v]->b;
        //  meshVertex[v].Color.w = mesh->mColors[v]->a;
        //}
      }
      //Bones info
      SkeletalMesh* skeletal = new SkeletalMesh;
      if (0 < mesh->mNumBones) {
        for (uint32 j = 0; j < mesh->mNumBones; ++j) {
          auto bone = mesh->mBones[j];
          uint32 BoneIndex = 0;
          String BoneName(bone->mName.data);

          if (skeletal->m_bonesMap.find(BoneName) == skeletal->m_bonesMap.end()) {
            BoneIndex = skeletal->m_numBones;
            skeletal->m_numBones++;
            Bone bi;
            skeletal->m_bonesInfo.push_back(bi);
          }
          else {
            BoneIndex = skeletal->m_bonesMap[BoneName];
          }

          skeletal->m_bonesMap[BoneName] = BoneIndex;
          //Checar si funciona el memcopy
          std::memcpy(&skeletal->m_bonesInfo[BoneIndex].offset,
            &mesh->mBones[j]->mOffsetMatrix,
            sizeof(JDMatrix4));

          for (uint32 k = 0; k < mesh->mBones[j]->mNumWeights; ++k) {
            uint32 verID = mesh->mBones[j]->mWeights[k].mVertexId;
            float weight = mesh->mBones[j]->mWeights[k].mWeight;
            for (uint32 b = 0; b < 4; ++b)
            {
              if (meshVertex[verID].boneWeights[b] == 0)
              {
                meshVertex[verID].boneIDs[b] = BoneIndex;
                meshVertex[verID].boneWeights[b] = weight;
                break;
              }
            }
          }
        }
      }
      else
      {
        for (uint32 j = 0; j < numVertex; j++) {
          for (uint32 b = 0; b < 4; ++b) {
              meshVertex[j].boneWeights[b] = 1;
          }
        }
      }

      SPtr<DefaultVertexData> spVetexData(meshVertex);
      newMesh->m_skeletalSData.reset(skeletal);
      newMesh->setVertexData(spVetexData);

      newMesh->setVertexBuffer(g_graphicsApi().createVertexBuffer(numVertex, 
                                                                  sizeof(DefaultVertexData), 
                                                                  meshVertex));
      newMesh->m_bonesTransforms.clear();
      newMesh->m_bonesTransforms.resize(skeletal->m_numBones);
      //Index Info
      for (uint32 indx = 0; indx < numIndex; ++indx)
      {
        meshIndex[indx] = (uint32)indis[indx];
      }
      SPtr<uint32> spIndex(meshIndex);
      newMesh->setIndex(spIndex);
      newMesh->setIndexNum(numIndex);

      newMesh->setIndexBuffer(g_graphicsApi().createIndexBuffer(numIndex, meshIndex));

      newMesh->setAlbedoTexture(g_ResourceMan().DEFAULT_TEXTURE_WHITE);
      newMesh->setNormalTexture(g_ResourceMan().DEFAULT_TEXTURE_NORMAL);
      newMesh->setSpecularTexture(g_ResourceMan().DEFAULT_TEXTURE_ERROR);
      newMesh->setMetalnessTexture(g_ResourceMan().DEFAULT_TEXTURE_ERROR);
      newMesh->setRoughnessTexture(g_ResourceMan().DEFAULT_TEXTURE_ERROR);

      const aiMaterial* pMaterial = m_aiScene->mMaterials[mesh->mMaterialIndex];

      //m_Textures[i] = NULL;
      if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        aiString PathMat;
        if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &PathMat, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
        {
          SPtr<Resource> albedo = loadResourceFromFile(PathMat.C_Str(), RESOURCE_TYPE::TEXTURE);
          if (nullptr == albedo)
          {
            newMesh->setAlbedoTexture(g_ResourceMan().DEFAULT_TEXTURE_ERROR);
          }
          else
          {
            SPtr<Texture2D> al(albedo, reinterpret_cast<Texture2D*>(albedo.get()));
            newMesh->setAlbedoTexture(al);
          }
        }
      }
      else
      {
        newMesh->setAlbedoTexture(g_ResourceMan().DEFAULT_TEXTURE_WHITE);
      }
      /////////////////////////////////////////////////////////////////////////
      // Load Material info
      /////////////////////////////////////////////////////////////////////////
      if (pMaterial->GetTextureCount(aiTextureType_NORMALS) > 0)
      {
        aiString PathMat;
        if (pMaterial->GetTexture(aiTextureType_NORMALS, 0, &PathMat, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
        {
          SPtr<Resource> normal = loadResourceFromFile(PathMat.C_Str(), RESOURCE_TYPE::TEXTURE);
          if (nullptr == normal)
          {
            newMesh->setAlbedoTexture(g_ResourceMan().DEFAULT_TEXTURE_NORMAL);
          }
          else
          {
            SPtr<Texture2D> no(normal, reinterpret_cast<Texture2D*>(normal.get()));
            newMesh->setAlbedoTexture(no);
          }
        }
      }
      else
      {
        newMesh->setAlbedoTexture(g_ResourceMan().DEFAULT_TEXTURE_ERROR);
      }
      SPtr<Mesh> meshToAdd(newMesh);
      newModel->addMesh(meshToAdd);
    }

    /////////////////////////////////////////////////////////////////////////
    // Load Animation Info
    /////////////////////////////////////////////////////////////////////////

    std::memcpy(&newModel->m_global_inverse_transform,
                &m_aiScene->mRootNode->mTransformation,
                sizeof(JDMatrix4));
    newModel->m_global_inverse_transform.invert();
    ModelNodes* rootNode = new ModelNodes();

    newModel->m_animationNodes.reset(rootNode);
    loadModelNodes(newModel->m_animationNodes, m_aiScene->mRootNode);
    newModel->m_AnimationsList.push_back((char*)"None");
    for (uint32 i = 0; i < m_aiScene->mNumAnimations; ++i)
    {
      AnimationsData* newAnimation = new AnimationsData;
      newAnimation->m_name = m_aiScene->mAnimations[i]->mName.C_Str();
      newModel->m_AnimationsList.push_back((char*)newAnimation->m_name.c_str());
      if (m_aiScene->mAnimations[i]->mTicksPerSecond != 0.0)
      {
        newAnimation->m_ticks_per_second = (float)m_aiScene->mAnimations[i]->mTicksPerSecond;
      }
      else
      {
        newAnimation->m_ticks_per_second = 25.0f;
      }
      newAnimation->m_duration = (float)m_aiScene->mAnimations[i]->mDuration;
      uint32 numchanels;
      numchanels = newAnimation->m_numChannels = m_aiScene->mAnimations[i]->mNumChannels;
      newAnimation->m_channels.resize(numchanels);
      for (uint32 j = 0; j < numchanels; ++j)
      {
        SPtr<AnimationNode> newAnimNode(new AnimationNode);
        newAnimNode->m_nodeName = m_aiScene->mAnimations[i]->mChannels[j]->mNodeName.C_Str();

        newAnimNode->m_numPositionKeys = m_aiScene->mAnimations[i]->mChannels[j]->mNumPositionKeys;
        newAnimNode->m_positionKeys.resize(newAnimNode->m_numPositionKeys);
        for (uint32 key = 0; key < newAnimNode->m_numPositionKeys; ++key) {
          newAnimNode->m_positionKeys[key].m_time =
            m_aiScene->mAnimations[i]->mChannels[j]->mPositionKeys[key].mTime;
          newAnimNode->m_positionKeys[key].m_value.x =
            m_aiScene->mAnimations[i]->mChannels[j]->mPositionKeys[key].mValue.x;
          newAnimNode->m_positionKeys[key].m_value.y =
            m_aiScene->mAnimations[i]->mChannels[j]->mPositionKeys[key].mValue.y;
          newAnimNode->m_positionKeys[key].m_value.z =
            m_aiScene->mAnimations[i]->mChannels[j]->mPositionKeys[key].mValue.z;
        }
        newAnimNode->m_numRotationKeys = m_aiScene->mAnimations[i]->mChannels[j]->mNumRotationKeys;
        newAnimNode->m_rotationKeys.resize(newAnimNode->m_numRotationKeys);
        for (uint32 key = 0; key < newAnimNode->m_numRotationKeys; ++key) {
          newAnimNode->m_rotationKeys[key].m_time =
            m_aiScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mTime;
          newAnimNode->m_rotationKeys[key].m_value.x =
            m_aiScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mValue.x;
          newAnimNode->m_rotationKeys[key].m_value.y =
            m_aiScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mValue.y;
          newAnimNode->m_rotationKeys[key].m_value.z =
            m_aiScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mValue.z;
          newAnimNode->m_rotationKeys[key].m_value.w =
            m_aiScene->mAnimations[i]->mChannels[j]->mRotationKeys[key].mValue.w;
        }

        newAnimNode->m_numScalingKeys = m_aiScene->mAnimations[i]->mChannels[j]->mNumScalingKeys;
        newAnimNode->m_scalingKeys.resize(newAnimNode->m_numScalingKeys);
        for (uint32 key = 0; key < newAnimNode->m_numScalingKeys; ++key) {
          newAnimNode->m_scalingKeys[key].m_time =
            m_aiScene->mAnimations[i]->mChannels[j]->mScalingKeys[key].mTime;
          newAnimNode->m_scalingKeys[key].m_value.x =
            m_aiScene->mAnimations[i]->mChannels[j]->mScalingKeys[key].mValue.x;
          newAnimNode->m_scalingKeys[key].m_value.y =
            m_aiScene->mAnimations[i]->mChannels[j]->mScalingKeys[key].mValue.y;
          newAnimNode->m_scalingKeys[key].m_value.z =
            m_aiScene->mAnimations[i]->mChannels[j]->mScalingKeys[key].mValue.z;
        }

        newAnimation->m_channels[j] = newAnimNode;
      }
      SPtr<AnimationsData> anim(newAnimation);
      newModel->m_animations.push_back(anim);
    }
    if (0 < m_aiScene->mNumAnimations)
    {
      //newModel->m_currentAnimation = newModel->m_animations[0];
      newModel->m_numAnimations = (uint32)m_aiScene->mNumAnimations;
    }
    //newModel->m_boneBuffer = g_graphicsApi().getConstantBufferBones();
    SPtr<Model> modelCreate(newModel);
    m_models.push_back(modelCreate);
    String name = getFileName(path);
    char* cname = new char[name.size() + 1];
    strcpy(cname, name.c_str());
    m_modelsNames.push_back(cname);
    return modelCreate;
  }

  void 
  ResourceManager::loadModelNodes(WeakSptr<ModelNodes> myNode, const aiNode* ainode) {
    SPtr <ModelNodes> node = myNode.lock();
    uint32 numChil;
    node->m_name = ainode->mName.C_Str();
    numChil = node->m_numChildren = (uint32)ainode->mNumChildren;
    node->m_numMeshes = (uint32)ainode->mNumMeshes;
    std::memcpy(&node->m_transform,
            &ainode->mTransformation,
            sizeof(JDMatrix4));

    node->m_children.resize(numChil);
    for (uint32 i = 0; i < numChil; ++i)
    {
      ModelNodes* newNode = new ModelNodes;
      newNode->m_parent = node;
      node->m_children[i].reset(newNode);
      loadModelNodes(node->m_children[i], ainode->mChildren[i]);
    }
  }

  SPtr<Resource> ResourceManager::loadTexture(const char* path)
  {
    String pa = path;
    SPtr<Texture2D> newTex;
    if (pa.find(".dds") == String::npos) {
      newTex = g_graphicsApi().LoadShaderResourceFromFile(path, false);
    }
    else
    {
      newTex = g_graphicsApi().LoadShaderResourceFromFile(path, true);
    }
    if (NULL == newTex.get())
    {
      return nullptr;
    }
    m_textures.push_back(newTex);
    String name = getFileName(path);
    char* cname = new char[name.size() + 1];
    strcpy(cname, name.c_str());
    m_texturesNames.push_back(cname);
    return newTex;
  }

  uint32
  ResourceManager::createHash(const char* path) const {
    uint32 iterator = 0;
    uint32 hash = 0;
    while (path[iterator]!='\0')
    {
      hash = hash * 101 + path[iterator];
      ++iterator;
    }

    return hash;
  }

  String 
  ResourceManager::getFileName(const char* path) const {
    char* token = NULL;
    char* nextToken = NULL;
    String name = path;
    token = strtok_s((char*)name.c_str(), "\\/", &nextToken);
    while ('\0' != nextToken[0])
    {
      token = nextToken;
      token = strtok_s(token, "\\/", &nextToken);
    }
    name = token;
    return name;
  }
}
