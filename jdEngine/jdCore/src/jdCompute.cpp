#include "jdCompute.h"
#include "jdLogger.h"
namespace jdEngineSDK {

  Compute::Compute()
  {
  }

  Compute::~Compute()
  {
  }

  int 
  Compute::init(cl_context_properties cps[], void* device, String extension) {
    return 0;
    int out = 0;
    getPlatforms();
    if (!checkCurrentPlatformSupportsExtension(extension)) {
      return 1;
    }
    if (extension == "cl_nv_d3d11_sharing")
    {
      getDevicesD3D11(device);
    }
    cl_context_properties _cps[] = { CL_CONTEXT_PLATFORM, 
                                    (cl_context_properties)m_selectedPlatform, 
                                    CL_CONTEXT_D3D11_DEVICE_NV, 
                                    (intptr_t)device,
                                    CL_CONTEXT_INTEROP_USER_SYNC, 
                                    CL_FALSE, 
                                    0 };
    //getDevices();
    createContext(&_cps[0]);
    createCommandqueue();
    return 0;
  }

  int
  Compute::getPlatforms() {
    cl_int out = 0;

    //Get the num of platforms
    out = clGetPlatformIDs(0, nullptr, &m_numPlatforms);
    m_allPlatforms.resize(m_numPlatforms);
    //get platforms
    out = clGetPlatformIDs(m_numPlatforms, &m_allPlatforms[0], &m_numPlatforms);

    m_selectedPlatform = m_allPlatforms[0];
    return out;
  }

  String 
  Compute::getPlatformInfo() {
    String out = "";

    String platform_version;
    String platform_name;
    String platform_vendor;
    String platform_extensions;
    platform_version.resize(255, '\0');
    platform_name.resize(255, '\0');
    platform_vendor.resize(255, '\0');
    platform_extensions.resize(1024, '\0');

    clGetPlatformInfo(m_selectedPlatform, CL_PLATFORM_VERSION, 255, &platform_version[0], nullptr);
    clGetPlatformInfo(m_selectedPlatform, CL_PLATFORM_NAME, 255, &platform_name[0], nullptr);
    clGetPlatformInfo(m_selectedPlatform, CL_PLATFORM_VENDOR, 255, &platform_vendor[0], nullptr);
    clGetPlatformInfo(m_selectedPlatform, CL_PLATFORM_EXTENSIONS, 1024, &platform_extensions[0], nullptr);

    out += "Platform version : " + platform_version + "\n";
    out += "Platform name : " + platform_version + "\n";
    out += "Platform vendor : " + platform_vendor + "\n";
    out += "Platform extentions : " + platform_extensions + "\n";
    


    return out;
  }

  String 
  Compute::getAllPlatformsInfo() {
    String out = "";

    String platform_version;
    String platform_name;
    String platform_vendor;
    String platform_extensions;
    platform_version.resize(255, '\0');
    platform_name.resize(255, '\0');
    platform_vendor.resize(255, '\0');
    platform_extensions.resize(1024, '\0');

    for (auto& platform : m_allPlatforms) {
      clGetPlatformInfo(platform, CL_PLATFORM_VERSION, 255, &platform_version[0], nullptr);
      clGetPlatformInfo(platform, CL_PLATFORM_NAME, 255, &platform_name[0], nullptr);
      clGetPlatformInfo(platform, CL_PLATFORM_VENDOR, 255, &platform_vendor[0], nullptr);
      clGetPlatformInfo(platform, 
                        CL_PLATFORM_EXTENSIONS, 
                        1024, 
                        &platform_extensions[0], 
                        nullptr);

      out += "Platform version : " + platform_version + "\n";
      out += "Platform name : " + platform_version + "\n";
      out += "Platform vendor : " + platform_vendor + "\n";
      out += "Platform extentions : " + platform_extensions + "\n";
    }

    return out;
  }

  int Compute::setPlatform(int platformIndex) {
    if (m_numPlatforms <= 0) {
      //g_Logger().Log("");
      return 1;
    }

    if (platformIndex < 0 || platformIndex >= m_numPlatforms) {
      m_selectedPlatform = m_allPlatforms[0];
      //Warning
      //g_Logger().Log("");
      return 0;
    }

    m_selectedPlatform = m_allPlatforms[platformIndex];
    return 0;
  }

  int
  Compute::getDevices() {
    cl_int out = 0;
    out = clGetDeviceIDs(m_selectedPlatform, CL_DEVICE_TYPE_ALL, 0, nullptr, &m_numDevices);
    m_allDevices.resize(m_numDevices);

    out = clGetDeviceIDs(m_selectedPlatform,
                         CL_DEVICE_TYPE_ALL,
                         m_numDevices,
                         &m_allDevices[0],
                         nullptr);

    m_selectedDevice = m_allDevices[0];

    return out;
  }

  String 
  Compute::getDeviceInfo() {
    String out;
    String device_name;
    String device_vendor;
    String driver_version;
    String device_profile;
    String device_version;
    String device_extensions;
    String device_platform;
    String device_double_fp_config;
    device_name.resize(255, '\0');
    device_vendor.resize(255, '\0');
    driver_version.resize(255, '\0');
    device_profile.resize(255, '\0');
    device_version.resize(255, '\0');
    device_extensions.resize(1024, '\0');
    device_platform.resize(1024, '\0');
    device_double_fp_config.resize(1024, '\0');

    clGetDeviceInfo(m_selectedDevice, CL_DEVICE_NAME, 255, &device_name[0], nullptr);
    clGetDeviceInfo(m_selectedDevice, CL_DEVICE_VENDOR, 255, &device_vendor[0], nullptr);
    clGetDeviceInfo(m_selectedDevice, CL_DRIVER_VERSION, 255, &driver_version[0], nullptr);
    clGetDeviceInfo(m_selectedDevice, CL_DEVICE_PROFILE, 255, &device_profile[0], nullptr);
    clGetDeviceInfo(m_selectedDevice, CL_DEVICE_VERSION, 255, &device_version[0], nullptr);
    clGetDeviceInfo(m_selectedDevice, 
                    CL_DEVICE_EXTENSIONS, 
                    1024, 
                    &device_extensions[0], 
                    nullptr);
    clGetDeviceInfo(m_selectedDevice, CL_DEVICE_PLATFORM, 1024, &device_platform[0], nullptr);
    clGetDeviceInfo(m_selectedDevice, 
                    CL_DEVICE_DOUBLE_FP_CONFIG, 
                    1024, 
                    &device_double_fp_config[0], 
                    nullptr);

    out += "Device name: " + device_name + "\n";
    out += "Device vendor: " + device_vendor + "\n";
    out += "Driver version: " + driver_version + "\n";
    out += "Device profile: " + device_profile + "\n";
    out += "Device version: " + device_version + "\n";
    out += "Device extensions: " + device_extensions + "\n";
    out += "Device platform: " + device_platform + "\n";
    out += "Device doble fp config: " + device_double_fp_config + "\n";

    return out;
  }

  String 
  Compute::getAllDevicesInfo() {
    String out;
    String device_name;
    String device_vendor;
    String driver_version;
    String device_profile;
    String device_version;
    String device_extensions;
    String device_platform;
    String device_double_fp_config;
    device_name.resize(255, '\0');
    device_vendor.resize(255, '\0');
    driver_version.resize(255, '\0');
    device_profile.resize(255, '\0');
    device_version.resize(255, '\0');
    device_extensions.resize(1024, '\0');
    device_platform.resize(1024, '\0');
    device_double_fp_config.resize(1024, '\0');

    for (auto& device : m_allDevices) {
      clGetDeviceInfo(device, CL_DEVICE_NAME, 255, &device_name[0], nullptr);
      clGetDeviceInfo(device, CL_DEVICE_VENDOR, 255, &device_vendor[0], nullptr);
      clGetDeviceInfo(device, CL_DRIVER_VERSION, 255, &driver_version[0], nullptr);
      clGetDeviceInfo(device, CL_DEVICE_PROFILE, 255, &device_profile[0], nullptr);
      clGetDeviceInfo(device, CL_DEVICE_VERSION, 255, &device_version[0], nullptr);
      clGetDeviceInfo(device,
                      CL_DEVICE_EXTENSIONS,
                      1024,
                      &device_extensions[0],
                      nullptr);
      clGetDeviceInfo(device, CL_DEVICE_PLATFORM, 1024, &device_platform[0], nullptr);
      clGetDeviceInfo(device,
                      CL_DEVICE_DOUBLE_FP_CONFIG,
                      1024,
                      &device_double_fp_config[0],
                      nullptr);

      out += "Device name: " + device_name + "\n";
      out += "Device vendor: " + device_vendor + "\n";
      out += "Driver version: " + driver_version + "\n";
      out += "Device profile: " + device_profile + "\n";
      out += "Device version: " + device_version + "\n";
      out += "Device extensions: " + device_extensions + "\n";
      out += "Device platform: " + device_platform + "\n";
      out += "Device doble fp config: " + device_double_fp_config + "\n";
    }

    return out;
  }

  int 
  Compute::createContext(cl_context_properties* cps) {
    cl_int out = 0;
    m_context = clCreateContext(cps, 1, &m_selectedDevice, nullptr, nullptr, &out);
    return 0;
  }

  bool 
  Compute::checkCurrentPlatformSupportsExtension(String extension) {
    String extension_string;
    cl_int status = 0;
    extension_string.resize(1024, '\0');
    getAllPlatformsInfo();
    for (auto& platform : m_allPlatforms) {
      status = clGetPlatformInfo(platform,
                                 CL_PLATFORM_EXTENSIONS,
                                 1024,
                                 &extension_string[0],
                                 NULL);
      size_t extStringStart;

      extStringStart = extension_string.find(extension, extension.size());;
      if (extStringStart == String::npos) {
        std::cout << "Platform doesn't support " << extension << std::endl;
        continue;
      }
      m_selectedPlatform = platform;
      break;
    }
    //status = clGetPlatformInfo(m_selectedPlatform,
    //                           CL_PLATFORM_EXTENSIONS,
    //                           sizeof(extension_string),
    //                           &extension_string[0],
    //                           NULL);
    //size_t extStringStart;
    //
    //extStringStart = extension_string.find(extension, extension.size());;
    //if (extStringStart == String::npos) {
    //  std::cout << "Platform doesn't support " << extension <<std::endl;
    //}
    std::cout << "Platform supports " << extension <<std::endl;
    return true;
  }

  int 
  Compute::createCommandqueue() {

    //String kernel_code2 = "void kernel Histogram(global const image2D_t myImage) {"
    //  "int id = get_global_id(0);"
    //  "for(int i=0; i< height[0]; i++) {"
    //  "int realId= i*width[0] + id;"
    //  "atomic_add(&output1[input1[realId]],1);"
    //  "atomic_add(&output2[input2[realId]],1);"
    //  "}"
    //  "};";
    ////
    //const char** code2 = (const char**)new char[1];
    //code2[0] = kernel_code2.c_str();
    //
    //size_t codeSize = kernel_code2.size();
    //cl_int error = 0;
    //cl_program program2 = clCreateProgramWithSource(_context, 1, code2, &codeSize, &error);
    //errOut(error);
    //
    //errOut(clBuildProgram(program2, 1, &_device, nullptr, nullptr, nullptr));
    //
    //cl_command_queue command_queue2 = clCreateCommandQueue(_context, _device, 0, &error);
    //errOut(error);
    //
    //cl_kernel Histogram = clCreateKernel(program2, "Histogram", &error);
    //errOut(error);
    //
    //cl_int status = 0;
    //m_commandqueue = clCreateCommandQueue(m_context, m_selectedDevice, 0, &status);
    //if (0 != status)
    //{
    //  g_Logger().Log("clCreateCommandQueue error");
    //  return status;
    //}
    return 0;
  }

  int 
  Compute::ShareDX11BufferWithCL(void* texture) {
    int status = 0;

    ptrToFunctionCreateFromD3D11Texture2DKHR = 
      (clCreateFromD3D11Texture2DKHR_fn)clGetExtensionFunctionAddressForPlatform(m_selectedPlatform, "clCreateFromD3D11Texture2DKHR");

    m_imagen = ptrToFunctionCreateFromD3D11Texture2DKHR(m_context,
                                                        CL_MEM_WRITE_ONLY, 
                                                        (ID3D11Texture2D*)texture,
                                                        0, 
                                                        &status);
    if (status == 0)
    {
      printf("Successfully shared!\n");
    }
    else
    {
      printf("Sharing failed\n");
    }
    return status;
  }

  int 
  Compute::getDevicesD3D11(void* device) {
    cl_int status = 0;
    ptrToFunctionGetDeviceIDsD3D11KHR =
      (clGetDeviceIDsFromD3D11NV_fn)clGetExtensionFunctionAddressForPlatform(m_selectedPlatform,
                                                                              "clGetDeviceIDsFromD3D11NV");
    
    cl_uint numDevs = 0;
    //careful with the g_pd3DDevice
    status = ptrToFunctionGetDeviceIDsD3D11KHR(m_selectedPlatform,
                                               CL_D3D11_DEVICE_NV,
                                               (void*)device,
                                               CL_PREFERRED_DEVICES_FOR_D3D11_NV,
                                               0,
                                               NULL,
                                               &numDevs);
    if (0 != status)
    {
      g_Logger().Log("Failed on clGetDeviceIDsFromD3D11KHR");
      return status;
    }
    cl_device_id* devID = NULL;
    m_allDevices.resize(numDevs);
    //m_allDevices = (cl_device_id*)malloc(sizeof(cl_device_id) * numDevs);
    ptrToFunctionGetDeviceIDsD3D11KHR(m_selectedPlatform, 
                                      CL_D3D11_DEVICE_NV, 
                                      (void*)device, 
                                       CL_PREFERRED_DEVICES_FOR_D3D11_NV, 
                                       numDevs, 
                                       &m_allDevices[0],
                                       NULL);
    m_selectedDevice = m_allDevices[0];
    return status;
  }

  JD_CORE_EXPORT Compute& 
  g_Compute() {
    return Compute::instance();
  }
}
