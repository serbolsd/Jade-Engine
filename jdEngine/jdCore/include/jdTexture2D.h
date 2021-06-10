/*****************************************************************************/
/**
 * @file    jdTexture2D.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    02/JUN/2020
 * @brief   Virtual Rendet Target class for any Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once
#include "jdPrerequisitesCore.h"
#include "jdResource.h"

namespace jdEngineSDK {

  class Texture2D : public Resource {
   public:
    /**
     * @brief default constructor
     */
    Texture2D() = default;

    /**
     * @brief virtual destructor
     */
    virtual ~Texture2D() {};

    /**
     * @brief get pointer to texture
     * Qparam index is the index of texture
     * @return pointer to texture
     */
    virtual void*
    getTexture(uint32 /*index*/ = 0) { return nullptr; };

    ///**
    // * @brief virtual function to release the texture
    // */
    //virtual void
    //release() {};
    void 
    setName(const String& name) {
      m_name = name;
    }

    String
    getName() const{
      return m_name;
    };

    uint32
    getWidth(){
      return m_width;
    }

    uint32
    getHeight(){
      return m_width;
    }

    uint32
    getChannels(){
      return m_channels;
    }

    uint32
    getMipLevels(){
      return m_mipLevel;
    }

    unsigned char*
    getData(){
      return m_data;
    }

    //virtual void
    //release() override {};
   protected:
    String m_name = "defaultName";
    uint32 m_width;
    uint32 m_height; 
    uint32 m_channels;
    uint32 m_mipLevel;
    unsigned char* m_data = nullptr;
  };
}
