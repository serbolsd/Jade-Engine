#include "jdTestApp.h"

testApp::testApp() {
  m_wndPosition = { -1, -1 };
  m_clientSize= {640, 480};
  m_wndTitle = "jdEngine App";
  m_fullScreen = false;
}

testApp::testApp(const JDPoint& winPos, 
                 const JDPoint& winSize, 
                 const String& winTitle, 
                 bool fullScreen) {
  m_wndPosition = winPos;
  m_clientSize = winSize;
  m_wndTitle = winTitle;
  m_fullScreen = fullScreen;
}

void 
testApp::onCreate() {
  m_rtv = g_graphicsApi().getRenderTargetView();

  ViewPort vp;
  vp.Width = (float)m_clientSize.x;
  vp.Height = (float)m_clientSize.y;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.m_topLeftX = 0;
  vp.m_topLeftY = 0;
  g_graphicsApi().setViewPort(vp);

  g_graphicsApi().setRenderTarget(m_rtv);

  m_progShader = g_graphicsApi().loadShaderFromFile("data/shader/Tutorial072.fx",
                                                    "VS",
                                                    "vs_5_0",
                                                    "data/shader/Tutorial072.fx",
                                                    "PS",
                                                    "ps_5_0");
  g_graphicsApi().setProgramShader(m_progShader);

  m_inLayoutElements.resize(2);

  m_inLayoutElements[0] = {"POSITION", 
                            0, 
                            FORMAT::E::FORMAT_R32G32B32A32_FLOAT, 
                            0, 
                            0, 
                            INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 
                            0 };
  m_inLayoutElements[1] = {"TEXCOORD", 
                           0, 
                           FORMAT::E::FORMAT_R32G32_FLOAT, 
                           0, 
                           16,
                           INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 
                           0 };

  m_inLayOut = g_graphicsApi().CreateInputLayout(m_progShader.get()->getVertexShader(), 
                                                 &m_inLayoutElements[0], 
                                                 (uint32)m_inLayoutElements.size());
  //g_graphicsApi().reflectInputLayout(progShader.get()->getVertexShader());

  g_graphicsApi().setInputLayout(m_inLayOut);

  SimpleVertex vertices[24] =
  {
    { JDVector4(-1.0f, 1.0f, -1.0f, 1.0f), JDVector2(0.0f, 0.0f) },
    { JDVector4(1.0f, 1.0f, -1.0f, 1.0f), JDVector2(1.0f, 0.0f) },
    { JDVector4(1.0f, 1.0f, 1.0f, 1.0f), JDVector2(1.0f, 1.0f) },
    { JDVector4(-1.0f, 1.0f, 1.0f, 1.0f), JDVector2(0.0f, 1.0f) },
    
    { JDVector4(-1.0f, -1.0f, -1.0f, 1.0f), JDVector2(0.0f, 0.0f) },
    { JDVector4(1.0f, -1.0f, -1.0f, 1.0f), JDVector2(1.0f, 0.0f) },
    { JDVector4(1.0f, -1.0f, 1.0f, 1.0f), JDVector2(1.0f, 1.0f) },
    { JDVector4(-1.0f, -1.0f, 1.0f, 1.0f), JDVector2(0.0f, 1.0f) },
    
    { JDVector4(-1.0f, -1.0f, 1.0f, 1.0f), JDVector2(0.0f, 0.0f) },
    { JDVector4(-1.0f, -1.0f, -1.0f, 1.0f), JDVector2(1.0f, 0.0f) },
    { JDVector4(-1.0f, 1.0f, -1.0f, 1.0f), JDVector2(1.0f, 1.0f) },
    { JDVector4(-1.0f, 1.0f, 1.0f, 1.0f), JDVector2(0.0f, 1.0f) },
    
    { JDVector4(1.0f, -1.0f, 1.0f, 1.0f), JDVector2(0.0f, 0.0f) },
    { JDVector4(1.0f, -1.0f, -1.0f, 1.0f), JDVector2(1.0f, 0.0f) },
    { JDVector4(1.0f, 1.0f, -1.0f, 1.0f), JDVector2(1.0f, 1.0f) },
    { JDVector4(1.0f, 1.0f, 1.0f, 1.0f), JDVector2(0.0f, 1.0f) },
    
    { JDVector4(-1.0f, -1.0f, -1.0f, 1.0f), JDVector2(0.0f, 0.0f) },
    { JDVector4(1.0f, -1.0f, -1.0f, 1.0f), JDVector2(1.0f, 0.0f) },
    { JDVector4(1.0f, 1.0f, -1.0f, 1.0f), JDVector2(1.0f, 1.0f) },
    { JDVector4(-1.0f, 1.0f, -1.0f, 1.0f), JDVector2(0.0f, 1.0f) },
    
    { JDVector4(-1.0f, -1.0f, 1.0f, 1.0f), JDVector2(0.0f, 0.0f) },
    { JDVector4(1.0f, -1.0f, 1.0f, 1.0f), JDVector2(1.0f, 0.0f) },
    { JDVector4(1.0f, 1.0f, 1.0f, 1.0f), JDVector2(1.0f, 1.0f) },
    { JDVector4(-1.0f, 1.0f, 1.0f, 1.0f), JDVector2(0.0f, 1.0f) },
  };

  m_vertexB = g_graphicsApi().createVertexBuffer(24, sizeof(SimpleVertex), &vertices);
  g_graphicsApi().setVertexBuffer(m_vertexB);

  uint32 indices[] =
  {
    3,1,0,
    2,1,3,
    
    6,4,5,
    7,4,6,
    
    11,9,8,
    10,9,11,
    
    14,12,13,
    15,12,14,
    
    19,17,16,
    18,17,19,
    
    22,20,21,
    23,20,22
  };

  m_indexB = g_graphicsApi().createIndexBuffer(36, indices);
  g_graphicsApi().setIndexBuffer(m_indexB);

  m_neverChangeB = g_graphicsApi().CreateConstantBuffer(sizeof(CBNeverChanges));

  m_changeOnResizeB = g_graphicsApi().CreateConstantBuffer(sizeof(CBChangeOnResize));

  m_changeEveryFrameB = g_graphicsApi().CreateConstantBuffer(sizeof(CBChangesEveryFrame));

  JDVector4 Eye(0.0f, 3.0f, -6.0f, 0.0f);
  JDVector4 At(0.0f, 1.0f, 0.0f, 0.0f);
  JDVector4 Up(0.0f, 1.0f, 0.0f, 0.0f);
  m_neverChanges.mView = createViewMatrix(Eye, At, Up);
  m_neverChanges.mView.transpose();

  m_changeOnResize.mProjection = createProjectionPerspectiveMatrix(Math::HALF_PI / 2, 
                                                                   (float)m_clientSize.x, 
                                                                   (float)m_clientSize.y, 
                                                                   0.01f, 
                                                                   100.0f);
  m_changeOnResize.mProjection.transpose();
  JDMatrix4 world;
  world.identity();
  JDVector4 color(0.5f, 0, 0.5f, 1);
  m_changeEveryFrame.mWorld = world;
  m_changeEveryFrame.vMeshColor = color;

  g_graphicsApi().updateSubresource(m_neverChangeB, &m_neverChanges);
  g_graphicsApi().updateSubresource(m_changeOnResizeB, &m_changeOnResize);
  g_graphicsApi().updateSubresource(m_changeEveryFrameB, &m_changeEveryFrame);

  //g_graphicsApi().setConstanBuffer(neverChangeB,0);
  g_graphicsApi().VertexShaderSetConstanBuffer(m_neverChangeB, 0);
  g_graphicsApi().PixelShaderSetConstanBuffer(m_neverChangeB, 0);
  //g_graphicsApi().setConstanBuffer(changeOnResizeB,1);
  g_graphicsApi().VertexShaderSetConstanBuffer(m_changeOnResizeB, 1);
  g_graphicsApi().PixelShaderSetConstanBuffer(m_changeOnResizeB, 1);
  //g_graphicsApi().setConstanBuffer(changeEveryFrameB,2);
  g_graphicsApi().VertexShaderSetConstanBuffer(m_changeEveryFrameB, 2);
  g_graphicsApi().PixelShaderSetConstanBuffer(m_changeEveryFrameB, 2);

  SPtr<Sampler> samplerLineal = g_graphicsApi().CreateSampleLinearState();
  g_graphicsApi().setSampler(samplerLineal, 0);

  g_graphicsApi().setPrimitiveTopology(PRIMITIVE_TOPOLOGY_FORMAT::TRIANGLELIST);

  initImGui();
}

void 
testApp::onRender() {
  g_graphicsApi().Clear(m_rtv, 0, 0, 0, 1);
  g_graphicsApi().ClearDepthStencil(m_rtv);
  //rot += 0.0001;
  ////g_changeEveryFrame.mWorld.rotateY(Radian(rot));
  //
  g_graphicsApi().updateSubresource(m_changeEveryFrameB, &m_changeEveryFrame);
  
  g_graphicsApi().setVertexBuffer(m_vertexB);
  g_graphicsApi().setIndexBuffer(m_indexB);

  g_graphicsApi().DrawIndex(36);
  
  //cam.ImGui();
  ImGui::Begin("Escala", 0);
  ImGui::SliderFloat("scale", &m_scale, 0.001f, 1);
  ImGui::End();

  ImGui::Render();
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void 
testApp::onResize(int32 width, int32 height) {
  auto clientSize = m_window.getSize();
  m_clientSize.x = clientSize.x;
  m_clientSize.y = clientSize.y;

  g_graphicsApi().resizeSwapChain(width, height);

  m_rtv = g_graphicsApi().getRenderTargetView();
  g_graphicsApi().setRenderTarget(m_rtv);

  m_changeOnResize.mProjection = createProjectionPerspectiveMatrix(Math::HALF_PI / 2, 
                                                                   (float)m_clientSize.x, 
                                                                   (float)m_clientSize.y, 
                                                                   0.01f, 
                                                                   100.0f);
  m_changeOnResize.mProjection.transpose();
  g_graphicsApi().updateSubresource(m_changeOnResizeB, &m_changeOnResize);
}

void 
testApp::onMouseButtonPressed(int32 button, int32 /*x*/, int32 /*y*/) {
  ImGuiIO& io = ImGui::GetIO();
  if (!ImGui::IsAnyMouseDown() && nullptr == ::GetCapture())
  {
    ::SetCapture(m_window.getSystemHandle());
  }
  io.MouseDown[static_cast<int32>(button)] = true;
}

void 
testApp::onMouseButtonReleased(int32 button, int32 /*x*/, int32 /*y*/) {
  ImGuiIO& io = ImGui::GetIO();
  if (!ImGui::IsAnyMouseDown() && ::GetCapture() == m_window.getSystemHandle())
  {
    ::ReleaseCapture();
  }
  io.MouseDown[static_cast<int32>(button)] = false;
}

void
testApp::onUpdate(const float& /*deltaTime*/) {
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();
}

void 
testApp::initImGui() {

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGui_ImplWin32_Init(m_window.getSystemHandle());
  ImGui_ImplDX11_Init(reinterpret_cast<ID3D11Device*>(g_graphicsApi().getDevice()),
                      reinterpret_cast<ID3D11DeviceContext*>(g_graphicsApi().getDeviceContex()));
  ImGui::StyleColorsDark();
}

void 
testApp::onDestroy() {
  ImGui_ImplDX11_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();
}

