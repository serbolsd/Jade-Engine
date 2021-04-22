#include "jdRenderShaderTests.h"
#include <jdComponentTransform.h>
#include <jdComponentRenderModel.h>
#include <jdComponentLight.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <CL/cl.h>
using std::cout;
using std::endl;
namespace jdEngineSDK {
  RenderShaderTestingApi::RenderShaderTestingApi()
  {
  }

  RenderShaderTestingApi::~RenderShaderTestingApi()
  {
  }

  void 
  RenderShaderTestingApi::init(void* wndHandle, const JDPoint& clientSize) {
    m_wndHandle = wndHandle;
    m_clientSize = clientSize;
    onCreate();
  }

  void 
  RenderShaderTestingApi::onUpdate(const float& deltaTime) {

    //UpdateCameraInterpolate(deltaTime);
    m_DBezierCB.m_world = m_world;
    m_DBezierCB.m_world.scale(m_worldScale);
    //m_DBezierCB.m_world = m_DBezierCB.m_world.invert();
    m_DBezierCB.m_view = m_debugCam->getMatrixView();
    //m_DBezierCB.m_view = m_DBezierCB.m_view.invert();
    m_DBezierCB.m_proj = m_debugCam->getMatrixProjection();
    //m_DBezierCB.m_proj = m_DBezierCB.m_proj.invert();


    g_graphicsApi().updateSubresource(m_CBBezier, &m_DBezierCB);
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
  }

  void 
  RenderShaderTestingApi::onRender() {
    g_graphicsApi().setRenderTarget(m_rtv);
    g_graphicsApi().setViewPort(m_viewPort);
    g_graphicsApi().Clear(m_rtv, 1, 1, 1, 1);
    g_graphicsApi().ClearDepthStencil(m_rtv);
    g_graphicsApi().setProgramShader(m_PSBasic);
    //Set Primitive topology
    g_graphicsApi().setPrimitiveTopology(PRIMITIVE_TOPOLOGY_FORMAT::CONTROL_POINT_PATCHLIST_16);
    g_graphicsApi().setVertexBuffer(m_figureVB);
    g_graphicsApi().Draw(64);
    

    g_graphicsApi().setPrimitiveTopology(PRIMITIVE_TOPOLOGY_FORMAT::TRIANGLELIST);
    g_graphicsApi().setVertexBuffer(m_cubeVB);
    g_graphicsApi().setIndexBuffer(m_cubeIB);
    g_graphicsApi().DrawIndex(36);
    //g_graphicsApi().DrawIndex(0);
    
    ImGui::Begin("Inspector", NULL);
    JDVector4 pos = m_debugCam->getPositionVector();

    ImGui::InputFloat3("View Position", &pos.x, 2);
    ImGui::DragFloat("World Scale", &m_worldScale,0.5f,0.5f,1000.0f);

    ImGui::End();

    //g_graphicsApi().Present();


    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
  }

  void 
  RenderShaderTestingApi::onDestroy() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
  }

  void 
  RenderShaderTestingApi::onResize(int32 width, int32 height) {
    if (!*m_windowHasFocus)
    {
      return;
    }
    m_rtv = g_graphicsApi().getRenderTargetView();
    g_graphicsApi().setRenderTarget(m_rtv);
    g_CameraMan().resizeCameraProjection(width, height);
    m_debugCam->resize(width, height);

    m_DBezierCB.m_proj = m_debugCam->getMatrixProjection();
    m_DBezierCB.m_view = m_debugCam->getMatrixView();
    g_graphicsApi().updateSubresource(m_CBBezier, &m_DBezierCB);

    ImGui::GetIO().DisplaySize = ImVec2((float)width, (float)height);
    ImGui::GetIO().DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    m_viewPort.Width = (float)width;
    m_viewPort.Height = (float)height;
    m_viewPort.MinDepth = 0.0f;
    m_viewPort.MaxDepth = 1.0f;
  }

  void 
  RenderShaderTestingApi::onMouseButtonPressed(int32 button, int32 x, int32 y) {
    ImGuiIO& io = ImGui::GetIO();
    if (!ImGui::IsAnyMouseDown() && nullptr == ::GetCapture())
    {
      ::SetCapture((HWND)m_wndHandle);
    }
    io.MouseDown[static_cast<int32>(button)] = true;
  }

  void 
  RenderShaderTestingApi::onTextEnterd(UNICHAR unicode) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacterUTF16((unsigned short)unicode);
  }

  void 
  RenderShaderTestingApi::onKeyPressed(int32 code, 
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
  RenderShaderTestingApi::onKeyReleased(int32 code, 
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
  RenderShaderTestingApi::onMouseButtonReleased(int32 button, int32 x, int32 y) {
    ImGuiIO& io = ImGui::GetIO();
    if (!ImGui::IsAnyMouseDown() && ::GetCapture() == m_wndHandle)
    {
      ::ReleaseCapture();
    }
    io.MouseDown[static_cast<int32>(button)] = false;
  }

  void 
  RenderShaderTestingApi::onCreate() {
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

    //Crete constant Buffers
    m_CBBezier = g_graphicsApi().CreateConstantBuffer(sizeof(CBBezier));

    // Get the default Camera
    m_debugCam = g_CameraMan().getDebugCamera();

    //m_DChangeOnResize.mProjection.transpose();
    m_world.identity();
    m_DBezierCB.m_world.identity();
    m_DBezierCB.m_view = m_debugCam->getMatrixView();
    m_DBezierCB.m_proj = m_debugCam->getMatrixProjection();

    //Update buffers
    g_graphicsApi().updateSubresource(m_CBBezier, &m_DBezierCB);

    //Set contant buffers
    g_graphicsApi().VertexShaderSetConstanBuffer(m_CBBezier, 0);

    //Set Primitive topology
    g_graphicsApi().setPrimitiveTopology(PRIMITIVE_TOPOLOGY_FORMAT::TRIANGLELIST);

    SimpleVertex BezierFigure[64] = {
   	JDVector4(1.0f, 0.5f, 0.0f),
   	JDVector4(1.0f, -0.5f, 0.5f ),
   	JDVector4(0.5f, -0.3536f, 1.354f ),
   	JDVector4(0.0f, -0.3536f, 1.354f ),
   	JDVector4(1.0f, -0.1667f, 0.0f ),
   	JDVector4(1.0f, -0.1667f, 0.5f ),
   	JDVector4(0.5f, -0.1179f, 1.118f ),
   	JDVector4(0.0f, -0.1179f, 1.118f ),
   	JDVector4(1.0f, 0.1667f, 0.0f ),
   	JDVector4(1.0f, 0.1667f, 0.5f ),
   	JDVector4(0.5f, 0.1179f, 0.8821f ),
   	JDVector4(0.0f, 0.1179f, 0.8821f ),
   	JDVector4(1.0f, 0.5f, 0.0f ),
   	JDVector4(1.0f, 0.5f, 0.5f ),
   	JDVector4(0.5f, 0.3536f, 0.6464f ),
   	JDVector4(0.0f, 0.3536f, 0.6464f ),
   	JDVector4(0.0f, -0.3536f, 1.354f ),
   	JDVector4(-0.5f, -0.3536f, 1.354f ),
   	JDVector4(-1.5f, 0.0f, 0.5f ),
   	JDVector4(-1.5f, 0.0f, 0.0f ),
   	JDVector4(0.0f, -0.1179f, 1.118f ),
   	JDVector4(-0.5f, -0.1179f, 1.118f ),
   	JDVector4(-1.167f, 0.0f, 0.5f ),
   	JDVector4(-1.167f, 0.0f, 0.0f ),
   	JDVector4(0.0f, 0.1179f, 0.8821f ),
   	JDVector4(-0.5f, 0.1179f, 0.8821f ),
   	JDVector4(-0.8333f, 0.0f, 0.5f ),
   	JDVector4(-0.8333f, 0.0f, 0.0f ),
   	JDVector4(0.0f, 0.3536f, 0.6464f ),
   	JDVector4(-0.5f, 0.3536f, 0.6464f ),
   	JDVector4(-0.5f, 0.0f, 0.5f ),
   	JDVector4(-0.5f, 0.0f, 0.0f ),
   	JDVector4(-1.5f, 0.0f, 0.0f ),
   	JDVector4(-1.5f, 0.0f, -0.5f ),
   	JDVector4(-0.5f, 0.3536f, -1.354f ),
   	JDVector4(0.0f, 0.3536f, -1.354f ),
   	JDVector4(-1.167f, 0.0f, 0.0f ),
   	JDVector4(-1.167f, 0.0f, -0.5f ),
   	JDVector4(-0.5f, 0.1179f, -1.118f ),
   	JDVector4(0.0f, 0.1179f, -1.118f ),
   	JDVector4(-0.8333f, 0.0f, 0.0f ),
   	JDVector4(-0.8333f, 0.0f, -0.5f ),
   	JDVector4(-0.5f, -0.1179f, -0.8821f ),
   	JDVector4(0.0f, -0.1179f, -0.8821f ),
   	JDVector4(-0.5f, 0.0f, 0.0f ),
   	JDVector4(-0.5f, 0.0f, -0.5f ),
   	JDVector4(-0.5f, -0.3536f, -0.6464f ),
   	JDVector4(0.0f, -0.3536f, -0.6464f ),
   	JDVector4(0.0f, 0.3536f, -1.354f ),
   	JDVector4(0.5f, 0.3536f, -1.354f ),
   	JDVector4(1.0f, 0.5f, -0.5f ),
   	JDVector4(1.0f, 0.5f, 0.0f ),
   	JDVector4(0.0f, 0.1179f, -1.118f ),
   	JDVector4(0.5f, 0.1179f, -1.118f ),
   	JDVector4(1.0f, 0.1667f, -0.5f ),
   	JDVector4(1.0f, 0.1667f, 0.0f ),
   	JDVector4(0.0f, -0.1179f, -0.8821f ),
   	JDVector4(0.5f, -0.1179f, -0.8821f ),
   	JDVector4(1.0f, -0.1667f, -0.5f ),
   	JDVector4(1.0f, -0.1667f, 0.0f ),
   	JDVector4(0.0f, -0.3536f, -0.6464f ),
   	JDVector4(0.5f, -0.3536f, -0.6464f ),
   	JDVector4(1.0f, -0.5f, -0.5f ),
   	JDVector4(1.0f, -0.5f, 0.0f ),};

    m_figureVB = g_graphicsApi().createVertexBuffer(64,
                                                    sizeof(SimpleVertex), 
                                                    &BezierFigure[0]);

    // Create vertex buffer
    SimpleVertex vertices[] =
    {
        { JDVector4(-1.0f, 1.0f, -1.0f,1.0f)},
        { JDVector4(1.0f, 1.0f, -1.0f,1.0f)},
        { JDVector4(1.0f, 1.0f, 1.0f ,1.0f)},
        { JDVector4(-1.0f, 1.0f, 1.0f ,1.0f)},

        { JDVector4(-1.0f, -1.0f, -1.0f ,1.0f)},
        { JDVector4(1.0f, -1.0f, -1.0f ,1.0f)},
        { JDVector4(1.0f, -1.0f, 1.0f,1.0f)},
        { JDVector4(-1.0f, -1.0f, 1.0f ,1.0f)},
         
        { JDVector4(-1.0f, -1.0f, 1.0f ,1.0f)},
        { JDVector4(-1.0f, -1.0f, -1.0f ,1.0f)},
        { JDVector4(-1.0f, 1.0f, -1.0f,1.0f)},
        { JDVector4(-1.0f, 1.0f, 1.0f,1.0f)},

        { JDVector4(1.0f, -1.0f, 1.0f ,1.0f)},
        { JDVector4(1.0f, -1.0f, -1.0f ,1.0f)},
        { JDVector4(1.0f, 1.0f, -1.0f ,1.0f)},
        { JDVector4(1.0f, 1.0f, 1.0f,1.0f)},
          
        { JDVector4(-1.0f, -1.0f, -1.0f,1.0f)},
        { JDVector4(1.0f, -1.0f, -1.0f ,1.0f)},
        { JDVector4(1.0f, 1.0f, -1.0f,1.0f)},
        { JDVector4(-1.0f, 1.0f, -1.0f ,1.0f)},

        { JDVector4(-1.0f, -1.0f, 1.0f ,1.0f)},
        { JDVector4(1.0f, -1.0f, 1.0f ,1.0f)},
        { JDVector4(1.0f, 1.0f, 1.0f,1.0f)},
        { JDVector4(-1.0f, 1.0f, 1.0f ,1.0f)},
    };
    m_cubeVB = g_graphicsApi().createVertexBuffer(24,
                                                  sizeof(SimpleVertex), 
                                                  &vertices[0]);
    uint32 indices[] =
    {
        3, 1, 0,
        2, 1, 3,

        6, 4, 5,
        7, 4, 6,

        11,9, 8 ,
        10,9, 11,

        14,12,13,
        15,12,14,

        19,17,16,
        18,17,19,

        22,20,21,
        23,20,22
    };
    m_cubeIB = g_graphicsApi().createIndexBuffer(36 ,indices);
    initImGui();
  }

  void 
  RenderShaderTestingApi::initImGui() { 
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
  RenderShaderTestingApi::imguiDockScreen() {
  }

}