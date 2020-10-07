/*****************************************************************************/
/**
 * @file    jdGameObject.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    03/AGO/2020
 * @brief   Module of resource manager
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once
#include "jdPrerequisitesCore.h"

#include <jdModulo.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>
#include <assimp/cimport.h>

#include "jdResource.h"
#include "jdModel.h"


namespace jdEngineSDK {
  class Texture2D;

		class JD_CORE_EXPORT ResourceManager : public Module<ResourceManager>
		{
		 public:
    /**
     * @brief default constructor
     */
				ResourceManager() = default;

    /**
     * @brief default destructor
     */
    ~ResourceManager() { release(); };

    /**
     * @brief function to release the resources
     */
    void
    release() {};

    /**
     * @brief function to load a resources from file
     * @param path is the resource's path
     * @param type is the respurce's type to load
     * @return a shared pointer with the resourcer (return nullptr if couldn't load)
     * @note the resource is save in the resources manager and give it a id
     */
    SPtr<Resource>
    loadResourceFromFile(const char* path, const RESOURCE_TYPE::E& type);

    /**
     * @brief function check if a resources already loaded
     * @param path is the resource's path
     * @return true if the resource already loaded
     */
    bool
    isLoadedResources(const char* path);

    /**
     * @brief to set the api objetc selected
     */
    void
    setObject(ResourceManager* resourceMan) {
      ResourceManager::_instance() = resourceMan;
    }

    void
    onStartUp();

    Vector<SPtr<Model>> m_models;
    Vector<char*> m_modelsNames;
    Vector<SPtr<Texture2D>> m_textures;
    Vector<char*> m_texturesNames;

    SPtr<Texture2D> DEFAULT_TEXTURE_ERROR;
    SPtr<Texture2D> DEFAULT_TEXTURE_TRANSPARENT;
    SPtr<Texture2D> DEFAULT_TEXTURE_BLACK;
    SPtr<Texture2D> DEFAULT_TEXTURE_WHITE;
    SPtr<Texture2D> DEFAULT_TEXTURE_NORMAL;
    SPtr<Texture2D> DEFAULT_TEXTURE_CHESS;

		 private:
    SPtr<Resource>
    loadModel(const char* path);

    void
    loadModelNodes(WeakSptr<ModelNodes> myNode, const aiNode* ainode);

    void
    getModelNodesChildren(WeakSptr<ModelNodes> myNode, const aiNode* ainode);

    SPtr<Resource>
    loadTexture(const char* path);

    uint32
    createHash(const char* path) const;

    String
    getFileName(const char* path) const;

    friend class GraphicApi;
    Map<uint32, SPtr<Resource>> m_resources;

    const aiScene* m_aiScene = new const aiScene;
    Assimp::Importer m_aiImporter;
		};

  JD_CORE_EXPORT ResourceManager&
  g_ResourceMan();
}
