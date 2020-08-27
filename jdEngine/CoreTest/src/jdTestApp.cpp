#include "jdTestApp.h"
#include <jdComponentTransform.h>
#include <jdComponentRenderModel.h>
#include <imfilebrowser.h>

testApp::testApp() {
  m_wndPosition = { -1, -1 };
  m_clientSize= {1000, 800};
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

  m_first = g_graphicsApi().createRenderTarget(m_clientSize.x, m_clientSize.y, 2, true);

  ViewPort vp;
  vp.Width = (float)m_clientSize.x;
  vp.Height = (float)m_clientSize.y;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.m_topLeftX = 0;
  vp.m_topLeftY = 0;
  g_graphicsApi().setViewPort(vp);

  g_graphicsApi().setRenderTarget(m_rtv);

  m_progShader = g_graphicsApi().loadShaderFromFile("data/shader/TestDifiuse.fx",
                                                    "VS",
                                                    "vs_5_0",
                                                    //"data/shader/Tutorial072.fx",
                                                    "data/shader/TestDifiuse.fx",
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

  //m_inLayOut = g_graphicsApi().CreateInputLayout(m_progShader.get()->getVertexShader(), 
  //                                               &m_inLayoutElements[0], 
  //                                               (uint32)m_inLayoutElements.size());
  m_inLayOut = g_graphicsApi().reflectInputLayout(m_progShader.get()->getVertexShader());

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

  JDVector4 Eye(0.0f, 20.0f, -150.0f, 0.0f);
  JDVector4 At(0.0f, 0.0f, 0.0f, 0.0f);
  JDVector4 Up(0.0f, 1.0f, 0.0f, 0.0f);
  m_neverChanges.mView = createViewMatrix(Eye, At, Up);
  m_neverChanges.mView.transpose();

  m_changeOnResize.mProjection = createProjectionPerspectiveMatrix(Math::HALF_PI / 2, 
                                                                   (float)m_clientSize.x, 
                                                                   (float)m_clientSize.y, 
                                                                   0.01f, 
                                                                   10000.0f);
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
  g_graphicsApi().SetBonesConstanBuffer();
  SPtr<Sampler> samplerLineal = g_graphicsApi().CreateSampleLinearState();
  g_graphicsApi().setSampler(samplerLineal, 0);

  g_graphicsApi().setPrimitiveTopology(PRIMITIVE_TOPOLOGY_FORMAT::TRIANGLELIST);

  initImGui();
}

void 
testApp::onRender() {

  {
    g_graphicsApi().setRenderTarget(m_first);
    g_graphicsApi().Clear(m_first, 0.2f, 0.2f, 0.2f, 1);
    g_graphicsApi().ClearDepthStencil(m_first);
    //SceneGraph::instance().m_root->draw();
    for (auto object : SceneGraph::instance().m_GObjects)
    {
      auto component =
        object->getComponent(COMPONENT_TYPE::TRANSFORM);
      CTransform* trans = reinterpret_cast<CTransform*>(component.get());
      m_changeEveryFrame.mWorld = trans->getMatrixTransform();

      g_graphicsApi().updateSubresource(m_changeEveryFrameB, &m_changeEveryFrame);

      object->draw();
    }
    //g_graphicsApi().setVertexBuffer(m_vertexB);
    //g_graphicsApi().setIndexBuffer(m_indexB);

    //g_graphicsApi().DrawIndex(36);
  }

  g_graphicsApi().setRenderTarget(m_rtv);
  g_graphicsApi().Clear(m_rtv, 0, 0, 0, 1);
  g_graphicsApi().ClearDepthStencil(m_rtv);
  //rot += 0.0001;
  ////g_changeEveryFrame.mWorld.rotateY(Radian(rot));
  //
  g_graphicsApi().updateSubresource(m_changeEveryFrameB, &m_changeEveryFrame);
  
  imguiDockScreen();
  ImGui::SetNextWindowSize(ImVec2(m_clientSize.x * 0.5f, m_clientSize.y * 0.5f));
  ImGui::Begin("Scene Window");
  ImGui::SetWindowSize(ImVec2(m_clientSize.x * 0.5f, m_clientSize.y * 0.5f));
  //get the mouse position
  ImVec2 pos = ImGui::GetCursorScreenPos();

  ImGui::GetWindowDrawList()->AddImage(
    m_first.get()->getRenderTexture(),
    ImVec2(ImGui::GetCursorScreenPos()),
    ImVec2(ImGui::GetCursorScreenPos().x + m_clientSize.x / 2,
      ImGui::GetCursorScreenPos().y + m_clientSize.y / 2));
  ImGui::End();
  bool yes = true;
  if (!ImGui::Begin("SceneGraph", &yes))
  {
    ImGui::End();
    return;
  }
  imGuiShowObject("child", SceneGraph::instance().m_root);
  ImGui::End();

  //ImGui::ShowDemoWindow();


  imGuiInspectorObject();

  if (m_loadingFile)
  {
    imGuiLoadResourceFile();
  }

  ImGui::Render();
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

}

void 
testApp::onResize(int32 width, int32 height) {
  ImGui::GetIO().DisplaySize = ImVec2((float)width, (float)height);
  ImGui::GetIO().DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
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
testApp::onTextEnterd(UNICHAR unicode) {
  ImGuiIO& io = ImGui::GetIO();
  io.AddInputCharacterUTF16((unsigned short) unicode);
}

void
testApp::onKeyPressed(int32 code, bool /*alt*/, bool /*control*/, bool /*shift*/, bool /*system*/) {
  ImGuiIO& io = ImGui::GetIO();
  if (code >= 0)
  {
    io.KeysDown[static_cast<int32>(code)] = true;
  }

}

void
testApp::onKeyReleased(int32 code, bool /*alt*/, bool /*control*/, bool /*shift*/, bool /*system*/) {
  ImGuiIO& io = ImGui::GetIO();
  if (code >= 0)
  {
    io.KeysDown[static_cast<int32>(code)] = false;
  }
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
testApp::onUpdate(const float& deltaTime) {
  SceneGraph::instance().onUpdate(deltaTime);
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
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  

  // (optional) set browser properties
  m_fileDialog.SetTitle("LoadResource");
  m_fileDialog.SetTypeFilters({ ".h", ".cpp" });
}

void 
testApp::imguiDockScreen(){
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
          m_fileDialog.SetTypeFilters({ ".fbx",".FBX", ".obj" });
          m_fileDialog.Open();
          m_loadingFile = true;
        }
        if (ImGui::MenuItem("Image")) {
          m_typeResourceToLoad = RESOURCE_TYPE::TEXTURE;
          m_fileDialog.SetTypeFilters({".jpg", ".png", ".dds" });
          m_fileDialog.Open();
          m_loadingFile = true;
        }
        ImGui::EndMenu();
      }
      
      ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
  }

  ImGui::End();
}

void 
testApp::imGuiShowObject(const char* /*nameObject*/, WeakSptr<GameObject> child) {
  auto object = child.lock();
  if (ImGui::Checkbox(object->getName().c_str(), &object->selected))
  {
    if (SceneGraph::instance().selectedObjet != object)
    {
      if (nullptr != SceneGraph::instance().selectedObjet)
      {
        SceneGraph::instance().selectedObjet->selected = false;
      }
      SceneGraph::instance().selectedObjet = object; 
    }
  }
  // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
  ImGui::AlignTextToFramePadding();   // Text and Tree nodes are less high than framed widgets, here we add vertical spacing to make the tree lines equal high.
  if (0 < object->m_children.size())
  {
    if (ImGui::TreeNode("children: "))
    {
      for (auto son : object->m_children)
      {
        imGuiShowObject(son->getName().c_str(),son);
      }
      ImGui::TreePop();
    }
  }
}

void 
testApp::imGuiInspectorObject() {
  ImGui::Begin("Inspector");
  if (nullptr == SceneGraph::instance().selectedObjet)
  {
    ImGui::End();
    return;
  }
  if (ImGui::Button("Add child Object"))
  {
    SceneGraph::instance().createGameObject();
  }
  char* name = (char*)SceneGraph::instance().selectedObjet->getName().c_str();
  ImGui::InputText("Name", name, 255);
  SceneGraph::instance().selectedObjet->setName(String(name));
  auto component =
    SceneGraph::instance().selectedObjet->getComponent(COMPONENT_TYPE::TRANSFORM);
  if (nullptr != component)
  {
    ImGui::Separator();
    ImGui::Text("Transform");
    CTransform* trans = reinterpret_cast<CTransform*>(component.get());
    ImGui::DragFloat3("Rotacion", &trans->rotation.x, 0.1f);
    ImGui::DragFloat3("Position", &trans->position.x, 0.1f);
    ImGui::DragFloat3("Scale", &trans->scale.x, 0.1f);
  }
  component =
    SceneGraph::instance().selectedObjet->getComponent(COMPONENT_TYPE::RENDERMODEL);
  if (nullptr != component)
  {
    showRenderModelComponent();
  }

  ImGui::Separator();
  if (ImGui::Button("AddComponent") && !m_showComponentImGui)
  {
    m_showComponentImGui = true;
  }
  ImGui::End();
  if (m_showComponentImGui)
  {
    ImGuiAddComponent();
  }
}

void 
testApp::showRenderModelComponent() {
  ImGui::Separator();
  ImGui::Text("Render Model");
  auto Model = SceneGraph::instance().selectedObjet.get();
  auto rModel = reinterpret_cast<CRenderModel*>(Model->getComponent(COMPONENT_TYPE::RENDERMODEL).get());
  String g_UnitOptionPreviw = ResourceManager::instance().m_modelsNames[Model->m_modelOption];
  if (ImGui::BeginCombo("Models", g_UnitOptionPreviw.c_str()))
  {
    int
    modelOption = Model->m_modelOption;
    g_UnitOptionPreviw = "";
    ImGui::ListBox("Model", &modelOption,
                   &ResourceManager::instance().m_modelsNames[0], 
                   (int)ResourceManager::instance().m_modelsNames.size());
    //static String g_UnitOptionPreviw = ResourceManager::instance().m_modelsNames[0];
    g_UnitOptionPreviw = ResourceManager::instance().m_modelsNames[modelOption];
    if (modelOption>0)
    {
      rModel->setModel(ResourceManager::instance().m_models[modelOption-1]);
    }
    else
    {
      rModel->m_model = nullptr;
    }
    Model->m_modelOption = modelOption;
    ImGui::EndCombo();
  }
  if (nullptr != rModel->m_model)
  {
    for (auto m : rModel->m_model->m_meshes)
    {
      g_UnitOptionPreviw = ResourceManager::instance().m_texturesNames[m->m_albedoOption];
      if (ImGui::CollapsingHeader(m->getName().c_str())) {
        ImGui::Text("Albedo");
        ImGui::Image(m->getAlbedoTexture()->getTexture(), ImVec2(40, 40));
        if (ImGui::BeginCombo("Textures##1", g_UnitOptionPreviw.c_str()))
        {
          int albedolOption = m->m_albedoOption;
          g_UnitOptionPreviw = "";
          ImGui::ListBox("Texture##1", &albedolOption,
            &ResourceManager::instance().m_texturesNames[0],
            (int)ResourceManager::instance().m_texturesNames.size());
          //static String g_UnitOptionPreviw = ResourceManager::instance().m_texturesNames[0];
          g_UnitOptionPreviw = ResourceManager::instance().m_texturesNames[albedolOption];
          if (albedolOption > 0)
          {
            m->setAlbedoTexture(ResourceManager::instance().m_textures[albedolOption - 1]);
          }
          else
          {
            m->setAlbedoTexture(ResourceManager::instance().DEFAULT_TEXTURE_ERROR);
          }
          m->m_albedoOption = albedolOption;
          ImGui::EndCombo();
        }

      }
    }
  }
}

void 
testApp::ImGuiAddComponent() {
  ImGui::Begin("Components");
  static ImGuiTextFilter filter;
  filter.Draw();
  const char* lines[] = { "Transform", "RenderModel"};
  for (int i = 0; i < IM_ARRAYSIZE(lines); i++)
  {
    if (filter.PassFilter(lines[i]))
    {
      if (ImGui::Button(lines[i]))
      {
        switch (i)
        {
         case 0:
          SceneGraph::instance().selectedObjet->addComponent(COMPONENT_TYPE::TRANSFORM);
          break;
         case 1:
          SceneGraph::instance().selectedObjet->addComponent(COMPONENT_TYPE::RENDERMODEL);
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
testApp::imGuiLoadResourceFile() {
  
  m_fileDialog.Display();
  if (m_fileDialog.HasSelected())
  {
    ResourceManager::instance().loadResourceFromFile(m_fileDialog.GetSelected().string().c_str(), 
                                                     m_typeResourceToLoad);
    std::cout << "Selected filename" << m_fileDialog.GetSelected().string() << std::endl;
    m_fileDialog.ClearSelected();
    m_loadingFile = false;
  }
}

void 
testApp::onDestroy() {
  ImGui_ImplDX11_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();
}

