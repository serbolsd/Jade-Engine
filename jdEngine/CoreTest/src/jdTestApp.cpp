#include "jdTestApp.h"
#include <jdComponentTransform.h>
#include <jdComponentRenderModel.h>
#include <jdComponentLight.h>
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
  m_debugCam = g_CameraMan().getDebugCamera();
  return;
  m_rtv = g_graphicsApi().getRenderTargetView();

  m_first = g_graphicsApi().createRenderTarget(m_clientSize.x, m_clientSize.y, 5, true);
  m_cameraRT = g_graphicsApi().createRenderTarget(m_clientSize.x, m_clientSize.y, 5, true);
  m_cameraInterpolate = g_graphicsApi().createRenderTarget(m_clientSize.x, 
                                                           m_clientSize.y, 
                                                           5, 
                                                           true);
  m_deferred = g_graphicsApi().createRenderTarget(m_clientSize.x, m_clientSize.y, 5, true, 4);
  m_RTlights = g_graphicsApi().createRenderTarget(m_clientSize.x, m_clientSize.y, 5, true, 4);
  m_ambientOclusion = g_graphicsApi().createRenderTarget(m_clientSize.x, 
                                                         m_clientSize.y, 
                                                         5, 
                                                         false, 
                                                         1);
  m_ambientOclusionBlured = g_graphicsApi().createRenderTarget(m_clientSize.x, 
                                                               m_clientSize.y, 
                                                               5, 
                                                               false, 
                                                               1);
  m_RTLuminance = g_graphicsApi().createRenderTarget(m_clientSize.x, 
                                                     m_clientSize.y, 
                                                     5, 
                                                     false, 
                                                     1);
  m_RTBright = g_graphicsApi().createRenderTarget(m_clientSize.x, 
                                                  m_clientSize.y, 
                                                  5, 
                                                  false, 
                                                  1);
  m_RTColorAO = g_graphicsApi().createRenderTarget(m_clientSize.x, 
                                                   m_clientSize.y, 
                                                   5, 
                                                   false, 
                                                   1);
  m_RTBlurH = g_graphicsApi().createRenderTarget((uint32)(m_clientSize.x), 
                                                 (uint32)(m_clientSize.y), 
                                                 5, 
                                                 false, 
                                                 1,
                                                 0.5);
  m_RTBlurV = g_graphicsApi().createRenderTarget((uint32)(m_clientSize.x), 
                                                 (uint32)(m_clientSize.y), 
                                                 5, 
                                                 false, 
                                                 1,
                                                 0.5);

  m_viewPort.Width = (float)m_clientSize.x;
  m_viewPort.Height = (float)m_clientSize.y;
  m_viewPort.MinDepth = 0.0f;
  m_viewPort.MaxDepth = 1.0f;
  m_viewPort.m_topLeftX = 0;
  m_viewPort.m_topLeftY = 0;
  m_viewPortScene = m_viewPort;
  g_graphicsApi().setViewPort(m_viewPort);

  g_graphicsApi().setRenderTarget(m_rtv);
  //TestDeferred
  m_progShader = g_graphicsApi().loadShaderFromFile("data/shader/TestCubeMapMultiLight.fx",
                                                    "VS",
                                                    "vs_5_0",
                                                    "data/shader/TestCubeMapMultiLight.fx",
                                                    "PS",
                                                    "ps_5_0");
  
  m_progShaderWF = g_graphicsApi().loadShaderFromFile("data/shader/wireFrameShader.fx",
                                                      "VS",
                                                      "vs_5_0",
                                                      "data/shader/wireFrameShader.fx",
                                                      "PS",
                                                      "ps_5_0");

  m_progShaderDeferred = g_graphicsApi().loadShaderFromFile("data/shader/TestDeferred.fx",
                                                            "VS",
                                                            "vs_5_0",
                                                            "data/shader/TestDeferred.fx",
                                                            "PS",
                                                            "ps_5_0");

  m_progShaderLights = g_graphicsApi().loadShaderFromFile("data/shader/LightsPass.fx",
                                                          "VS",
                                                          "vs_5_0",
                                                          "data/shader/LightsPass.fx",
                                                          "PS",
                                                          "ps_5_0");
  
  m_progShaderAO = g_graphicsApi().loadShaderFromFile("data/shader/TestAmbientOclusion.fx",
                                                      "VS",
                                                      "vs_5_0",
                                                      "data/shader/TestAmbientOclusion.fx",
                                                      "PS",
                                                      "ps_5_0");

  m_progSLuminance = g_graphicsApi().loadShaderFromFile("data/shader/Luminance.fx",
                                                        "VS",
                                                        "vs_5_0",
                                                        "data/shader/Luminance.fx",
                                                        "PS",
                                                        "ps_5_0");
  
  m_progSBright = g_graphicsApi().loadShaderFromFile("data/shader/Birght.fx",
                                                      "VS",
                                                      "vs_5_0",
                                                      "data/shader/Birght.fx",
                                                      "PS",
                                                      "ps_5_0"); 

  m_progSBColorAO = g_graphicsApi().loadShaderFromFile("data/shader/ColorWithAO.fx",
                                                       "VS",
                                                       "vs_5_0",
                                                       "data/shader/ColorWithAO.fx",
                                                       "PS",
                                                       "ps_5_0");

  SPtr<SHADER_DEFINES> Sdefines(new SHADER_DEFINES);
  Sdefines->addDefine("HORIZONTAL");
  m_progSBBlurH = g_graphicsApi().loadShaderFromFile("data/shader/BlurAndAdd.fx",
                                                     "VS",
                                                     "vs_5_0",
                                                     "data/shader/BlurAndAdd.fx",
                                                     "Blur",
                                                     "ps_5_0", Sdefines);
  m_progSBBlurV = g_graphicsApi().loadShaderFromFile("data/shader/BlurAndAdd.fx",
                                                     "VS",
                                                     "vs_5_0",
                                                     "data/shader/BlurAndAdd.fx",
                                                     "Blur",
                                                     "ps_5_0");
  m_progSBAdd = g_graphicsApi().loadShaderFromFile("data/shader/BlurAndAdd.fx",
                                                   "VS",
                                                   "vs_5_0",
                                                   "data/shader/BlurAndAdd.fx",
                                                   "Add",
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

  m_lightsB = g_graphicsApi().CreateConstantBuffer(sizeof(CBLights));
  m_AOB = g_graphicsApi().CreateConstantBuffer(sizeof(CBAO));
  m_BrightB = g_graphicsApi().CreateConstantBuffer(sizeof(CBBright));
  m_BlurB = g_graphicsApi().CreateConstantBuffer(sizeof(CBMipLevels));

  m_lights.light[0].m_lightColor = { 1,1,1,1 };
  m_lights.light[0].m_lightDirection = { -1,0,1 };
  m_lights.light[0].m_lightPosition = { 0,0,0 };
  m_lights.light[0].m_type = 0;
  //m_lights.light[1].m_lightColor = { 1,1,1,0 };
  //m_lights.light[1].m_lightDirection = { 1,0,1,0 };
  //m_lights.light[1].m_lightPosition = { 0,0,0, };
  //m_lights.light[1].m_type = 0;
  m_lights.light[0].m_numberOfLights = 1;

  JDVector3 Eye(0.0f, 130, -195);
  JDVector3 At(0.0f, 130.0f, 0.0f);
  JDVector3 Up(0.0f, 1.0f, 0.0f);
  //m_neverChanges.mView = createViewMatrix(Eye, At, Up);
  //m_neverChanges.mView.transpose();
  //
  //m_changeOnResize.mProjection = createProjectionPerspectiveMatrix(Math::HALF_PI / 2, 
  //                                                                 (float)m_clientSize.x, 
  //                                                                 (float)m_clientSize.y, 
  //                                                                 0.01f, 
  //                                                                 10000.0f);
  m_debugCam = g_CameraMan().getDebugCamera();

  //m_changeOnResize.mProjection.transpose();
  m_neverChanges.m_view = m_debugCam->getMatrixView();
  m_neverChanges.m_viewInv = m_neverChanges.m_view;
  m_neverChanges.m_viewInv.invert();
  
  m_changeOnResize.m_projection = m_debugCam->getMatrixProjection();
  m_changeOnResize.m_projectionInv = m_changeOnResize.m_projection;
  m_changeOnResize.m_projectionInv.invert();

  m_changeOnResize.m_viewProjection = m_neverChanges.m_view * m_changeOnResize.m_projection;
  m_changeOnResize.m_viewProjectionInv = m_changeOnResize.m_viewProjection;
  m_changeOnResize.m_viewProjectionInv.invert();
  JDMatrix4 world;
  world.identity();
  JDVector4 color(0.5f, 0, 0.5f, 1);

  m_changeEveryFrame.m_world = world;
  m_changeEveryFrame.m_worldInv = world;
  m_changeEveryFrame.m_worldInv.invert();

  m_changeEveryFrame.m_worldProj = world * m_changeOnResize.m_projection;
  m_changeEveryFrame.m_worldProjInv = m_changeEveryFrame.m_worldProj;
  m_changeEveryFrame.m_worldProjInv.invert();

  m_changeEveryFrame.m_worldView = world * m_neverChanges.m_view;
  m_changeEveryFrame.m_worldViewInv = m_changeEveryFrame.m_worldView;
  m_changeEveryFrame.m_worldViewInv.invert();

  m_changeEveryFrame.m_worldViewProj = m_changeEveryFrame.m_worldView * 
                                       m_changeOnResize.m_projection;
  m_changeEveryFrame.m_worldViewProjInv = m_changeEveryFrame.m_worldViewProj;
  m_changeEveryFrame.m_worldViewProjInv.invert();
  m_changeEveryFrame.m_viewPosition = m_debugCam->getPositionVector();
  m_changeEveryFrame.vMeshColor = color;

  g_graphicsApi().updateSubresource(m_neverChangeB, &m_neverChanges);
  g_graphicsApi().updateSubresource(m_changeOnResizeB, &m_changeOnResize);
  g_graphicsApi().updateSubresource(m_changeEveryFrameB, &m_changeEveryFrame);
  g_graphicsApi().updateSubresource(m_lightsB, &m_lights);
  g_graphicsApi().updateSubresource(m_AOB, &m_AOData);
  g_graphicsApi().updateSubresource(m_BrightB, &m_BrightData);
  g_graphicsApi().updateSubresource(m_BlurB, &m_mipDataBlur);

  //g_graphicsApi().setConstanBuffer(neverChangeB,0);
  g_graphicsApi().VertexShaderSetConstanBuffer(m_neverChangeB, 0);
  g_graphicsApi().PixelShaderSetConstanBuffer(m_neverChangeB, 0);
  //g_graphicsApi().setConstanBuffer(changeOnResizeB,1);
  g_graphicsApi().VertexShaderSetConstanBuffer(m_changeOnResizeB, 1);
  g_graphicsApi().PixelShaderSetConstanBuffer(m_changeOnResizeB, 1);
  //g_graphicsApi().setConstanBuffer(changeEveryFrameB,2);
  g_graphicsApi().VertexShaderSetConstanBuffer(m_changeEveryFrameB, 2);
  g_graphicsApi().PixelShaderSetConstanBuffer(m_changeEveryFrameB, 2);

  g_graphicsApi().VertexShaderSetConstanBuffer(m_lightsB, 4);
  g_graphicsApi().PixelShaderSetConstanBuffer(m_lightsB, 4);
  g_graphicsApi().SetBonesConstanBuffer();
  SPtr<Sampler> samplerLineal = g_graphicsApi().CreateSampleLinearState();
  g_graphicsApi().setSampler(samplerLineal, 0);

  g_graphicsApi().setPrimitiveTopology(PRIMITIVE_TOPOLOGY_FORMAT::TRIANGLELIST);

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
  g_ResourceMan().loadResourceFromFile("data/textures/noivern_occlusion.png",
                                       RESOURCE_TYPE::TEXTURE);
  g_ResourceMan().loadResourceFromFile("data/textures/noivern_roughness.png",
                                       RESOURCE_TYPE::TEXTURE);
  g_ResourceMan().loadResourceFromFile("data/textures/pedestal_albedo.png",
                                       RESOURCE_TYPE::TEXTURE);
  g_ResourceMan().loadResourceFromFile("data/textures/pedestal_normals.png",
                                       RESOURCE_TYPE::TEXTURE);
  g_ResourceMan().loadResourceFromFile("data/textures/pedestal_metallic.png",
                                       RESOURCE_TYPE::TEXTURE);
  g_ResourceMan().loadResourceFromFile("data/textures/pedestal_occlusion.png",
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


  g_graphicsApi().PixelShaderSetShaderResources(m_ambientCubeMap, 4);

  //Rasterize states
  m_defaultRasState = g_graphicsApi().createRasterizeState();
  m_wireframeRasState = 
    g_graphicsApi().createRasterizeState(RASTERIZER_FILL_MODE::D3D11_FILL_WIREFRAME);

  SPtr<GameObject> light = SceneGraph::instance().createGameObject();
  light->setName("mainLight");
  SPtr<Component> mainLight = light->addComponent(COMPONENT_TYPE::LIGHT);
  CLight* ml = reinterpret_cast<CLight*>(mainLight.get());
  ml->setIdArray(m_lightCreated);
  ++m_lightCreated;

  SPtr<GameObject> mainCam = SceneGraph::instance().createGameObject();
  mainCam->setName("mainCamera");
  SPtr<Component> cam = g_CameraMan().getMainCamera();
  mainCam->addComponent(COMPONENT_TYPE::CAMERA, cam);

  initImGui();
}

void 
testApp::onRender() {  
  g_Render().onRender();
  return;
  imguiDockScreen();
  if (m_bSceneWindowNativeSize)
  {
    ImGui::SetNextWindowSize(ImVec2(m_clientSize.x * 0.5f, m_clientSize.y * 0.5f));
  }
  ImGui::Begin("SceneGraph");
  ImGui::CloseCurrentPopup();
  imGuiShowObject("child", SceneGraph::instance().m_root);
  ImGui::End();
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

  if (m_loadingFile)
  {
    imGuiLoadResourceFile();
  }

  {
    g_graphicsApi().setRenderTarget(m_first);
    g_graphicsApi().setViewPort(m_viewPortScene);
    g_graphicsApi().Clear(m_first, 0.2f, 0.2f, 0.2f, 1);
    g_graphicsApi().ClearDepthStencil(m_first);
    changeCameraDataBuffer(m_debugCam);
    g_graphicsApi().PixelShaderSetShaderResources(m_ambientCubeMap, 4);
    for (auto object : SceneGraph::instance().m_GObjects)
    {
      auto component =
        object->getComponent(COMPONENT_TYPE::TRANSFORM);
      CTransform* trans = reinterpret_cast<CTransform*>(component.get());
      m_changeEveryFrame.m_world = trans->getMatrixTransform();

      g_graphicsApi().updateSubresource(m_changeEveryFrameB, &m_changeEveryFrame);

      object->draw();
    }
  }

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
  if (m_firstFrame)
  {
    onResizeSceneWindow(size.x, size.y);
    m_sceneSize = size;
  }
  if (m_sceneSize != size)
  {
    onResizeSceneWindow(size.x, size.y);
    m_sceneSize = size;
  }
  ImGui::Image(m_first.get()->getRenderTexture(), wsize);
  ImGui::End();

  ImGui::Render();
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

}

void 
testApp::renderDeferred() {
  {
    if (!m_showDeferred)
    {
      return;
    }
    
    geometryPass();
    lightsPass();
    LuminancePass();
    BrightPass();
    AmbientOculsionPass();
    BlurPassAO();

    ImGui::Begin("Deferred",
                 &m_showDeferred);
    ImVec2 wsize = { (float)m_sceneSize.x,(float)m_sceneSize.y};
    ImGui::Text("Postions");
    ImGui::Image(m_deferred.get()->getRenderTexture(0), wsize);
    ImGui::Text("Normals");
    ImGui::Image(m_deferred.get()->getRenderTexture(1), wsize);
    ImGui::Text("Albedo");
    ImGui::Image(m_deferred.get()->getRenderTexture(2), wsize);
    ImGui::Text("Depth");
    ImGui::Image(m_deferred.get()->getRenderTexture(3), wsize);
    ImGui::Text("Diffuse");
    ImGui::Image(m_RTlights.get()->getRenderTexture(0), wsize);
    ImGui::Text("Specular");
    ImGui::Image(m_RTlights.get()->getRenderTexture(1), wsize);
    ImGui::Text("Ambient");
    ImGui::Image(m_RTlights.get()->getRenderTexture(2), wsize);
    ImGui::Text("Addition");
    ImGui::Image(m_RTlights.get()->getRenderTexture(3), wsize);
    ImGui::Text("Luminance");
    ImGui::Image(m_RTLuminance.get()->getRenderTexture(0), wsize);
    ImGui::Text("Bright");
    ImGui::Image(m_RTBright.get()->getRenderTexture(0), wsize);
    ImGui::Text("AmbientOculsionBeforeBlur");
    ImGui::Image(m_ambientOclusion->getRenderTexture(0), wsize);
    ImGui::Text("BlurH");
    ImGui::Image(m_RTBlurH->getRenderTexture(), wsize);
    ImGui::Text("BlurV");
    ImGui::Image(m_RTBlurV->getRenderTexture(), wsize);

    AddPass(m_ambientOclusionBlured, m_RTBlurH, m_RTBlurV);
    ColorWithAmbientOculsionPass();
    
    ImGui::Text("AmbientOculsionAfterBlur");
    ImGui::Image(m_ambientOclusionBlured->getRenderTexture(0), wsize);
    ImGui::Text("Light Color AO");
    ImGui::Image(m_RTColorAO->getRenderTexture(0), wsize);
    ImGui::End();

    if (m_bWireframe)
    {
      g_graphicsApi().setRasterizeState(m_wireframeRasState);
      g_graphicsApi().setProgramShader(m_progShaderWF);
    }
    else
    {
      g_graphicsApi().setRasterizeState(m_defaultRasState);
      g_graphicsApi().setProgramShader(m_progShader);
    }
  }
}

void 
testApp::geometryPass() {
  g_graphicsApi().setRenderTarget(m_deferred);
  g_graphicsApi().Clear(m_deferred, 0, 0, 0, 1);
  g_graphicsApi().ClearDepthStencil(m_deferred);
  g_graphicsApi().setProgramShader(m_progShaderDeferred);
  changeCameraDataBuffer(m_debugCam);
  g_graphicsApi().PixelShaderSetShaderResources(m_ambientCubeMap, 4);
  for (auto object : SceneGraph::instance().m_GObjects)
  {
    auto component =
      object->getComponent(COMPONENT_TYPE::TRANSFORM);
    CTransform* trans = reinterpret_cast<CTransform*>(component.get());
    m_changeEveryFrame.m_world = trans->getMatrixTransform();

    g_graphicsApi().updateSubresource(m_changeEveryFrameB, &m_changeEveryFrame);

    object->draw();
  }
  g_graphicsApi().generateMipMap(m_deferred);
}

void 
testApp::lightsPass() {
  g_graphicsApi().setRenderTarget(m_RTlights);
  g_graphicsApi().Clear(m_RTlights, 0.2f, 0.2f, 0.2f, 1);
  g_graphicsApi().ClearDepthStencil(m_RTlights);
  g_graphicsApi().setProgramShader(m_progShaderLights);
  changeCameraDataBuffer(m_debugCam);
  g_graphicsApi().PixelShaderSetShaderResources(m_ambientCubeMap, 4);
  for (auto object : SceneGraph::instance().m_GObjects)
  {
    auto component =
      object->getComponent(COMPONENT_TYPE::TRANSFORM);
    CTransform* trans = reinterpret_cast<CTransform*>(component.get());
    m_changeEveryFrame.m_world = trans->getMatrixTransform();

    g_graphicsApi().updateSubresource(m_changeEveryFrameB, &m_changeEveryFrame);

    object->draw();
  }
  g_graphicsApi().generateMipMap(m_RTlights);
}

void 
testApp::LuminancePass() {
  g_graphicsApi().setProgramShader(m_progSLuminance);
  g_graphicsApi().setRenderTarget(m_RTLuminance);
  g_graphicsApi().Clear(m_RTLuminance, 0.2f, 0.2f, 0.2f, 1);
  g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTlights, 0, 0);
  g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTlights, 1, 1);
  g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTlights, 2, 2);
  g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTlights, 3, 3);
  g_ResourceMan().setSAQ();
  g_graphicsApi().generateMipMap(m_RTLuminance);
}

void 
testApp::BrightPass() {
  g_graphicsApi().setProgramShader(m_progSBright);
  g_graphicsApi().setRenderTarget(m_RTBright);
  g_graphicsApi().Clear(m_RTBright, 0.2f, 0.2f, 0.2f, 1);
  g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTlights, 0, 0);
  g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTlights, 1, 1);
  g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTlights, 2, 2);
  g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTLuminance, 3, 0);
  g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTlights, 4, 3);
  g_graphicsApi().PixelShaderSetConstanBuffer(m_BrightB, 6);
  g_ResourceMan().setSAQ();
  g_graphicsApi().generateMipMap(m_RTBright);
}

void 
testApp::AmbientOculsionPass() {
  g_graphicsApi().setProgramShader(m_progShaderAO);
  g_graphicsApi().setRenderTarget(m_ambientOclusion);
  g_graphicsApi().Clear(m_ambientOclusion, 1, 1, 1, 1);
  g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_deferred, 0, 0);
  g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_deferred, 1, 1);
  g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_deferred, 2, 3);
  g_graphicsApi().PixelShaderSetConstanBuffer(m_AOB, 5);
  g_ResourceMan().setSAQ();
  g_graphicsApi().generateMipMap(m_ambientOclusion);
}

void 
testApp::ColorWithAmbientOculsionPass() {
  g_graphicsApi().setProgramShader(m_progSBColorAO);
  g_graphicsApi().setRenderTarget(m_RTColorAO);
  g_graphicsApi().Clear(m_RTColorAO, 0.2f, 0.2f, 0.2f, 1);
  g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTlights, 0, 0);
  g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTlights, 1, 1);
  g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTlights, 2, 2);
  g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_ambientOclusionBlured, 3, 0);
  g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_RTlights, 4, 3);
  g_ResourceMan().setSAQ();
  g_graphicsApi().generateMipMap(m_RTColorAO);
}

void 
testApp::BlurPassAO() {

  g_graphicsApi().setProgramShader(m_progSBBlurH);
  g_graphicsApi().PixelShaderSetConstanBuffer(m_BlurB,7);
  g_graphicsApi().setRenderTarget(m_RTBlurH);
  //viewport escalado
  ViewPort tmp = m_viewPortScene;
  tmp.Height = m_viewPortScene.Height * 0.5;
  tmp.Width = m_viewPortScene.Width * 0.5;
  g_graphicsApi().setViewPort(tmp);
  g_graphicsApi().Clear(m_RTBlurH, 0.2f, 0.2f, 0.2f, 1);
  g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_ambientOclusion, 0, 0);
  g_ResourceMan().setSAQ();
  g_graphicsApi().generateMipMap(m_RTBlurH);

  g_graphicsApi().setProgramShader(m_progSBBlurV);
  g_graphicsApi().PixelShaderSetConstanBuffer(m_BlurB,7);
  g_graphicsApi().setRenderTarget(m_RTBlurV);
  g_graphicsApi().Clear(m_RTBlurV, 0.2f, 0.2f, 0.2f, 1);
  g_graphicsApi().PixelShaderSetShaderResourcesFromRT(m_ambientOclusion, 0, 0);
  g_ResourceMan().setSAQ();
  g_graphicsApi().generateMipMap(m_RTBlurV);
  //
  //viewport normal
  g_graphicsApi().setViewPort(m_viewPortScene);
}

void 
testApp::AddPass(SPtr<RenderTarget> target, 
                 SPtr<RenderTarget> renderTarget1, 
                 SPtr<RenderTarget> renderTarget2, 
                 uint32 slotOfTarget1, 
                 uint32 slotOfTarget2) {

  g_graphicsApi().setProgramShader(m_progSBAdd);
  g_graphicsApi().PixelShaderSetConstanBuffer(m_BlurB,7);
  g_graphicsApi().setRenderTarget(target);
  g_graphicsApi().Clear(target, 0.2f, 0.2f, 0.2f, 1);
  g_graphicsApi().PixelShaderSetShaderResourcesFromRT(renderTarget1, 0, slotOfTarget1);
  g_graphicsApi().PixelShaderSetShaderResourcesFromRT(renderTarget2, 1, slotOfTarget2);
  g_ResourceMan().setSAQ();
  g_graphicsApi().generateMipMap(target);
}

void
testApp::onResize(int32 width, int32 height) {
  if (!m_windowHasFocus)
  {
    return;
  }
  g_Render().onResize(width,height);
  //ImGui::GetIO().DisplaySize = ImVec2((float)width, (float)height);
  //ImGui::GetIO().DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
  //m_viewPort.Width = (float)width;
  //m_viewPort.Height = (float)height;
  //m_viewPort.MinDepth = 0.0f;
  //m_viewPort.MaxDepth = 1.0f;
  //m_viewPortScene = m_viewPort;
  g_graphicsApi().resizeSwapChain(width, height);
}

void 
testApp::onResizeSceneWindow(uint32 width, uint32 height) {
  //auto clientSize = m_window.getSize();
  //m_clientSize.x = clientSize.x;
  //m_clientSize.y = clientSize.y;
  if (!m_windowHasFocus)
  {
    return;
  }
  m_rtv = g_graphicsApi().getRenderTargetView();
  g_graphicsApi().setRenderTarget(m_rtv);
  g_CameraMan().resizeCameraProjection(width,height);
  m_debugCam->resize(width, height);
  //m_changeOnResize.m_projection = createProjectionPerspectiveMatrix(Math::HALF_PI / 2,
  //                                                                  (float)width,
  //                                                                  (float)height,
  //                                                                  0.01f,
  //                                                                  100000.0f);
  m_changeOnResize.m_projection = m_debugCam->getMatrixProjection();
  m_changeOnResize.m_projection.transpose();
  m_changeOnResize.m_projectionInv = m_changeOnResize.m_projection;
  m_changeOnResize.m_projectionInv.invert();
  g_graphicsApi().updateSubresource(m_changeOnResizeB, &m_changeOnResize);
  m_inputAPI->resize(width, height);
  //m_viewPortScene.Width = width;
  //m_viewPortScene.Height = height;
}

void 
testApp::onMouseButtonPressed(int32 button, int32 x, int32 y) {
  g_Render().onMouseButtonPressed(button, x, y);
  return;
  ImGuiIO& io = ImGui::GetIO();
  if (!ImGui::IsAnyMouseDown() && nullptr == ::GetCapture())
  {
    ::SetCapture(m_window.getSystemHandle());
  }
  io.MouseDown[static_cast<int32>(button)] = true;
}

void 
testApp::onTextEnterd(UNICHAR unicode) {
  g_Render().onTextEnterd(unicode);
  return;
  ImGuiIO& io = ImGui::GetIO();
  io.AddInputCharacterUTF16((unsigned short) unicode);
}

void
testApp::onKeyPressed(int32 code, bool alt, bool control, bool shift, bool system) {
  g_Render().onKeyPressed(code,alt,control, shift, system);
  //ImGuiIO& io = ImGui::GetIO();
  //if (code >= 0)
  //{
  //  io.KeysDown[static_cast<int32>(code)] = true;
  //}
  //if (code == sf::Keyboard::W || 
  //    code == sf::Keyboard::S || 
  //    code == sf::Keyboard::A || 
  //    code == sf::Keyboard::D || 
  //    code == sf::Keyboard::Q || 
  //    code == sf::Keyboard::E || 
  //    code == sf::Keyboard::Right || 
  //    code == sf::Keyboard::Left || 
  //    code == sf::Keyboard::Up || 
  //    code == sf::Keyboard::Down) {
  //  if (code == sf::Keyboard::W)
  //  {
  //    m_debugCam->traslate(0, 0, 1);
  //  }
  //  if (code == sf::Keyboard::S)
  //  {
  //    m_debugCam->traslate(0, 0, -1);
  //  }
  //  if (code == sf::Keyboard::A)
  //  {
  //    m_debugCam->traslate(-1, 0, 0);
  //  }
  //  if (code == sf::Keyboard::D)
  //  {
  //    m_debugCam->traslate(1, 0, 0);
  //  }
  //  if (code == sf::Keyboard::Q)
  //  {
  //    m_debugCam->traslate(0, -1, 0);
  //  }
  //  if (code == sf::Keyboard::E)
  //  {
  //    m_debugCam->traslate(0, 1, 0);
  //  }
  //  if (code == sf::Keyboard::Left)
  //  {
  //    m_debugCam->rotate(0, Degree(10));
  //  }
  //  if (code == sf::Keyboard::Right)
  //  {
  //    m_debugCam->rotate(0, Degree(-10));
  //  }
  //  if (code == sf::Keyboard::Up)
  //  {
  //    m_debugCam->rotate(Degree(10), 0);
  //  }
  //  if (code == sf::Keyboard::Down)
  //  {
  //    m_debugCam->rotate(Degree(-10), 0);
  //  }
  //  m_neverChanges.m_view = m_debugCam->getMatrixView();
  //  m_neverChanges.m_viewInv = m_neverChanges.m_view;
  //  m_neverChanges.m_viewInv.invert();
  //
  //  m_changeOnResize.m_viewProjection = m_neverChanges.m_view * m_changeOnResize.m_projection;
  //  m_changeOnResize.m_viewProjectionInv = m_changeOnResize.m_viewProjection;
  //  m_changeOnResize.m_viewProjectionInv.invert();
  //
  //  m_changeEveryFrame.m_worldProj = m_changeEveryFrame.m_world * 
  //                                   m_changeOnResize.m_projection;
  //  m_changeEveryFrame.m_worldProjInv = m_changeEveryFrame.m_worldProj;
  //  m_changeEveryFrame.m_worldProjInv.invert();
  //  
  //  m_changeEveryFrame.m_worldView = m_changeEveryFrame.m_world * m_neverChanges.m_view;
  //  m_changeEveryFrame.m_worldViewInv = m_changeEveryFrame.m_worldView;
  //  m_changeEveryFrame.m_worldViewInv.invert();
  //  
  //  m_changeEveryFrame.m_worldViewProj = m_changeEveryFrame.m_worldView * 
  //                                       m_changeOnResize.m_projection;
  //  m_changeEveryFrame.m_worldViewProjInv = m_changeEveryFrame.m_worldViewProj;
  //  m_changeEveryFrame.m_worldViewProjInv.invert();
  //
  //  m_changeEveryFrame.m_viewPosition = m_debugCam->getPositionVector();
  //
  //  g_graphicsApi().updateSubresource(m_neverChangeB, &m_neverChanges);
  //}
}

void
testApp::onKeyReleased(int32 code, bool alt, bool control, bool shift, bool system) {
  g_Render().onKeyReleased(code, alt, control, shift, system);
  //ImGuiIO& io = ImGui::GetIO();
  //if (code >= 0)
  //{
  //  io.KeysDown[static_cast<int32>(code)] = false;
  //}
}

void
testApp::onMouseButtonReleased(int32 button, int32 x, int32 y) {
  g_Render().onMouseButtonReleased(button, x, y);
  //ImGuiIO& io = ImGui::GetIO();
  //if (!ImGui::IsAnyMouseDown() && ::GetCapture() == m_window.getSystemHandle())
  //{
  //  ::ReleaseCapture();
  //}
  //io.MouseDown[static_cast<int32>(button)] = false;
}

void
testApp::onUpdate(const float& deltaTime) {
  SceneGraph::instance().onUpdate(deltaTime);
  g_Render().onUpdate(deltaTime);
  return;
  UpdateCameraInterpolate(deltaTime);
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();
}

void 
testApp::handleWindownput(const float& deltaTime) {

  //if (m_inputAPI->getKeyUp(KEYBOARD::kKeyA))
  //{
  //  m_debugCam->rotate(Degree(-25 * deltaTime), 0);
  //  changeCameraDataBuffer(m_debugCam);
  //}
  //if (m_inputAPI->getKey(KEYBOARD::kKeyW) ||
  //  m_inputAPI->getGamepadAxis(GAMEPAD_AXIS::kPadButtonLeftStickY) > 0 ||
  //  m_inputAPI->getKey(KEYBOARD::kKeyS) ||
  //  m_inputAPI->getKey(KEYBOARD::kKeyA) ||
  //  m_inputAPI->getKey(KEYBOARD::kKeyD) ||
  //  m_inputAPI->getKey(KEYBOARD::kKeyQ) ||
  //  m_inputAPI->getKey(KEYBOARD::kKeyE) ||
  //  m_inputAPI->getKey(KEYBOARD::kKeyRight) ||
  //  m_inputAPI->getKey(KEYBOARD::kKeyLeft) ||
  //  m_inputAPI->getKey(KEYBOARD::kKeyUp) ||
  //  m_inputAPI->getKey(KEYBOARD::kKeyDown)) {
  //}
//  m_inputAPI->getGamepadButtonDown(GAMEPAD_BUTTON::kPadButton18);
  m_inputAPI->getGamepadAxis(GAMEPAD_AXIS::kPadButtonLeftStickX);
  //if (m_inputAPI->getMouseButtonDown(MOUSE_BUTTON::kMouseButtonLeft))
  //{
  //  m_debugCam->rotate(Degree(-25 * deltaTime), 0);
  //  changeCameraDataBuffer(m_debugCam);
  //}
  if (m_inputAPI->getKey(KEYBOARD::kKeyW) || 
      m_inputAPI->getGamepadAxis(GAMEPAD_AXIS::kPadButtonLeftStickY) > 0) {
    m_debugCam->traslate(0, 0, 1 * deltaTime);
    changeCameraDataBuffer(m_debugCam);
  }
  if (m_inputAPI->getKey(KEYBOARD::kKeyS))
  {
    m_debugCam->traslate(0, 0, -1 * deltaTime);
    changeCameraDataBuffer(m_debugCam);
  }
  if (m_inputAPI->getKey(KEYBOARD::kKeyA))
  {
    m_debugCam->traslate(-1 * deltaTime, 0, 0);
    changeCameraDataBuffer(m_debugCam);
  }
  if (m_inputAPI->getKey(KEYBOARD::kKeyD))
  {
    m_debugCam->traslate(1, 0 * deltaTime, 0);
    changeCameraDataBuffer(m_debugCam);
  }
  if (m_inputAPI->getKey(KEYBOARD::kKeyQ))
  {
    m_debugCam->traslate(0, -1 * deltaTime, 0);
    changeCameraDataBuffer(m_debugCam);
  }
  if (m_inputAPI->getKey(KEYBOARD::kKeyE))
  {
    m_debugCam->traslate(0, 1 * deltaTime, 0);
    changeCameraDataBuffer(m_debugCam);
  }
  if (m_inputAPI->getKey(KEYBOARD::kKeyLeft))
  {
    m_debugCam->rotate(0, Degree(25 * deltaTime));
    changeCameraDataBuffer(m_debugCam);
  }
  if (m_inputAPI->getKey(KEYBOARD::kKeyRight))
  {
    m_debugCam->rotate(0, Degree(-25 * deltaTime));
    changeCameraDataBuffer(m_debugCam);
  }
  if (m_inputAPI->getKey(KEYBOARD::kKeyUp))
  {
    m_debugCam->rotate(Degree(25 * deltaTime), 0);
    changeCameraDataBuffer(m_debugCam);
  }
  if (m_inputAPI->getKey(KEYBOARD::kKeyDown))
  {
    m_debugCam->rotate(Degree(-25 * deltaTime), 0);
    changeCameraDataBuffer(m_debugCam);
  }
  g_Render().handleWindownput(deltaTime);
   /* m_neverChanges.m_view = m_debugCam->getMatrixView();
    m_neverChanges.m_viewInv = m_neverChanges.m_view;
    m_neverChanges.m_viewInv.invert();

    m_changeOnResize.m_viewProjection = m_neverChanges.m_view * m_changeOnResize.m_projection;
    m_changeOnResize.m_viewProjectionInv = m_changeOnResize.m_viewProjection;
    m_changeOnResize.m_viewProjectionInv.invert();

    m_changeEveryFrame.m_worldProj = m_changeEveryFrame.m_world * 
                                     m_changeOnResize.m_projection;
    m_changeEveryFrame.m_worldProjInv = m_changeEveryFrame.m_worldProj;
    m_changeEveryFrame.m_worldProjInv.invert();
    
    m_changeEveryFrame.m_worldView = m_changeEveryFrame.m_world * m_neverChanges.m_view;
    m_changeEveryFrame.m_worldViewInv = m_changeEveryFrame.m_worldView;
    m_changeEveryFrame.m_worldViewInv.invert();
    
    m_changeEveryFrame.m_worldViewProj = m_changeEveryFrame.m_worldView * 
                                         m_changeOnResize.m_projection;
    m_changeEveryFrame.m_worldViewProjInv = m_changeEveryFrame.m_worldViewProj;
    m_changeEveryFrame.m_worldViewProjInv.invert();

    m_changeEveryFrame.m_viewPosition = m_debugCam->getPositionVector();

    g_graphicsApi().updateSubresource(m_neverChangeB, &m_neverChanges);*/
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

  //ImGui::FileBrowser fileDialog(ImGuiFileBrowserFlags_CloseOnEsc);
  //m_fileDialog = fileDialog;
  // (optional) set browser properties
  m_fileDialog.reset(new ImGui::FileBrowser(ImGuiFileBrowserFlags_CloseOnEsc |
                                            ImGuiFileBrowserFlags_MultipleSelection));
  m_fileDialog->SetTitle("LoadResource");
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
      ImGui::Checkbox("SceneWindowNativeSize", &m_bSceneWindowNativeSize);
      ImGui::Checkbox("ShowAmbientOptions", &m_bAmbientOptions);
      ImGui::Checkbox("Wireframe", &m_bWireframe);
      ImGui::Checkbox("CameraInterpolate", &m_cameraInterpolateMenuAtive);
      ImGui::Checkbox("Deferred", &m_showDeferred);
      ImGui::Checkbox("AO options", &m_showAOOptions);
      ImGui::Checkbox("Bright options", &m_showBrightOptions);
      ImGui::Checkbox("Blur options", &m_showBlurOptions);
      if (m_bWireframe)
      {
        g_graphicsApi().setRasterizeState(m_wireframeRasState);
        g_graphicsApi().setProgramShader(m_progShaderWF);
      }
      else
      {
        g_graphicsApi().setRasterizeState(m_defaultRasState);
        g_graphicsApi().setProgramShader(m_progShader);
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
  bool hasChildren = false;
  bool isOpen = false;
  if (0<object->m_children.size())
  {
    hasChildren = true;
  }

  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth |
                             ImGuiTreeNodeFlags_OpenOnArrow |
                             ImGuiTreeNodeFlags_OpenOnDoubleClick;
  if (nullptr == object->getParent())
  {
    flags |= ImGuiTreeNodeFlags_DefaultOpen ;
    ImGui::SetNextTreeNodeOpen(true);
  }
  if (!hasChildren) {
    flags |= ImGuiTreeNodeFlags_Leaf |
             ImGuiTreeNodeFlags_Bullet;
  }
  isOpen = ImGui::TreeNodeEx(object->getName().c_str(), flags);
  //else
  //{
  //  ImGui::Selectable(object->getName().c_str());
  //}

  if (ImGui::IsItemClicked()) {
    object->selected = true;
    if (SceneGraph::instance().selectedObjet != object)
    {
      if (nullptr != SceneGraph::instance().selectedObjet)
      {
        SceneGraph::instance().selectedObjet->selected = false;
      }
      SceneGraph::instance().selectedObjet = object;
    }
  }

    if (isOpen) {


      ImGui::AlignTextToFramePadding();   // Text and Tree nodes are less high than framed widgets, here we add vertical spacing to make the tree lines equal high.
      if (0 < object->m_children.size())
      {
        for (auto son : object->m_children)
        {
          imGuiShowObject(son->getName().c_str(), son);
        }
      }
      ImGui::TreePop();
    }
  
  
  // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
  
}

void 
testApp::imGuiInspectorObject() {
  ImGui::CloseCurrentPopup();
  ImGui::Begin("Inspector",NULL);
  JDVector3 pos = { m_changeEveryFrame.m_viewPosition.x,
                    m_changeEveryFrame.m_viewPosition.y,
                    m_changeEveryFrame.m_viewPosition.z };

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
    if (rot!= trans->euler)
    {
      trans->rotation = { Degree(rot.x), Degree(rot.y), Degree(rot.z) };
      trans->euler = rot;
    }
  }
  component =
    SceneGraph::instance().selectedObjet->getComponent(COMPONENT_TYPE::RENDERMODEL);
  if (nullptr != component)
  {
    showRenderModelComponent();
  }
  component =
    SceneGraph::instance().selectedObjet->getComponent(COMPONENT_TYPE::LIGHT);
  if (nullptr != component)
  {
    showLightComponent();
  }

  component =
    SceneGraph::instance().selectedObjet->getComponent(COMPONENT_TYPE::CAMERA);
  if (nullptr != component)
  {
    showCameraComponent();
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
  auto rModel = 
    reinterpret_cast<CRenderModel*>(Model->getComponent(COMPONENT_TYPE::RENDERMODEL).get());
  String g_OptionPreviw = ResourceManager::instance().m_modelsNames[Model->m_modelOption];
  int32 modelOption = Model->m_modelOption;
  ImGui::Button(ResourceManager::instance().m_modelsNames[modelOption], ImVec2(80, 80));
  if (ImGui::BeginDragDropTarget())
  {
    const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Modeldrop");
    if (payload)
    {
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
  if (nullptr != rModel->m_model)
  {
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
testApp::showLightComponent() {
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
  m_lights.light[id].m_lightDirection = dir;
  m_lights.light[id].m_lightColor = clight->m_color;
  g_graphicsApi().updateSubresource(m_lightsB, &m_lights);
}

void 
testApp::showCameraComponent() {
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
  g_graphicsApi().setRenderTarget(m_cameraRT);
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
  }
  ImVec2 size = { (float)m_sceneSize.x, (float)m_sceneSize.y+30 };
  size.x /= 2;
  size.y /= 2;
  ImGui::SetNextWindowSize(size);
  ImGui::Begin("Camera preview",
               nullptr,
               ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
  ImVec2 wsize = ImGui::GetWindowSize();
  wsize.y -= 30;
  ImGui::Image(m_cameraRT.get()->getRenderTexture(), wsize);
  ImGui::End();
}

void 
testApp::showRenderModelMaterias(CRenderModel* rModel) {
  ImGui::AlignTextToFramePadding();
  String g_OptionPreviw;
  uint32 conter = 0;
  for (auto m : rModel->m_model->m_meshes)
  {
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
      if (ImGui::BeginDragDropTarget())
      {
        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("textdrop");
        if (payload)
        {
          albedolOption = *reinterpret_cast<uint32*>(payload->Data);
        }
        ImGui::EndDragDropTarget();
      }
      if (ImGui::BeginCombo(typeText.c_str(), g_OptionPreviw.c_str()))
      {
        g_OptionPreviw = "";
        ImGui::ListBox("Texture##1", &albedolOption,
                       &ResourceManager::instance().m_texturesNames[0],
                       (int)ResourceManager::instance().m_texturesNames.size());
        ImGui::EndCombo();
      }
      if (albedolOption != (int32)m->m_albedoOption)
      {
        //static String g_UnitOptionPreviw = ResourceManager::instance().m_texturesNames[0];
        if (albedolOption > 0)
        {
          m->setAlbedoTexture(ResourceManager::instance().m_textures[albedolOption - 1]);
        }
        else
        {
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
      if (ImGui::BeginDragDropTarget())
      {
        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("textdrop");
        if (payload)
        {
          normalOption = *reinterpret_cast<uint32*>(payload->Data);
        }
        ImGui::EndDragDropTarget();
      }
      if (ImGui::BeginCombo(typeText.c_str(), g_OptionPreviw.c_str()))
      {
        g_OptionPreviw = "";
        ImGui::ListBox("Texture##2", &normalOption,
          &ResourceManager::instance().m_texturesNames[0],
          (int)ResourceManager::instance().m_texturesNames.size());
        ImGui::EndCombo();
      }
      if (normalOption != (int32)m->m_NormalOption)
      {
        if (normalOption > 0)
        {
          m->setNormalTexture(ResourceManager::instance().m_textures[normalOption - 1]);
        }
        else
        {
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
      if (ImGui::BeginDragDropTarget())
      {
        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("textdrop");
        if (payload)
        {
          specularOption = *reinterpret_cast<uint32*>(payload->Data);
        }
        ImGui::EndDragDropTarget();
      }
      if (ImGui::BeginCombo(typeText.c_str(), g_OptionPreviw.c_str()))
      {
        g_OptionPreviw = "";
        ImGui::ListBox("Texture##3", &specularOption,
          &ResourceManager::instance().m_texturesNames[0],
          (int)ResourceManager::instance().m_texturesNames.size());
        ImGui::EndCombo();
      }
      if (specularOption != (int32)m->m_specularOption)
      {
        if (specularOption > 0)
        {
          m->setSpecularTexture(ResourceManager::instance().m_textures[specularOption - 1]);
        }
        else
        {
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
      if (ImGui::BeginDragDropTarget())
      {
        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("textdrop");
        if (payload)
        {
          roughOption = *reinterpret_cast<uint32*>(payload->Data);
        }
        ImGui::EndDragDropTarget();
      }
      if (ImGui::BeginCombo(typeText.c_str(), g_OptionPreviw.c_str()))
      {
        g_OptionPreviw = "";
        ImGui::ListBox("Texture##4", &roughOption,
          &ResourceManager::instance().m_texturesNames[0],
          (int)ResourceManager::instance().m_texturesNames.size());
        ImGui::EndCombo();
      }
      if (roughOption != (int32)m->m_roughOption)
      {
        if (roughOption > 0)
        {
          m->setRoughnessTexture(ResourceManager::instance().m_textures[roughOption - 1]);
        }
        else
        {
          m->setRoughnessTexture(ResourceManager::instance().DEFAULT_TEXTURE_WHITE);
        }
        m->m_roughOption = roughOption;
      }
    }
    ++conter;
  }
}

void 
testApp::ImGuiAddComponent() {
  ImGui::Begin("Components",&m_showComponentImGui);
  static ImGuiTextFilter filter;
  filter.Draw("Filter");
  const char* lines[] = { "Transform", "RenderModel","Light","Camera" };
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
testApp::addLightComponent() {
  if (m_lightCreated>=20) {
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
  m_lights.light[0].m_numberOfLights = m_lightCreated;
  g_graphicsApi().updateSubresource(m_lightsB, &m_lights);
}

void 
testApp::addCameraComponent() {
  ImGui::Begin("new Camera", &m_addigCamera);
  ImGui::InputText("Camera Name", m_newCameraName.data(), 25);
  ImGui::DragFloat("Camera Near",&m_newCamNear);
  ImGui::DragFloat("Camera Far",&m_newCamFar);
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
testApp::imGuiLoadResourceFile() {
  
  m_fileDialog->Display();
  if (m_fileDialog->HasSelected())
  {
    uint32 files = (uint32)m_fileDialog->GetMultiSelected().size();
    for (uint32 i = 0; i < files; ++i)
    {
      g_ResourceMan().loadResourceFromFile(m_fileDialog->GetMultiSelected()[i].string().c_str(),
                                           m_typeResourceToLoad);
    }
    //std::cout << "Selected filename" << m_fileDialog->GetSelected().string() << std::endl;
    m_fileDialog->ClearSelected();
    m_loadingFile = false;
  }
}

void 
testApp::showAmbientOption() {
  if (!m_bAmbientOptions)
  {
    return;
  }
  if (ImGui::Begin("AmbientOption", &m_bAmbientOptions))
  {
    String optionPreviw = ResourceManager::instance().m_texturesNames[m_ambientCubeMapOption];
    //ImGui::Text("Roughness");
    ImGui::Image(m_ambientCubeMap->getTexture(), ImVec2(40, 40));
    if (ImGui::BeginCombo("CubeMap", optionPreviw.c_str()))
    {
      int option = m_ambientCubeMapOption;
      optionPreviw = "";
      ImGui::ListBox("Texture##5", &option,
        &ResourceManager::instance().m_texturesNames[0],
        (int)ResourceManager::instance().m_texturesNames.size());
      if (option != (int32)m_ambientCubeMapOption)
      {
        //static String g_UnitOptionPreviw = ResourceManager::instance().m_texturesNames[0];
        if (option > 0)
        {
          m_ambientCubeMap = (ResourceManager::instance().m_textures[option - 1]);
        }
        else
        {
          m_ambientCubeMap = (ResourceManager::instance().DEFAULT_TEXTURE_WHITE);
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
testApp::showAmbientOclusionOption() {
  if (!m_showAOOptions)
  {
    return;
  }
  if (ImGui::Begin("AmbientOption", &m_showAOOptions))
  {
    ImGui::DragFloat("Sample Radius", &m_AOData.g_sample_radius, 0.001f, 0, 0, "%.5f");
    ImGui::DragFloat("Scale", &m_AOData.g_scale, 0.001f, 0, 0, "%.5f");
    ImGui::DragFloat("Bias",&m_AOData.g_Bias, 0.001f, 0, 0, "%.5f");
    ImGui::DragFloat("Intensity", &m_AOData.g_intensity, 0.001f, 0, 0, "%.5f");
    g_graphicsApi().updateSubresource(m_AOB, &m_AOData);
    ImGui::End();
  }
}

void 
testApp::showBrightOption() {
  if (!m_showBrightOptions)
  {
    return;
  }
  if (ImGui::Begin("Birght Options", &m_showBrightOptions))
  {
    ImGui::DragFloat("BrightLod", &m_BrightData.BrightLod_BloomThreshold.x, 1, 0, 0, "%.0f");
    ImGui::DragFloat("BloomThreshold", &m_BrightData.BrightLod_BloomThreshold.y, 0.01f, 0, 0, "%.3f");
    g_graphicsApi().updateSubresource(m_BrightB, &m_BrightData);
    ImGui::End();
  }
}

void 
testApp::showBlurOption() {
  if (!m_showBlurOptions)
  {
    return;
  }
  if (ImGui::Begin("Blur Options", &m_showBlurOptions))
  {
    ImGui::DragInt("Mip Level 0",&m_mipDataBlur.mipLevel0);
    ImGui::DragInt("Mip Level 1",&m_mipDataBlur.mipLevel1);
    ImGui::DragInt("Mip Level 2",&m_mipDataBlur.mipLevel2);
    ImGui::DragInt("Mip Level 3",&m_mipDataBlur.mipLevel3);
    g_graphicsApi().updateSubresource(m_BlurB, &m_mipDataBlur);
    ImGui::End();
  }
}

void 
testApp::showModels() {
  //if (!m_windowHasFocus)
  //{
  //  return;
  //}
  ImGui::Begin("Models##30");
  uint32 numModels = (uint32)g_ResourceMan().m_models.size();
  ImVec2 size = ImGui::GetWindowSize();
  if (size.x < 80)
  {
    size.x = 80;
    ImGui::SetWindowSize(size);
  }
  int32 alocator = int32((size.x - 40) / 80);
  if (alocator<1)
  {
    alocator = 1;
  }
  for (uint32 n = 0; n < numModels; ++n) {
    //ImGui::PushID(n);
    if ((n % alocator) != 0)
      ImGui::SameLine();

    //ImGui::SetNextItemWidth(60);
    ImGui::BeginGroup();
    //ImGui::al();
    String child = "child";
    child += n;
    ImGui::BeginChild(child.c_str(), ImVec2(80, 80));
    ImGui::Button(g_ResourceMan().m_modelsNames[n+1], ImVec2(60, 60));
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
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
testApp::showTexturesResources() {
  //if (!m_windowHasFocus)
  //{
  //  return;
  //}
  ImGui::Begin("Textures##30");
  uint32 numText = (uint32)g_ResourceMan().m_textures.size();
  ImVec2 size = ImGui::GetWindowSize();
  if (size.x<80)
  {
    size.x = 80;
    ImGui::SetWindowSize(size);
  }
  int32 alocator = int32((size.x-40) / 80);
  if (alocator < 1)
  {
    alocator = 1;
  }
  for (uint32 n = 0; n < numText; ++n) {
    //ImGui::PushID(n);
    if ((n % alocator) != 0)
      ImGui::SameLine();

    //ImGui::SetNextItemWidth(60);
    ImGui::BeginGroup();
    //ImGui::al();
    String child = "child";
    child += n;
    ImGui::BeginChild(child.c_str(), ImVec2(80, 140));
    ImGui::ImageButton(g_ResourceMan().m_textures[n]->getTexture(), ImVec2(60, 60));
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
      uint32 info = n + 1;
      ImGui::SetDragDropPayload("textdrop", &info, sizeof(const char*));
      ImGui::TextUnformatted(g_ResourceMan().m_texturesNames[n + 1]);
      ImGui::EndDragDropSource();
    }
    ImGui::TextWrapped(g_ResourceMan().m_texturesNames[n + 1], ImVec2(65, 20));
    ImGui::EndChild();
    ImGui::EndGroup();
    //ImGui::PopID();
  }
  ImGui::End();
}

void 
testApp::showCameraInterpolateMenu() {
  if (!m_cameraInterpolateMenuAtive)
  {
    return;
  }
  ImGui::Begin("Camera Interpolate Menu", &m_cameraInterpolateMenuAtive);
  if (nullptr != m_tempCam)
  {
    {
      g_graphicsApi().setRenderTarget(m_cameraInterpolate);
      g_graphicsApi().Clear(m_cameraInterpolate, 0.2f, 0.2f, 0.2f, 1);
      g_graphicsApi().ClearDepthStencil(m_cameraInterpolate);
      changeCameraDataBuffer(m_tempCam);
      for (auto object : SceneGraph::instance().m_GObjects)
      {
        auto component =
          object->getComponent(COMPONENT_TYPE::TRANSFORM);
        CTransform* trans = reinterpret_cast<CTransform*>(component.get());
        m_changeEveryFrame.m_world = trans->getMatrixTransform();

        g_graphicsApi().updateSubresource(m_changeEveryFrameB, &m_changeEveryFrame);

        object->draw();
      }
    }
    ImVec2 size = { (float)m_sceneSize.x, (float)m_sceneSize.y + 30 };
    size.x /= 2;
    size.y /= 2;
    //ImGui::SetNextWindowSize(size);
    ImGui::Image(m_cameraInterpolate.get()->getRenderTexture(), size);
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
testApp::UpdateCameraInterpolate(const float& deltaTime) {
  if (g_CameraMan().m_interpolating)
  {
    m_tempCam = g_CameraMan().interpolateCameras(m_interpolateCam1,
                                                 m_interpolateCam2,
                                                 deltaTime,
                                                 m_interpolationCamerasTime);
  }
  

}

void 
testApp::onDestroy() {
  //ImGui_ImplDX11_Shutdown();
  //ImGui_ImplWin32_Shutdown();
  //ImGui::DestroyContext();
}

void 
testApp::changeCameraDataBuffer(WeakSptr<Camera> cam) {
  return;
  SPtr<Camera> camera = cam.lock();

  m_neverChanges.m_view = camera->getMatrixView();
  m_neverChanges.m_viewInv = m_neverChanges.m_view;
  m_neverChanges.m_viewInv.invert();
  
  m_changeOnResize.m_projection = camera->getMatrixProjection();
  m_changeOnResize.m_projection.transpose();
  m_changeOnResize.m_projectionInv = m_changeOnResize.m_projection;
  m_changeOnResize.m_projectionInv.invert();

  m_changeOnResize.m_viewProjection = m_neverChanges.m_view * m_changeOnResize.m_projection;
  m_changeOnResize.m_viewProjectionInv = m_changeOnResize.m_viewProjection;
  m_changeOnResize.m_viewProjectionInv.invert();

  m_changeEveryFrame.m_worldViewProj = m_changeEveryFrame.m_worldView * 
                                       m_changeOnResize.m_projection;
  m_changeEveryFrame.m_worldViewProjInv = m_changeEveryFrame.m_worldViewProj;
  m_changeEveryFrame.m_worldViewProjInv.invert();
  m_changeEveryFrame.m_viewPosition = camera->getPositionVector();

  g_graphicsApi().updateSubresource(m_neverChangeB, &m_neverChanges);
  g_graphicsApi().updateSubresource(m_changeOnResizeB, &m_changeOnResize);
  g_graphicsApi().updateSubresource(m_changeEveryFrameB, &m_changeEveryFrame);
  g_graphicsApi().updateSubresource(m_lightsB, &m_lights);
}

