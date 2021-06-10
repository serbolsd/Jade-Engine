/*****************************************************************************/
/**
 * @file    jdResource.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    03/AGO/2020
 * @brief   virtual class to any resource
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once
#include "jdPrerequisitesCore.h"

namespace jdEngineSDK {
  namespace RESOURCE_TYPE {
    enum E {
      UNKNOWTYPE = 0,
      MODEL,
      JDMODEL,
      TEXTURE,
      AUDIO,
      VIDEO,
      FILE,
      NUMRESOUCESTYPES
    };
  }

		class Resource
		{
    friend class ResourceManager;
		 public:
    /**
     * @brief default constructor
     */
    Resource() = default;

    /**
     * @brief default destructor
     */
    ~Resource() { release(); };

    /**
     * @brief function to set the resource's id
     * @param id is a unsigned int 32 with the id to set
     */
    void
    setID(uint32 id){
      m_ID = id;
    };

    /**
     * @brief function to get the resource's id
     * @return a unsigned int 32 with the resource's id
     */
    uint32
    getID() const{
      return m_ID;
    }

    /**
     * @brief function to release the resource
     */
    virtual void
    release() {};

    /**
     * @brief function to set the resource's name
     * @param names is the name to set
     */
    void 
    setName(const String& name) {
      m_name = name;
    }

    /**
     * @brief function to get the resource's name
     * @return a string with the resource's name
     */
    String
    getName() const{
      return m_name;
    };

   protected:
    /**
     * @brief member id for identify the resource
     */
    uint32 m_ID = 0;

    /**
     * @brief member type for identify the resource type
     */
    RESOURCE_TYPE::E m_type = RESOURCE_TYPE::UNKNOWTYPE;

    /**
     * @brief member name for save resource´s name
     */
    String m_name = "defaultName";
		};
}
