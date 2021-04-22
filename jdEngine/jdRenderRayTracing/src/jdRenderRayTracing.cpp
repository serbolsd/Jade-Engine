#include "jdRenderRayTracing.h"
#include <jdComponentTransform.h>
#include <jdComponentRenderModel.h>
#include <jdComponentLight.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <CL/cl.h>
using std::cout;
using std::endl;
namespace jdEngineSDK {
  RenderRayTracingApi::RenderRayTracingApi() {
  }

  RenderRayTracingApi::~RenderRayTracingApi() {
    delete []m_image;
    delete []m_pixels;
  }

  void 
  RenderRayTracingApi::init(void* wndHandle, const JDPoint& clientSize) {
    m_wndHandle = wndHandle;
    m_clientSize = clientSize;
    onCreate();
  }

  void 
  RenderRayTracingApi::onUpdate(const float& deltaTime) {
    m_elapseTime += deltaTime;

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    updateImagen();
    //if (m_elapseTime >6)
    //{
    //  m_elapseTime = 0;
    //}
    //else
    //{
    //  return;
    //}
   


  }

  void 
  RenderRayTracingApi::onRender() {
    g_graphicsApi().setRenderTarget(m_rtv);
    g_graphicsApi().setViewPort(m_viewPort);
    g_graphicsApi().Clear(m_rtv, 1, 1, 1, 1);
    g_graphicsApi().ClearDepthStencil(m_rtv);
    
    ImGui::Begin("Test", NULL);
    ImGui::Image(m_testRT->getTexture(), ImVec2(700, 500));
    ImGui::End();

    ImGui::Begin("Noise", NULL); 
    ImGui::Image(m_testNoise->getTexture(), ImVec2(700, 500));
    if (ImGui::Button("Reset Noise")) {
      ResetNoise();
    }
    ImGui::DragInt("Noise scale", &m_NoiseScale, 1.0f, 1);
    ImGui::DragInt("Noise Multiply", &m_NoiseMultiply, 1.0f, 1);
    ImGui::End();

    ImGui::Begin("Inspector", NULL);
    auto pos = m_debugCam->getPositionVector();
    JDVector3 origin = { pos.x, pos.y, pos.z };

    ImGui::InputFloat3("View Position", &origin.x, 2);
    ImGui::Separator();

    ImGui::DragInt("Depth Of Ray", &m_depthOfRay, 1.0f, 1, 20);
    ImGui::DragInt("PixelPerFrame", &m_pixelPerFrame, 1.0f, 1, 10000);
    ImGui::Separator();
    auto sp = &m_spheres[1];
    if (ImGui::TreeNode("Spheres")) {
      ImGui::Text("Spehere1");
      ImGui::DragFloat3("Position##1", &sp->center[0]);
      ImGui::DragFloat("Radius##1", &sp->radius, 0.20f, 0.2f,100.0f);
      sp->radius2 = sp->radius * sp->radius;
      ImGui::ColorEdit3("Color##1", &sp->surfaceColor[0]);
      ImGui::DragFloat("Reflection##1", &sp->reflection, 0.01f, 0.0f, 1.0f);
      ImGui::DragFloat("Trasparency##1", &sp->transparency, 0.01f, 0.0f, 1.0f);
      ImGui::ColorEdit3("Emission color##1", &sp->emissionColor[0]);
      ImGui::Separator();

      sp = &m_spheres[2];
      ImGui::Text("Spehere2");
      ImGui::DragFloat3("Position##2", &sp->center[0]);
      ImGui::DragFloat("Radius##2", &sp->radius, 0.20f, 0.2f, 100.0f);
      sp->radius2 = sp->radius * sp->radius;
      ImGui::ColorEdit3("Color##2", &sp->surfaceColor[0]);
      ImGui::DragFloat("Reflection##2", &sp->reflection, 0.01f, 0.0f, 1.0f);
      ImGui::DragFloat("Trasparency##2", &sp->transparency, 0.01f, 0.0f, 1.0f);
      ImGui::ColorEdit3("Emission color##2", &sp->emissionColor[0]);
      ImGui::Separator();

      sp = &m_spheres[3];
      ImGui::Text("Spehere3");
      ImGui::DragFloat3("Position##3", &sp->center[0]);
      ImGui::DragFloat("Radius##3", &sp->radius, 0.20f, 0.2f, 100.0f);
      sp->radius2 = sp->radius * sp->radius;
      ImGui::ColorEdit3("Color##3", &sp->surfaceColor[0]);
      ImGui::DragFloat("Reflection##3", &sp->reflection, 0.01f, 0.0f, 1.0f);
      ImGui::DragFloat("Trasparency##3", &sp->transparency, 0.01f, 0.0f, 1.0f);
      ImGui::ColorEdit3("Emission color##3", &sp->emissionColor[0]);
      ImGui::Separator();

      sp = &m_spheres[4];
      ImGui::Text("Spehere4");
      ImGui::DragFloat3("Position##4", &sp->center[0]);
      ImGui::DragFloat("Radius##4", &sp->radius, 0.20f, 0.2f, 100.0f);
      sp->radius2 = sp->radius * sp->radius;
      ImGui::ColorEdit3("Color##4", &sp->surfaceColor[0]);
      ImGui::DragFloat("Reflection##4", &sp->reflection, 0.01f, 0.0f, 1.0f);
      ImGui::DragFloat("Trasparency##4", &sp->transparency, 0.01f, 0.0f, 1.0f);
      ImGui::ColorEdit3("Emission color##4", &sp->emissionColor[0]);
      ImGui::Separator();
      ImGui::TreePop();
    }
    

    sp = &m_spheres[0];
    ImGui::Text("Floor");
    ImGui::DragFloat3("Position##5", &sp->center[0]);
    ImGui::DragFloat("Radius##5", &sp->radius, 1.0f, 0.2f, 10000.0f);
    sp->radius2 = sp->radius * sp->radius;
    ImGui::ColorEdit3("Color##5", &sp->surfaceColor[0]);
    ImGui::DragFloat("Reflection##5", &sp->reflection, 0.01f, 0.0f, 1.0f);
    ImGui::DragFloat("Trasparency##5", &sp->transparency, 0.01f, 0.0f, 1.0f);
    ImGui::ColorEdit3("Emission color##5", &sp->emissionColor[0]);
    ImGui::Separator();

    sp = &m_spheres[5];
    ImGui::Text("Light");
    ImGui::DragFloat3("Position##6", &sp->center[0]);
    ImGui::DragFloat("Radius##6", &sp->radius, 0.20f, 0.2f, 100.0f);
    sp->radius2 = sp->radius * sp->radius;
    ImGui::ColorEdit3("Color##6", &sp->surfaceColor[0]);
    ImGui::DragFloat("Reflection##6", &sp->reflection, 0.01f, 0.0f, 1.0f);
    ImGui::DragFloat("Trasparency##6", &sp->transparency, 0.01f, 0.0f, 1.0f);
    ImGui::ColorEdit3("Emission color##6", &sp->emissionColor[0]);
    ImGui::Separator();

    if (ImGui::Button("Update Imagen")) {
      updateImagen();
    }
    ImGui::End();


    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
  }

  void 
  RenderRayTracingApi::onDestroy() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
  }

  void 
  RenderRayTracingApi::onResize(int32 width, int32 height) {
    if (!*m_windowHasFocus)
    {
      return;
    }
    m_rtv = g_graphicsApi().getRenderTargetView();
    g_graphicsApi().setRenderTarget(m_rtv);
    g_CameraMan().resizeCameraProjection(width, height);
    m_debugCam->resize(width, height);

    ImGui::GetIO().DisplaySize = ImVec2((float)width, (float)height);
    ImGui::GetIO().DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    m_viewPort.Width = (float)width;
    m_viewPort.Height = (float)height;
    m_viewPort.MinDepth = 0.0f;
    m_viewPort.MaxDepth = 1.0f;
  }

  void 
  RenderRayTracingApi::onMouseButtonPressed(int32 button, int32 x, int32 y) {
    ImGuiIO& io = ImGui::GetIO();
    if (!ImGui::IsAnyMouseDown() && nullptr == ::GetCapture())
    {
      ::SetCapture((HWND)m_wndHandle);
    }
    io.MouseDown[static_cast<int32>(button)] = true;
  }

  void 
  RenderRayTracingApi::onTextEnterd(UNICHAR unicode) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacterUTF16((unsigned short)unicode);
  }

  void 
  RenderRayTracingApi::onKeyPressed(int32 code, 
                                       bool alt, 
                                       bool 
                                       control, 
                                       bool shift, 
                                       bool system) {
    ImGuiIO& io = ImGui::GetIO();
    if (code >= 0)
    {
      io.KeysDown[static_cast<int32>(code)] = true;
    }

  }

  void 
  RenderRayTracingApi::onKeyReleased(int32 code, 
                                        bool alt, 
                                        bool control, 
                                        bool shift, 
                                        bool system) {
    ImGuiIO& io = ImGui::GetIO();
    if (code >= 0)
    {
      io.KeysDown[static_cast<int32>(code)] = false;
    }
  }

  void 
  RenderRayTracingApi::onMouseButtonReleased(int32 button, int32 x, int32 y) {
    ImGuiIO& io = ImGui::GetIO();
    if (!ImGui::IsAnyMouseDown() && ::GetCapture() == m_wndHandle)
    {
      ::ReleaseCapture();
    }
    io.MouseDown[static_cast<int32>(button)] = false;
  }

  void 
  RenderRayTracingApi::onCreate() {
    //Get render target view created when start up the graphic api
    m_rtv = g_graphicsApi().getRenderTargetView();

    // Set ViewPort data
    m_viewPort.Width = (float)m_clientSize.x;
    m_viewPort.Height = (float)m_clientSize.y;
    m_viewPort.MinDepth = 0.0f;
    m_viewPort.MaxDepth = 1.0f;
    m_viewPort.m_topLeftX = 0;
    m_viewPort.m_topLeftY = 0;
    //Set ViewPort
    g_graphicsApi().setViewPort(m_viewPort);
    //Set Render Target View
    g_graphicsApi().setRenderTarget(m_rtv);

    //Load the shaders
    //Load the basic shader
    m_PSBasic = g_graphicsApi().loadShaderFromFile("data/shader/ShaderTest/Basic.fx",
                                                   "BASIC_VS",
                                                   "vs_5_0",
                                                   "data/shader/ShaderTest/Basic.fx",
                                                   "BASIC_PS",
                                                   "ps_5_0");

    //Set forward Shader
    g_graphicsApi().setProgramShader(m_PSBasic);

    //Create an Input Layout with a reflect of Forware Shader
    m_inLayOut = g_graphicsApi().reflectInputLayout(m_PSBasic.get()->getVertexShader());

    //Set the Input Layout
    g_graphicsApi().setInputLayout(m_inLayOut);


    // Get the default Camera
    m_debugCam = g_CameraMan().getDebugCamera();

    //Set Primitive topology
    g_graphicsApi().setPrimitiveTopology(PRIMITIVE_TOPOLOGY_FORMAT::TRIANGLELIST);

    m_image = new unsigned char[m_imgWidth * m_imgHeight * 4] {0};
    m_pixels = new JDVector3[m_imgWidth * m_imgHeight];

    m_pixelOrder.resize(m_imgWidth * m_imgHeight);

    int32 in = 0;
    for (int32 y = 0; y <m_imgHeight; ++y) {
      for (int32 x = 0; x < m_imgWidth; x++) {
        m_pixelOrder[in] = { x,y };
        ++in;
      }
    }
    m_pixelCount = 0;
    m_currentPixelCount = 0;
    m_maxPixelCount = m_imgWidth * m_imgHeight;

    m_invImgWidth = 1 / float(m_imgWidth);
    m_invImgHeight = 1 / float(m_imgHeight);
      
    //m_debugCam->getAspectRatio();
    m_fov = 30;
    m_aspectratio = m_imgWidth / float(m_imgHeight);
    m_aspectratio = m_debugCam->getAspectRatio();
    m_angle = Math::tan(M_PI * 0.5f * m_fov / 180.0f);
    m_angle = m_debugCam->getFovAngle().valueRadians();
    // position, radius, surface color, reflectivity, transparency, emission color
    //Floor
    m_spheres.push_back(jdRTSphere(JDVector3(0.0, -10004, -20), 10000, JDVector3(0.20, 0.20, 0.20), 0, 1.0));
    //Visible Spheres
    m_spheres.push_back(jdRTSphere(JDVector3(0.0, 0, -20), 4, JDVector3(1.00, 0.32, 0.36), 0.5, 0.2));
    m_spheres.push_back(jdRTSphere(JDVector3(5.0, -1, -15), 2, JDVector3(0.90, 0.76, 0.46), 1, 0.0));
    m_spheres.push_back(jdRTSphere(JDVector3(5.0, 0, -25), 3, JDVector3(0.65, 0.77, 0.97), 1, 0.0));
    m_spheres.push_back(jdRTSphere(JDVector3(-5.5, 0, -15), 3, JDVector3(0.90, 0.90, 0.90), 1, 0.0));
    // light
    m_spheres.push_back(jdRTSphere(JDVector3(0.0, 20, -30), 3, JDVector3(0.00, 0.00, 0.00), 0, 0.0, JDVector3(3,3,3)));
    

    uint32 index = 0;
    uint32 imgIndex = 0;
    for (uint32 y = 0; y < m_imgHeight; ++y) {
      for (uint32 x = 0; x < m_imgWidth; ++x) {
        float xx = (2 * ((x + 0.5) * m_invImgWidth) - 1) * m_angle * m_aspectratio;
        float yy = (1 - 2 * ((y + 0.5) * m_invImgHeight)) * m_angle;
        JDVector3 raydir(xx, yy, -1);
        raydir.normalize();
        index = y * m_imgWidth + x;
        auto pos = m_debugCam->getPositionVector();
        JDVector3 origin = { pos.x, pos.y, pos.z };
        m_pixels[index] = Trace(origin, raydir, m_spheres, 0);
        imgIndex = index * 4;
        m_image[imgIndex] = (unsigned char)(Math::Min(float(1), m_pixels[index].x) * 255);
        m_image[imgIndex + 1] = (unsigned char)(Math::Min(float(1), m_pixels[index].y) * 255);
        m_image[imgIndex + 2] = (unsigned char)(Math::Min(float(1), m_pixels[index].z) * 255);
        m_image[imgIndex + 3] = 255;
      }
    }
    m_testRT = g_graphicsApi().CreatTextureFromArray(m_image, m_imgWidth, m_imgHeight, 4);


    m_imgNoise = new unsigned char[m_imgWidth * m_imgHeight * 4];
    ResetNoise();
    initImGui();
  }

  void 
  RenderRayTracingApi::initImGui() { 
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplWin32_Init(m_wndHandle);
    ImGui_ImplDX11_Init(reinterpret_cast<ID3D11Device*>(g_graphicsApi().getDevice()),
      reinterpret_cast<ID3D11DeviceContext*>(g_graphicsApi().getDeviceContex()));

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
      //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigFlags |= ImGuiDockNodeFlags_PassthruCentralNode;   
    //io.ConfigFlags |= ImGuiBackendFlags_HasMouseCursors;       // Enable Keyboard Controls
    //io.ConfigWindowsResizeFromEdges = true;         // Enable Multi-Viewport / Platform Windows
    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      style.WindowRounding = 0.0f;
      style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    //ImGui::FileBrowser fileDialog(ImGuiFileBrowserFlags_CloseOnEsc);
    //m_fileDialog = fileDialog;
    // (optional) set browser properties
    //m_fileDialog.reset(new ImGui::FileBrowser(ImGuiFileBrowserFlags_CloseOnEsc |
    //                                          ImGuiFileBrowserFlags_MultipleSelection));
    //m_fileDialog->SetTitle("LoadResource");
  }

  void 
  RenderRayTracingApi::imguiDockScreen() {
  }

  JDVector3 
  RenderRayTracingApi::Trace(const JDVector3& rayorig, 
                             const JDVector3& raydir, 
                             const Vector<jdRTSphere>& spheres, 
                             const int& depth) {
    float tnear = INFINITY;
    const jdRTSphere* sphere = NULL;

    for (unsigned i = 0; i < spheres.size(); ++i) {
      float t0 = INFINITY, t1 = INFINITY;
      if (spheres[i].intersect(rayorig, raydir, t0, t1)) {
        if (t0 < 0) t0 = t1;
        if (t0 < tnear) {
          tnear = t0;
          sphere = &spheres[i];
        }
      }
    }

    if (!sphere) return JDVector3(2,2,2);
    // color of the ray/surfaceof the object intersected by the ray 
    JDVector3 surfaceColor = { 0,0,0 }; 
    // point of intersection 
    JDVector3 phit = rayorig + raydir * tnear;
    // normal at the intersection point 
    JDVector3 nhit = phit - sphere->center; 
    // normalize normal direction
    nhit.normalize();  
    // If the normal and the view direction are not opposite to each other
    // reverse the normal direction. That also means we are inside the sphere so set
    // the inside bool to true. Finally reverse the sign of IdotN which we want
    // positive.
    float bias = 1e-4; // add some bias to the point from which we will be tracing 
    bool inside = false;
    if (raydir.dot(nhit) > 0) {
      nhit = -nhit;
      inside = true;
    }

    if ((sphere->transparency > 0 || sphere->reflection > 0) && depth < m_depthOfRay) {
      float facingratio = -raydir.dot(nhit);
      // change the mix value to tweak the effect
      float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1);
      // compute reflection direction (not need to normalize because all vectors
      // are already normalized)
      JDVector3 refldir = raydir - nhit * 2 * raydir.dot(nhit);
      refldir.normalize();
      JDVector3 reflection = Trace(phit + nhit * bias, refldir, spheres, depth + 1);
      JDVector3 refraction = {0,0,0};
      reflection *= sphere->reflection;
      // if the sphere is also transparent compute refraction ray (transmission)
      if (sphere->transparency) {
        float ior = 1.1, eta = (inside) ? ior : 1 / ior; // are we inside or outside the surface? 
        float cosi = (float)-nhit.dot(raydir);
        float k = 1 - (float)eta * (float)eta * (float)(1 - cosi * cosi);
        if (k < 0) {
          k = 0;
        }
        JDVector3 newDir = raydir * (float)eta;
        JDVector3 hitDir = nhit * (float)(eta * cosi - Math::sqrt(k));
        JDVector3 refrdir = newDir + hitDir;
        refrdir.normalize();
        refraction = Trace(phit - nhit * bias, refrdir, spheres, depth + 1);
      }
      // the result is a mix of reflection and refraction (if the sphere is transparent)
      surfaceColor = (
        reflection * fresneleffect +
        refraction * (1 - fresneleffect) * sphere->transparency) * sphere->surfaceColor;
    }
    else {
     
    }

    // it's a diffuse object, no need to raytrace any further
    for (unsigned i = 0; i < spheres.size(); ++i) {
      if (spheres[i].emissionColor.x > 0) {
        // this is a light
        JDVector3 transmission = { 1,1,1 };
        JDVector3 lightDirection = spheres[i].center - phit;
        lightDirection.normalize();
        for (unsigned j = 0; j < spheres.size(); ++j) {
          if (i != j) {
            float t0, t1;
            if (spheres[j].intersect(phit + nhit * bias, lightDirection, t0, t1)) {
              transmission = { 0,0,0 };
              break;
            }
          }
        }
        surfaceColor += sphere->surfaceColor * transmission *
          std::max(float(0), nhit.dot(lightDirection)) * spheres[i].emissionColor;
      }
    }

    return surfaceColor + sphere->emissionColor;
  }

  void 
  RenderRayTracingApi::updateImagen() {


    uint32 index = 0;
    uint32 imgIndex = 0;
    m_currentPixelCount = m_pixelCount + m_pixelPerFrame;
    for (int32 i = m_pixelCount;i < m_currentPixelCount; ++i,++m_pixelCount) {
      if (m_pixelCount >= m_maxPixelCount)
      {
        m_pixelCount = 0;
        break;
      }
      int32 x = m_pixelOrder[m_pixelCount].x;
      int32 y = m_pixelOrder[m_pixelCount].y;

      float xx = (2 * ((x + 0.5) * m_invImgWidth) - 1) * m_angle * m_aspectratio;
      float yy = (1 - 2 * ((y + 0.5) * m_invImgHeight)) * m_angle;
      JDVector3 raydir(xx,yy,1);
      JDVector3 raydirC(xx,yy,1);
      raydir *= m_debugCam->getFrontVector();

      raydir += raydirC;
      raydir.normalize();
      index = y * m_imgWidth + x;
      auto pos = m_debugCam->getPositionVector();
      JDVector3 origin = { pos.x, pos.y, pos.z };
      m_pixels[index] = Trace(origin, raydir, m_spheres, 0);
      imgIndex = index * 4;
      m_image[imgIndex] = (unsigned char)(Math::Min(float(1), m_pixels[index].x) * 255);
      m_image[imgIndex + 1] = (unsigned char)(Math::Min(float(1), m_pixels[index].y) * 255);
      m_image[imgIndex + 2] = (unsigned char)(Math::Min(float(1), m_pixels[index].z) * 255);
      m_image[imgIndex + 3] = 255;
      
    }

    //V1
    //for (uint32 y = 0; y < m_imgHeight; ++y) {
    //  for (uint32 x = 0; x < m_imgWidth; ++x) {
    //    float xx = (2 * ((x + 0.5) * m_invImgWidth) - 1) * m_angle * m_aspectratio;
    //    float yy = (1 - 2 * ((y + 0.5) * m_invImgHeight)) * m_angle;
    //    JDVector3 raydir(xx, yy, -1);
    //    raydir.normalize();
    //    index = y * m_imgWidth + x;
    //    auto pos = m_debugCam->getPositionVector();
    //    JDVector3 origin = { pos.x, pos.y, pos.z };
    //    m_pixels[index] = Trace(origin, raydir, m_spheres, 0);
    //    imgIndex = index * 4;
    //    m_image[imgIndex] = (unsigned char)(Math::Min(float(1), m_pixels[index].x) * 255);
    //    m_image[imgIndex + 1] = (unsigned char)(Math::Min(float(1), m_pixels[index].y) * 255);
    //    m_image[imgIndex + 2] = (unsigned char)(Math::Min(float(1), m_pixels[index].z) * 255);
    //    m_image[imgIndex + 3] = 255;
    //  }
    //}
    m_testRT = g_graphicsApi().CreatTextureFromArray(m_image, m_imgWidth, m_imgHeight, 4);
  }

  void 
  RenderRayTracingApi::ResetNoise() {
    uint32 index = 0;
    uint32 imgIndex = 0;
    uint32 iSecret;

    /* initialize random seed: */
    srand(time(NULL));

    /* generate secret number between 1 and 10: */
    iSecret = rand() % 10 + 1;

    m_pn.reset(iSecret);
    for (uint32 y = 0; y < m_imgHeight; ++y) {
      for (uint32 x = 0; x < m_imgWidth; ++x) {
        float xx = (float)x / (float)m_imgWidth;
        float yy = (float)y / (float)m_imgHeight;

        float n = m_pn.noise(m_NoiseScale * xx, m_NoiseScale * yy, 0.8f);

        //float n = m_NoiseMultiply * m_pn.noise(xx, yy, 0.8);
        n = n - floor(n);

        index = y * m_imgWidth + x;
        imgIndex = index * 4;
        m_imgNoise[imgIndex] = floor(255 * n);
        m_imgNoise[imgIndex + 1] = floor(255 * n);
        m_imgNoise[imgIndex + 2] = floor(255 * n);
        m_imgNoise[imgIndex + 3] = 255;
      }
    }
    m_testNoise = g_graphicsApi().CreatTextureFromArray(m_imgNoise, m_imgWidth, m_imgHeight, 4);

  }


}