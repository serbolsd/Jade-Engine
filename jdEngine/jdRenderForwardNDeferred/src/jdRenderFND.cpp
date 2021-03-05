#include "jdRenderFND.h"
#include <jdComponentTransform.h>
#include <jdComponentRenderModel.h>
#include <jdComponentLight.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <CL/cl.h>
using std::cout;
using std::endl;
namespace jdEngineSDK {
  RenderFNDApi::RenderFNDApi() {
  }

  RenderFNDApi::~RenderFNDApi() {
    onDestroy();
  }

  void 
  RenderFNDApi::init(void* wndHandle, const JDPoint& clientSize) {
    m_wndHandle = wndHandle;
    m_clientSize = clientSize;
    onCreate();
    //LoadHistogram();
    //initImGui();
    g_Logger().Log("TODO CHIDO");
    float num1, num2;
    Random::RandomBoxMuller(num1, num2, 5, 10);
    g_Logger().Log("Number 1 is : %d Number 12 is : % d",num1,num2);
  }

  void RenderFNDApi::onUpdate(const float& deltaTime) {
    UpdateCameraInterpolate(deltaTime);
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
  }

  void 
  RenderFNDApi::onRender() {
    imguiDockScreen();
    imguiSceneGraph();
    imGuiInspectorObject();
    if (m_addigCamera)
    {
      addCameraComponent();
    }
    showTexturesResources();
    showModels();
    showAmbientOption();
    showAmbientOclusionOption();
    showBrightOption();
    showBlurOption();
    showCameraInterpolateMenu();
    showHistorgramOption();

    if (m_loadingFile)
    {
      imGuiLoadResourceFile();
    }

    changeCameraDataBuffer(m_debugCam);
    renderForward(m_RTForward);
    renderDeferred();

    g_graphicsApi().setRenderTarget(m_rtv);
    g_graphicsApi().setViewPort(m_viewPort);
    g_graphicsApi().Clear(m_rtv, 0, 0, 0, 1);
    g_graphicsApi().ClearDepthStencil(m_rtv);
    ImGui::Begin("Scene Window",
                 nullptr,
                 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImVec2 wsize = ImGui::GetWindowSize();
    wsize.y -= 30;
    JDPoint size = { (int32)wsize.x, (int32)wsize.y };
    if (m_firstFrame) {
      onResizeSceneWindow(size.x, size.y);
      m_firstFrame = false;
      m_sceneSize = size;
    }
    if (m_sceneSize != size) {
      onResizeSceneWindow(size.x, size.y);
      m_sceneSize = size;
    }
    ImGui::Image(m_RTForward.get()->getRenderTexture(), wsize);
    ImGui::End();
    
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

  }

  void 
  RenderFNDApi::onDestroy() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
  }

  void 
  RenderFNDApi::onResize(int32 width, int32 height) {
    ImGui::GetIO().DisplaySize = ImVec2((float)width, (float)height);
    ImGui::GetIO().DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    m_viewPort.Width = (float)width;
    m_viewPort.Height = (float)height;
    m_viewPort.MinDepth = 0.0f;
    m_viewPort.MaxDepth = 1.0f;
    m_viewPortScene = m_viewPort;
  }

  void 
  RenderFNDApi::onMouseButtonPressed(int32 button, int32, int32) {
    ImGuiIO& io = ImGui::GetIO();
    if (!ImGui::IsAnyMouseDown() && nullptr == ::GetCapture())
    {
      ::SetCapture((HWND)m_wndHandle);
    }
    io.MouseDown[static_cast<int32>(button)] = true;
  }

  void 
  RenderFNDApi::onTextEnterd(UNICHAR unicode) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacterUTF16((unsigned short)unicode);
  }

  void 
  RenderFNDApi::onKeyPressed(int32 code, bool alt, bool control, bool shift, bool system) {
    ImGuiIO& io = ImGui::GetIO();
    if (code >= 0)
    {
      io.KeysDown[static_cast<int32>(code)] = true;
    }
  }

  void 
  RenderFNDApi::onKeyReleased(int32 code, bool alt, bool control, bool shift, bool system) {
    ImGuiIO& io = ImGui::GetIO();
    if (code >= 0)
    {
      io.KeysDown[static_cast<int32>(code)] = false;
    }
  }

  void 
  RenderFNDApi::onMouseButtonReleased(int32 button, int32 x, int32 y) {
    ImGuiIO& io = ImGui::GetIO();
    if (!ImGui::IsAnyMouseDown() && ::GetCapture() == m_wndHandle)
    {
      ::ReleaseCapture();
    }
    io.MouseDown[static_cast<int32>(button)] = false;
  }

  void
  RenderFNDApi::onCreate() {
    //Get render target view created when start up the graphic api
    m_rtv = g_graphicsApi().getRenderTargetView();

    //Create RenderTargets
    //Create forward render target
    m_RTForward = g_graphicsApi().createRenderTarget((uint32)m_clientSize.x, 
                                                     (uint32)m_clientSize.y, 
                                                     5, // number of mip
                                                     true); //have depth
    //Create camera render target
    m_RTcamera = g_graphicsApi().createRenderTarget((uint32)m_clientSize.x, 
                                                    (uint32)m_clientSize.y, 
                                                    5, 
                                                    true);
    //Create camera interpolate render target
    m_RTcameraInterpolate = g_graphicsApi().createRenderTarget((uint32)m_clientSize.x, 
                                                               (uint32)m_clientSize.y, 
                                                               5, 
                                                               true);
    //Create gemoetry render target
    m_RTGeometry = g_graphicsApi().createRenderTarget((uint32)m_clientSize.x, 
                                                      (uint32)m_clientSize.y, 
                                                      5, 
                                                      true, 
                                                      4);
    //Create lights render target
    m_RTLights = g_graphicsApi().createRenderTarget((uint32)m_clientSize.x, 
                                                    (uint32)m_clientSize.y, 
                                                    5, 
                                                    true, 
                                                    4);
    //Create Ambient Oclusion render target
    m_RTAmbientOclusion = g_graphicsApi().createRenderTarget((uint32)m_clientSize.x, 
                                                             (uint32)m_clientSize.y, 
                                                             5, 
                                                             false, 
                                                             1);
    //Create Ambient Oclusion Blured render target
    m_RTAOclusionBlured = g_graphicsApi().createRenderTarget((uint32)m_clientSize.x, 
                                                             (uint32)m_clientSize.y, 
                                                             5, 
                                                             false, 
                                                             1);
    //Create Luminance render target
    m_RTLuminance = g_graphicsApi().createRenderTarget((uint32)m_clientSize.x, 
                                                       (uint32)m_clientSize.y, 
                                                       5, 
                                                       false, 
                                                       1);
    //Create Bright render target
    m_RTBright = g_graphicsApi().createRenderTarget((uint32)m_clientSize.x, 
                                                    (uint32)m_clientSize.y, 
                                                    5, 
                                                    false, 
                                                    1);
    //Create Color Ambient Oclusion render target
    m_RTColorAO = g_graphicsApi().createRenderTarget((uint32)m_clientSize.x, 
                                                     (uint32)m_clientSize.y, 
                                                     5, 
                                                     false, 
                                                     1);
    //Create BlurH render target
    m_RTBlurH = g_graphicsApi().createRenderTarget((uint32)(m_clientSize.x), 
                                                   (uint32)(m_clientSize.y), 
                                                   5, 
                                                   false, 
                                                   1,
                                                   0.5);
    //Create BlurV render target
    m_RTBlurV = g_graphicsApi().createRenderTarget((uint32)(m_clientSize.x), 
                                                   (uint32)(m_clientSize.y), 
                                                   5, 
                                                   false, 
                                                   1,
                                                   0.5);

    //Create AddBright render target
    m_RTAddBright = g_graphicsApi().createRenderTarget((uint32)(m_clientSize.x),
                                                       (uint32)(m_clientSize.y), 
                                                       5, 
                                                       false, 
                                                       1); 

    //Create BlurAdded render target
    m_RTBlurAdded = g_graphicsApi().createRenderTarget((uint32)(m_clientSize.x),
                                                       (uint32)(m_clientSize.y), 
                                                       5, 
                                                       false, 
                                                       1); 
    //Create ToneMap render target
    m_RTToneMap = g_graphicsApi().createRenderTarget((uint32)(m_clientSize.x),
                                                       (uint32)(m_clientSize.y), 
                                                       5, 
                                                       false, 
                                                       1);
    //Create ToneMap render target
    m_RTShadowMap = g_graphicsApi().createRenderTarget((uint32)(m_clientSize.x),
                                                       (uint32)(m_clientSize.y), 
                                                       5, 
                                                       false, 
                                                       1);

    //Create ToneMap Final target
    m_RTFinal = g_graphicsApi().createRenderTarget((uint32)(m_clientSize.x),
                                                    (uint32)(m_clientSize.y), 
                                                    5, 
                                                    false, 
                                                    1);

    // Set ViewPort data
    m_viewPort.Width = (float)m_clientSize.x;
    m_viewPort.Height = (float)m_clientSize.y;
    m_viewPort.MinDepth = 0.0f;
    m_viewPort.MaxDepth = 1.0f;
    m_viewPort.m_topLeftX = 0;
    m_viewPort.m_topLeftY = 0;
    m_viewPortScene = m_viewPort;
    //Set ViewPort
    g_graphicsApi().setViewPort(m_viewPort);
    //Set Render Target View
    g_graphicsApi().setRenderTarget(m_rtv);

    //Load the shaders
    //Load the forward shader
    m_PSForward = g_graphicsApi().loadShaderFromFile("data/shader/TestCubeMapMultiLight.fx",
                                                     "VS",
                                                     "vs_5_0",
                                                     "data/shader/TestCubeMapMultiLight.fx",
                                                     "PS",
                                                     "ps_5_0");
    //Load wireFrame Shader
    m_PSWireFrame = g_graphicsApi().loadShaderFromFile("data/shader/wireFrameShader.fx",
                                                       "VS",
                                                       "vs_5_0",
                                                       "data/shader/wireFrameShader.fx",
                                                       "PS",
                                                       "ps_5_0");
    
    //Load Geometry Shader
    m_PSGeometry = g_graphicsApi().loadShaderFromFile("data/shader/TestDeferred.fx",
                                                      "VS",
                                                      "vs_5_0",
                                                      "data/shader/TestDeferred.fx",
                                                      "PS",
                                                      "ps_5_0");
    //Load Light Shader
    m_PSLights = g_graphicsApi().loadShaderFromFile("data/shader/LightsPass.fx",
                                                            "VS",
                                                            "vs_5_0",
                                                            "data/shader/LightsPass.fx",
                                                            "PS",
                                                            "ps_5_0");
    //Load Ambient Oclsuion Shader
    m_PSAOclusion = g_graphicsApi().loadShaderFromFile("data/shader/TestAmbientOclusion.fx",
                                                       "VS",
                                                       "vs_5_0",
                                                       "data/shader/TestAmbientOclusion.fx",
                                                       "PS",
                                                       "ps_5_0");
    //Load Luminance Shader
    m_PSLuminance = g_graphicsApi().loadShaderFromFile("data/shader/Luminance.fx",
                                                       "VS",
                                                       "vs_5_0",
                                                       "data/shader/Luminance.fx",
                                                       "PS",
                                                       "ps_5_0");
    //Load Bright Shader
    m_PSBright = g_graphicsApi().loadShaderFromFile("data/shader/Birght.fx",
                                                    "VS",
                                                    "vs_5_0",
                                                    "data/shader/Birght.fx",
                                                    "PS",
                                                    "ps_5_0"); 
    //Load Color Ambient Oclusion Shader
    m_PSBColorAO = g_graphicsApi().loadShaderFromFile("data/shader/ColorWithAO.fx",
                                                      "VS",
                                                      "vs_5_0",
                                                      "data/shader/ColorWithAO.fx",
                                                      "PS",
                                                      "ps_5_0");
    //Load Color Ambient Oclusion Shader
    m_PSToneMap = g_graphicsApi().loadShaderFromFile("data/shader/toneMap.fx",
                                                      "VS",
                                                      "vs_5_0",
                                                      "data/shader/toneMap.fx",
                                                      "toneMap",
                                                      "ps_5_0");
      //Create a define for a blurH shader
    SPtr<SHADER_DEFINES> Sdefines(new SHADER_DEFINES);
      //Add the define
    Sdefines->addDefine("HORIZONTAL");
    //Load Horizontal Blur Shader
    m_PSBBlurH = g_graphicsApi().loadShaderFromFile("data/shader/BlurAndAdd.fx",
                                                    "VS",
                                                    "vs_5_0",
                                                    "data/shader/BlurAndAdd.fx",
                                                    "Blur",
                                                    "ps_5_0", Sdefines);
    //Load Vertical Blur Shader
    m_PSBBlurV = g_graphicsApi().loadShaderFromFile("data/shader/BlurAndAdd.fx",
                                                    "VS",
                                                    "vs_5_0",
                                                    "data/shader/BlurAndAdd.fx",
                                                    "Blur",
                                                    "ps_5_0");
    //Load Addition Shader
    m_PSAdd = g_graphicsApi().loadShaderFromFile("data/shader/BlurAndAdd.fx",
                                                 "VS",
                                                 "vs_5_0",
                                                 "data/shader/BlurAndAdd.fx",
                                                 "Add",
                                                 "ps_5_0");

    //Load Addition Shader
    m_PSShadowMap = g_graphicsApi().loadShaderFromFile("data/shader/shadowMap.fx",
                                                       "VS",
                                                       "vs_5_0",
                                                       "data/shader/shadowMap.fx",
                                                       "ShadowMap",
                                                       "ps_5_0");

    //Load geomtry Shader
    m_PSBillboard = g_graphicsApi().loadShaderFromFile("data/shader/BillBoard.fx",
                                                       "VS_Billboard",
                                                       "vs_5_0",
                                                       "data/shader/BillBoard.fx",
                                                       "PS_Billboard",
                                                       "ps_5_0");

    m_GSBillboard = g_graphicsApi().loadGeometryShaderFromFile("data/shader/BillBoard.fx",
                                                               "GS_Billboard",
                                                               "gs_5_0");



    //Set forward Shader
    g_graphicsApi().setProgramShader(m_PSForward);

    //Create an Input Layout with a reflect of Forware Shader
    m_inLayOut = g_graphicsApi().reflectInputLayout(m_PSForward.get()->getVertexShader());
    m_inLayOutBillBoard = g_graphicsApi().reflectInputLayout(m_PSBillboard->getVertexShader());

    //Set the Input Layout
    g_graphicsApi().setInputLayout(m_inLayOut);

    //Crete constant Buffers
    m_CBneverChange = g_graphicsApi().CreateConstantBuffer(sizeof(CBNeverChanges));
    m_CBchangeOnResize = g_graphicsApi().CreateConstantBuffer(sizeof(CBChangeOnResize));
    m_CBchangeEveryFrame = g_graphicsApi().CreateConstantBuffer(sizeof(CBChangesEveryFrame));
    m_CBlights = g_graphicsApi().CreateConstantBuffer(sizeof(CBLights));
    m_CBAO = g_graphicsApi().CreateConstantBuffer(sizeof(CBAO));
    m_CBBright = g_graphicsApi().CreateConstantBuffer(sizeof(CBBright));
    m_CBBlur = g_graphicsApi().CreateConstantBuffer(sizeof(CBMipLevels));

    // Get the default Camera
    m_debugCam = g_CameraMan().getDebugCamera();

    //Set basic data in constan buffers structs
    //m_DChangeOnResize.mProjection.transpose();
    m_DNeverChanges.m_view = m_debugCam->getMatrixView();
    m_DNeverChanges.m_viewInv = m_DNeverChanges.m_view;
    m_DNeverChanges.m_viewInv.invert();

    m_DChangeOnResize.m_projection = m_debugCam->getMatrixProjection();
    m_DChangeOnResize.m_projectionInv = m_DChangeOnResize.m_projection;
    m_DChangeOnResize.m_projectionInv.invert();

    m_DChangeOnResize.m_viewProjection = m_DNeverChanges.m_view * 
                                         m_DChangeOnResize.m_projection;
    m_DChangeOnResize.m_viewProjectionInv = m_DChangeOnResize.m_viewProjection;
    m_DChangeOnResize.m_viewProjectionInv.invert();
    JDMatrix4 world;
    world.identity();
    JDVector4 color(0.5f, 0, 0.5f, 1);

    m_DChangeEveryFrame.m_world = world;
    m_DChangeEveryFrame.m_worldInv = world;
    m_DChangeEveryFrame.m_worldInv.invert();

    m_DChangeEveryFrame.m_worldProj = world * m_DChangeOnResize.m_projection;
    m_DChangeEveryFrame.m_worldProjInv = m_DChangeEveryFrame.m_worldProj;
    m_DChangeEveryFrame.m_worldProjInv.invert();

    m_DChangeEveryFrame.m_worldView = world * m_DNeverChanges.m_view;
    m_DChangeEveryFrame.m_worldViewInv = m_DChangeEveryFrame.m_worldView;
    m_DChangeEveryFrame.m_worldViewInv.invert();

    m_DChangeEveryFrame.m_worldViewProj = m_DChangeEveryFrame.m_worldView *
                                          m_DChangeOnResize.m_projection;
    m_DChangeEveryFrame.m_worldViewProjInv = m_DChangeEveryFrame.m_worldViewProj;
    m_DChangeEveryFrame.m_worldViewProjInv.invert();
    m_DChangeEveryFrame.m_viewPosition = m_debugCam->getPositionVector();
    m_DChangeEveryFrame.vMeshColor = color;

    //Create Main Light
    m_DLights.light[0].m_lightColor = { 1,1,1,1 };
    m_DLights.light[0].m_lightDirection = { -1,0,1 };
    m_DLights.light[0].m_lightPosition = { 0,0,0 };
    m_DLights.light[0].m_type = 0;
    m_DLights.light[0].m_numberOfLights = 1;//This is only for the first sctruc

    //Update buffers
    g_graphicsApi().updateSubresource(m_CBneverChange, &m_DNeverChanges);
    g_graphicsApi().updateSubresource(m_CBchangeOnResize, &m_DChangeOnResize);
    g_graphicsApi().updateSubresource(m_CBchangeEveryFrame, &m_DChangeEveryFrame);
    g_graphicsApi().updateSubresource(m_CBlights, &m_DLights);
    g_graphicsApi().updateSubresource(m_CBAO, &m_DAOclusion);
    g_graphicsApi().updateSubresource(m_CBBright, &m_DBright);
    g_graphicsApi().updateSubresource(m_CBBlur, &m_DmipDataBlur);

    //Set contant buffers
    //g_graphicsApi().setConstanBuffer(neverChangeB,0);
    g_graphicsApi().VertexShaderSetConstanBuffer(m_CBneverChange, 0);
    g_graphicsApi().PixelShaderSetConstanBuffer(m_CBneverChange, 0);
    //g_graphicsApi().setConstanBuffer(changeOnResizeB,1);
    g_graphicsApi().VertexShaderSetConstanBuffer(m_CBchangeOnResize, 1);
    g_graphicsApi().PixelShaderSetConstanBuffer(m_CBchangeOnResize, 1);
    //g_graphicsApi().setConstanBuffer(changeEveryFrameB,2);
    g_graphicsApi().VertexShaderSetConstanBuffer(m_CBchangeEveryFrame, 2);
    g_graphicsApi().PixelShaderSetConstanBuffer(m_CBchangeEveryFrame, 2);

    g_graphicsApi().VertexShaderSetConstanBuffer(m_CBlights, 4);
    g_graphicsApi().PixelShaderSetConstanBuffer(m_CBlights, 4);

    //Internal function no creat bones buffer
    g_graphicsApi().SetBonesConstanBuffer();

    //Create and set default sampler lineal
    SPtr<Sampler> samplerLineal = g_graphicsApi().CreateSampleLinearState();
    g_graphicsApi().setSampler(samplerLineal, 0);

    //Set Primitive topology
    g_graphicsApi().setPrimitiveTopology(PRIMITIVE_TOPOLOGY_FORMAT::TRIANGLELIST);

    //Load Resources
      //Load defaultTextures
    g_ResourceMan().loadResourceFromFile("data/textures/black.png",
                                         RESOURCE_TYPE::TEXTURE);
    g_ResourceMan().loadResourceFromFile("data/textures/white.png",
                                         RESOURCE_TYPE::TEXTURE);
    SPtr<Resource> cubeResource =
      g_ResourceMan().loadResourceFromFile("data/textures/LightCube.dds",
                                           RESOURCE_TYPE::TEXTURE);
    SPtr<Texture2D> cubeMap(cubeResource, reinterpret_cast<Texture2D*>(cubeResource.get()));
    m_ambientCubeMap = cubeMap;
    m_ambientCubeMapOption = 3;

    //Load Models with it´s textures
    //load spidergwen
    //g_ResourceMan().loadResourceFromFile("data/models/Happy Idle.fbx",
    //g_ResourceMan().loadResourceFromFile("data/models/Dancing.fbx",
    //                                     RESOURCE_TYPE::MODEL);
    //g_ResourceMan().loadResourceFromFile("data/textures/GwenStacyDifuselMap.jpg",
    //                                     RESOURCE_TYPE::TEXTURE);
    //g_ResourceMan().loadResourceFromFile("data/textures/GwenStacyDifuselMapV2.jpg",
    //                                     RESOURCE_TYPE::TEXTURE);
    //g_ResourceMan().loadResourceFromFile("data/textures/GwenStacyNormalMap.jpg",
    //                                     RESOURCE_TYPE::TEXTURE);
    //g_ResourceMan().loadResourceFromFile("data/textures/GwenStacySpecularMap.jpg",
    //                                     RESOURCE_TYPE::TEXTURE);
    //g_ResourceMan().loadResourceFromFile("data/textures/GwenStacyMetallnessMap.jpg",
    //                                     RESOURCE_TYPE::TEXTURE);


    //load noivern
    g_ResourceMan().loadResourceFromFile("data/models/Noivern.fbx",
                                         RESOURCE_TYPE::MODEL);
    g_ResourceMan().loadResourceFromFile("data/textures/noivern_albedo.png",
                                         RESOURCE_TYPE::TEXTURE);
    g_ResourceMan().loadResourceFromFile("data/textures/noivern_normals.png",
                                         RESOURCE_TYPE::TEXTURE);
    g_ResourceMan().loadResourceFromFile("data/textures/noivern_roughness.png",
                                         RESOURCE_TYPE::TEXTURE);
    g_ResourceMan().loadResourceFromFile("data/textures/pedestal_albedo.png",
                                         RESOURCE_TYPE::TEXTURE);
    g_ResourceMan().loadResourceFromFile("data/textures/pedestal_normals.png",
                                         RESOURCE_TYPE::TEXTURE);
    g_ResourceMan().loadResourceFromFile("data/textures/pedestal_metallic.png",
                                         RESOURCE_TYPE::TEXTURE);
    g_ResourceMan().loadResourceFromFile("data/textures/pedestal_roughness.png",
                                         RESOURCE_TYPE::TEXTURE);
    SPtr<GameObject> tmp = SceneGraph::instance().createGameObject();
    tmp->addComponent(COMPONENT_TYPE::RENDERMODEL);
    auto rModel =
      reinterpret_cast<CRenderModel*>(tmp->getComponent(COMPONENT_TYPE::RENDERMODEL).get());
    rModel->setModel(g_ResourceMan().getModel("data/models/Noivern.fbx"));
    rModel->m_model->m_meshes[0]->setAlbedoTexture(g_ResourceMan().getTexture("data/textures/noivern_albedo.png"));
    rModel->m_model->m_meshes[0]->setNormalTexture(g_ResourceMan().getTexture("data/textures/noivern_normals.png"));
    rModel->m_model->m_meshes[0]->setSpecularTexture(g_ResourceMan().getTexture("data/textures/black.png"));
    rModel->m_model->m_meshes[0]->setRoughnessTexture(g_ResourceMan().getTexture("data/textures/noivern_roughness.png"));
    rModel->m_model->m_meshes[1]->setAlbedoTexture(g_ResourceMan().getTexture("data/textures/pedestal_albedo.png"));
    rModel->m_model->m_meshes[1]->setNormalTexture(g_ResourceMan().getTexture("data/textures/pedestal_normals.png"));
    rModel->m_model->m_meshes[1]->setSpecularTexture(g_ResourceMan().getTexture("data/textures/pedestal_metallic.png"));
    rModel->m_model->m_meshes[1]->setRoughnessTexture(g_ResourceMan().getTexture("data/textures/pedestal_roughness.png"));

    //load cyberWarrior
    g_ResourceMan().loadResourceFromFile("data/models/cyberWarrior.fbx",
      RESOURCE_TYPE::MODEL);
    g_ResourceMan().loadResourceFromFile("data/textures/soldier/TM.png",
      RESOURCE_TYPE::TEXTURE);
    g_ResourceMan().loadResourceFromFile("data/textures/soldier/NM.png",
      RESOURCE_TYPE::TEXTURE);
    g_ResourceMan().loadResourceFromFile("data/textures/soldier/Metal.png",
      RESOURCE_TYPE::TEXTURE);
    g_ResourceMan().loadResourceFromFile("data/textures/soldier/Rough.png",
      RESOURCE_TYPE::TEXTURE);
    g_ResourceMan().loadResourceFromFile("data/textures/soldier/material0_Base_Color.png",
      RESOURCE_TYPE::TEXTURE);
    g_ResourceMan().loadResourceFromFile("data/textures/soldier/material0_Normal_DirectX.png",
      RESOURCE_TYPE::TEXTURE);
    g_ResourceMan().loadResourceFromFile("data/textures/soldier/material0_Metallic.png",
      RESOURCE_TYPE::TEXTURE);
    g_ResourceMan().loadResourceFromFile("data/textures/soldier/material0_Roughness.png",
      RESOURCE_TYPE::TEXTURE);

    //Load Trooper
    //g_ResourceMan().loadResourceFromFile("data/models/trooper.fbx",
    //                                     RESOURCE_TYPE::MODEL);
    //g_ResourceMan().loadResourceFromFile("data/textures/Trooper/Helmet.001_Base_Color.jpg",
    //                                     RESOURCE_TYPE::TEXTURE);
    //g_ResourceMan().loadResourceFromFile("data/textures/Trooper/Helmet.001_Normal_OpenGL.jpg",
    //                                     RESOURCE_TYPE::TEXTURE);
    //g_ResourceMan().loadResourceFromFile("data/textures/Trooper/Helmet.001_Metallic.jpg",
    //                                     RESOURCE_TYPE::TEXTURE);
    //g_ResourceMan().loadResourceFromFile("data/textures/Trooper/Helmet.001_Roughness.jpg",
    //                                     RESOURCE_TYPE::TEXTURE);
    //g_ResourceMan().loadResourceFromFile("data/textures/Trooper/LowerBody.001_Base_Color.jpg",
    //                                     RESOURCE_TYPE::TEXTURE);
    //g_ResourceMan().loadResourceFromFile("data/textures/Trooper/LowerBody.001_Normal_OpenGL.jpg",
    //                                     RESOURCE_TYPE::TEXTURE);
    //g_ResourceMan().loadResourceFromFile("data/textures/Trooper/LowerBody.001_Metallic.jpg",
    //                                     RESOURCE_TYPE::TEXTURE);
    //g_ResourceMan().loadResourceFromFile("data/textures/Trooper/LowerBody.001_Roughness.jpg",
    //                                     RESOURCE_TYPE::TEXTURE);
    //g_ResourceMan().loadResourceFromFile("data/textures/Trooper/UpperBody.001_Base_Color.jpg",
    //                                     RESOURCE_TYPE::TEXTURE);
    //g_ResourceMan().loadResourceFromFile("data/textures/Trooper/UpperBody.001_Normal_OpenGL.jpg",
    //                                     RESOURCE_TYPE::TEXTURE);
    //g_ResourceMan().loadResourceFromFile("data/textures/Trooper/UpperBody.001_Metallic.jpg",
    //                                     RESOURCE_TYPE::TEXTURE);
    //g_ResourceMan().loadResourceFromFile("data/textures/Trooper/UpperBody.001_Roughness.jpg",
    //                                     RESOURCE_TYPE::TEXTURE);

    //Set the cube map
    g_graphicsApi().PixelShaderSetShaderResources(m_ambientCubeMap, 4);

    //Rasterize states
    m_defaultRasState = g_graphicsApi().createRasterizeState();
    m_wireframeRasState =
      g_graphicsApi().createRasterizeState(RASTERIZER_FILL_MODE::D3D11_FILL_WIREFRAME);

    SPtr<GameObject> light = SceneGraph::instance().createGameObject();
    m_slight = light;
    light->setName("mainLight");
    SPtr<Component> mainLight = light->addComponent(COMPONENT_TYPE::LIGHT);
    CLight* ml = reinterpret_cast<CLight*>(mainLight.get());
    ml->setIdArray(m_lightCreated);
    ++m_lightCreated;

    SPtr<GameObject> mainCam = SceneGraph::instance().createGameObject();
    mainCam->setName("mainCamera");
    SPtr<Component> cam = g_CameraMan().getMainCamera();
    mainCam->addComponent(COMPONENT_TYPE::CAMERA, cam);

    m_tempParticle.m_pos = { 0,0,0,0 };
    m_tempParticle.m_size = { 1000,1000 };
    m_particleVB = g_graphicsApi().createVertexBuffer(1, 
                                                      sizeof(ParticleData), 
                                                      &m_tempParticle);

    initImGui();
  }

  void 
  RenderFNDApi::initImGui() {
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
    m_fileDialog.reset(new ImGui::FileBrowser(ImGuiFileBrowserFlags_CloseOnEsc |
                                              ImGuiFileBrowserFlags_MultipleSelection));
    m_fileDialog->SetTitle("LoadResource");

  }

#define errOut(x) if (0 != x) {std::cout<<"Error de salida: "<< x << std::endl; return x; }
  int 
  RenderFNDApi::LoadHistogram() {
    cl_uint num_platforms = 0;
    errOut(clGetPlatformIDs(0, nullptr, &num_platforms));

    Vector<cl_platform_id> all_platforms;
    all_platforms.resize(num_platforms);

    errOut(clGetPlatformIDs(num_platforms, &all_platforms[0], &num_platforms));

    String platform_version;
    String platform_name;
    String platform_vendor;
    String platform_extensions;
    platform_version.resize(255, '\0');
    platform_name.resize(255, '\0');
    platform_vendor.resize(255, '\0');
    platform_extensions.resize(1024, '\0');

    for (auto& platform : all_platforms) {
      clGetPlatformInfo(platform, CL_PLATFORM_VERSION, 255, &platform_version[0], nullptr);
      clGetPlatformInfo(platform, CL_PLATFORM_NAME, 255, &platform_name[0], nullptr);
      clGetPlatformInfo(platform, CL_PLATFORM_VENDOR, 255, &platform_vendor[0], nullptr);
      clGetPlatformInfo(platform, CL_PLATFORM_EXTENSIONS, 1024, &platform_extensions[0], nullptr);
      cout << "Platform version: " << platform_version << endl;
      cout << "Platform name: " << platform_name << endl;
      cout << "Platform vendor: " << platform_vendor << endl;
      cout << "Platform extentions: " << platform_extensions << endl;
    }
    cl_platform_id selected_platform = all_platforms[0];

    cl_uint numDevices = 0;
    errOut(clGetDeviceIDs(selected_platform, CL_DEVICE_TYPE_ALL, 0, nullptr, &numDevices));

    Vector<cl_device_id> all_devices;
    all_devices.resize(numDevices);

    errOut(clGetDeviceIDs(selected_platform,
      CL_DEVICE_TYPE_ALL,
      numDevices,
      &all_devices[0],
      nullptr));

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

    for (auto& device : all_devices) {
      clGetDeviceInfo(device, CL_DEVICE_NAME, 255, &device_name[0], nullptr);
      clGetDeviceInfo(device, CL_DEVICE_VENDOR, 255, &device_vendor[0], nullptr);
      clGetDeviceInfo(device, CL_DRIVER_VERSION, 255, &driver_version[0], nullptr);
      clGetDeviceInfo(device, CL_DEVICE_PROFILE, 255, &device_profile[0], nullptr);
      clGetDeviceInfo(device, CL_DEVICE_VERSION, 255, &device_version[0], nullptr);
      clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, 1024, &device_extensions[0], nullptr);
      clGetDeviceInfo(device, CL_DEVICE_PLATFORM, 1024, &device_platform[0], nullptr);
      clGetDeviceInfo(device, CL_DEVICE_DOUBLE_FP_CONFIG, 1024, &device_double_fp_config[0], nullptr);
      cout << "Device name: " << device_name << endl;
      cout << "Device vendor: " << device_vendor << endl;
      cout << "Driver version: " << driver_version << endl;
      cout << "Device profile: " << device_profile << endl;
      cout << "Device version: " << device_version << endl;
      cout << "Device extensions: " << device_extensions << endl;
      cout << "Device platform: " << device_platform << endl;
      cout << "Device doble fp config: " << device_double_fp_config << endl;
    }

    cl_device_id selected_device = all_devices[0];

    cl_int error;
    cl_context context = clCreateContext(nullptr, 
                                         1, 
                                         &selected_device, 
                                         nullptr, 
                                         nullptr, 
                                         &error);
    errOut(error);
    String kernel_code2 = "void kernel Histogram(global const int* RED,"
                                                "global const int* GREEN,"
                                                "global const int* BLUE,"
                                                "global const int* ALPHA,"
                                                "global int* oRED,"
                                                "global int* oGREEN,"
                                                "global int* oBLUE,"
                                                "global int* oALPHA) {"
                                                  "int id = get_global_id(0);"
                                                  "atomic_add(&oRED[RED[id]],1);"
                                                  "atomic_add(&oGREEN[GREEN[id]],1);"
                                                  "atomic_add(&oBLUE[BLUE[id]],1);"
                                                  "atomic_add(&oALPHA[ALPHA[id]],1);"
                                                "};";
    const char** code2 = (const char**)new char[1];
    code2[0] = kernel_code2.c_str();

    size_t codeSize = kernel_code2.size();

    cl_program program2 = clCreateProgramWithSource(context, 1, code2, &codeSize, &error);
    errOut(error);

    errOut(clBuildProgram(program2, 1, &selected_device, nullptr, nullptr, nullptr));

    cl_command_queue command_queue2 = clCreateCommandQueue(context, 
                                                           selected_device, 
                                                           0, 
                                                           &error);
    errOut(error);

    cl_kernel Histogram = clCreateKernel(program2, "Histogram", &error);
    errOut(error);

    int width, height, channels;
    unsigned char* img = stbi_load("cartoon-building.png", &width, &height, &channels, 4);
    int pixelSize = width * height;
    //Input
    cl_mem buffer_red = clCreateBuffer(context, 
                                       CL_MEM_READ_WRITE, 
                                       sizeof(int) * pixelSize, 
                                       nullptr, 
                                       &error);
    errOut(error);
    cl_mem buffer_green = clCreateBuffer(context, 
                                         CL_MEM_READ_WRITE, 
                                         sizeof(int) * pixelSize, 
                                         nullptr, 
                                         &error);
    errOut(error);
    cl_mem buffer_blue = clCreateBuffer(context, 
                                        CL_MEM_READ_WRITE, 
                                        sizeof(int) * 
                                        pixelSize, 
                                        nullptr, 
                                        &error);
    errOut(error);
    cl_mem buffer_alpha = clCreateBuffer(context, 
                                         CL_MEM_READ_WRITE, 
                                         sizeof(int) * 
                                         pixelSize, 
                                         nullptr,
                                         &error);
    errOut(error);
    //Ouput
    cl_mem buffer_oRed = clCreateBuffer(context, 
                                        CL_MEM_READ_WRITE, 
                                        sizeof(int) * 256, 
                                        nullptr, 
                                        &error);
    errOut(error);
    cl_mem buffer_oGreen = clCreateBuffer(context, 
                                          CL_MEM_READ_WRITE, 
                                          sizeof(int) * 256, 
                                          nullptr, 
                                          &error);
    errOut(error);
    cl_mem buffer_oBlue = clCreateBuffer(context, 
                                         CL_MEM_READ_WRITE, 
                                         sizeof(int) * 256, 
                                         nullptr, 
                                         &error);
    errOut(error);
    cl_mem buffer_oAlpha = clCreateBuffer(context, 
                                          CL_MEM_READ_WRITE, 
                                          sizeof(int) * 256, 
                                          nullptr, 
                                          &error);
    errOut(error);

    Vector<int> red;
    red.resize(pixelSize);
    m_redHisto.resize(pixelSize);

    Vector<int>  green;
    green.resize(pixelSize);
    m_greenHisto.resize(pixelSize);

    Vector<int>  blue;
    blue.resize(pixelSize);
    m_blueHisto.resize(pixelSize);

    Vector<int>  Alpha;
    Alpha.resize(pixelSize);
    int size = pixelSize * 3;
    int currentPixel = 0;
    for (int j = 0; j < height; j++)
    {
      for (int i = 0; i < width; i++)
      {
        unsigned char* pixelOffset = img + (i + height * j) * channels;
        red[currentPixel] = pixelOffset[0];
        green[currentPixel] = channels >= 2 ? pixelOffset[1] : 0;
        blue[currentPixel] = channels >= 3 ? pixelOffset[2] : 0;
        Alpha[currentPixel] = channels >= 4 ? pixelOffset[3] : 255;
        ++currentPixel;
      }
    }

    int oRed[256] = { 0 };
    int oGreen[256] = { 0 };
    int oBlue[256] = { 0 };
    int oAlpha[256] = { 0 };
    clEnqueueWriteBuffer(command_queue2, 
                         buffer_red, 
                         CL_TRUE, 
                         0, 
                         sizeof(int) * pixelSize, 
                         red.data(), 
                         0, 
                         nullptr, 
                         nullptr);
    clEnqueueWriteBuffer(command_queue2, 
                         buffer_green, 
                         CL_TRUE, 
                         0, 
                         sizeof(int) * pixelSize, 
                         green.data(), 
                         0, 
                         nullptr, 
                         nullptr);
    clEnqueueWriteBuffer(command_queue2, 
                         buffer_blue, 
                         CL_TRUE, 
                         0, 
                         sizeof(int) * pixelSize, 
                         blue.data(),
                         0,
                         nullptr, 
                         nullptr);
    clEnqueueWriteBuffer(command_queue2, 
                         buffer_alpha, 
                         CL_TRUE, 
                         0, 
                         sizeof(int) * pixelSize, 
                         Alpha.data(), 0, nullptr, nullptr);
    clEnqueueWriteBuffer(command_queue2, 
                         buffer_oRed, 
                         CL_TRUE, 
                         0, 
                         sizeof(int) * 256, 
                         oRed, 
                         0, 
                         nullptr, 
                         nullptr);
    clEnqueueWriteBuffer(command_queue2, 
                         buffer_oGreen, 
                         CL_TRUE, 
                         0, 
                         sizeof(int) * 256, 
                         oGreen, 
                         0, 
                         nullptr, 
                         nullptr);
    clEnqueueWriteBuffer(command_queue2, 
                         buffer_oBlue, 
                         CL_TRUE, 
                         0, 
                         sizeof(int) * 256, 
                         oBlue,
                         0, 
                         nullptr, 
                         nullptr);
    clEnqueueWriteBuffer(command_queue2, 
                         buffer_oAlpha, 
                         CL_TRUE, 
                         0, 
                         sizeof(int) * 256, 
                         oAlpha, 
                         0, 
                         nullptr, 
                         nullptr);

    errOut(clSetKernelArg(Histogram, 0, sizeof(cl_mem), (void*)&buffer_red));
    errOut(clSetKernelArg(Histogram, 1, sizeof(cl_mem), (void*)&buffer_green));
    errOut(clSetKernelArg(Histogram, 2, sizeof(cl_mem), (void*)&buffer_blue));
    errOut(clSetKernelArg(Histogram, 3, sizeof(cl_mem), (void*)&buffer_alpha));
    errOut(clSetKernelArg(Histogram, 4, sizeof(cl_mem), (void*)&buffer_oRed));
    errOut(clSetKernelArg(Histogram, 5, sizeof(cl_mem), (void*)&buffer_oGreen));
    errOut(clSetKernelArg(Histogram, 6, sizeof(cl_mem), (void*)&buffer_oBlue));
    errOut(clSetKernelArg(Histogram, 7, sizeof(cl_mem), (void*)&buffer_oAlpha));

    size_t globalWorkSize = pixelSize;
    clEnqueueNDRangeKernel(command_queue2, 
                           Histogram, 
                           1, 
                           nullptr, 
                           &globalWorkSize, 
                           nullptr, 
                           0, 
                           nullptr, 
                           nullptr);

    errOut(clFinish(command_queue2));

    clEnqueueReadBuffer(command_queue2, 
                        buffer_oRed, 
                        CL_TRUE, 
                        0, 
                        sizeof(int) * 256, 
                        oRed, 
                        0, 
                        nullptr, 
                        nullptr);
    clEnqueueReadBuffer(command_queue2, 
                        buffer_oGreen, 
                        CL_TRUE, 
                        0, 
                        sizeof(int) * 256, 
                        oGreen, 
                        0, 
                        nullptr, 
                        nullptr);
    clEnqueueReadBuffer(command_queue2, 
                        buffer_oBlue, 
                        CL_TRUE, 
                        0, 
                        sizeof(int) * 256, 
                        oBlue, 
                        0, 
                        nullptr, 
                        nullptr);
    clEnqueueReadBuffer(command_queue2, 
                        buffer_oAlpha, 
                        CL_TRUE, 
                        0, 
                        sizeof(int) * 256, 
                        oAlpha, 
                        0, 
                        nullptr, 
                        nullptr);
    for (int i = 0; i < 256; ++i) {
      m_redHisto[i] = (float)oRed[i];
      m_greenHisto[i] = (float)oGreen[i];
      m_blueHisto[i] = (float)oBlue[i];
    }
  }

  void 
  RenderFNDApi::showHistorgramOption() {
    if (!m_showHistogram) {
      return;
    }
    if (ImGui::Begin("Histogram", &m_showHistogram)) {
      ImGui::PlotHistogram("red", 
                           m_redHisto.data(), 
                           256, 
                           0, 
                           0, 
                           3.402823466e+38F, 
                           3.402823466e+38F, 
                           ImVec2(400, 100));
      ImGui::PlotHistogram("green",
                           m_greenHisto.data(), 
                           256, 
                           0, 
                           0, 
                           3.402823466e+38F, 
                           3.402823466e+38F, 
                           ImVec2(400, 100));
      ImGui::PlotHistogram("blue", 
                           m_blueHisto.data(), 
                           256, 
                           0, 
                           0, 
                           3.402823466e+38F, 
                           3.402823466e+38F, 
                           ImVec2(400, 100));
      ImGui::End();
    }
  }

  void
  RenderFNDApi::UpdateCameraInterpolate(const float& deltaTime) {
    if (g_CameraMan().m_interpolating)
    {
      m_tempCam = g_CameraMan().interpolateCameras(m_interpolateCam1,
                                                   m_interpolateCam2,
                                                   deltaTime,
                                                   m_interpolationCamerasTime);
    }
  }

  void 
  RenderFNDApi::changeCameraDataBuffer(WeakSptr<Camera> cam) {
    SPtr<Camera> camera = cam.lock();

    m_DNeverChanges.m_view = camera->getMatrixView();
    m_DNeverChanges.m_viewInv = m_DNeverChanges.m_view;
    m_DNeverChanges.m_viewInv.invert();

    m_DChangeOnResize.m_projection = camera->getMatrixProjection();
    m_DChangeOnResize.m_projection.transpose();
    m_DChangeOnResize.m_projectionInv = m_DChangeOnResize.m_projection;
    m_DChangeOnResize.m_projectionInv.invert();

    m_DChangeOnResize.m_viewProjection = m_DNeverChanges.m_view * 
                                         m_DChangeOnResize.m_projection;
    m_DChangeOnResize.m_viewProjectionInv = m_DChangeOnResize.m_viewProjection;
    m_DChangeOnResize.m_viewProjectionInv.invert();

    m_DChangeEveryFrame.m_worldViewProj = m_DChangeEveryFrame.m_worldView *
                                          m_DChangeOnResize.m_projection;
    m_DChangeEveryFrame.m_worldViewProjInv = m_DChangeEveryFrame.m_worldViewProj;
    m_DChangeEveryFrame.m_worldViewProjInv.invert();
    m_DChangeEveryFrame.m_viewPosition = camera->getPositionVector();

    g_graphicsApi().updateSubresource(m_CBneverChange, &m_DNeverChanges);
    g_graphicsApi().updateSubresource(m_CBchangeOnResize, &m_DChangeOnResize);
    g_graphicsApi().updateSubresource(m_CBchangeEveryFrame, &m_DChangeEveryFrame);
    g_graphicsApi().updateSubresource(m_CBlights, &m_DLights);
  }

  void 
  RenderFNDApi::addCameraComponent() {
    ImGui::Begin("new Camera", &m_addigCamera);
    ImGui::InputText("Camera Name", m_newCameraName.data(), 25);
    ImGui::DragFloat("Camera Near", &m_newCamNear);
    ImGui::DragFloat("Camera Far", &m_newCamFar);
    auto component =
      SceneGraph::instance().selectedObjet->getComponent(COMPONENT_TYPE::TRANSFORM);
    CTransform* trans = reinterpret_cast<CTransform*>(component.get());
    m_newCamEye = trans->worldPosition;
    m_newCamUp = trans->up;
    m_newCamAt = m_newCamEye + trans->forward;
    if (ImGui::Button("Create Camera"))
    {
      SPtr<Camera> newC = g_CameraMan().createCamera(m_newCameraName,
                                                     m_newCamEye,
                                                     m_newCamUp,
                                                     m_newCamAt,
                                                     m_newCamNear,
                                                     m_newCamFar);
      m_newCamNear = 0.01f;
      m_newCamFar = 1000.0f;
      SPtr<Component> cam =
        SceneGraph::instance().selectedObjet->addComponent(COMPONENT_TYPE::CAMERA, newC);
      m_addigCamera = false;
    }
    if (ImGui::Button("Cancel"))
    {
      m_addigCamera = false;
    }
    ImGui::End();
  }

  void 
  RenderFNDApi::imguiDockScreen() {
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
      ImGuiViewport* viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport->GetWorkPos());
      ImGui::SetNextWindowSize(viewport->GetWorkSize());
      ImGui::SetNextWindowViewport(viewport->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
      window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", 0, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
      ImGui::PopStyleVar(2);
    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
      ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
      ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    static bool open = false;
    if (ImGui::BeginMenuBar())
    {

      if (ImGui::BeginMenu("File"))
      {
        if (ImGui::BeginMenu("Import"))
        {
          if (ImGui::MenuItem("Model")) {
            m_typeResourceToLoad = RESOURCE_TYPE::MODEL;
            //m_fileDialog->SetTypeFilters({ ".fbx.FBX.obj",".fbx",".FBX", ".obj" });
            m_fileDialog->SetTypeFilters({ ".fbx",".FBX", ".obj" });
            m_fileDialog->Open();
            m_loadingFile = true;
          }
          if (ImGui::MenuItem("Image")) {
            m_typeResourceToLoad = RESOURCE_TYPE::TEXTURE;
            //m_fileDialog->SetTypeFilters({ ".jpg.png.dds",".jpg", ".png", ".dds" });
            m_fileDialog->SetTypeFilters({ ".jpg", ".png", ".dds" });
            m_fileDialog->Open();
            m_loadingFile = true;
          }
          ImGui::EndMenu();
        }

        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Options"))
      {
        //ImGui::Checkbox("SceneWindowNativeSize", &m_bSceneWindowNativeSize);
        ImGui::Checkbox("ShowAmbientOptions", &m_AmbientOptionsMenu);
        ImGui::Checkbox("Wireframe", &m_bWireframe);
        ImGui::Checkbox("CameraInterpolate", &m_cameraInterpolateMenu);
        ImGui::Checkbox("Deferred", &m_showDeferred);
        ImGui::Checkbox("AO options", &m_showAOOptions);
        ImGui::Checkbox("Bright options", &m_showBrightOptions);
        ImGui::Checkbox("Blur options", &m_showBlurOptions);
        ImGui::Checkbox("Histogram", &m_showHistogram);
        if (m_bWireframe)
        {
          g_graphicsApi().setRasterizeState(m_wireframeRasState);
          g_graphicsApi().setProgramShader(m_PSWireFrame);
        }
        else
        {
          g_graphicsApi().setRasterizeState(m_defaultRasState);
          g_graphicsApi().setProgramShader(m_PSForward);
        }
        ImGui::EndMenu();
      }

      ImGui::EndMenuBar();
    }

    ImGui::End();
  }

  void
  RenderFNDApi::imguiSceneGraph() {
    ImGui::Begin("SceneGraph");
    ImGui::CloseCurrentPopup();
    imGuiShowObject("child", SceneGraph::instance().m_root);
    ImGui::End();
  }

  void 
  RenderFNDApi::imGuiShowObject(const char* nameObject, WeakSptr<GameObject> child) {
    auto object = child.lock();
    bool hasChildren = false;
    bool isOpen = false;
    if (0 < object->m_children.size()) {
      hasChildren = true;
    }

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth |
                               ImGuiTreeNodeFlags_OpenOnArrow |
                               ImGuiTreeNodeFlags_OpenOnDoubleClick;
    if (nullptr == object->getParent()) {
      flags |= ImGuiTreeNodeFlags_DefaultOpen;
      ImGui::SetNextTreeNodeOpen(true);
    }

    if (!hasChildren) {
      flags |= ImGuiTreeNodeFlags_Leaf |
        ImGuiTreeNodeFlags_Bullet;
    }
    isOpen = ImGui::TreeNodeEx(object->getName().c_str(), flags);

    if (ImGui::IsItemClicked()) {
      object->selected = true;
      if (SceneGraph::instance().selectedObjet != object) {
        if (nullptr != SceneGraph::instance().selectedObjet) {
          SceneGraph::instance().selectedObjet->selected = false;
        }
        SceneGraph::instance().selectedObjet = object;
      }
    }

    if (isOpen) {
      // Text and Tree nodes are less high than framed widgets, 
      //here we add vertical spacing to make the tree lines equal high.
      ImGui::AlignTextToFramePadding(); 
      if (0 < object->m_children.size())
      {
        for (auto son : object->m_children)
        {
          imGuiShowObject(son->getName().c_str(), son);
        }
      }
      ImGui::TreePop();
    }
  }

  void 
  RenderFNDApi::imGuiInspectorObject() {
    ImGui::CloseCurrentPopup();
    ImGui::Begin("Inspector", NULL);
    JDVector3 pos = { m_DChangeEveryFrame.m_viewPosition.x,
                      m_DChangeEveryFrame.m_viewPosition.y,
                      m_DChangeEveryFrame.m_viewPosition.z };

    ImGui::InputFloat3("View Position", &pos.x, 2);
    ImGui::Separator();
    if (nullptr == SceneGraph::instance().selectedObjet) {
      ImGui::End();
      return;
    }
    if (ImGui::Button("Add child Object")) {
      SceneGraph::instance().createGameObject();
    }
    ImGui::Separator();

    char* name = (char*)SceneGraph::instance().selectedObjet->getName().c_str();
    ImGui::InputText("Name", name, 255);
    SceneGraph::instance().selectedObjet->setName(String(name));

    if (SceneGraph::instance().selectedObjet == SceneGraph::instance().m_root) {
      ImGui::End();
      return;
    }
    auto component =
      SceneGraph::instance().selectedObjet->getComponent(COMPONENT_TYPE::TRANSFORM);
    if (nullptr != component) {
      ImGui::Separator();
      ImGui::Text("Transform");
      CTransform* trans = reinterpret_cast<CTransform*>(component.get());
      JDVector3 rot = trans->euler;
      ImGui::DragFloat3("Rotacion", &rot.x, 0.1f);
      ImGui::DragFloat3("Position", &trans->position.x, 0.1f);
      ImGui::DragFloat3("Scale", &trans->scale.x, 0.1f);
      if (rot != trans->euler) {
        trans->rotation = { Degree(rot.x), Degree(rot.y), Degree(rot.z) };
        trans->euler = rot;
      }
    }
    component =
      SceneGraph::instance().selectedObjet->getComponent(COMPONENT_TYPE::RENDERMODEL);
    if (nullptr != component) {
      showRenderModelComponent();
    }
    component =
      SceneGraph::instance().selectedObjet->getComponent(COMPONENT_TYPE::LIGHT);
    if (nullptr != component) {
      showLightComponent();
    }

    component =
      SceneGraph::instance().selectedObjet->getComponent(COMPONENT_TYPE::CAMERA);
    if (nullptr != component) {
      showCameraComponent();
    }

    ImGui::Separator();
    if (ImGui::Button("AddComponent") && !m_showComponentImGui) {
      m_showComponentImGui = true;
    }
    ImGui::End();
    if (m_showComponentImGui) {
      ImGuiAddComponent();
    }
  }

  void 
  RenderFNDApi::showRenderModelComponent() {
    ImGui::Separator();
    ImGui::Text("Render Model");
    auto Model = SceneGraph::instance().selectedObjet.get();
    auto rModel =
      reinterpret_cast<CRenderModel*>(Model->getComponent(COMPONENT_TYPE::RENDERMODEL).get());
    String g_OptionPreviw = ResourceManager::instance().m_modelsNames[Model->m_modelOption];
    int32 modelOption = Model->m_modelOption;
    ImGui::Button(ResourceManager::instance().m_modelsNames[modelOption], ImVec2(80, 80));
    if (ImGui::BeginDragDropTarget()) {
      const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Modeldrop");
      if (payload) {
        modelOption = *reinterpret_cast<uint32*>(payload->Data);
      }
      ImGui::EndDragDropTarget();
    }
    if (ImGui::BeginCombo("Models", g_OptionPreviw.c_str())) {
      g_OptionPreviw = "";
      ImGui::ListBox("Model", &modelOption,
        &ResourceManager::instance().m_modelsNames[0],
        (int)ResourceManager::instance().m_modelsNames.size());
      ImGui::EndCombo();
    }
    if (modelOption != (int32)Model->m_modelOption) {
      if (modelOption > 0) {
        rModel->setModel(ResourceManager::instance().m_models[modelOption - 1]);
      }
      else {
        rModel->m_currentAnimation = nullptr;
        rModel->m_animationOption = 0;
        rModel->m_model = nullptr;
      }
      Model->m_modelOption = modelOption;
    }
    if (nullptr != rModel->m_model) {
      if (ImGui::TreeNode("Materials")) {
        showRenderModelMaterias(rModel);
        ImGui::TreePop();
      }

      g_OptionPreviw = rModel->m_model->m_AnimationsList[rModel->m_animationOption];
      if (ImGui::TreeNode("Animation")) {
        if (ImGui::BeginCombo("Current Animation", g_OptionPreviw.c_str())) {
          int32 animationOption = rModel->m_animationOption;
          g_OptionPreviw = "";
          ImGui::ListBox("animation##2", &animationOption,
            &rModel->m_model->m_AnimationsList[0],
            (int)rModel->m_model->m_AnimationsList.size());
          //static String g_UnitOptionPreviw = ResourceManager::instance().m_texturesNames[0];
          if (animationOption != (int32)rModel->m_animationOption) {
            if (animationOption > 0) {
              rModel->m_currentAnimation = rModel->m_model->m_animations[animationOption - 1];
            }
            else {
              rModel->m_currentAnimation = nullptr;
              rModel->noneAnimation();
            }
            rModel->m_animationOption = animationOption;
          }
          ImGui::EndCombo();
        }
        if (rModel->m_animationOption > 0) {
          ImGui::SliderFloat("Time",
            &rModel->m_animatedTime,
            0,
            rModel->m_currentAnimation->m_duration);
          if (rModel->m_playAnimation) {
            if (ImGui::Button("Pause")) {
              rModel->m_playAnimation = false;
            }
          }
          else {
            if (ImGui::Button("Play")) {
              rModel->m_playAnimation = true;
            }
          }
        }
        ImGui::TreePop();
      }
    }
  }

  void 
  RenderFNDApi::showRenderModelMaterias(CRenderModel* rModel) {
    ImGui::AlignTextToFramePadding();
    String g_OptionPreviw;
    uint32 conter = 0;
    for (auto m : rModel->m_model->m_meshes) {
      std::stringstream ss;
      ss << conter;
      String str = ss.str();
      String typeText;

      g_OptionPreviw = ResourceManager::instance().m_texturesNames[m->m_albedoOption];
      if (ImGui::CollapsingHeader(m->getName().c_str())) {
        //SelectAlbedo
        typeText = "Albedo##";
        typeText += str;
        ImGui::Text("Albedo");
        int32 albedolOption = m->m_albedoOption;
        ImGui::Image(m->getAlbedoTexture()->getTexture(), ImVec2(40, 40));
        if (ImGui::BeginDragDropTarget()) {
          const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("textdrop");
          if (payload)
          {
            albedolOption = *reinterpret_cast<uint32*>(payload->Data);
          }
          ImGui::EndDragDropTarget();
        }
        if (ImGui::BeginCombo(typeText.c_str(), g_OptionPreviw.c_str())) {
          g_OptionPreviw = "";
          ImGui::ListBox("Texture##1", &albedolOption,
            &ResourceManager::instance().m_texturesNames[0],
            (int)ResourceManager::instance().m_texturesNames.size());
          ImGui::EndCombo();
        }
        if (albedolOption != (int32)m->m_albedoOption) {
          //static String g_UnitOptionPreviw = ResourceManager::instance().m_texturesNames[0];
          if (albedolOption > 0) {
            m->setAlbedoTexture(ResourceManager::instance().m_textures[albedolOption - 1]);
          }
          else {
            m->setAlbedoTexture(ResourceManager::instance().DEFAULT_TEXTURE_BLACK);
          }
          m->m_albedoOption = albedolOption;
        }
        //SelectNormal
        g_OptionPreviw = ResourceManager::instance().m_texturesNames[m->m_NormalOption];
        typeText = "Normal##";
        typeText += str;
        ImGui::Text("Normal");
        int32 normalOption = m->m_NormalOption;
        ImGui::Image(m->getNormalTexture()->getTexture(), ImVec2(40, 40));
        if (ImGui::BeginDragDropTarget()) {
          const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("textdrop");
          if (payload) {
            normalOption = *reinterpret_cast<uint32*>(payload->Data);
          }
          ImGui::EndDragDropTarget();
        }
        if (ImGui::BeginCombo(typeText.c_str(), g_OptionPreviw.c_str())) {
          g_OptionPreviw = "";
          ImGui::ListBox("Texture##2", &normalOption,
            &ResourceManager::instance().m_texturesNames[0],
            (int)ResourceManager::instance().m_texturesNames.size());
          ImGui::EndCombo();
        }
        if (normalOption != (int32)m->m_NormalOption) {
          if (normalOption > 0) {
            m->setNormalTexture(ResourceManager::instance().m_textures[normalOption - 1]);
          }
          else {
            m->setNormalTexture(ResourceManager::instance().DEFAULT_TEXTURE_NORMAL);
          }
          m->m_NormalOption = normalOption;
        }
        //SelectSpecularOrMetallic
        g_OptionPreviw = ResourceManager::instance().m_texturesNames[m->m_specularOption];
        typeText = "Specular/Metalic##";
        typeText += str;
        ImGui::Text("Specular/Metalic");
        int32 specularOption = m->m_specularOption;
        ImGui::Image(m->getSpecularTexture()->getTexture(), ImVec2(40, 40));
        if (ImGui::BeginDragDropTarget()) {
          const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("textdrop");
          if (payload) {
            specularOption = *reinterpret_cast<uint32*>(payload->Data);
          }
          ImGui::EndDragDropTarget();
        }
        if (ImGui::BeginCombo(typeText.c_str(), g_OptionPreviw.c_str())) {
          g_OptionPreviw = "";
          ImGui::ListBox("Texture##3", &specularOption,
            &ResourceManager::instance().m_texturesNames[0],
            (int)ResourceManager::instance().m_texturesNames.size());
          ImGui::EndCombo();
        }
        if (specularOption != (int32)m->m_specularOption) {
          if (specularOption > 0) {
            m->setSpecularTexture(ResourceManager::instance().m_textures[specularOption - 1]);
          }
          else {
            m->setSpecularTexture(ResourceManager::instance().DEFAULT_TEXTURE_WHITE);
          }
          m->m_specularOption = specularOption;
        }
        //selecetRoughness
        g_OptionPreviw = ResourceManager::instance().m_texturesNames[m->m_roughOption];
        typeText = "Roughness##";
        typeText += str;
        ImGui::Text("Roughness");
        int32 roughOption = m->m_roughOption;
        ImGui::Image(m->getRoughnessTexture()->getTexture(), ImVec2(40, 40));
        if (ImGui::BeginDragDropTarget()) {
          const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("textdrop");
          if (payload) {
            roughOption = *reinterpret_cast<uint32*>(payload->Data);
          }
          ImGui::EndDragDropTarget();
        }
        if (ImGui::BeginCombo(typeText.c_str(), g_OptionPreviw.c_str())) {
          g_OptionPreviw = "";
          ImGui::ListBox("Texture##4", &roughOption,
            &ResourceManager::instance().m_texturesNames[0],
            (int)ResourceManager::instance().m_texturesNames.size());
          ImGui::EndCombo();
        }
        if (roughOption != (int32)m->m_roughOption) {
          if (roughOption > 0) {
            m->setRoughnessTexture(ResourceManager::instance().m_textures[roughOption - 1]);
          }
          else {
            m->setRoughnessTexture(ResourceManager::instance().DEFAULT_TEXTURE_WHITE);
          }
          m->m_roughOption = roughOption;
        }
      }
      ++conter;
    }
  }

  void 
  RenderFNDApi::showLightComponent() {
    ImGui::Separator();
    auto Model = SceneGraph::instance().selectedObjet.get();
    auto clight =
      reinterpret_cast<CLight*>(Model->getComponent(COMPONENT_TYPE::LIGHT).get());
    uint32 id = clight->getIdArray();
    String g_OptionPreviw = ResourceManager::instance().m_modelsNames[Model->m_modelOption];
    JDVector4 dir = clight->getDirection();
    if (ImGui::TreeNode("Light")) {
      ImGui::DragFloat3("LightDirection",
        //&m_lights.light[id].m_lightDirection.x,
        &dir.x,
        0.01f,
        -1,
        1);
      if (ImGui::TreeNode("LightColor")) {
        ImGui::ColorPicker3("LightColor", &clight->m_color.x);
        ImGui::TreePop();
      }
      ImGui::TreePop();
    }
    m_DLights.light[id].m_lightDirection = dir;
    m_DLights.light[id].m_lightColor = clight->m_color;
    g_graphicsApi().updateSubresource(m_CBlights, &m_DLights);
  }

  void 
  RenderFNDApi::showCameraComponent() {
    ImGui::Separator();
    auto Model = SceneGraph::instance().selectedObjet.get();
    auto cCam = (Model->getComponent(COMPONENT_TYPE::CAMERA));
    SPtr<Camera> CAM(cCam, reinterpret_cast<Camera*>(cCam.get()));
    if (ImGui::TreeNode("Camera")) {
      ImGui::Text("Camera: ", CAM->getName().c_str());
      float _near = CAM->getNear();
      float _currNear = _near;
      float _far = CAM->getFar();
      float _currFar = _far;
      ImGui::DragFloat("Near",
                       &_currNear,
                       0.1f,
                       0.0001f,
                       1000000.0f);
      ImGui::DragFloat("Far",
                       &_currFar,
                       1,
                       0.0001f,
                       1000000.0f);
      if (_currFar != _far || _currNear != _near)
      {
        CAM->adjustNearAndFar(_currNear, _currFar);
      }
      ImGui::TreePop();
    }
    changeCameraDataBuffer(CAM);
    /*g_graphicsApi().setRenderTarget(m_cameraRT);
    g_graphicsApi().Clear(m_cameraRT, 0.2f, 0.2f, 0.2f, 1);
    g_graphicsApi().ClearDepthStencil(m_cameraRT);
    //SceneGraph::instance().m_root->draw();
    for (auto object : SceneGraph::instance().m_GObjects)
    {
      auto component =
        object->getComponent(COMPONENT_TYPE::TRANSFORM);
      CTransform* trans = reinterpret_cast<CTransform*>(component.get());
      m_changeEveryFrame.m_world = trans->getMatrixTransform();

      g_graphicsApi().updateSubresource(m_changeEveryFrameB, &m_changeEveryFrame);

      object->draw();
    }*/
    renderForward(m_RTcamera);
    ImVec2 size = { (float)m_sceneSize.x, (float)m_sceneSize.y + 30 };
    size.x /= 2;
    size.y /= 2;
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Camera preview",
      nullptr,
      ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImVec2 wsize = ImGui::GetWindowSize();
    wsize.y -= 30;
    ImGui::Image(m_RTcamera.get()->getRenderTexture(), wsize);
    ImGui::End();
  }

  void 
  RenderFNDApi::ImGuiAddComponent() {
    ImGui::Begin("Components", &m_showComponentImGui);
    static ImGuiTextFilter filter;
    filter.Draw("Filter");
    const char* lines[] = { "Transform", "RenderModel","Light","Camera" };
    for (int i = 0; i < IM_ARRAYSIZE(lines); i++) {
      if (filter.PassFilter(lines[i])) {
        if (ImGui::Button(lines[i])) {
          switch (i) {
           case 0:
            SceneGraph::instance().selectedObjet->addComponent(COMPONENT_TYPE::TRANSFORM);
            break;
           case 1:
            SceneGraph::instance().selectedObjet->addComponent(COMPONENT_TYPE::RENDERMODEL);
            break;
           case 2:
            addLightComponent();
            break;
           case 3:
            m_newCameraName = "camera";
            m_newCameraName += std::to_string(CameraManager::instance().getNumberOfCameras());
            m_addigCamera = true;
            break;
           default:
            break;
          }
          m_showComponentImGui = false;
        }
      }
    }
    ImGui::End();
  }

  void 
  RenderFNDApi::addLightComponent() {
    if (m_lightCreated >= 20) {
      return;
    }
    SPtr<Component> newLight =
      SceneGraph::instance().selectedObjet->addComponent(COMPONENT_TYPE::LIGHT);
    if (nullptr == newLight) {
      return;
    }
    CLight* ml = reinterpret_cast<CLight*>(newLight.get());
    ml->setIdArray(m_lightCreated);
    ++m_lightCreated;
    m_DLights.light[0].m_numberOfLights = m_lightCreated;
    g_graphicsApi().updateSubresource(m_CBlights, &m_DLights);
  }

  void 
  RenderFNDApi::showTexturesResources() {
    ImGui::Begin("Textures##30");
    uint32 numText = (uint32)g_ResourceMan().m_textures.size();
    ImVec2 size = ImGui::GetWindowSize();
    if (size.x < 80) {
      size.x = 80;
      ImGui::SetWindowSize(size);
    }
    int32 alocator = int32((size.x - 40) / 80);
    if (alocator < 1) {
      alocator = 1;
    }
    for (uint32 n = 0; n < numText; ++n) {
      if ((n % alocator) != 0)
        ImGui::SameLine();

      ImGui::BeginGroup();
      String child = "child";
      child += n;
      ImGui::BeginChild(child.c_str(), ImVec2(80, 140));
      ImGui::ImageButton(g_ResourceMan().m_textures[n]->getTexture(), ImVec2(60, 60));
      if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
        uint32 info = n + 1;
        ImGui::SetDragDropPayload("textdrop", &info, sizeof(const char*));
        ImGui::TextUnformatted(g_ResourceMan().m_texturesNames[n + 1]);
        ImGui::EndDragDropSource();
      }
      ImGui::TextWrapped(g_ResourceMan().m_texturesNames[n + 1], ImVec2(65, 20));
      ImGui::EndChild();
      ImGui::EndGroup();
    }
    ImGui::End();
  }

  void 
  RenderFNDApi::showModels() {
    ImGui::Begin("Models##30");
    uint32 numModels = (uint32)g_ResourceMan().m_models.size();
    ImVec2 size = ImGui::GetWindowSize();
    if (size.x < 80) {
      size.x = 80;
      ImGui::SetWindowSize(size);
    }
    int32 alocator = int32((size.x - 40) / 80);
    if (alocator < 1) {
      alocator = 1;
    }
    for (uint32 n = 0; n < numModels; ++n) {
      if ((n % alocator) != 0)
        ImGui::SameLine();

      ImGui::BeginGroup();
      String child = "child";
      child += n;
      ImGui::BeginChild(child.c_str(), ImVec2(80, 80));
      ImGui::Button(g_ResourceMan().m_modelsNames[n + 1], ImVec2(60, 60));
      if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
        uint32 info = n + 1;
        ImGui::SetDragDropPayload("Modeldrop", &info, sizeof(const char*));
        ImGui::TextUnformatted(g_ResourceMan().m_modelsNames[n + 1]);
        ImGui::EndDragDropSource();
      }
      ImGui::EndChild();
      ImGui::EndGroup();
      //ImGui::PopID();
    }
    ImGui::End();
  }

  void 
  RenderFNDApi::showAmbientOption() {
    if (!m_AmbientOptionsMenu) {
      return;
    }
    if (ImGui::Begin("AmbientOption", &m_AmbientOptionsMenu)) {
      String optionPreviw = g_ResourceMan().m_texturesNames[m_ambientCubeMapOption];
      ImGui::Image(m_ambientCubeMap->getTexture(), ImVec2(40, 40));
      if (ImGui::BeginCombo("CubeMap", optionPreviw.c_str())) {
        int option = m_ambientCubeMapOption;
        optionPreviw = "";
        ImGui::ListBox("Texture##5", 
                       &option,
                       &g_ResourceMan().m_texturesNames[0],
                       (int)g_ResourceMan().m_texturesNames.size());
        if (option != (int32)m_ambientCubeMapOption) {
          if (option > 0) {
            m_ambientCubeMap = (g_ResourceMan().m_textures[option - 1]);
          }
          else {
            m_ambientCubeMap = (g_ResourceMan().DEFAULT_TEXTURE_WHITE);
          }
          m_ambientCubeMapOption = option;
          g_graphicsApi().PixelShaderSetShaderResources(m_ambientCubeMap, 4);
        }
        ImGui::EndCombo();
      }
      ImGui::End();
    }
  }

  void 
  RenderFNDApi::showAmbientOclusionOption() {
    if (!m_showAOOptions) {
      return;
    }
    if (ImGui::Begin("AmbientOption", &m_showAOOptions)) {
      ImGui::DragFloat("Sample Radius", &m_DAOclusion.g_sample_radius, 0.001f, 0, 0, "%.5f");
      ImGui::DragFloat("Scale", &m_DAOclusion.g_scale, 0.001f, 0, 0, "%.5f");
      ImGui::DragFloat("Bias", &m_DAOclusion.g_Bias, 0.001f, 0, 0, "%.5f");
      ImGui::DragFloat("Intensity", &m_DAOclusion.g_intensity, 0.001f, 0, 0, "%.5f");
      ImGui::DragInt("Blur Passes",&m_bluredAOPassesNum,1.0f,0,20);
      g_graphicsApi().updateSubresource(m_CBAO, &m_DAOclusion);
      ImGui::End();
    }
  }

  void 
  RenderFNDApi::showBrightOption() {
    if (!m_showBrightOptions) {
      return;
    }
    if (ImGui::Begin("Birght Options", &m_showBrightOptions)) {
      ImGui::DragFloat("BrightLod", &m_DBright.BrightLod_BloomThreshold.x, 1, 0, 0, "%.0f");
      ImGui::DragFloat("BloomThreshold", &m_DBright.BrightLod_BloomThreshold.y, 0.01f, 0, 0, "%.3f");
      ImGui::DragFloat("Exposure", &m_DBright.BrightLod_BloomThreshold.z, 0.01f, 0, 0, "%.3f");
      ImGui::DragFloat("Bloom Multiplier", &m_DBright.BrightLod_BloomThreshold.w, 0.01f, 0, 0, "%.3f");
      ImGui::DragInt("Blur Passes",&m_bluredBirghtPassesNum,1.0f,0,20);
      g_graphicsApi().updateSubresource(m_CBBright, &m_DBright);
      ImGui::End();
    }
  }

  void 
  RenderFNDApi::showBlurOption() {
    if (!m_showBlurOptions) {
      return;
    }
    if (ImGui::Begin("Blur Options", &m_showBlurOptions)) {
      ImGui::DragInt("Mip Level 0", &m_DmipDataBlur.mipLevel0);
      ImGui::DragInt("Mip Level 1", &m_DmipDataBlur.mipLevel1);
      ImGui::DragInt("Mip Level 2", &m_DmipDataBlur.mipLevel2);
      ImGui::DragInt("Mip Level 3", &m_DmipDataBlur.mipLevel3);
      g_graphicsApi().updateSubresource(m_CBBlur, &m_DmipDataBlur);
      ImGui::End();
    }
  }

  void 
  RenderFNDApi::showCameraInterpolateMenu() {
    if (!m_cameraInterpolateMenu)
    {
      return;
    }
    ImGui::Begin("Camera Interpolate Menu", &m_cameraInterpolateMenu);
    if (nullptr != m_tempCam)
    {
      {
        g_graphicsApi().setRenderTarget(m_RTcameraInterpolate);
        g_graphicsApi().Clear(m_RTcameraInterpolate, 0.2f, 0.2f, 0.2f, 1);
        g_graphicsApi().ClearDepthStencil(m_RTcameraInterpolate);
        changeCameraDataBuffer(m_tempCam);
        for (auto object : SceneGraph::instance().m_GObjects)
        {
          auto component =
            object->getComponent(COMPONENT_TYPE::TRANSFORM);
          CTransform* trans = reinterpret_cast<CTransform*>(component.get());
          m_DChangeEveryFrame.m_world = trans->getMatrixTransform();

          g_graphicsApi().updateSubresource(m_CBchangeEveryFrame, &m_DChangeEveryFrame);

          object->draw();
        }
      }
      ImVec2 size = { (float)m_sceneSize.x, (float)m_sceneSize.y + 30 };
      size.x /= 2;
      size.y /= 2;
      //ImGui::SetNextWindowSize(size);
      ImGui::Image(m_RTcameraInterpolate.get()->getRenderTexture(), size);
    }
    else
    {
      ImVec2 size = { (float)m_sceneSize.x, (float)m_sceneSize.y + 30 };
      size.x /= 2;
      size.y /= 2;
      //ImGui::SetNextWindowSize(size);
      ImGui::Image(g_ResourceMan().DEFAULT_TEXTURE_BLACK->getTexture(), size);
    }
    if (!g_CameraMan().m_interpolating)
    {
      String optionPreviw = g_CameraMan().m_camerasName[m_camInterId1];
      if (ImGui::BeginCombo("Camera 1", optionPreviw.c_str()))
      {
        int option = m_camInterId1;
        optionPreviw = "";
        ImGui::ListBox("camera 1##1", &option,
          &g_CameraMan().m_camerasName[0],
          (int)g_CameraMan().m_camerasName.size());
        if (option != (int32)m_camInterId1)
        {
          //static String g_UnitOptionPreviw = ResourceManager::instance().m_texturesNames[0];
          if (option > 0)
          {
            m_interpolateCam1 = g_CameraMan().getCamera(g_CameraMan().m_camerasName[option]);
          }
          else
          {
            m_interpolateCam1 = nullptr;
          }
          m_camInterId1 = option;
        }
        ImGui::EndCombo();
      }
      optionPreviw = g_CameraMan().m_camerasName[m_camInterId2];
      if (ImGui::BeginCombo("Camera 2", optionPreviw.c_str()))
      {
        int option = m_camInterId2;
        optionPreviw = "";
        ImGui::ListBox("camera 2##1", &option,
          &g_CameraMan().m_camerasName[0],
          (int)g_CameraMan().m_camerasName.size());
        if (option != (int32)m_camInterId2)
        {
          //static String g_UnitOptionPreviw = ResourceManager::instance().m_texturesNames[0];
          if (option > 0)
          {
            m_interpolateCam2 = g_CameraMan().getCamera(g_CameraMan().m_camerasName[option]);
          }
          else
          {
            m_interpolateCam2 = nullptr;
          }
          m_camInterId2 = option;
        }
        ImGui::EndCombo();
      }
      ImGui::DragFloat("Interpolation Time", &m_interpolationCamerasTime, 1.0f, 1.0f, 1000.0f);
      if (nullptr != m_interpolateCam1 && nullptr != m_interpolateCam2)
      {
        if (ImGui::Button("Play"))
        {
          g_CameraMan().m_interpolating = true;
        }
      }
    }
    ImGui::End();
  }

  void 
  RenderFNDApi::imGuiLoadResourceFile() {
    m_fileDialog->Display();
    if (m_fileDialog->HasSelected()) {
      uint32 files = (uint32)m_fileDialog->GetMultiSelected().size();
      for (uint32 i = 0; i < files; ++i) {
        g_ResourceMan().loadResourceFromFile(m_fileDialog->GetMultiSelected()[i].string().c_str(),
          m_typeResourceToLoad);
      }
      //std::cout << "Selected filename" << m_fileDialog->GetSelected().string() << std::endl;
      m_fileDialog->ClearSelected();
      m_loadingFile = false;
    }
  }

  void 
  RenderFNDApi::renderForward(SPtr<RenderTarget> rt) {
    g_graphicsApi().setRenderTarget(rt);
    g_graphicsApi().setViewPort(m_viewPortScene);
    g_graphicsApi().Clear(rt, 0.2f, 0.2f, 0.2f, 1);
    g_graphicsApi().ClearDepthStencil(rt);
    g_graphicsApi().setProgramShader(m_PSForward);
    g_graphicsApi().PixelShaderSetShaderResources(m_ambientCubeMap, 4);
    for (auto object : SceneGraph::instance().m_GObjects) {
      auto component =
        object->getComponent(COMPONENT_TYPE::TRANSFORM);
      CTransform* trans = reinterpret_cast<CTransform*>(component.get());
      m_DChangeEveryFrame.m_world = trans->getMatrixTransform();

      g_graphicsApi().updateSubresource(m_CBchangeEveryFrame, &m_DChangeEveryFrame);

      object->draw();
    }   

    g_graphicsApi().setPrimitiveTopology(PRIMITIVE_TOPOLOGY_FORMAT::POINTLIST);
    g_graphicsApi().setInputLayout(m_inLayOutBillBoard);

    g_graphicsApi().setProgramShader(m_PSBillboard);
    g_graphicsApi().setGeometryShader(m_GSBillboard);
    g_graphicsApi().GeometryShaderSetConstanBuffer(m_CBneverChange, 0);
    g_graphicsApi().GeometryShaderSetConstanBuffer(m_CBchangeOnResize, 1);
    g_graphicsApi().GeometryShaderSetConstanBuffer(m_CBchangeEveryFrame, 2);
    //g_graphicsApi().updateSubresource(m_CBchangeEveryFrame, &m_DChangeEveryFrame);

    g_graphicsApi().setVertexBuffer(m_particleVB);
    g_graphicsApi().DrawInstanced(1,1);
    g_graphicsApi().removeGeometryShader();
    g_graphicsApi().setProgramShader(m_PSForward);

    g_graphicsApi().setPrimitiveTopology(PRIMITIVE_TOPOLOGY_FORMAT::TRIANGLELIST);
    g_graphicsApi().setInputLayout(m_inLayOut);
  }

  void 
  RenderFNDApi::renderDeferred() {
    if (!m_showDeferred)
    {
      return;
    }

    geometryPass();
    shadowPass();
    lightsPass();
    AmbientOculsionPass();
    BlurPass(m_RTAmbientOclusion);

    ImGui::Begin("Deferred",
      &m_showDeferred);
    ImVec2 wsize = { (float)m_sceneSize.x,(float)m_sceneSize.y };
    ImGui::Text("Postions");
    ImGui::Image(m_RTGeometry.get()->getRenderTexture(0), wsize);
    ImGui::Text("Normals");
    ImGui::Image(m_RTGeometry.get()->getRenderTexture(1), wsize);
    ImGui::Text("Albedo");
    ImGui::Image(m_RTGeometry.get()->getRenderTexture(2), wsize);
    ImGui::Text("Depth");
    ImGui::Image(m_RTGeometry.get()->getRenderTexture(3), wsize);
    ImGui::Text("Diffuse");
    ImGui::Image(m_RTLights.get()->getRenderTexture(0), wsize);
    ImGui::Text("Specular");
    ImGui::Image(m_RTLights.get()->getRenderTexture(1), wsize);
    ImGui::Text("Ambient");
    ImGui::Image(m_RTLights.get()->getRenderTexture(2), wsize);
    ImGui::Text("Addition");
    ImGui::Image(m_RTLights.get()->getRenderTexture(3), wsize);
    ImGui::Text("Shado Map");
    ImGui::Image(m_RTShadowMap.get()->getRenderTexture(0), wsize);
    ImGui::Text("AmbientOculsionBeforeBlur");
    ImGui::Image(m_RTAmbientOclusion->getRenderTexture(0), wsize);

    AddPass(m_RTAOclusionBlured, m_RTBlurH, m_RTBlurV);
    for (uint32 i = 0; i < (uint32)m_bluredAOPassesNum; i++)
    {
      BlurPass(m_RTAOclusionBlured);
      AddPass(m_RTAOclusionBlured, m_RTBlurH, m_RTBlurV);
    }

    ColorWithAmbientOculsionPass();

    ImGui::Text("BlurH");
    ImGui::Image(m_RTBlurH->getRenderTexture(), wsize);
    ImGui::Text("BlurV");
    ImGui::Image(m_RTBlurV->getRenderTexture(), wsize);
    ImGui::Text("AmbientOculsionAfterBlur");
    ImGui::Image(m_RTAOclusionBlured->getRenderTexture(0), wsize);
    ImGui::Text("Light Color AO");
    ImGui::Image(m_RTColorAO->getRenderTexture(0), wsize);

    LuminancePass();
    BrightPass();
    AddBirghtPass();
    toneMapPass();

    ImGui::Text("Luminance");
    ImGui::Image(m_RTLuminance.get()->getRenderTexture(0), wsize);
    ImGui::Text("Bright");
    ImGui::Image(m_RTBright.get()->getRenderTexture(0), wsize);
    ImGui::Text("Add Bright");
    ImGui::Image(m_RTAddBright.get()->getRenderTexture(0), wsize);
    ImGui::Text("Tone map");
    ImGui::Image(m_RTToneMap.get()->getRenderTexture(0), wsize);
    ImGui::End();

    if (m_bWireframe)
    {
      g_graphicsApi().setRasterizeState(m_wireframeRasState);
      g_graphicsApi().setProgramShader(m_PSWireFrame);
    }
    else
    {
      g_graphicsApi().setRasterizeState(m_defaultRasState);
      g_graphicsApi().setProgramShader(m_PSForward);
    }
  }

  void 
  RenderFNDApi::shadowPass() {
    JDMatrix4 depthProj = createProjectionOrthographicMatrix(-500, 500, -500, 500, -200, 800);
    //JDMatrix4 depthProj = createProjectionPerspectiveMatrix(Math::HALF_PI / 2, float(1000 / 1000),3, 10000.0f);
    auto component = m_slight->getComponent(COMPONENT_TYPE::TRANSFORM);
    if (nullptr == component) {
      return;
    }
    CTransform* trans = reinterpret_cast<CTransform*>(component.get());
    JDVector4 eye = m_DLights.light[0].m_lightPosition;
    JDVector4 at = eye + m_DLights.light[0].m_lightDirection;
    JDMatrix4 depthView = createViewMatrix(trans->position, 
                                           trans->position + trans->forward,
                                           trans->up);
    depthView.transpose();
    depthProj.transpose();
    m_DChangeEveryFrame.m_depthWVP = depthProj * depthView;
    //m_DChangeEveryFrame.m_depthWVP = m_DChangeOnResize.m_projection* m_DNeverChanges.m_view;

    g_graphicsApi().updateSubresource(m_CBchangeEveryFrame, &m_DChangeEveryFrame);

    g_graphicsApi().setRenderTarget(m_RTShadowMap);
    g_graphicsApi().Clear(m_RTShadowMap, 1, 1, 1, 1);
    g_graphicsApi().ClearDepthStencil(m_RTShadowMap);
    g_graphicsApi().setProgramShader(m_PSShadowMap);
    for (auto object : SceneGraph::instance().m_GObjects)
    {
      auto component2 =
        object->getComponent(COMPONENT_TYPE::TRANSFORM);
      CTransform* trans2 = reinterpret_cast<CTransform*>(component2.get());
      m_DChangeEveryFrame.m_world = trans2->getMatrixTransform();

      g_graphicsApi().updateSubresource(m_CBchangeEveryFrame, &m_DChangeEveryFrame);

      object->draw();
    }
    g_graphicsApi().generateMipMap(m_RTShadowMap);

  }

  void 
  RenderFNDApi::geometryPass() {
    g_graphicsApi().setRenderTarget(m_RTGeometry);
    g_graphicsApi().Clear(m_RTGeometry, 0, 0, 0, 1);
    g_graphicsApi().ClearDepthStencil(m_RTGeometry);
    g_graphicsApi().setProgramShader(m_PSGeometry);
    changeCameraDataBuffer(m_debugCam);
    g_graphicsApi().PixelShaderSetShaderResources(m_ambientCubeMap, 4);
    for (auto object : SceneGraph::instance().m_GObjects)
    {
      auto component =
        object->getComponent(COMPONENT_TYPE::TRANSFORM);
      CTransform* trans = reinterpret_cast<CTransform*>(component.get());
      m_DChangeEveryFrame.m_world = trans->getMatrixTransform();
  
      g_graphicsApi().updateSubresource(m_CBchangeEveryFrame, &m_DChangeEveryFrame);
  
      object->draw();
    }
    g_graphicsApi().generateMipMap(m_RTGeometry);
  }
  
  void 
  RenderFNDApi::lightsPass() {
    g_graphicsApi().setRenderTarget(m_RTLights);
    g_graphicsApi().Clear(m_RTLights, 0.2f, 0.2f, 0.2f, 1);
    g_graphicsApi().ClearDepthStencil(m_RTLights);
    g_graphicsApi().setProgramShader(m_PSLights);
    changeCameraDataBuffer(m_debugCam);
    g_graphicsApi().PixelShaderSetShaderResources(m_ambientCubeMap, 4);
    g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTShadowMap, 5, 0);
    for (auto object : SceneGraph::instance().m_GObjects)
    {
      auto component =
        object->getComponent(COMPONENT_TYPE::TRANSFORM);
      CTransform* trans = reinterpret_cast<CTransform*>(component.get());
      m_DChangeEveryFrame.m_world = trans->getMatrixTransform();
  
      g_graphicsApi().updateSubresource(m_CBchangeEveryFrame, &m_DChangeEveryFrame);
  
      object->draw();
    }
    g_graphicsApi().generateMipMap(m_RTLights);
  }
  
  void 
  RenderFNDApi::LuminancePass() {
    g_graphicsApi().setProgramShader(m_PSLuminance);
    g_graphicsApi().setRenderTarget(m_RTLuminance);
    g_graphicsApi().Clear(m_RTLuminance, 0.2f, 0.2f, 0.2f, 1);
    g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTLights, 0, 0);
    g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTLights, 1, 1);
    g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTLights, 2, 2);
    g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTColorAO, 3, 0);
    g_ResourceMan().setSAQ();
    g_graphicsApi().generateMipMap(m_RTLuminance);
  }
  
  void 
  RenderFNDApi::BrightPass() {
    g_graphicsApi().setProgramShader(m_PSBright);
    g_graphicsApi().setRenderTarget(m_RTBright);
    g_graphicsApi().Clear(m_RTBright, 0.2f, 0.2f, 0.2f, 1);
    g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTLights, 0, 0);
    g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTLights, 1, 1);
    g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTLights, 2, 2);
    g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTLuminance, 3, 0);
    g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTColorAO, 4, 0);
    g_graphicsApi().PixelShaderSetConstanBuffer(m_CBBright, 6);
    g_ResourceMan().setSAQ();
    g_graphicsApi().generateMipMap(m_RTBright);
  }
  
  void 
  RenderFNDApi::AmbientOculsionPass() {
    g_graphicsApi().setProgramShader(m_PSAOclusion);
    g_graphicsApi().setRenderTarget(m_RTAmbientOclusion);
    g_graphicsApi().Clear(m_RTAmbientOclusion, 1, 1, 1, 1);
    g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTGeometry, 0, 0);
    g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTGeometry, 1, 1);
    g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTGeometry, 2, 3);
    g_graphicsApi().PixelShaderSetConstanBuffer(m_CBAO, 5);
    g_ResourceMan().setSAQ();
    g_graphicsApi().generateMipMap(m_RTAmbientOclusion);
  }
  
  void 
  RenderFNDApi::ColorWithAmbientOculsionPass() {
    g_graphicsApi().setProgramShader(m_PSBColorAO);
    g_graphicsApi().setRenderTarget(m_RTColorAO);
    g_graphicsApi().Clear(m_RTColorAO, 0.2f, 0.2f, 0.2f, 1);
    g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTLights, 0, 0);
    g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTLights, 1, 1);
    g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTLights, 2, 2);
    g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTAOclusionBlured, 3, 0);
    g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTLights, 4, 3);
    g_ResourceMan().setSAQ();
    g_graphicsApi().generateMipMap(m_RTColorAO);
  }
  
  void 
  RenderFNDApi::BlurPass(SPtr<RenderTarget> rtToBlur) {
  
    g_graphicsApi().setProgramShader(m_PSBBlurH);
    g_graphicsApi().PixelShaderSetConstanBuffer(m_CBBlur,7);
    g_graphicsApi().setRenderTarget(m_RTBlurH);
    //viewport escalado
    ViewPort tmp = m_viewPortScene;
    tmp.Height = m_viewPortScene.Height * 0.5f;
    tmp.Width = m_viewPortScene.Width * 0.5f;
    g_graphicsApi().setViewPort(tmp);
    g_graphicsApi().Clear(m_RTBlurH, 0.2f, 0.2f, 0.2f, 1);
    g_graphicsApi().PixelShaderSetShaderResourcesFromRT(rtToBlur, 0, 0);
    g_ResourceMan().setSAQ();
    g_graphicsApi().generateMipMap(m_RTBlurH);
  
    g_graphicsApi().setProgramShader(m_PSBBlurV);
    g_graphicsApi().PixelShaderSetConstanBuffer(m_CBBlur,7);
    g_graphicsApi().setRenderTarget(m_RTBlurV);
    g_graphicsApi().Clear(m_RTBlurV, 0.2f, 0.2f, 0.2f, 1);
    g_graphicsApi().PixelShaderSetShaderResourcesFromRT(rtToBlur, 0, 0);
    g_ResourceMan().setSAQ();
    g_graphicsApi().generateMipMap(m_RTBlurV);
    //
    //viewport normal
    g_graphicsApi().setViewPort(m_viewPortScene);
  }
  
  void 
  RenderFNDApi::AddPass(SPtr<RenderTarget> target, 
                        SPtr<RenderTarget> renderTarget1, 
                        SPtr<RenderTarget> renderTarget2, 
                        uint32 slotOfTarget1, 
                        uint32 slotOfTarget2) {
  
    g_graphicsApi().setProgramShader(m_PSAdd);
    g_graphicsApi().PixelShaderSetConstanBuffer(m_CBBlur,7);
    g_graphicsApi().setRenderTarget(target);
    g_graphicsApi().Clear(target, 0.2f, 0.2f, 0.2f, 1);
    g_graphicsApi().PixelShaderSetShaderResourcesFromRT(renderTarget1, 0, slotOfTarget1);
    g_graphicsApi().PixelShaderSetShaderResourcesFromRT(renderTarget2, 1, slotOfTarget2);
    g_ResourceMan().setSAQ();
    g_graphicsApi().generateMipMap(target);
  }

  void 
  RenderFNDApi::AddBirghtPass() {
    BlurPass(m_RTBright);
    AddPass(m_RTBlurAdded, m_RTBlurH, m_RTBlurV);
    for (uint32 i = 0; i < m_bluredBirghtPassesNum; i++)
    {
      BlurPass(m_RTBlurAdded);
      AddPass(m_RTBlurAdded, m_RTBlurH, m_RTBlurV);
    }
    AddPass(m_RTAddBright, m_RTBlurAdded, m_RTColorAO);
  }

  void 
  RenderFNDApi::toneMapPass() {
    g_graphicsApi().setProgramShader(m_PSToneMap);
    g_graphicsApi().PixelShaderSetConstanBuffer(m_CBBright, 6);
    g_graphicsApi().setRenderTarget(m_RTToneMap);
    g_graphicsApi().Clear(m_RTToneMap, 0.2f, 0.2f, 0.2f, 1);
    g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTAddBright, 0, 0);
    g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTColorAO, 1, 0);
    g_ResourceMan().setSAQ();
    g_graphicsApi().generateMipMap(m_RTToneMap);
  }

  void 
  RenderFNDApi::onResizeSceneWindow(uint32 width, uint32 height) {
    if (!*m_windowHasFocus)
    {
      return;
    }
    m_rtv = g_graphicsApi().getRenderTargetView();
    g_graphicsApi().setRenderTarget(m_rtv);
    g_CameraMan().resizeCameraProjection(width, height);
    m_debugCam->resize(width, height);

    m_DChangeOnResize.m_projection = m_debugCam->getMatrixProjection();
    m_DChangeOnResize.m_projection.transpose();
    m_DChangeOnResize.m_projectionInv = m_DChangeOnResize.m_projection;
    m_DChangeOnResize.m_projectionInv.invert();
    g_graphicsApi().updateSubresource(m_CBchangeOnResize, &m_DChangeOnResize);
  }
}