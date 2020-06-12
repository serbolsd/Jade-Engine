/*****************************************************************************/
/**
 * @file    jdPoint.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    06/JUN/2020
 * @brief   Structure for three dimensional planes.
 *
 * Structure for three dimensional planes.
 * Stores the coefficients as Xx+Yy+Zz=W. Note that this is different from many
 * other Plane classes that use Xx+Yy+Zz+W=0.
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once

/*****************************************************************************/
/**
 * Includes
 */
 /*****************************************************************************/
#include "jdPrerequisitesUtil.h"
#include "jdVector3.h"
#include "jdVector4.h"
#include "jdMatrix4.h"