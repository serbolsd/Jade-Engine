/*****************************************************************************/
/**
 * @file    jdRenderFND.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    06/DIC/2020
 * @brief   class for Render with forward and deferred shader metod
 *
 *
 *
 * @bug     No known bugs.
 */
 /*****************************************************************************/
#pragma once
#include "jdRenderFNDPrerequisites.h"

#include <jdRender.h>
#include <jdGraphicApi.h>
#include <jdCameraManager.h>
#include <jdPoint.h>
#include <SFML/Window.hpp>
#include <imgui.h>
#include <ImGuizmo.h>
#include <ImSequencer.h>
//#include <ImZoomSlider.h>
#include <imfilebrowser.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

namespace jdEngineSDK {
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
    JDMatrix4 m_depthWVP;
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

  struct CBAO
  {
    float g_sample_radius = 2.0f;
    float g_scale = 1.0f;
    float g_Bias = 0.080f;
    float g_intensity = 2.0f;
  };

  struct CBBright
  {
    //x = BeightLod y = BloomThreshold;
    JDVector4 BrightLod_BloomThreshold = { 1,0.01f,0,0 };
  };

  struct CBMipLevels
  {
    int32 mipLevel0 = 0;
    int32 mipLevel1 = 0;
    int32 mipLevel2 = 0;
    int32 mipLevel3 = 0;
  };

  struct ParticleData
  {
    JDVector4 m_pos = {0, 0, 0, 0};
    JDVector2 m_size = {0, 0};
  };

  class RenderFNDApi : public RenderApi {
   public:
    /**
     * @brief Default constructor
     */
    RenderFNDApi();

    /**
     * @brief Default destructor
     */
    ~RenderFNDApi();

    /**
     * @brief function to call to init the render
     * @param wndHandle is the windowHandle
     * @param clientSize is size of the window's client
     */
    void
    init(void* wndHandle, const JDPoint& clientSize) override;

    /**
     * @brief function to call on update
     * @param deltaTime is de elapse time with the last frame
     */
    void
    onUpdate(const float& /*deltaTime*/) override;

    /**
     * @brief function to call on render
     */
    void
    onRender() override;

    /**
     * @brief function to call when shut down
     */
    void
    onDestroy() override;

    /**
     * @brief funtion to call back, check the inputs of window
     */
    void
    handleWindownput(const float& deltaTime) override;

    /**
     * @brief function to call on resize
     * @param width is the new width
     * @param height is the new height
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
    onMouseButtonPressed(int32 button, int32 x, int32 y) override;

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

   private:
    /**
     * @brief function to call on crete
     */
    void
    onCreate();

    /**
     * @brief function to Init imgui
     */
    void
    initImGui();

    /**
      * @brief the data to never change buffer
      */
    int LoadHistogram();

    /**
     * @brief Histogram RED data
     */
    Vector<float> m_redHisto;

    /**
     * @brief Histogram GREEN data
     */
    Vector<float> m_greenHisto;

    /**
     * @brief Histogram BLUE data
     */
    Vector<float> m_blueHisto;

    /**
     * @brief function to show Histogram window
     */
    void
    showHistorgramOption();

    /**
     * @brief function to update the camera interpolate
     */
    void 
    UpdateCameraInterpolate(const float& deltaTime);

    /**
     * @brief function to update de constant buffer whos needs camera data
     * @param cam is the came to get the data
     */
    void
    changeCameraDataBuffer(WeakSptr<Camera> cam);

    /**
     * @brief function to add a camera componente to a object
     */
    void
    addCameraComponent();

    /**
     * @brief function to set dockers of imgui and set the menuBar
     */
    void
    imguiDockScreen();

    /**
     * @brief function to draw the scene graph window
     */
    void
    imguiSceneGraph();

    /**
     * @brief function show objects and it´s children on imgui
     */
    void
    imGuiShowObject(const char* nameObject, WeakSptr<GameObject> child);

    /**
     * @brief function show the inspecto window
     */
    void
    imGuiInspectorObject();

    /**
     * @brief show the Render model component data
     */
    void
    showRenderModelComponent();

    /**
     * @brief show the Render model materials
     */
    void
    showRenderModelMaterias(CRenderModel* rModel);
    
    /**
     * @brief show the light component data
     */
    void
    showLightComponent();
    
    /**
     * @brief show the camera component data
     */
    void
    showCameraComponent();
    
    /**
     * @brief show the add component window
     */
    void
    ImGuiAddComponent();

    /**
     * @brief function to show new ligth component data, create and set to object
     */
    void
    addLightComponent();

    /**
     * @brief function to show texture loaded
     */
    void
    showTexturesResources();

    /**
     * @brief function to show models loaded
     */
    void
    showModels();

    /**
     * @brief function to show ambient option window
     */
    void
    showAmbientOption();

    /**
     * @brief function to show ambient oclusion option window
     */
    void
    showAmbientOclusionOption();

    /**
     * @brief function to show bright option window
     */
    void
    showBrightOption();

    /**
     * @brief function to show blur option window
     */
    void
    showBlurOption();

    /**
     * @brief function to show Camera Interpolate Menu window
     */
    void
    showCameraInterpolateMenu();

    /**
     * @brief function to loading resources
     */
    void
    imGuiLoadResourceFile();

//////////////////////////////////////////////////////////////////////////////////////////////
// Passes
//////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief function to draw with forward shader
     */
    void
    renderForward(SPtr<RenderTarget> rt);

    /**
     * @brief function to draw with deferred metod
     */
    void
    renderDeferred();

    /**
     * @brief function to call to render
     */
    void
    shadowPass();
    SPtr<GameObject> m_slight;
    
    /**
     * @brief function to call to render
     */
    void
    geometryPass();
    
    /**
     * @brief function to call to render
     */
    void
    lightsPass();
    
    /**
     * @brief function to call to render
     */
    void
    LuminancePass();
    
    /**
     * @brief function to call to render
     */
    void
    BrightPass();
    
    /**
     * @brief function to call to render
     */
    void
    AmbientOculsionPass();
    
    /**
     * @brief function to call to render
     */
    void
    ColorWithAmbientOculsionPass();
    
    /**
     * @brief function to call to render
     */
    void
    BlurPass(SPtr<RenderTarget> rtToBlur);
    
    /**
     * @brief function to call to add two renderTargets
     */
    void
    AddPass(SPtr<RenderTarget> target, 
             SPtr<RenderTarget> renderTarget1, 
             SPtr<RenderTarget> renderTarget2, 
             uint32 slotOfTarget1 = 0, 
             uint32 slotOfTarget2 = 0);

    /**
     * @brief function to call to render
     */
    void
    AddBirghtPass();

    /**
     * @brief function to call to render
     */
    void
    toneMapPass();

    /**
     * @brief function to call when resize the scen window
     * @param width is the new width of scene window
     * @param height is the new height of scene window
     */
    void
    onResizeSceneWindow(uint32 width, uint32 height);
    
    void
    EditTransform();

    void
    showGizmoSelectedObject();
//////////////////////////////////////////////////////////////////////////////////////////////
// Render Targets
//////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief shared pointer to render target view
     */
    SPtr<RenderTargetView> m_rtv = nullptr;

    /**
     * @brief shared pointer forward render target
     */
    SPtr<RenderTarget> m_RTForward = nullptr;

    /**
     * @brief shared pointer camera render target
     * @note use the data of camera selected
     */
    SPtr<RenderTarget> m_RTcamera = nullptr;

    /**
     * @brief shared pointer to camera interpolate render target
     */
    SPtr<RenderTarget> m_RTcameraInterpolate = nullptr;

    /**
     * @brief shared pointer to geometry render target to geometry pass
     */
    SPtr<RenderTarget> m_RTGeometry = nullptr;

    /**
     * @brief shared pointer to lights render target to lights pass
     */
    SPtr<RenderTarget> m_RTLights = nullptr;

    /**
     * @brief shared pointer to Ambient Oclussion render target to AO pass
     */
    SPtr<RenderTarget> m_RTAmbientOclusion = nullptr;

    /**
     * @brief shared pointer to Ambient Oclussion Blurec (addition pass) render target
     */
    SPtr<RenderTarget> m_RTAOclusionBlured = nullptr;

    /**
     * @brief shared pointer to Luminance render target to luminance pass
     */
    SPtr<RenderTarget> m_RTLuminance = nullptr;

    /**
     * @brief shared pointer to Bright render target to Bright pass
     */
    SPtr<RenderTarget> m_RTBright = nullptr;

    /**
     * @brief shared pointer to Colored AO render target to Colored AO pass
     */
    SPtr<RenderTarget> m_RTColorAO = nullptr;

    /**
     * @brief shared pointer to BlurH render target to Horizontal blur
     */
    SPtr<RenderTarget> m_RTBlurH = nullptr;

    /**
     * @brief shared pointer to BlurV render target to Vertical blur
     */
    SPtr<RenderTarget> m_RTBlurV = nullptr;

    /**
     * @brief shared pointer to Blur added render target to the two blur added blur
     */
    SPtr<RenderTarget> m_RTBlurAdded = nullptr;

    /**
     * @brief shared pointer to AddBright render target to Add birght
     */
    SPtr<RenderTarget> m_RTAddBright = nullptr;

    /**
     * @brief shared pointer to ToneMap render target to tone map
     */
    SPtr<RenderTarget> m_RTToneMap = nullptr;

    /**
     * @brief shared pointer to ShadoMap render target to shado map
     */
    SPtr<RenderTarget> m_RTShadowMap = nullptr;

    /**
     * @brief shared pointer to Final render target
     */
    SPtr<RenderTarget> m_RTFinal = nullptr;

//////////////////////////////////////////////////////////////////////////////////////////////
// Shaders
//////////////////////////////////////////////////////////////////////////////////////////////
    /**
       * @brief shared pointer to a program shader for Forward render
       */
    SPtr<ProgramShader> m_PSForward = nullptr;

    /**
     * @brief shared pointer to a program shader for show the wireframe Forward render
     */
    SPtr<ProgramShader> m_PSWireFrame = nullptr;

    /**
     * @brief shared pointer to a program shader to Geometry pass
     */
    SPtr<ProgramShader> m_PSGeometry = nullptr;

    /**
     * @brief shared pointer to a program shader to Lights pass
     */
    SPtr<ProgramShader> m_PSLights = nullptr;

    /**
     * @brief shared pointer to a program shader to Ambient Oclusion pass
     */
    SPtr<ProgramShader> m_PSAOclusion = nullptr;

    /**
     * @brief shared pointer to a program shader to Luminance pass
     */
    SPtr<ProgramShader> m_PSLuminance = nullptr;

    /**
     * @brief shared pointer to a program shader to Bright pass
     */
    SPtr<ProgramShader> m_PSBright = nullptr;

    /**
     * @brief shared pointer to a program shader to Color Ambient Oclusion pass
     */
    SPtr<ProgramShader> m_PSBColorAO = nullptr;

    /**
     * @brief shared pointer to a program shader to Horizontal Blur pass
     */
    SPtr<ProgramShader> m_PSBBlurH = nullptr;

    /**
     * @brief shared pointer to a program shader to Vertical Blur pass
     */
    SPtr<ProgramShader> m_PSBBlurV = nullptr;

    /**
     * @brief shared pointer to a program shader to Addition pass
     */
    SPtr<ProgramShader> m_PSAdd = nullptr;

    /**
     * @brief shared pointer to a program shader to ToneMap
     */
    SPtr<ProgramShader> m_PSToneMap = nullptr;

    /**
     * @brief shared pointer to a program shader to shadowMap
     */
    SPtr<ProgramShader> m_PSShadowMap = nullptr;

    /**
     * @brief shared pointer to a program shader to shadowMap
     */
    SPtr<ProgramShader> m_PSBillboard = nullptr;
    SPtr<GeometryShader> m_GSBillboard = nullptr;

//////////////////////////////////////////////////////////////////////////////////////////////
// Constant Buffers
//////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief shared ponter to a constant buffer never change
     */
    SPtr<ConstantBuffer> m_CBneverChange = nullptr;

    /**
     * @brief shared ponter to a constant buffer cahnge on resize
     */
    SPtr<ConstantBuffer> m_CBchangeOnResize = nullptr;

    /**
     * @brief shared ponter to a constant buffer cahnge every fram
     */
    SPtr<ConstantBuffer> m_CBchangeEveryFrame = nullptr;

    /**
     * @brief shared ponter to a constant buffer with light data
     */
    SPtr<ConstantBuffer> m_CBlights = nullptr;

    /**
     * @brief shared ponter to a constant buffer with Ambient Oclusion Data
     */
    SPtr<ConstantBuffer> m_CBAO = nullptr;

    /**
     * @brief shared ponter to a constant buffer with bright data
     */
    SPtr<ConstantBuffer> m_CBBright = nullptr;

    /**
     * @brief shared ponter to a constant buffer with Blur data
     */
    SPtr<ConstantBuffer> m_CBBlur = nullptr;

//////////////////////////////////////////////////////////////////////////////////////////////
// Data of Constant Buffers
//////////////////////////////////////////////////////////////////////////////////////////////
    /**
      * @brief the data to never change buffer
      */
    CBNeverChanges m_DNeverChanges;

    /**
     * @brief the data to never change on resize
     */
    CBChangeOnResize m_DChangeOnResize;

    /**
     * @brief the data to never change every frame
     */
    CBChangesEveryFrame m_DChangeEveryFrame;

    /**
     * @brief the data to never change every frame
     */
    CBLights m_DLights;

    /**
     * @brief the data of Ambient Occlusion
     */
    CBAO m_DAOclusion;

    /**
     * @brief the data of Ambient Occlusion
     */
    int32 m_bluredAOPassesNum = 4;


    /**
     * @brief the data of bright
     */
    CBBright m_DBright;

    /**
     * @brief the data of Ambient Occlusion
     */
    int32 m_bluredBirghtPassesNum = 4;

    /**
     * @brief the data to mips for Blur
     */
    CBMipLevels m_DmipDataBlur;

    /**
     * @brief number of light created
     */
    uint32 m_lightCreated = 0;


//////////////////////////////////////////////////////////////////////////////////////////////
// View Ports
//////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief the viewPort of the window
     */
    ViewPort m_viewPort;

    /**
     * @brief the viewPort of the scene window
     */
    ViewPort m_viewPortScene;

//////////////////////////////////////////////////////////////////////////////////////////////
// Input Layout
//////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief shared ponter to a input layout
     */
    SPtr<InputLayout> m_inLayOut = nullptr;

    /**
     * @brief shared ponter to a input layout
     */
    SPtr<InputLayout> m_inLayOutBillBoard = nullptr;


//////////////////////////////////////////////////////////////////////////////////////////////
// Cameras
//////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief share pointer to debug camera
     * @note the debug camera is the camera how you can see the scene all time
     */
    SPtr<Camera> m_debugCam = nullptr;

    /**
     * @brief referene of the start cmaera to interpolate
     */
    SPtr<Camera> m_interpolateCam1 = nullptr;

    /**
     * @brief referene of the end cmaera to interpolate
     */
    SPtr<Camera> m_interpolateCam2 = nullptr;

    /**
     * @brief a temp camera with the interpolate of the two before
     */
    SPtr<Camera> m_tempCam = nullptr;

    /**
     * @brief the time to realize the interpolation
     */
    float m_interpolationCamerasTime = 1.0f;

    /**
     * @brief to create a camera component, its name
     */
    String m_newCameraName;

    /**
     * @brief to create a camera component, its eye
     */
    JDVector3 m_newCamEye = { 0,0,0 };

    /**
     * @brief to create a camera component, its up
     */
    JDVector3 m_newCamUp = { 0,1,0 };

    /**
     * @brief to create a camera component, its at
     */
    JDVector3 m_newCamAt = { 0,0,-1 };

    /**
     * @brief to create a camera component, its near
     */
    float m_newCamNear = 0.01f;

    /**
     * @brief to create a camera component, its far
     */
    float m_newCamFar = 10000.0f;

    /**
     * @brief to create a camera component, its projection type
     */
    CAMERA_PROJECTION_TYPE::E projType = CAMERA_PROJECTION_TYPE::PERSPECTIVE;

    /**
     * @brief index of camera interpolate start
     */
    uint32 m_camInterId1 = 0;

    /**
     * @brief index of camera interpolate end
     */
    uint32 m_camInterId2 = 0;

//////////////////////////////////////////////////////////////////////////////////////////////
// Ambient Options
//////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief texture with the current cube map for the ambient
     */
    SPtr<Texture2D> m_ambientCubeMap = nullptr;

    /**
     * @brief index of the current cube map for the ambient
     * @note this is only to identifique on the resource manager
     */
    uint32 m_ambientCubeMapOption = 0;

//////////////////////////////////////////////////////////////////////////////////////////////
// Resources Options
//////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief the resourece's type to load
     */
    RESOURCE_TYPE::E m_typeResourceToLoad = RESOURCE_TYPE::UNKNOWTYPE;

    /**
     * @brief To open the load file imgui window
     */
    bool m_loadingFile = false;

    /**
     * @brief To add camera
     */
    bool m_addigCamera = false;

//////////////////////////////////////////////////////////////////////////////////////////////
// To show Windows
//////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief bool to see ambient options
     */
    bool m_AmbientOptionsMenu = false;

    /**
     * @brief bool to see ambient options
     */
    bool m_cameraInterpolateMenu = false;

    /**
     * @brief To show the deferredTextures
     */
    bool m_showDeferred = false;

    /**
     * @brief To show the Ambient Oclusion Opctions
     */
    bool m_showAOOptions = false;

    /**
     * @brief To show the Bright Opctions
     */
    bool m_showBrightOptions = false;

    /**
     * @brief To show the Bright Options
     */
    bool m_showBlurOptions = false;

    /**
     * @brief To show the Bright Options
     */
    bool m_showHistogram = false;

//////////////////////////////////////////////////////////////////////////////////////////////
// Rasterize states
//////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Default Rasterize state
     */
    SPtr<RasterizeState> m_defaultRasState = nullptr;

    /**
     * @brief Rasterize state for wire frame
     */
    SPtr<RasterizeState> m_wireframeRasState = nullptr;

    /**
     * @brief bool to see wire frame
     */
    bool m_bWireframe = false;

    /**
     * @brief bool to see wire frame
     */
    bool m_bWireframeAlreadyActive = false;

//////////////////////////////////////////////////////////////////////////////////////////////
// ImGui
//////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief the window data to load files
     */
    SPtr<ImGui::FileBrowser> m_fileDialog;

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
     * @brief size of the scene window
     */
    JDPoint m_sceneSize = {800, 800};

    /**
     * @brief to know is the first frae
     */
    bool m_firstFrame = true;

    ParticleData m_tempParticle;
    SPtr<VertexBuffer> m_particleVB;


    int32 m_lastIdGizmo = 0;
    ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;

  };
  
  extern "C" JD_PLUGIN_EXPORT RenderApi *
  createRenderAPI() {
    RenderFNDApi* pDX = new RenderFNDApi();
    return pDX;
  }
}