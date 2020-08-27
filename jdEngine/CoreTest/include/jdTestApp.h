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
  JDMatrix4 mView;
};

/**
 * @brief struct to contant buffer changes on resize data
 */
struct CBChangeOnResize
{
  JDMatrix4 mProjection;
};

/**
 * @brief struct to contant buffer changes every frame data
 */
struct CBChangesEveryFrame
{
  JDMatrix4 mWorld;
  JDVector4 vMeshColor;
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
  ImGuiAddComponent();

  void
  imGuiLoadResourceFile();

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
   * @brief shared pointer to render target view
   */
  SPtr<RenderTargetView> m_rtv = nullptr;
  SPtr<RenderTarget> m_first = nullptr;
  
  /**
   * @brief shared pointer to a program shader
   */
  SPtr<ProgramShader> m_progShader = nullptr;
  
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
  
  float m_scale = 0.5f;

  bool dockMenuOpen = false;

  bool m_showComponentImGui = false;

  bool m_loadingFile = false;

  ImGui::FileBrowser m_fileDialog;

  RESOURCE_TYPE::E m_typeResourceToLoad = RESOURCE_TYPE::UNKNOWTYPE;
};
