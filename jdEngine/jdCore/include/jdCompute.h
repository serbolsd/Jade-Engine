/*****************************************************************************/
/**
 * @file    jdCompute.h
 * @author  Sergio Juan Diaz Carmona
 * @date    08/02/2021
 * @brief   class for logger data.
 *
 *
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/
#pragma once

#include "jdPrerequisitesCore.h"

#include <jdModulo.h>
#include <CL/cl.h>
#include <cl/cl_d3d11.h>
#include <CL/cl_d3d11_ext.h>

namespace jdEngineSDK {
  class JD_CORE_EXPORT Compute : public Module<Compute>
  {
   public:
    /**
     * @brief Default constructor for the Compute class.
     */
    Compute();

    /**
     * @brief Default destructor for the compute class.
     */
    ~Compute();

    /**
     * @brief Function to start the compute
     */
    int
    init(cl_context_properties cps[], void* device, String extension);

    /**
     * @brief Function to get all platfroms
     */
    int
    getPlatforms();

    String
    getPlatformInfo();

    String
    getAllPlatformsInfo();

    /**
     * @brief Function to select a platfrom
     * @param platformIndex is the index of the vector of all platforms to select
     */
    int
    setPlatform(int platformIndex = 0);

    /**
     * @brief Function to get all devices
     */
    int
    getDevices();

    String
    getDeviceInfo();

    String
    getAllDevicesInfo();

    /**
     * @brief Function to create context
     */
    int
    createContext(cl_context_properties cps[]);

    bool
    checkCurrentPlatformSupportsExtension(String extension);

    int
    createCommandqueue();

    int
    ShareDX11BufferWithCL(void* texture);
  
  private:
    /**
     * @brief Function to get all devices with d3d11 extention
     */
    int
    getDevicesD3D11(void* device);

    /**
     * @brief The num of platforms.
     */
    cl_uint m_numPlatforms = 0;

    /**
     * @brief vector with all platforms.
     */
    Vector<cl_platform_id> m_allPlatforms;

    /**
     * @brief The selected Platform.
     */
    cl_platform_id m_selectedPlatform;

    /**
     * @brief The num of devices.
     */
    cl_uint m_numDevices = 0;

    /**
     * @brief vector with all devices.
     */
    Vector<cl_device_id> m_allDevices;

    /**
     * @brief The selected device.
     */
    cl_device_id m_selectedDevice;

    /**
     * @brief The context of compute.
     */
    cl_context m_context;

    cl_command_queue m_commandqueue;

    clGetDeviceIDsFromD3D11NV_fn ptrToFunctionGetDeviceIDsD3D11KHR = NULL;

    clCreateFromD3D11Texture2DKHR_fn ptrToFunctionCreateFromD3D11Texture2DKHR = NULL;

    cl_mem m_imagen;
  };

  JD_CORE_EXPORT Compute&
  g_Compute();
}