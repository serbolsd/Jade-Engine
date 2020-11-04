/*****************************************************************************/
/**
 * @file    jdComponentTransform.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    07/AGO/2020
 * @brief   Class for Transform component
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once
#include "jdPrerequisitesCore.h"

#include <jdVector4.h>
#include <jdMatrix4.h>
#include <jdQuaternion.h>

#include "jdComponent.h"

namespace jdEngineSDK {

  class GameObject;
		class JD_CORE_EXPORT CTransform : public Component
		{
    /**
     * @brief friend class of GameObjetc
     */
    friend class GameObject;
		 public:
    /**
     * @brief Default constructor
     */
				CTransform() { m_type = COMPONENT_TYPE::TRANSFORM; };

    /**
     * @brief Default destructor
     */
    ~CTransform() {};
   
    /**
     * @brief funtion to upadate  the matrix
     * @param deltatime is the elpase timen between frames
     */
    void
    Update(const float& deltatime) override;

    /**
     * @brief function to get the transform matrix
     */
    JDMatrix4
    getMatrixTransform() const;

    /**
     * @brief postion
     */
    JDVector3 position = { 0, 0, 0 };

    /**
     * @brief postion
     */
    JDVector3 worldPosition = { 0, 0, 0 };

    /**
     * @brief rotation in degree
     */
    Quaternion rotation = { 0, 0, 0 };

        /**
     * @brief rotation in degree
     */
    JDVector3 euler = { 0, 0, 0 };

    /**
     * @brief acis scale
     */
    JDVector3 scale = { 1, 1, 1 };
    
    /**
     * @brief the transformation matrix in the world
     */
    JDMatrix4 matTransform;

    /**
     * @brief the transformation matrix in local
     */
    JDMatrix4 matLocalTransform;

    /**
     * @brief the vector forward 
     * @note the third row in the matrix
     */
    JDVector3 forward = { 0, 0, 0 };

    /**
     * @brief the vector right
     * @note the firts row in the matrix
     */
    JDVector3 right = { 0, 0, 0 };

    /**
     * @brief the vector up
     * @note the second row in the matrix
     */
    JDVector3 up = { 0, 0, 0 };

    /**
     * @brief the vector forward in the world
     * @note the third row in the matrix
     */
    JDVector3 worldForward = { 0, 0, 0 };

    /**
     * @brief the vector right in the world
     * @note the firts row in the matrix
     */
    JDVector3 worldRight = { 0, 0, 0 };

    /**
     * @brief the vector up in the world
     * @note the second row in the matrix
     */
    JDVector3 worldUp = { 0, 0, 0 };

   private:
		};
}
