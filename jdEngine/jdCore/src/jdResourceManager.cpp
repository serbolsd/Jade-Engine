#define STB_IMAGE_IMPLEMENTATION

#include "jdResource.h"
#include "jdResourceManager.h"
#include "jdGraphicApi.h"
#include "jdSaveData.h"

#include <stb_image.h>
namespace jdEngineSDK {
  ResourceManager&
    g_ResourceMan() {
    return ResourceManager::instance();
  }

  SPtr<Resource>
  ResourceManager::loadResourceFromFile(const char* path,
                                        const RESOURCE_TYPE::E& type) {
    uint32 newId = createHash(path);
    if (isLoadedResources(path))
    {
      return m_resources.find(newId)->second;
    }
    String p = path;
    if (p.find(".jdt") != String::npos)
    {
      return loadMultipleTexture(path);
    }

    SPtr<Resource> newResource;
    switch (type)
    {
    case RESOURCE_TYPE::MODEL:
      newResource = loadModel(path);
      break;
    case RESOURCE_TYPE::JDMODEL:
      newResource = loadJDModel(path);
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
    m_resources.insert(std::pair <uint32, SPtr<Resource>>(newId, newResource));/////////////////EY LO VAS A OCUPAR
    return newResource;
  }

  bool
    ResourceManager::isLoadedResources(const char* path) {
    uint32 id = createHash(path);//mandar id en vez de string
    auto found = m_resources.find(id);
    if (found != m_resources.end())
    {
      return true;
    }
    return false;
  }

  SPtr<Model>
    ResourceManager::getModel(const char* modelName) {
    uint32 id = createHash(modelName);
    return getModel(id);
  }

  SPtr<Model>
  ResourceManager::getModel(uint32 modelID) {
    auto found = m_resources.find(modelID);
    if (found != m_resources.end())
    {
      auto mod = found->second;
      return SPtr<Model>(mod, reinterpret_cast<Model*>(mod.get()));
    }
    return nullptr;
  }

  SPtr<Texture2D>
  ResourceManager::getTexture(const char* textureName){
    uint32 id = createHash(textureName);
    return getTexture(id);
  }

  SPtr<Texture2D> 
  ResourceManager::getTexture(uint32 textureID) {
    auto found = m_resources.find(textureID);
    if (found != m_resources.end())
    {
      auto mod = found->second;
      return SPtr<Texture2D>(mod, reinterpret_cast<Texture2D*>(mod.get()));
    }
    return DEFAULT_TEXTURE_BLACK;
  }

  void 
  ResourceManager::createSAQ() {
    m_SAQ;
    Model* newModel = new Model;
    Mesh* newMesh = new Mesh;
    DefaultVertexData* meshVertex = new DefaultVertexData[4];
    meshVertex[0].Pos = { -1,-1,0,1 };
    meshVertex[1].Pos = { -1,1,0,1 };
    meshVertex[2].Pos = { 1,-1,0,1 };
    meshVertex[3].Pos = { 1,1,0,1 };

    meshVertex[0].Norm = { 0,0,-1,0 };
    meshVertex[1].Norm = { 0,0,-1,0 };
    meshVertex[2].Norm = { 0,0,-1,0 };
    meshVertex[3].Norm = { 0,0,-1,0 };

    meshVertex[0].TexCoord = { 0, 1 };
    meshVertex[1].TexCoord = { 0, 0 };
    meshVertex[2].TexCoord = { 1, 1 };
    meshVertex[3].TexCoord = { 1, 0 };

    uint32* meshIndex = new uint32[2*3];
    meshIndex[0] = 0;
    meshIndex[1] = 1;
    meshIndex[2] = 2;
    meshIndex[3] = 2;
    meshIndex[4] = 1;
    meshIndex[5] = 3;

    SPtr<DefaultVertexData> spVetexData(meshVertex);
    newMesh->setVertexData(spVetexData);

    newMesh->setVertexBuffer(g_graphicsApi().createVertexBuffer(4,
                                                                sizeof(DefaultVertexData),
                                                                meshVertex));

    SPtr<uint32> spIndex(meshIndex);
    newMesh->setIndex(spIndex);
    newMesh->setIndexNum(6);

    newMesh->setIndexBuffer(g_graphicsApi().createIndexBuffer(6, meshIndex));

    SPtr<Mesh> meshToAdd(newMesh);
    newModel->addMesh(meshToAdd);
    m_SAQ.reset(newModel);
  }

  void 
  ResourceManager::setSAQ() {
    auto mesh = m_SAQ->m_meshes[0];
    g_graphicsApi().setVertexBuffer(mesh.get()->getVertexBuffer());
    g_graphicsApi().setIndexBuffer(mesh.get()->getIndenBuffer());

    g_graphicsApi().DrawIndex(mesh.get()->getIndexNum());
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
    createSAQ();
  }

  bool 
  ResourceManager::saveProject(String projectName) {
    std::ofstream projectFile(projectName, std::ios::binary);
    if (!projectFile)
    {
      return false;
    }



    uint32 numModels= m_models.size();
    uint32 numResources= m_resources.size();
    projectFile.write((char*)&numResources, sizeof(uint32));

    uint32 numTextures = m_textures.size();
    for (uint32 i = 0; i < numTextures; ++i) {
      auto cTexture = m_textures[i];
      FILE_BASE_INFO binfo;
      binfo.NameSize = cTexture->getName().size();
      binfo.DirectionSize = 0;
      binfo.id = cTexture->getID();
      binfo.type = TYPESAVED::TEXTURE;

      projectFile.write((char*)&binfo, sizeof(FILE_BASE_INFO));
      projectFile.write((char*)cTexture->getName().data(), sizeof(char) * binfo.NameSize);

      TEXTURE_INFO tData;
      tData.height = cTexture->getHeight();
      tData.width = cTexture->getWidth();
      tData.channels = cTexture->getChannels();
      unsigned char* data = cTexture->getData();
      tData.textureSize = tData.height * tData.width * 4;

      projectFile.write((char*)&tData, sizeof(TEXTURE_INFO));

      projectFile.write((char*)data, sizeof(unsigned char) * tData.textureSize);
    }


    for (uint32 i = 0; i < numModels; ++i) {
      auto cModel = m_models[i];
      FILE_BASE_INFO binfo;
      binfo.NameSize = cModel->getName().size();
      binfo.DirectionSize = 0;
      binfo.id = cModel->getID();
      binfo.type = TYPESAVED::MODEL;

      projectFile.write((char*)&binfo, sizeof(FILE_BASE_INFO));
      projectFile.write((char*)cModel->getName().data(), sizeof(char) * binfo.NameSize);
      
      MODEL_INFO modelinfo;
      modelinfo.numMeshes = cModel->m_meshes.size();
      modelinfo.numAnimations = cModel->m_animations.size();
      modelinfo.globalInverseTransform = cModel->m_global_inverse_transform;
      if (nullptr != cModel->m_animationNodes) {
        modelinfo.animationNodes = true;
      }
      else {
        modelinfo.animationNodes = false;
      }

      projectFile.write((char*)&modelinfo, sizeof(MODEL_INFO));

      for (uint32 m = 0; m < modelinfo.numMeshes; ++m) {
        auto cMesh = cModel->m_meshes[m];
        MESH_INFO meshInfo;
        meshInfo.nameSize = cMesh->getName().size();

        meshInfo.numVertex= cMesh->m_numVertex;
        meshInfo.numIndex = cMesh->m_numIndex;
        meshInfo.numBonesTransform = cMesh->m_bonesTransforms.size();
        if (nullptr == cMesh->m_skeletalSData) {
          meshInfo.numBones = 0;
          meshInfo.num_bonesMap = 0;
          meshInfo.numBonesInfo = 0;
        }
        else {
          meshInfo.numBones = cMesh->m_skeletalSData->m_numBones;
          meshInfo.num_bonesMap = cMesh->m_skeletalSData->m_bonesMap.size();
          meshInfo.numBonesInfo = cMesh->m_skeletalSData->m_bonesInfo.size();
        }
        meshInfo.albedoID = cMesh->getAlbedoTexture()->getID();
        meshInfo.normalID = cMesh->getNormalTexture()->getID();
        meshInfo.specID = cMesh->getSpecularTexture()->getID();
        meshInfo.roughnessID = cMesh->getRoughnessTexture()->getID();

        projectFile.write((char*)&meshInfo, sizeof(MESH_INFO));
        projectFile.write((char*)cMesh->getName().data(),
                          sizeof(char) * meshInfo.nameSize);

        //write vertexData
        projectFile.write((char*)cMesh->m_vertexData.get(), 
                          sizeof(DefaultVertexData) * meshInfo.numVertex);
        //write index
        projectFile.write((char*)cMesh->m_index.get(), sizeof(uint32) * meshInfo.numIndex);
        //write bone transform
        projectFile.write((char*)cMesh->m_bonesTransforms.data(), 
                          sizeof(JDMatrix4) * meshInfo.numBonesTransform);
        if (nullptr != cMesh->m_skeletalSData && meshInfo.numBonesInfo > 0) {
          //write bones Iinfo
          projectFile.write((char*)cMesh->m_skeletalSData->m_bonesInfo.data(),
                            sizeof(Bone) * meshInfo.numBonesInfo);

          auto bMap = cMesh->m_skeletalSData->m_bonesMap;
          //write boneMap
          for (auto b = bMap.begin(); b != bMap.end(); ++b) {
            uint32 size = b->first.size();
            projectFile.write((char*)&size, sizeof(uint32));
            projectFile.write((char*)b->first.data(), sizeof(char) * size);
            projectFile.write((char*)&b->second, sizeof(uint32));
          }
        }

      }

      for (uint32 a = 0; a < modelinfo.numAnimations; ++a) {
        auto anim = cModel->m_animations[a];
        ANIMATION_INFO aInfo;
        aInfo.nameSize = anim->m_name.size();
        aInfo.duration = anim->m_duration;
        aInfo.ticks_per_second = anim->m_ticks_per_second;
        aInfo.numChannels = anim->m_numChannels;
        aInfo.numNodes = anim->m_channels.size();
        projectFile.write((char*)&aInfo, sizeof(ANIMATION_INFO));
        projectFile.write((char*)anim->m_name.data(), sizeof(char) * aInfo.nameSize);

        auto animNode = anim->m_channels;
        for (uint32 n = 0; n < aInfo.numNodes; ++n) {
          ANIMATION_NODE_INFO aNodeInfo;
          aNodeInfo.nameSize = animNode[n]->m_nodeName.size();
          aNodeInfo.numPositionKeys = animNode[n]->m_numPositionKeys;
          aNodeInfo.numScalingKeys = animNode[n]->m_numScalingKeys;
          aNodeInfo.numRotationKeys = animNode[n]->m_numRotationKeys;
          projectFile.write((char*)&aNodeInfo, sizeof(ANIMATION_NODE_INFO));
          projectFile.write((char*)animNode[n]->m_nodeName.data(),
                            sizeof(char) * aNodeInfo.nameSize);

          projectFile.write((char*)animNode[n]->m_positionKeys.data(),
                            sizeof(PositionKey) * aNodeInfo.numPositionKeys);
          projectFile.write((char*)animNode[n]->m_scalingKeys.data(),
                            sizeof(ScalingKey) * aNodeInfo.numScalingKeys);
          projectFile.write((char*)animNode[n]->m_rotationKeys.data(),
                            sizeof(RotationKey) * aNodeInfo.numRotationKeys);

        }
      }
      saveModelNodes(projectFile, cModel->m_animationNodes);
    }

  
    projectFile.close();
    return true;
  }

  void 
  ResourceManager::saveModelNodes(std::ofstream& file, SPtr<ModelNodes> Node) {
    ANIMATION_MODEL_NODES mNodeInfo;
    mNodeInfo.nameSize = Node->m_name.size();
    mNodeInfo.numChildren = Node->m_numChildren;
    mNodeInfo.numMeshes = Node->m_numMeshes;
    mNodeInfo.transform = Node->m_transform;
    file.write((char*)&mNodeInfo, sizeof(ANIMATION_MODEL_NODES));
    file.write((char*)Node->m_name.data(), sizeof(char) * mNodeInfo.nameSize);
    
    for (uint32 i = 0; i < mNodeInfo.numChildren; ++i) {
      saveModelNodes(file, Node->m_children[i]);
    }
  }

  bool
  ResourceManager::loadProject(String projectName) {
    std::ifstream projectFile(projectName, std::ios::binary);
    if (!projectFile) {
      return false;
    }
    
    uint32 numResources = 0;
    projectFile.read((char*)&numResources, sizeof(uint32));
    for (uint32 i = numResources; i > 0; --i) {
      SPtr<Resource> newResource;

      FILE_BASE_INFO binfo;
      projectFile.read((char*)&binfo, sizeof(FILE_BASE_INFO));
      String name;
      name.resize(binfo.NameSize);
      projectFile.read((char*)name.data(), sizeof(char) * binfo.NameSize);

      if (binfo.type == TYPESAVED::MODEL) {

        Model* newModel = new Model;
        newModel->setID(binfo.id);
        newModel->m_name = name;
        newModel->m_AnimationsList.push_back((char*)"None");

        MODEL_INFO modelinfo;
        projectFile.read((char*)&modelinfo, sizeof(MODEL_INFO));

        for (uint32 m = modelinfo.numMeshes; m > 0; --m) {
          Mesh* newMesh = new Mesh;
          MESH_INFO meshInfo;
          projectFile.read((char*)&meshInfo, sizeof(MESH_INFO));
          
          newMesh->m_name.resize(meshInfo.nameSize);
          projectFile.read((char*)newMesh->m_name.data(), sizeof(char) * meshInfo.nameSize);
          //read vertexData
          newMesh->m_numVertex = meshInfo.numVertex;
          auto vData = new DefaultVertexData[meshInfo.numVertex];
          projectFile.read((char*)vData,
                           sizeof(DefaultVertexData) * meshInfo.numVertex);
          newMesh->m_vertexData.reset(vData);
          //creata vertex buffer
          newMesh->setVertexBuffer(g_graphicsApi().createVertexBuffer(meshInfo.numVertex,
                                                                      sizeof(DefaultVertexData), 
                                                                      vData));

          //read index
          newMesh->m_numIndex = meshInfo.numIndex;
          auto index = new uint32[meshInfo.numIndex];
          projectFile.read((char*)index, sizeof(uint32) * meshInfo.numIndex);
          newMesh->m_index.reset(index);
          //create index buffer
          newMesh->setIndexBuffer(g_graphicsApi().createIndexBuffer(meshInfo.numIndex, index));


          //read bone transform
          newMesh->m_bonesTransforms.resize(meshInfo.numBonesTransform);
          projectFile.read((char*)newMesh->m_bonesTransforms.data(),
                           sizeof(JDMatrix4) * meshInfo.numBonesTransform);
          SkeletalMesh* skeletal = new SkeletalMesh;
          if (meshInfo.num_bonesMap > 0) {
            skeletal->m_numBones = meshInfo.numBones;
            //write bones Iinfo
            skeletal->m_bonesInfo.resize(meshInfo.numBonesInfo);
            projectFile.read((char*)skeletal->m_bonesInfo.data(),
                             sizeof(Bone) * meshInfo.numBonesInfo);
          
            auto bMap = &skeletal->m_bonesMap;
            //write boneMap
            for (auto b = 0; b < meshInfo.num_bonesMap; ++b) {
              uint32 size = 0;
              String key;
              uint32 value = 0;
              projectFile.read((char*)&size, sizeof(uint32));
              key.resize(size);
              projectFile.read((char*)key.data(), sizeof(char) * size);
              projectFile.read((char*)&value, sizeof(uint32));
              bMap->insert(std::pair<String, uint32>(key, value));
            }
          }
          newMesh->m_skeletalSData.reset(skeletal);

          newMesh->setAlbedoTexture(g_ResourceMan().DEFAULT_TEXTURE_WHITE);
          newMesh->setNormalTexture(g_ResourceMan().DEFAULT_TEXTURE_NORMAL);
          newMesh->setSpecularTexture(g_ResourceMan().DEFAULT_TEXTURE_ERROR);
          newMesh->setMetalnessTexture(g_ResourceMan().DEFAULT_TEXTURE_ERROR);
          newMesh->setRoughnessTexture(g_ResourceMan().DEFAULT_TEXTURE_ERROR);

          if (meshInfo.albedoID != 0) {
            auto texture = getTexture(meshInfo.albedoID);
            for (uint32 i = 0; i < m_texturesNames.size(); ++i) {
              String name = g_ResourceMan().m_texturesNames[i];
              if (name == texture->getName()) {
                newMesh->m_albedoOption = i;
                newMesh->setAlbedoTexture(texture);
                break;
              }
            }
          }
          if (meshInfo.normalID != 0) {
            auto texture = getTexture(meshInfo.normalID);
            for (uint32 i = 0; i < m_texturesNames.size(); ++i) {
              String name = g_ResourceMan().m_texturesNames[i];
              if (name == texture->getName()) {
                newMesh->m_NormalOption = i;
                newMesh->setNormalTexture(texture);
                break;
              }
            }
          }
          if (meshInfo.specID != 0) {
            auto texture = getTexture(meshInfo.specID);
            for (uint32 i = 0; i < m_texturesNames.size(); ++i) {
              String name = g_ResourceMan().m_texturesNames[i];
              if (name == texture->getName()) {
                newMesh->m_specularOption = i;
                newMesh->setSpecularTexture(texture);
                break;
              }
            }
          }
          if (meshInfo.roughnessID != 0) {
            auto texture = getTexture(meshInfo.roughnessID);
            for (uint32 i = 0; i < m_texturesNames.size(); ++i) {
              String name = g_ResourceMan().m_texturesNames[i];
              if (name == texture->getName()) {
                newMesh->m_albedoOption = i;
                newMesh->setRoughnessTexture(texture);
                break;
              }
            }
          }

          SPtr<Mesh> meshToAdd(newMesh);
          newModel->addMesh(meshToAdd);
        }
        newModel->m_global_inverse_transform = modelinfo.globalInverseTransform;
        newModel->m_animations.resize(modelinfo.numAnimations);

        for (uint32 a = 0; a < modelinfo.numAnimations; ++a) {
          newModel->m_animations[a].reset(new AnimationsData);
          auto anim = newModel->m_animations[a];
          ANIMATION_INFO aInfo;
          projectFile.read((char*)&aInfo, sizeof(ANIMATION_INFO));
          anim->m_name.resize(aInfo.nameSize);
          projectFile.read((char*)anim->m_name.data(), sizeof(char) * aInfo.nameSize);
          newModel->m_AnimationsList.push_back((char*)anim->m_name.c_str());
          anim->m_duration = aInfo.duration;
          anim->m_ticks_per_second = aInfo.ticks_per_second;
          anim->m_numChannels = aInfo.numChannels;
          anim->m_channels.resize(aInfo.numNodes);

          for (uint32 n = 0; n < aInfo.numNodes; ++n) {

            anim->m_channels[n].reset(new AnimationNode);
            ANIMATION_NODE_INFO aNodeInfo;
            projectFile.read((char*)&aNodeInfo, sizeof(ANIMATION_NODE_INFO));
            anim->m_channels[n]->m_nodeName.resize(aNodeInfo.nameSize);
            projectFile.read((char*)anim->m_channels[n]->m_nodeName.data(),
                              sizeof(char) * aNodeInfo.nameSize);
            anim->m_channels[n]->m_numPositionKeys = aNodeInfo.numPositionKeys;
            anim->m_channels[n]->m_numScalingKeys = aNodeInfo.numScalingKeys;
            anim->m_channels[n]->m_numRotationKeys = aNodeInfo.numRotationKeys;

            anim->m_channels[n]->m_positionKeys.resize(aNodeInfo.numPositionKeys);
            anim->m_channels[n]->m_scalingKeys.resize(aNodeInfo.numScalingKeys);
            anim->m_channels[n]->m_rotationKeys.resize(aNodeInfo.numRotationKeys);

            projectFile.read((char*)anim->m_channels[n]->m_positionKeys.data(),
                             sizeof(PositionKey) * aNodeInfo.numPositionKeys);
            projectFile.read((char*)anim->m_channels[n]->m_scalingKeys.data(),
                             sizeof(ScalingKey) * aNodeInfo.numScalingKeys);
            projectFile.read((char*)anim->m_channels[n]->m_rotationKeys.data(),
                             sizeof(RotationKey) * aNodeInfo.numRotationKeys);

          }
        }
        ModelNodes* rootNode = new ModelNodes();

        newModel->m_animationNodes.reset(rootNode);
        loadModelNodes(projectFile, newModel->m_animationNodes);

        SPtr<Model> modelCreate(newModel);
        m_models.push_back(modelCreate);
        String n = name;
        char* cname = new char[name.size() + 1];
        strcpy(cname, name.c_str());
        m_modelsNames.push_back(cname);

        newResource = modelCreate;
      }
      else {
        SPtr<Texture2D> newTex;
        TEXTURE_INFO tData;
        projectFile.read((char*)&tData, sizeof(TEXTURE_INFO));
        unsigned char* data = new unsigned char[tData.textureSize];
        projectFile.read((char*)data, sizeof(unsigned char)* tData.textureSize);
        tData.textureSize = tData.height * tData.width * 4;
        
        newTex = g_graphicsApi().CreatTextureFromArray(data, tData.width, tData.height, 4);
        if (NULL == newTex.get()) {
          continue;
        }
        m_textures.push_back(newTex);
        char* cname = new char[name.size() + 1];
        strcpy(cname, name.c_str());
        m_texturesNames.push_back(cname);
        newTex->setName(name);
        newResource = newTex;
      }
      newResource->setName(name);
      newResource.get()->setID(binfo.id);
      m_resources.insert(std::pair <uint32, SPtr<Resource>>(binfo.id, newResource));/////////////////EY LO VAS A OCUPAR
    }
    projectFile.close();
    return true;
  }

  void 
  ResourceManager::loadModelNodes(std::ifstream& file, SPtr<ModelNodes> Node) {
    ANIMATION_MODEL_NODES mNodeInfo;
    file.read((char*)&mNodeInfo, sizeof(ANIMATION_MODEL_NODES));
    Node->m_name.resize(mNodeInfo.nameSize) ;
    file.read((char*)Node->m_name.data(), sizeof(char) * mNodeInfo.nameSize);
    Node->m_numChildren = mNodeInfo.numChildren;
    Node->m_numMeshes = mNodeInfo.numMeshes;
    Node->m_transform = mNodeInfo.transform;

    Node->m_children.resize(mNodeInfo.numChildren);
      
    for (uint32 i = 0; i < mNodeInfo.numChildren; ++i) {
      ModelNodes* newNode = new ModelNodes;
      newNode->m_parent = Node;
      Node->m_children[i].reset(newNode);
      loadModelNodes(file, Node->m_children[i]);
    }
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
      newMesh->m_numVertex = numVertex;

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
          String dir = m_textureFolder + getFileNameWithoutExtention(PathMat.C_Str()) + ".png";
          SPtr<Resource> albedo = loadResourceFromFile(dir.c_str(), RESOURCE_TYPE::TEXTURE);
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
          String dir = m_textureFolder + getFileNameWithoutExtention(PathMat.C_Str()) + ".png";
          SPtr<Resource> normal = loadResourceFromFile(dir.c_str(), RESOURCE_TYPE::TEXTURE);
          if (nullptr == normal)
          {
            newMesh->setNormalTexture(g_ResourceMan().DEFAULT_TEXTURE_NORMAL);
          }
          else
          {
            SPtr<Texture2D> no(normal, reinterpret_cast<Texture2D*>(normal.get()));
            newMesh->setNormalTexture(no);
          }
        }
      }
      else
      {
        newMesh->setNormalTexture(g_ResourceMan().DEFAULT_TEXTURE_ERROR);
      }

      if (pMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0)
      {
        aiString PathMat;
        if (pMaterial->GetTexture(aiTextureType_SPECULAR, 0, &PathMat, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
        {
          String dir = m_textureFolder + getFileNameWithoutExtention(PathMat.C_Str()) + ".png";
          SPtr<Resource> spec = loadResourceFromFile(dir.c_str(), RESOURCE_TYPE::TEXTURE);
          if (nullptr == spec)
          {
            newMesh->setSpecularTexture(g_ResourceMan().DEFAULT_TEXTURE_NORMAL);
          }
          else
          {
            SPtr<Texture2D> no(spec, reinterpret_cast<Texture2D*>(spec.get()));
            newMesh->setSpecularTexture(no);
          }
        }
      }
      else
      {
        newMesh->setSpecularTexture(g_ResourceMan().DEFAULT_TEXTURE_ERROR);
      }

      if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) > 0)
      {
        aiString PathMat;
        if (pMaterial->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &PathMat, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
        {
          String dir = m_textureFolder + getFileNameWithoutExtention(PathMat.C_Str()) + ".png";
          SPtr<Resource> roug = loadResourceFromFile(dir.c_str(), RESOURCE_TYPE::TEXTURE);
          if (nullptr == roug)
          {
            newMesh->setRoughnessTexture(g_ResourceMan().DEFAULT_TEXTURE_NORMAL);
          }
          else
          {
            SPtr<Texture2D> no(roug, reinterpret_cast<Texture2D*>(roug.get()));
            newMesh->setRoughnessTexture(no);
          }
        }
      }
      else
      {
        newMesh->setRoughnessTexture(g_ResourceMan().DEFAULT_TEXTURE_ERROR);
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

  SPtr<Resource> 
  ResourceManager::loadJDModel(const char* path) {
    uint32 sizeModelInfo = sizeof(MODEL_INFO);
    uint32 sizeMeshInfo = sizeof(MESH_INFO);
    uint32 sizeVertexInfo = sizeof(DefaultVertexData);


    Model* newModel = new Model;
    //newModel->setID(binfo.id);
    //newModel->m_name = name;
    newModel->m_AnimationsList.push_back((char*)"None");
    std::ifstream projectFile(path, std::ios::binary);
    if (!projectFile) {
      return nullptr;
    }
    MODEL_INFO modelinfo;
    projectFile.read((char*)&modelinfo, sizeof(MODEL_INFO));

    for (uint32 m = modelinfo.numMeshes; m > 0; --m) {
      Mesh* newMesh = new Mesh;
      MESH_INFO meshInfo;
      projectFile.read((char*)&meshInfo, sizeof(MESH_INFO));

      newMesh->m_name.resize(meshInfo.nameSize);
      projectFile.read((char*)newMesh->m_name.data(), sizeof(char) * meshInfo.nameSize);
      //read vertexData
      newMesh->m_numVertex = meshInfo.numVertex;
      auto vData = new DefaultVertexData[meshInfo.numVertex];
      projectFile.read((char*)vData, sizeof(DefaultVertexData) * meshInfo.numVertex);
      newMesh->m_vertexData.reset(vData);
      //creata vertex buffer
      newMesh->setVertexBuffer(g_graphicsApi().createVertexBuffer(meshInfo.numVertex,
                                                                  sizeof(DefaultVertexData),
                                                                  vData));

      //read index
      newMesh->m_numIndex = meshInfo.numIndex;
      auto index = new uint32[meshInfo.numIndex];
      projectFile.read((char*)index, sizeof(uint32) * meshInfo.numIndex);
      newMesh->m_index.reset(index);
      //create index buffer
      newMesh->setIndexBuffer(g_graphicsApi().createIndexBuffer(meshInfo.numIndex, index));


      //read bone transform
      newMesh->m_bonesTransforms.resize(meshInfo.numBonesTransform);
      projectFile.read((char*)newMesh->m_bonesTransforms.data(),
        sizeof(JDMatrix4) * meshInfo.numBonesTransform);
      SkeletalMesh* skeletal = new SkeletalMesh;
      if (meshInfo.num_bonesMap > 0) {
        skeletal->m_numBones = meshInfo.numBones;
        //write bones Iinfo
        skeletal->m_bonesInfo.resize(meshInfo.numBonesInfo);
        projectFile.read((char*)skeletal->m_bonesInfo.data(),
          sizeof(Bone) * meshInfo.numBonesInfo);

        auto bMap = &skeletal->m_bonesMap;
        //write boneMap
        for (auto b = 0; b < meshInfo.num_bonesMap; ++b) {
          uint32 size = 0;
          String key;
          uint32 value = 0;
          projectFile.read((char*)&size, sizeof(uint32));
          key.resize(size);
          projectFile.read((char*)key.data(), sizeof(char) * size);
          projectFile.read((char*)&value, sizeof(uint32));
          bMap->insert(std::pair<String, uint32>(key, value));
        }
      }
      newMesh->m_skeletalSData.reset(skeletal);

      newMesh->setAlbedoTexture(g_ResourceMan().DEFAULT_TEXTURE_WHITE);
      newMesh->setNormalTexture(g_ResourceMan().DEFAULT_TEXTURE_NORMAL);
      newMesh->setSpecularTexture(g_ResourceMan().DEFAULT_TEXTURE_ERROR);
      newMesh->setMetalnessTexture(g_ResourceMan().DEFAULT_TEXTURE_ERROR);
      newMesh->setRoughnessTexture(g_ResourceMan().DEFAULT_TEXTURE_ERROR);

      if (meshInfo.albedoID != 0) {
        auto texture = getTexture(meshInfo.albedoID);
        for (uint32 i = 0; i < m_texturesNames.size(); ++i) {
          String name = g_ResourceMan().m_texturesNames[i];
          if (name == texture->getName()) {
            newMesh->m_albedoOption = i;
            newMesh->setAlbedoTexture(texture);
            break;
          }
        }
      }
      if (meshInfo.normalID != 0) {
        auto texture = getTexture(meshInfo.normalID);
        for (uint32 i = 0; i < m_texturesNames.size(); ++i) {
          String name = g_ResourceMan().m_texturesNames[i];
          if (name == texture->getName()) {
            newMesh->m_NormalOption = i;
            newMesh->setNormalTexture(texture);
            break;
          }
        }
      }
      if (meshInfo.specID != 0) {
        auto texture = getTexture(meshInfo.specID);
        for (uint32 i = 0; i < m_texturesNames.size(); ++i) {
          String name = g_ResourceMan().m_texturesNames[i];
          if (name == texture->getName()) {
            newMesh->m_specularOption = i;
            newMesh->setSpecularTexture(texture);
            break;
          }
        }
      }
      if (meshInfo.roughnessID != 0) {
        auto texture = getTexture(meshInfo.roughnessID);
        for (uint32 i = 0; i < m_texturesNames.size(); ++i) {
          String name = g_ResourceMan().m_texturesNames[i];
          if (name == texture->getName()) {
            newMesh->m_albedoOption = i;
            newMesh->setRoughnessTexture(texture);
            break;
          }
        }
      }

      SPtr<Mesh> meshToAdd(newMesh);
      newModel->addMesh(meshToAdd);
    }
    newModel->m_global_inverse_transform = modelinfo.globalInverseTransform;
    newModel->m_animations.resize(modelinfo.numAnimations);

    for (uint32 a = 0; a < modelinfo.numAnimations; ++a) {
      newModel->m_animations[a].reset(new AnimationsData);
      auto anim = newModel->m_animations[a];
      ANIMATION_INFO aInfo;
      projectFile.read((char*)&aInfo, sizeof(ANIMATION_INFO));
      anim->m_name.resize(aInfo.nameSize);
      projectFile.read((char*)anim->m_name.data(), sizeof(char) * aInfo.nameSize);
      newModel->m_AnimationsList.push_back((char*)anim->m_name.c_str());
      anim->m_duration = aInfo.duration;
      anim->m_ticks_per_second = aInfo.ticks_per_second;
      anim->m_numChannels = aInfo.numChannels;
      anim->m_channels.resize(aInfo.numNodes);

      for (uint32 n = 0; n < aInfo.numNodes; ++n) {

        anim->m_channels[n].reset(new AnimationNode);
        ANIMATION_NODE_INFO aNodeInfo;
        projectFile.read((char*)&aNodeInfo, sizeof(ANIMATION_NODE_INFO));
        anim->m_channels[n]->m_nodeName.resize(aNodeInfo.nameSize);
        projectFile.read((char*)anim->m_channels[n]->m_nodeName.data(),
          sizeof(char) * aNodeInfo.nameSize);
        anim->m_channels[n]->m_numPositionKeys = aNodeInfo.numPositionKeys;
        anim->m_channels[n]->m_numScalingKeys = aNodeInfo.numScalingKeys;
        anim->m_channels[n]->m_numRotationKeys = aNodeInfo.numRotationKeys;

        anim->m_channels[n]->m_positionKeys.resize(aNodeInfo.numPositionKeys);
        anim->m_channels[n]->m_scalingKeys.resize(aNodeInfo.numScalingKeys);
        anim->m_channels[n]->m_rotationKeys.resize(aNodeInfo.numRotationKeys);

        projectFile.read((char*)anim->m_channels[n]->m_positionKeys.data(),
          sizeof(PositionKey) * aNodeInfo.numPositionKeys);
        projectFile.read((char*)anim->m_channels[n]->m_scalingKeys.data(),
          sizeof(ScalingKey) * aNodeInfo.numScalingKeys);
        projectFile.read((char*)anim->m_channels[n]->m_rotationKeys.data(),
          sizeof(RotationKey) * aNodeInfo.numRotationKeys);

      }
    }
    ModelNodes* rootNode = new ModelNodes();

    newModel->m_animationNodes.reset(rootNode);
    //loadModelNodes(projectFile, newModel->m_animationNodes);

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
    newTex->setName(name);
    return newTex;
  }

  SPtr<Resource> 
  ResourceManager::loadMultipleTexture(const char* path) {
    String pa = path;
    SPtr<Texture2D> newTex;

    int32 width, height, channels;
    unsigned char* img = stbi_load(path, &width, &height, &channels, 4);

    int32 size = width * height;
    int32 sizeWithChannels = size * 4;
    unsigned char* AO = new unsigned char[sizeWithChannels];
    unsigned char* Metal = new unsigned char[sizeWithChannels];
    unsigned char* Roug = new unsigned char[sizeWithChannels];
    int32 realIndex = 0;
    for (int32 i = 0; i < size; ++i)
    {
      realIndex = i * 4;
      AO[realIndex] = img[realIndex];
      AO[realIndex + 1] = img[realIndex];
      AO[realIndex + 2] = img[realIndex];
      AO[realIndex + 3] = 255;

      Metal[realIndex] = img[realIndex + 1];
      Metal[realIndex + 1] = img[realIndex + 1];
      Metal[realIndex + 2] = img[realIndex + 1];
      Metal[realIndex + 3] = 255;

      Roug[realIndex] = img[realIndex + 2];
      Roug[realIndex + 1] = img[realIndex + 2];
      Roug[realIndex + 2] = img[realIndex + 2];
      Roug[realIndex + 3] = 255;
    }

    SPtr<Resource> newResource;
    String filename = getFileName(path);
    
    String name = "AO";
    String nameRe = "AO";
    name += filename;
    nameRe += filename;
    uint32 newId = createHash(name.c_str());
    newTex = g_graphicsApi().CreatTextureFromArray(AO, width, height, 4);
    m_textures.push_back(newTex);
    char* cname = new char[name.size() + 1];
    strcpy(cname, name.c_str());
    m_texturesNames.push_back(cname);
    newTex->setName(name);
    newResource = newTex;
    newResource->setName(nameRe);
    newResource.get()->setID(newId);
    m_resources.insert(std::pair <uint32, SPtr<Resource>>(newId, newResource));

    name = "METAL";
    nameRe = "METAL";
    name += filename;
    nameRe += filename;
    newId = createHash(name.c_str());
    newTex = g_graphicsApi().CreatTextureFromArray(Metal, width, height, 4);
    m_textures.push_back(newTex);
    char* cname2 = new char[name.size() + 1];
    strcpy(cname2, name.c_str());
    m_texturesNames.push_back(cname2);
    newTex->setName(name);
    newResource = newTex;
    newResource->setName(name);
    newResource.get()->setID(newId);
    m_resources.insert(std::pair <uint32, SPtr<Resource>>(newId, newResource));

    name = "ROUG";
    nameRe = "ROUG";
    name += filename;
    nameRe += filename;
    newId = createHash(name.c_str());
    newTex = g_graphicsApi().CreatTextureFromArray(Roug, width, height, 4);
    m_textures.push_back(newTex);
    char* cname3 = new char[name.size() + 1];
    strcpy(cname3, name.c_str());
    m_texturesNames.push_back(cname3);
    newTex->setName(name);
    newResource = newTex;
    newResource->setName(name);
    newResource.get()->setID(newId);
    m_resources.insert(std::pair <uint32, SPtr<Resource>>(newId, newResource));

    return newResource;
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

  String 
  ResourceManager::getFileNameWithoutExtention(const char* path) const {
    char* token = NULL;
    char* nextToken = NULL;
    String name = path;
    token = strtok_s((char*)name.c_str(), "\\/", &nextToken);
    while ('\0' != nextToken[0])
    {
      token = nextToken;
      token = strtok_s(token, "\\/", &nextToken);
    }
    token = strtok_s(token, ".", &nextToken);
    name = token;
    return name;
  }
}
