#pragma once
#include <jdGraphicApi.h>
#include <jdBaseApp.h>
#include <jdVector2.h>
#include <jdVector3.h>
#include <jdVector4.h>
#include <jdMatrix4.h>
#include <jdPoint.h>
#include <jdResource.h>
#include <imgui.h>
#include <imfilebrowser.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
using namespace jdEngineSDK; 

/**
 * @brief struct to vertex data
 */
struct SimpleVertex
{
  JDVector4 pos;
  JDVector2 textcoord;
};

/**
 * @brief struct to contant buffer never changes data
 */
struct CBNeverChanges
{
  JDMatrix4 m_view;
  JDMatrix4 m_viewInv;
};

/**
 * @brief struct to contant buffer changes on resize data
 */
struct CBChangeOnResize
{
  JDMatrix4 m_projection;
  JDMatrix4 m_projectionInv;
  JDMatrix4 m_viewProjection;
  JDMatrix4 m_viewProjectionInv;
};

/**
 * @brief struct to contant buffer changes every frame data
 */
struct CBChangesEveryFrame
{
  JDMatrix4 m_world;
  JDMatrix4 m_worldInv;
  JDMatrix4 m_worldView;
  JDMatrix4 m_worldViewInv;
  JDMatrix4 m_worldProj;
  JDMatrix4 m_worldProjInv;
  JDMatrix4 m_worldViewProj;
  JDMatrix4 m_worldViewProjInv;
  JDVector4 m_viewPosition;
  JDVector4 vMeshColor;
};

/**
 * @brief struct whit 20 lights data
 * @note type 0 = directional 1 = point 2 = spot
 */
struct lightStruct
{
  JDVector4 m_lightDirection = { 0, 0, 0, 0 };
  JDVector4 m_lightPosition = { 0, 0, 0, 0 };
  JDVector4 m_lightColor = { 0, 0, 0, 0 };
  uint32 m_type = 0;//0= directional 1=point 2= spot
  uint32 m_numberOfLights;
  uint32 m_relleno1;
  uint32 m_relleno2;
};

struct CBLights
{
  lightStruct light[20];
};

class testApp : public BaseApp
{
 public:
  /**
   * @brief Default constructor
   */
  testApp();
  
  /**
   * @brief constructor with window's params
   * @param winPos is the window's position to set
   * @param winSize is the window's client size to set
   * @param winTitle is the title of window to set
   * @param fullScreen is if you want the window on full screen
   */
  testApp(const JDPoint& winPos,
  		      const JDPoint& winSize,
  		      const String& winTitle,
  		      bool fullScreen);
  
  /**
   * @brief destructor
   */
  ~testApp() {};
 private:
  /**
   * @brief function to call when the app is being created, before the loop starts
   */
  void
  onCreate() override;

  /**
   * @brief function to call to render
   */
  void
  onRender() override;
  
  /**
   * @brief function to call when the window resized
   * @param width is the new width of client
   * @param height is the new width of height
   */
  void
  onResize(int32 width, int32 height) override;

  void
  onResizeSceneWindow(uint32 width, uint32 height);

  /**
   * @brief function to call when a mouse button pressed
   * @param button is the button code
   * @param x is the position of the mouse pointer, relative to the left of the window.
   * @param y is the position of the mouse pointer, relative to the top of the window.
   */
  void
  onMouseButtonPressed(int32 button,
                       int32 x, 
                       int32 y) override;

  /**
   * @brief function to call when text entered
   * @param unicode is the text that entered
   */
  void
  onTextEnterd(UNICHAR unicode) override;

  /**
   * @brief function to call when a key is pressed
   * @param code is the key code pressed
   * @param control is if the key control is pressing
   * @param shift is if the key shift is pressing
   * @param system is if the key system is pressing
   */
  void
  onKeyPressed(int32 code, 
               bool alt, 
               bool control, 
               bool shift, 
               bool system) override;

  /**
   * @brief function to call when a key is released
   * @param code is the key code pressed
   * @param control is if the key control is pressing
   * @param shift is if the key shift is pressing
   * @param system is if the key system is pressing
   */
  void
  onKeyReleased(int32 code, 
                bool alt, 
                bool control, 
                bool shift, 
                bool system) override;

  /**
   * @brief function to call when a mouse button released
   * @param button is the button code
   * @param x is the position of the mouse pointer, relative to the left of the window.
   * @param y is the position of the mouse pointer, relative to the top of the window.
   */
  void
  onMouseButtonReleased(int32 button,
                        int32 x, 
                        int32 y) override;

  /**
   * @brief function to call to update the data
   */
  void
  onUpdate(const float& deltaTime) override;

  /**
   * @brief init ImGui
   */
  void
  initImGui();

  void
  imguiDockScreen();

  void
  imGuiShowObject(const char* nameObject, WeakSptr<GameObject> child);

  void
  imGuiInspectorObject();

  void
  showRenderModelComponent();

  void
  showLightComponent();

  void
  showRenderModelMaterias(CRenderModel* rModel);

  void
  ImGuiAddComponent();

  void
  addLightComponent();

  void
  imGuiLoadResourceFile();

  void
  showAmbientOption();
 
  void
  showModels();

  void
  showTexturesResources();
  /**
   * @brief function to call when the app is being detroyed, after the loop ends
   */
  void
  onDestroy() override;
  
  /**
   * @brief the data to never change buffer
   */
  CBNeverChanges m_neverChanges;
  
  /**
   * @brief the data to never change on resize
   */
  CBChangeOnResize m_changeOnResize;
  
  /**
   * @brief the data to never change every frame
   */
  CBChangesEveryFrame m_changeEveryFrame;

  /**
   * @brief the data to never change every frame
   */
  CBLights m_lights;
  
  /**
   * @brief shared pointer to render target view
   */
  SPtr<RenderTargetView> m_rtv = nullptr;
  SPtr<RenderTarget> m_first = nullptr;
  
  /**
   * @brief shared pointer to a program shader
   */
  SPtr<ProgramShader> m_progShader = nullptr;

  /**
   * @brief shared pointer to a program shader
   */
  SPtr<ProgramShader> m_progShaderWF = nullptr;
  
  /**
   * @brief elemts of input layout
   */
  Vector<INPUT_LAYOUT_DESC> m_inLayoutElements;
  
  /**
   * @brief shared ponter to a input layout
   */
  SPtr<InputLayout> m_inLayOut = nullptr;
  
  /**
   * @brief shared ponter to a vertex buffer
   */
  SPtr<VertexBuffer> m_vertexB = nullptr;
  
  /**
   * @brief shared ponter to a index buffer
   */
  SPtr<IndexBuffer> m_indexB = nullptr;
  
  /**
   * @brief shared ponter to a constant buffer never change
   */
  SPtr<ConstantBuffer> m_neverChangeB = nullptr;
  
  /**
   * @brief shared ponter to a constant buffer cahnge on resize
   */
  SPtr<ConstantBuffer> m_changeOnResizeB = nullptr;
  
  /**
   * @brief shared ponter to a constant buffer cahnge every fram
   */
  SPtr<ConstantBuffer> m_changeEveryFrameB = nullptr;

  /**
   * @brief shared ponter to a constant buffer cahnge every fram
   */
  SPtr<ConstantBuffer> m_lightsB = nullptr;

  /**
   * @brief number of light created
   */
  uint32 m_lightCreated = 0;

  /**
   * @brief To imgui dock menu
   * @note to import resources and open options
   */
  bool dockMenuOpen = false;

  /**
   * @brief To show the button add component
   * @note only is false when the selected object is the root
   */
  bool m_showComponentImGui = false;

  /**
   * @brief To open the load file imgui window
   */
  bool m_loadingFile = false;

  /**
   * @brief the window data to load files
   */
  SPtr<ImGui::FileBrowser> m_fileDialog;

  /**
   * @brief the resourece's type to load
   */
  RESOURCE_TYPE::E m_typeResourceToLoad = RESOURCE_TYPE::UNKNOWTYPE;

  /**
   * @brief share pointer to debug camera
   * @note the debug camera is the camera how you can see the scene all time
   */
  SPtr<Camera> m_debugCam = nullptr;

  /**
   * @brief bool to adjust the scene window to the half size of the window
   * @note active to see the rendertexture of the secene in it's half real resoluiton
   */
  bool m_bSceneWindowNativeSize = false;

  /**
   * @brief bool to see ambient options
   */
  bool m_bAmbientOptions = false;

  /**
   * @brief bool to see ambient options
   */
  bool m_bWireframe = false;
  bool m_bWireframeAlreadyActive = false;

  SPtr<Texture2D> m_ambientCubeMap = nullptr;

  uint32 m_ambientCubeMapOption = 0;

  JDPoint m_sceneSize;

  SPtr<RasterizeState> m_defaultRasState = nullptr;
  SPtr<RasterizeState> m_wireframeRasState = nullptr;

};
