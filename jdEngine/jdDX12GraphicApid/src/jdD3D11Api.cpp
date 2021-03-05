#include "jdD3D11Api.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <jdResourceManager.h>
#include <jdMesh.h>
#include <jdCompute.h>

#include "jdD3D11ProgramShader.h"
#include "jdD3D11GeometryShader.h"
#include "jdD3D11ConstantBuffer.h"
#include "jdD3D11Sampler.h"
#include "jdD3D11InputLayout.h"
#include "jdD3D11RasterizeState.h"
#include "DDSTextureLoader11.h"


namespace jdEngineSDK {

  bool 
  DirectX11Api::createDevice() {
				// define device creation flags,  D3D11_CREATE_DEVICE_BGRA_SUPPORT needed to get Direct2D interoperability with Direct3D resources
				unsigned int createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

				// if in debug mode, create device with debug layer
#ifndef NDEBUG
				createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

				// create the device
    Vector<D3D_FEATURE_LEVEL> featureLevels = { D3D_FEATURE_LEVEL_11_0,
                                                D3D_FEATURE_LEVEL_10_1,
                                                D3D_FEATURE_LEVEL_10_0};
    D3D_FEATURE_LEVEL featureLevel;

    HRESULT hr = D3D11CreateDevice(NULL, 
                                   D3D_DRIVER_TYPE_HARDWARE, 
                                   0, 
                                   createDeviceFlags, 
                                   &featureLevels[0],
                                   static_cast<UINT>(featureLevels.size()), 
                                   D3D11_SDK_VERSION, 
                                   &m_device.m_pd3dDevice,
                                   &featureLevel, 
                                   &m_deviceContext.m_pd3dDeviceContext);
    if (FAILED(hr))
    {
      return false;
    }
    g_Compute().init(nullptr, m_device.m_pd3dDevice, "cl_nv_d3d11_sharing");

    return true;
  }

  bool 
  DirectX11Api::createSwapChain(void* hwind,
                                const FORMAT::E& format, 
                                uint32 width, 
                                uint32 height) {

    SPtr<IDXGIDevice> dxgiDevice = 0;
    HRESULT hr = m_device.m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
    if (FAILED(hr))
    {
      return false;
    };
    
    SPtr<IDXGIAdapter>dxgiAdapter = 0;
    hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
    if (FAILED(hr))
    {
      return false;
    };

    
    SPtr<IDXGIFactory> dxgiFactory = 0;
    hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
    if (FAILED(hr))
    {
      return false;
    };


    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT(format);
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = reinterpret_cast<HWND>(hwind);
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = true;
    m_swapChainFormat = format;

    hr = dxgiFactory->CreateSwapChain(m_device.m_pd3dDevice,
                                      &sd, 
                                      &m_swapChain.m_pdxgSwapChain);
    if (FAILED(hr))
    {
      return false;
    };

    createDefaultTextures();
    createConstantBufferBones();
    return true;
  }

  void 
  DirectX11Api::resizeSwapChain(uint32 width, uint32 height) {
    m_deviceContext.m_pd3dDeviceContext->OMSetRenderTargets(0, 0, 0);

    // Release all outstanding references to the swap chain's buffers.
    SAFE_RELEASE(m_RTV.get()->m_pRT.m_pRenderTarget[0]);
    SAFE_RELEASE(m_RTV.get()->m_pRT.m_texture);
    SAFE_RELEASE(m_RTV.get()->m_pDepthStencil);

    HRESULT hr;
    // Preserve the existing buffer count and format.
    // Automatically choose the width and height to match the client rect for HWNDs.
    hr = m_swapChain.m_pdxgSwapChain->ResizeBuffers(0, 
                                                    width,
                                                    height,
                                                    (DXGI_FORMAT)m_swapChainFormat, 
                                                    DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

    ID3D11Texture2D* pBackBuffer = NULL;
    hr = m_swapChain.m_pdxgSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(hr))
      return;

    hr = m_device.m_pd3dDevice->CreateRenderTargetView(pBackBuffer, 
                                                       NULL, 
                                                       &m_RTV->m_pRT.m_pRenderTarget[0]);
    pBackBuffer->Release();
    if (FAILED(hr))
      return;

    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory(&descDepth, sizeof(descDepth));
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;

    hr = m_device.m_pd3dDevice->CreateTexture2D(&descDepth,
                                                NULL, 
                                                &m_RTV->m_pRT.m_texture);
    if (FAILED(hr))
      return;

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory(&descDSV, sizeof(descDSV));
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;

    hr = m_device.m_pd3dDevice->CreateDepthStencilView(m_RTV->m_pRT.m_texture,
                                                       &descDSV, 
                                                       &m_RTV->m_pDepthStencil);
    if (FAILED(hr))
      return;

    for (uint32 i = 0; i < m_renderTargets.size(); ++i)
    {
      D3D11RenderTarget* rtResize =
        reinterpret_cast<D3D11RenderTarget*>(m_renderTargets[i].get());
      bool depth = false;
      if (nullptr != rtResize->m_pRT.m_textureForDepthStencil)
      {
        depth = true;
      }
      rtResize->release();
      uint32 numRenderTargets = rtResize->m_pRT.m_pRenderTarget.size();
      rtResize->m_pRT.m_ppSRV.clear();
      rtResize->m_pRT.m_ppSRV.resize(numRenderTargets);
      for (uint32 j = 0; j < numRenderTargets; ++j)
      {
        CD3D11_TEXTURE2D_DESC textureDesc;
        CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
        CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
        
        ///////////////////////// Map's Texture
        // Initialize the  texture description.
        ZeroMemory(&textureDesc, sizeof(textureDesc));
        
        // Setup the texture description.
        // We will have our map be a square
        // We will need to have this texture bound as a render target AND a shader resource
        textureDesc.Width = (width * rtResize->m_scale);
        textureDesc.Height = (height * rtResize->m_scale);
        textureDesc.MipLevels = rtResize->m_mipLeve;
        textureDesc.ArraySize = 1;
        //textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
        
        // Create the texture
        m_device.m_pd3dDevice->CreateTexture2D(&textureDesc, NULL, &rtResize->m_pRT.m_texture);
        
        /////////////////////// Map's Render Target
        // Setup the description of the render target view.
        renderTargetViewDesc.Format = textureDesc.Format;
        renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        renderTargetViewDesc.Texture2D.MipSlice = 0;
        
        // Create the render target view.
        m_device.m_pd3dDevice->CreateRenderTargetView(rtResize->m_pRT.m_texture,
                                                      &renderTargetViewDesc, 
                                                      &rtResize->m_pRT.m_pRenderTarget[j]);
        
        /////////////////////// Map's Shader Resource View
        // Setup the description of the shader resource view.
        shaderResourceViewDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
        shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
        shaderResourceViewDesc.Texture2D.MipLevels = rtResize->m_mipLeve;
        
        
        // Create the shader resource view.
        m_device.m_pd3dDevice->CreateShaderResourceView(rtResize->m_pRT.m_texture,
                                                        &shaderResourceViewDesc, 
                                                        &rtResize->m_pRT.m_ppSRV[j]);
      }
      if (depth)
      {
        //CD3D11_TEXTURE2D_DESC descDepth;
        ZeroMemory(&descDepth, sizeof(descDepth));
        descDepth.Width = (width * rtResize->m_scale);
        descDepth.Height = (height * rtResize->m_scale);
        descDepth.MipLevels = rtResize->m_mipLeve;
        descDepth.ArraySize = 1;
        descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        descDepth.SampleDesc.Count = 1;
        descDepth.SampleDesc.Quality = 0;
        descDepth.Usage = D3D11_USAGE_DEFAULT;
        descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        descDepth.CPUAccessFlags = 0;
        descDepth.MiscFlags = 0;
      
        m_device.m_pd3dDevice->CreateTexture2D(&descDepth, 
                                               NULL, 
                                               &rtResize->m_pRT.m_textureForDepthStencil);
        //D3D11_DEPTH_STENCIL_VIEW_DESC descDSV2;
        //ZeroMemory(&descDSV2, sizeof(descDSV2));
        //descDSV2.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        //descDSV2.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        //descDSV2.Texture2D.MipSlice = 0;
        m_device.m_pd3dDevice->CreateDepthStencilView(rtResize->m_pRT.m_textureForDepthStencil,
                                                      &descDSV, 
                                                      &rtResize->m_pDepthStencil);
      }
    }

    // Set up the viewport.
    D3D11_VIEWPORT vp;
    vp.Width = m_viewPort.Width = (float)width;
    vp.Height = m_viewPort.Height = (float)height;
    vp.MinDepth = m_viewPort.MinDepth;
    vp.MaxDepth = m_viewPort.MaxDepth;
    vp.TopLeftX = m_viewPort.m_topLeftX;
    vp.TopLeftY = m_viewPort.m_topLeftY;
    m_deviceContext.m_pd3dDeviceContext->RSSetViewports(1, &vp);
  }

  bool 
  DirectX11Api::createRenderTargetView(uint32 width, uint32 height) {
    if (nullptr != m_RTV)
    {
      return false;
    }

    D3D11RenderTargetView* view = new D3D11RenderTargetView();
    m_RTV.reset(view);
    // Create a render target view

    ID3D11Texture2D* pBackBuffer = NULL;
    HRESULT hr = m_swapChain.m_pdxgSwapChain->GetBuffer(0, 
                                                        __uuidof(ID3D11Texture2D), 
                                                        (LPVOID*)&pBackBuffer);
    if (FAILED(hr))
      return false;
    m_RTV->m_pRT.m_pRenderTarget.resize(1);
    hr = m_device.m_pd3dDevice->CreateRenderTargetView(pBackBuffer, 
                                                       NULL, 
                                                       &m_RTV->m_pRT.m_pRenderTarget[0]);
    pBackBuffer->Release();
    if (FAILED(hr))
      return false;

    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory(&descDepth, sizeof(descDepth));
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;

    hr = m_device.m_pd3dDevice->CreateTexture2D(&descDepth,
                                                NULL, 
                                                &m_RTV->m_pRT.m_texture);
    if (FAILED(hr))
      return false;

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory(&descDSV, sizeof(descDSV));
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;

    hr = m_device.m_pd3dDevice->CreateDepthStencilView(m_RTV->m_pRT.m_texture,
                                                       &descDSV, 
                                                       &m_RTV->m_pDepthStencil);
    if (FAILED(hr))
      return false;

    m_deviceContext.m_pd3dDeviceContext->OMSetRenderTargets(1, 
                                                            &m_RTV->m_pRT.m_pRenderTarget[0],
                                                            m_RTV->m_pDepthStencil);

    return true;
  }

  SPtr<RenderTarget> 
  DirectX11Api::createRenderTarget(uint32 width, 
                                   uint32 height, 
                                   uint32 mipLevels, 
                                   bool Depth,
                                   uint32 numRenderTargets,
                                   float scale) {
    if (0 >= numRenderTargets)
    {
      numRenderTargets = 1;
    }
    D3D11RenderTarget* newRender = new D3D11RenderTarget;
    newRender->m_pRT.m_ppSRV.resize(numRenderTargets);
    newRender->m_pRT.m_pRenderTarget.resize(numRenderTargets);
    newRender->m_scale = scale;
    for (uint32 i = 0; i < numRenderTargets; ++i)
    {
      CD3D11_TEXTURE2D_DESC textureDesc;
      CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
      CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
      
      ///////////////////////// Map's Texture
      // Initialize the  texture description.
      ZeroMemory(&textureDesc, sizeof(textureDesc));
      
      // Setup the texture description.
      // We will have our map be a square
      // We will need to have this texture bound as a render target AND a shader resource
      textureDesc.Width = (uint32)(width * scale);
      textureDesc.Height = (uint32)(height * scale);
      textureDesc.MipLevels = mipLevels;
      textureDesc.ArraySize = 1;
      //textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
      textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
      textureDesc.SampleDesc.Count = 1;
      textureDesc.Usage = D3D11_USAGE_DEFAULT;
      textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
      textureDesc.CPUAccessFlags = 0;
      textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_SHARED;
      
      newRender->m_mipLeve = mipLevels;
      // Create the texture
      m_device.m_pd3dDevice->CreateTexture2D(&textureDesc, NULL, &newRender->m_pRT.m_texture);
      
      /////////////////////// Map's Render Target
      // Setup the description of the render target view.
      renderTargetViewDesc.Format = textureDesc.Format;
      renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
      renderTargetViewDesc.Texture2D.MipSlice = 0;
      
      // Create the render target view.
      m_device.m_pd3dDevice->CreateRenderTargetView(newRender->m_pRT.m_texture, 
                                                    &renderTargetViewDesc, 
                                                    &newRender->m_pRT.m_pRenderTarget[i]);
      
      /////////////////////// Map's Shader Resource View
      // Setup the description of the shader resource view.
      shaderResourceViewDesc.Format = textureDesc.Format;
      shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
      shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
      shaderResourceViewDesc.Texture2D.MipLevels = mipLevels;
      // Create the shader resource view.
      //ID3D11ShaderResourceView* srv;
      m_device.m_pd3dDevice->CreateShaderResourceView(newRender->m_pRT.m_texture, 
                                                      &shaderResourceViewDesc, 
                                                      &newRender->m_pRT.m_ppSRV[i]);
      
    }

    //SPtr<ID3D11ShaderResourceView> SRV(srv);
    //newRender->m_pRT.m_ppSRV.push_back(srv);
    if (Depth)
    {
      CD3D11_TEXTURE2D_DESC descDepth;
      ZeroMemory(&descDepth, sizeof(descDepth));
      descDepth.Width = width * scale;
      descDepth.Height = height * scale;
      descDepth.MipLevels = mipLevels;
      descDepth.ArraySize = 1;
      descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
      descDepth.SampleDesc.Count = 1;
      descDepth.SampleDesc.Quality = 0;
      descDepth.Usage = D3D11_USAGE_DEFAULT;
      descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
      descDepth.CPUAccessFlags = 0;
      descDepth.MiscFlags = 0;
      
      m_device.m_pd3dDevice->CreateTexture2D(&descDepth, 
                                             NULL, 
                                             &newRender->m_pRT.m_textureForDepthStencil);
      D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
      ZeroMemory(&descDSV, sizeof(descDSV));
      descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
      descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
      descDSV.Texture2D.MipSlice = 0;
      m_device.m_pd3dDevice->CreateDepthStencilView(newRender->m_pRT.m_textureForDepthStencil, 
                                                    &descDSV, 
                                                    &newRender->m_pDepthStencil);
    }

    SPtr<RenderTarget> newRT(newRender);
    m_renderTargets.push_back(newRT);
    return newRT;
  }

  void 
  DirectX11Api::generateMipMap(WeakSptr<RenderTarget> renderTarget) {
    D3D11RenderTarget* ret = reinterpret_cast<D3D11RenderTarget*>(renderTarget.lock().get());
    uint32 numRt = ret->m_pRT.m_ppSRV.size();
    for (uint32 i = 0; i < numRt; i++)
    {
      m_deviceContext.m_pd3dDeviceContext->GenerateMips(ret->m_pRT.m_ppSRV[i]);
    }
  }

  SPtr<ProgramShader>
  DirectX11Api::loadShaderFromFile(const char* vertexFilePath, 
                                   const char* vertexMainFuntion, 
                                   const char* vertexShaderVersion,
                                   const char* pixelFilePath, 
                                   const char* pixelMainFuntion, 
                                   const char* pixelShaderVersion,
                                   SPtr<SHADER_DEFINES> defines) {

    D3D11ProgramShader* newProg = new D3D11ProgramShader();
    SPtr<VertexShader> vs(loadVertexShaderFromFile(vertexFilePath,
                          vertexMainFuntion, 
                          vertexShaderVersion,
                          defines));
    SPtr<D3D11VertexShader> dvs(vs, reinterpret_cast<D3D11VertexShader*>(vs.get()));
    newProg->m_VS = dvs;

    SPtr<PixelShader> ps(loadPixelShaderFromFile(pixelFilePath,
                         pixelMainFuntion,
                         pixelShaderVersion, 
                         defines));
    SPtr<D3D11PixelShader> dps(ps, reinterpret_cast<D3D11PixelShader*>(ps.get()));
    newProg->m_PS = dps;
    SPtr<ProgramShader> np(newProg);
    return np;
  }

  SPtr<VertexShader> 
  DirectX11Api::loadVertexShaderFromFile(const char* vertexFilePath,
                                         const char* vertexMainFuntion,
                                         const char* shaderVersion,
                                         SPtr<SHADER_DEFINES> defines) {
    D3D11VertexShader* tmpS = new D3D11VertexShader();
    ID3D10Blob* pErroMsg = nullptr;//for messages errors
    // Create the shaders
    // Read the Vertex Shader code from the file
    if (tmpS->m_pVBlob != nullptr)
    {
      std::cout << "Se sobreescribira el VertexShader" << std::endl;
      SAFE_RELEASE(tmpS->m_pVBlob);
      SAFE_RELEASE(tmpS->m_pdxVS);
    }
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertexFilePath, std::ios::in);
    if (VertexShaderStream.is_open())
    {
      std::stringstream ss;
      ss << VertexShaderStream.rdbuf();
      VertexShaderCode = ss.str();
      //cout << VertexShaderCode << "\n";
      VertexShaderStream.close();
      std::string toConsole = "The vertex shader in the directory: ";
      toConsole += vertexFilePath;;
      toConsole += " was read";
      std::cout << toConsole << std::endl;
    }
    else {
      std::string toBox = "Couldn't find the Vertex shader in the directory: ";
      toBox += vertexFilePath;
      MessageBox(NULL, toBox.c_str(), "Error to find shader", MB_ICONERROR | MB_OK);
      std::cout << ("Impossible to open %s. Are you in the right directory ?");
      std::cout << ("Don't forget to read the FAQ !\n", vertexFilePath);
      return nullptr;
    }
    auto vertexShader = VertexShaderCode;
    Vector<D3D10_SHADER_MACRO> macros;
    if (defines != nullptr)
    {
      uint32 numObjects = defines->m_defines.size();
      macros.resize(numObjects+1);

      for (uint32 i = 0; i < numObjects; ++i)
      {
        macros[i].Definition = defines->m_defines[i].c_str();
        macros[i].Name = defines->m_defines[i].c_str();
      }
      macros[numObjects].Definition = nullptr;
      macros[numObjects].Name = nullptr;
    }
    HRESULT hr = D3DCompile(vertexShader.c_str(),
                            vertexShader.length(),
                            nullptr,
                            macros.data(),
                            nullptr,
                            vertexMainFuntion,
                            shaderVersion,
                            D3DCOMPILE_ENABLE_STRICTNESS,
                            0,
                            &tmpS->m_pVBlob,
                            &pErroMsg);
    size_t foundErrorVertex;
    if (NULL != pErroMsg)
    {
      SIZE_T msgSize = pErroMsg->GetBufferSize();
      std::string msg;
      msg.resize(msgSize);
      memcpy(&msg[0], pErroMsg->GetBufferPointer(), msgSize);
      std::string toBox = "Couldn't read the vertex shader for: \n" + msg;
      std::cout << toBox.c_str() << std::endl;


      OutputDebugStringA(msg.c_str());
      foundErrorVertex=msg.find("error");
      if (foundErrorVertex != std::string::npos)
      {
        MessageBox(NULL, toBox.c_str(), "Error to read shader", MB_ICONERROR | MB_OK);
        std::cout << "can't compile vertex shader\n" << std::endl;
        SAFE_RELEASE(pErroMsg);
        SAFE_RELEASE(tmpS->m_pVBlob);
        return nullptr;
      }
    }
    m_device.m_pd3dDevice->CreateVertexShader(tmpS->m_pVBlob->GetBufferPointer(),
                                              tmpS->m_pVBlob->GetBufferSize(),
                                              NULL,
                                              &tmpS->m_pdxVS);
    


    VertexShaderStream.close();
    SAFE_RELEASE(pErroMsg);
    SPtr<VertexShader> ns(tmpS);
    return ns;
  }

  SPtr<PixelShader> 
  DirectX11Api::loadPixelShaderFromFile(const char* pixelFilePath, 
                                        const char* pixelMainFuntion, 
                                        const char* shaderVersion,
                                        SPtr<SHADER_DEFINES> defines) {
    D3D11PixelShader* tmpS = new D3D11PixelShader();
    ID3D10Blob* pErroMsg = nullptr;//for messages errors

    if (tmpS->m_pPBlob != nullptr)
    {
      std::cout << "Se sobreescribira el PixelShader" << std::endl;
      SAFE_RELEASE(tmpS->m_pPBlob);
      SAFE_RELEASE(tmpS->m_pdxPS);
    }
    //Read PixelShader
    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(pixelFilePath, std::ios::in);
    if (FragmentShaderStream.is_open())
    {
      std::stringstream sstr;
      sstr << FragmentShaderStream.rdbuf();
      FragmentShaderCode = sstr.str();
      //cout << FragmentShaderCode << "\n";
      FragmentShaderStream.close();
      std::string toConsole = "The Pixel shader in the directory: ";
      toConsole += pixelFilePath;
      toConsole += " was read";
      std::cout << toConsole << std::endl;
    }
    else
    {
      std::string toBox = "Couldn't find the Pixel shader in the directory ";
      toBox += pixelFilePath;
      MessageBox(NULL, toBox.c_str(), "Error to find shader", MB_ICONERROR | MB_OK);
      std::cout << ("Impossible to open %s. Are you in the right directory ? ");
      std::cout << ("Don't forget to read the FAQ !\n", pixelFilePath);
      return nullptr;
    }
    auto FrameShader = FragmentShaderCode;
    Vector<D3D10_SHADER_MACRO> macros;
    if (defines != nullptr)
    {
      uint32 numObjects = defines->m_defines.size();
      macros.resize(numObjects + 1);

      for (uint32 i = 0; i < numObjects; ++i)
      {
        macros[i].Definition = defines->m_defines[i].c_str();
        macros[i].Name = defines->m_defines[i].c_str();
      }
      macros[numObjects].Definition = nullptr;
      macros[numObjects].Name = nullptr;
    }
    //Microsoft::WRL::ComPtr<ID3DBlob> blob;
    D3DCompile(FragmentShaderCode.c_str(),
               FragmentShaderCode.length(),
               nullptr,
               macros.data(),
               nullptr,
               pixelMainFuntion,
               shaderVersion,
               D3DCOMPILE_ENABLE_STRICTNESS,
               0,
               &tmpS->m_pPBlob,
               &pErroMsg);
    if (pErroMsg)
    {
      SIZE_T msgSize = pErroMsg->GetBufferSize();
      std::string msg;
      msg.resize(msgSize);
      memcpy(&msg[0], pErroMsg->GetBufferPointer(), msgSize);
      std::string toBox = "Couldn't read the pixel shader for: \n";
      toBox += msg;
      std::cout << toBox.c_str() << std::endl;
      OutputDebugStringA(msg.c_str());

      if (msg.find("error") != std::string::npos)
      {
        MessageBox(NULL, toBox.c_str(), "Error to read shader", MB_ICONERROR | MB_OK);
        std::cout << "can't compile pixel shader\n" << std::endl;
        SAFE_RELEASE(pErroMsg);
        SAFE_RELEASE(tmpS->m_pPBlob);
        return nullptr;
      }
    }
    m_device.m_pd3dDevice->CreatePixelShader(tmpS->m_pPBlob->GetBufferPointer(),
                                             tmpS->m_pPBlob->GetBufferSize(),
                                             NULL,
                                             &tmpS->m_pdxPS);
    

    FragmentShaderStream.close();
    SAFE_RELEASE(pErroMsg);

    SPtr<PixelShader> ns(tmpS);
    return ns;
  }

  SPtr<GeometryShader>
  DirectX11Api::loadGeometryShaderFromFile(const char* GeometryFilePath,
                                           const char* GeometryMainFuntion,
                                           const char* shaderVersion, 
                                           SPtr<SHADER_DEFINES> defines) {
    D3D11GeometryShader* tmpS = new D3D11GeometryShader();
    ID3D10Blob* pErroMsg = nullptr;//for messages errors

    if (tmpS->m_pPBlob != nullptr)
    {
      std::cout << "Se sobreescribira el GeometryShader" << std::endl;
      SAFE_RELEASE(tmpS->m_pPBlob);
      SAFE_RELEASE(tmpS->m_pdxGS);
    }
    //Read GeometryShader
    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(GeometryFilePath, std::ios::in);
    if (FragmentShaderStream.is_open())
    {
      std::stringstream sstr;
      sstr << FragmentShaderStream.rdbuf();
      FragmentShaderCode = sstr.str();
      //cout << FragmentShaderCode << "\n";
      FragmentShaderStream.close();
      std::string toConsole = "The Geometry shader in the directory: ";
      toConsole += GeometryFilePath;
      toConsole += " was read";
      std::cout << toConsole << std::endl;
    }
    else
    {
      std::string toBox = "Couldn't find the Geometry shader in the directory ";
      toBox += GeometryFilePath;
      MessageBox(NULL, toBox.c_str(), "Error to find shader", MB_ICONERROR | MB_OK);
      std::cout << ("Impossible to open %s. Are you in the right directory ? ");
      std::cout << ("Don't forget to read the FAQ !\n", GeometryFilePath);
      return nullptr;
    }
    auto FrameShader = FragmentShaderCode;
    Vector<D3D10_SHADER_MACRO> macros;
    if (defines != nullptr)
    {
      uint32 numObjects = defines->m_defines.size();
      macros.resize(numObjects + 1);

      for (uint32 i = 0; i < numObjects; ++i)
      {
        macros[i].Definition = defines->m_defines[i].c_str();
        macros[i].Name = defines->m_defines[i].c_str();
      }
      macros[numObjects].Definition = nullptr;
      macros[numObjects].Name = nullptr;
    }
    //Microsoft::WRL::ComPtr<ID3DBlob> blob;
    D3DCompile(FragmentShaderCode.c_str(),
               FragmentShaderCode.length(),
               nullptr,
               macros.data(),
               nullptr,
               GeometryMainFuntion,
               shaderVersion,
               D3DCOMPILE_ENABLE_STRICTNESS,
               0,
               &tmpS->m_pPBlob,
               &pErroMsg);
    if (pErroMsg)
    {
      SIZE_T msgSize = pErroMsg->GetBufferSize();
      std::string msg;
      msg.resize(msgSize);
      memcpy(&msg[0], pErroMsg->GetBufferPointer(), msgSize);
      std::string toBox = "Couldn't read the pixel shader for: \n";
      toBox += msg;
      std::cout << toBox.c_str() << std::endl;
      OutputDebugStringA(msg.c_str());

      if (msg.find("error") != std::string::npos)
      {
        MessageBox(NULL, toBox.c_str(), "Error to read shader", MB_ICONERROR | MB_OK);
        std::cout << "can't compile pixel shader\n" << std::endl;
        SAFE_RELEASE(pErroMsg);
        SAFE_RELEASE(tmpS->m_pPBlob);
        return nullptr;
      }
    }
    m_device.m_pd3dDevice->CreateGeometryShader(tmpS->m_pPBlob->GetBufferPointer(),
                                                tmpS->m_pPBlob->GetBufferSize(),
                                                NULL,
                                                &tmpS->m_pdxGS);
    

    FragmentShaderStream.close();
    SAFE_RELEASE(pErroMsg);

    SPtr<GeometryShader> ns(tmpS);
    return ns;
  }

  void
  DirectX11Api::setViewPort(const ViewPort& vp) {
    // Setup the viewport
    D3D11_VIEWPORT vpData;
    m_viewPort = vp;
    vpData.Width = (FLOAT)vp.Width;
    vpData.Height = (FLOAT)vp.Height;
    vpData.MinDepth = vp.MinDepth;
    vpData.MaxDepth = vp.MaxDepth;
    vpData.TopLeftX = vp.m_topLeftX;
    vpData.TopLeftY = vp.m_topLeftY;
    m_deviceContext.m_pd3dDeviceContext->RSSetViewports(1, &vpData);

  }

  SPtr<VertexBuffer>
  DirectX11Api::createVertexBuffer(int32 numvertex,
                                   uint32 structSize, 
                                   void* vertex) {
    D3D11VertexBuffer* tmpVB = new D3D11VertexBuffer();
    D3D11_BUFFER_DESC bd;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    //	m_device.bd.ByteWidth = sizeof(CDevice::SimpleVertex) * 12;
    bd.ByteWidth = structSize * numvertex;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertex;
    m_device.m_pd3dDevice->CreateBuffer(&bd, &InitData, &tmpVB->m_pVBuf);
    tmpVB->sizeOfStruct = structSize;
    return SPtr<VertexBuffer>(tmpVB);
  }

  SPtr<IndexBuffer>
  DirectX11Api::createIndexBuffer(int32 numindices, uint32* index) {
    D3D11IndexBuffer* tmpIB = new D3D11IndexBuffer;

    D3D11_BUFFER_DESC bd;
    D3D11_SUBRESOURCE_DATA InitData;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(uint32) * numindices;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = index;
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

    m_device.m_pd3dDevice->CreateBuffer(&bd, &InitData, &tmpIB->m_pIBuf);
    return SPtr<IndexBuffer>(tmpIB);
  }

  void 
  DirectX11Api::createDefaultTextures() {

    Vector<JDVector4> imgpxls;
    int32 imgSize = 128;
    int32 scale = 4;
    JDVector4 magenta = { 229, 9, 127,255 };
    JDVector4 White = { 255, 255, 255,255 };
    JDVector4 Black = { 0, 0, 0,255 };
    JDVector4 lightBlue = { 50, 50, 255,255 };
    JDVector4 Blue = { 150, 150, 255,255 };
    JDVector4 Grey = { 80, 80, 80,255 };
    JDVector4 lightGrey = { 180, 180, 180,255 };

    // Create texture
    CD3D11_TEXTURE2D_DESC desc;
    desc.Width = imgSize;
    desc.Height = imgSize;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA initData;
    initData.SysMemPitch = imgSize * 4;
    initData.SysMemSlicePitch = imgSize * imgSize * 4;

    CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;

    imgpxls.clear();
    imgpxls.resize(imgSize * imgSize);
    for (int32 y = 0; y < imgSize; ++y)
    {
      for (int32 x = 0; x < imgSize; ++x)
      {
        float chessboard = float(x / scale + y / scale);
        imgpxls[y * imgSize + x] = chessboard;
      }
    }

    D3D11Texture2D* texdef = new D3D11Texture2D;
    initData.pSysMem = imgpxls.data();
    m_device.m_pd3dDevice->CreateTexture2D(&desc, &initData, &texdef->m_texture);
    texdef->m_ppSRV.resize(1);
    m_device.m_pd3dDevice->CreateShaderResourceView(texdef->m_texture, 
                                                    &srvDesc, 
                                                    &texdef->m_ppSRV[0]);
    DEFAULT_TEXTURE_ERROR.reset(texdef);

    imgpxls.clear();
    imgpxls.resize(imgSize * imgSize);
    for (int32 y = 0; y < imgSize; ++y)
    {
      for (int32 x = 0; x < imgSize; ++x)
      {
        int32 chessboard = x / scale + y / scale;
        imgpxls[y * imgSize + x] = chessboard % 2 ? White : Black;
      }
    }
    D3D11Texture2D* texchess = new D3D11Texture2D;
    initData.pSysMem = imgpxls.data();
    m_device.m_pd3dDevice->CreateTexture2D(&desc, &initData, &texchess->m_texture);
    texchess->m_ppSRV.resize(1);
    m_device.m_pd3dDevice->CreateShaderResourceView(texchess->m_texture, 
                                                    &srvDesc, 
                                                    &texchess->m_ppSRV[0]);
    DEFAULT_TEXTURE_CHESS.reset(texchess);

    imgpxls.clear();
    imgpxls.resize(imgSize * imgSize);
    for (int32 y = 0; y < imgSize; ++y)
    {
      for (int32 x = 0; x < imgSize; ++x)
      {
        //int32 chessboard = x / scale + y / scale;
        imgpxls[y * imgSize + x] = White;
      }
    }
    D3D11Texture2D* texwhite = new D3D11Texture2D;
    initData.pSysMem = imgpxls.data();
    m_device.m_pd3dDevice->CreateTexture2D(&desc, &initData, &texwhite->m_texture);
    texwhite->m_ppSRV.resize(1);
    m_device.m_pd3dDevice->CreateShaderResourceView(texwhite->m_texture, 
                                                    &srvDesc, 
                                                    &texwhite->m_ppSRV[0]);
    DEFAULT_TEXTURE_WHITE.reset(texwhite);

    imgpxls.clear();
    imgpxls.resize(imgSize * imgSize);
    for (int32 y = 0; y < imgSize; ++y)
    {
      for (int32 x = 0; x < imgSize; ++x)
      {
        //int32 chessboard = x / scale + y / scale;
        imgpxls[y * imgSize + x] = Black;
      }
    }
    D3D11Texture2D* texblack = new D3D11Texture2D;
    initData.pSysMem = imgpxls.data();
    m_device.m_pd3dDevice->CreateTexture2D(&desc, &initData, &texblack->m_texture);
    texblack->m_ppSRV.resize(1);
    m_device.m_pd3dDevice->CreateShaderResourceView(texblack->m_texture, 
                                                    &srvDesc, 
                                                    &texblack->m_ppSRV[0]);
    DEFAULT_TEXTURE_BLACK.reset(texblack);

    imgpxls.clear();
    imgpxls.resize(imgSize * imgSize);
    for (int32 y = 0; y < imgSize; ++y)
    {
      for (int32 x = 0; x < imgSize; ++x)
      {
        int32 chessboard = x / scale + y / scale;
        imgpxls[y * imgSize + x] = chessboard % 2 ? Blue : lightBlue;
      }
    }
    D3D11Texture2D* texnormal = new D3D11Texture2D;
    initData.pSysMem = imgpxls.data();
    m_device.m_pd3dDevice->CreateTexture2D(&desc, &initData, &texnormal->m_texture);
    texnormal->m_ppSRV.resize(1);
    m_device.m_pd3dDevice->CreateShaderResourceView(texnormal->m_texture, 
                                                    &srvDesc, 
                                                    &texnormal->m_ppSRV[0]);
    DEFAULT_TEXTURE_NORMAL.reset(texnormal);

    imgpxls.clear();
    imgpxls.resize(imgSize * imgSize);
    for (int32 y = 0; y < imgSize; ++y)
    {
      for (int32 x = 0; x < imgSize; ++x)
      {
        int32 chessboard = x / scale + y / scale;
        imgpxls[y * imgSize + x] = chessboard % 2 ? Grey : lightGrey;
      }
    }
    D3D11Texture2D* textrans = new D3D11Texture2D;
    initData.pSysMem = imgpxls.data();
    m_device.m_pd3dDevice->CreateTexture2D(&desc, &initData, &textrans->m_texture);
    textrans->m_ppSRV.resize(1);
    m_device.m_pd3dDevice->CreateShaderResourceView(textrans->m_texture, 
                                                    &srvDesc, 
                                                    &textrans->m_ppSRV[0]);
    DEFAULT_TEXTURE_TRANSPARENT.reset(textrans);
  }

  SPtr<ConstantBuffer>
  DirectX11Api::CreateConstantBuffer(uint32 sizeOfStruct) {
    D3D11ConstantBuffer* tmpB = new D3D11ConstantBuffer;

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.ByteWidth = sizeOfStruct;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;

    m_device.m_pd3dDevice->CreateBuffer(&bd, NULL, &tmpB->m_pBuffer);
    return SPtr<ConstantBuffer>(tmpB);
  }

  SPtr<Sampler>
  DirectX11Api::CreateSampleLinearState() {
    D3D11Sampler* tmpS = new D3D11Sampler;

    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    sampDesc.MipLODBias = 0;
    m_device.m_pd3dDevice->CreateSamplerState(&sampDesc, &tmpS->m_pSampler);
    return SPtr<Sampler>(tmpS);
  }

  SPtr<InputLayout> 
  DirectX11Api::CreateInputLayout(WeakSptr<VertexShader> vs, 
                                  INPUT_LAYOUT_DESC* elementsInput, 
                                  uint32 numElements)
  {
    D3D11VertexShader* tmpS = reinterpret_cast<D3D11VertexShader*>(vs.lock().get());
    D3D11InputLayout* input = new D3D11InputLayout;
    Vector<D3D11_INPUT_ELEMENT_DESC> layout;
    layout.resize(numElements);

    for (uint32 i = 0; i < numElements; i++)
    {
      layout[i].SemanticName = elementsInput[i].semanicName.c_str();
      layout[i].SemanticIndex = elementsInput[i].SemanticIndex;
      layout[i].Format = (DXGI_FORMAT)elementsInput[i].format;
      layout[i].InputSlot = elementsInput[i].InputSlot;
      layout[i].AlignedByteOffset = elementsInput[i].AlignedByteOffset;
      layout[i].InputSlotClass = (D3D11_INPUT_CLASSIFICATION)elementsInput[i].InputSlotClass;
      layout[i].InstanceDataStepRate = elementsInput[i].InstanceDataStepRate;
    }

    HRESULT hr = m_device.m_pd3dDevice->CreateInputLayout(&layout[0], 
                                                          numElements, 
                                                          tmpS->m_pVBlob->GetBufferPointer(),
                                                          tmpS->m_pVBlob->GetBufferSize(), 
                                                          &input->m_pdxInputLayout);
    if (FAILED(hr))
    {
      std::cout << "can't create InputLayout\n" << std::endl;
    }
    return SPtr<InputLayout>(input);
  }

  SPtr<InputLayout>
  DirectX11Api::reflectInputLayout(WeakSptr<VertexShader> vs) {
    D3D11VertexShader* tmpS = reinterpret_cast<D3D11VertexShader*>(vs.lock().get());
    D3D11InputLayout* input = new D3D11InputLayout;
    HRESULT hr=CreateInputLayoutDescFromVertexShaderSignature(tmpS->m_pVBlob,
                                                              m_device.m_pd3dDevice,
                                                              &input->m_pdxInputLayout);

    if(FAILED(hr))
    {
      std::cout << "can't create InputLayout\n" << std::endl;
    }
    return SPtr<InputLayout>(input);
  }

  SPtr<Texture2D> 
  DirectX11Api::LoadShaderResourceFromFile(const char* filePath, bool isDDS) {
    
    D3D11Texture2D* tex = new D3D11Texture2D;
    CD3D11_TEXTURE2D_DESC desc;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    
    std::vector<wchar_t> vec;
    size_t len = strlen(filePath);
    vec.resize(len + 1);
    mbstowcs(&vec[0], filePath, len);

    const wchar_t* fileName = &vec[0];
    if (isDDS)
    {
      desc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
      tex->m_ppSRV.resize(6);
      //DirectX::CreateDDSTextureFromFile(m_device.m_pd3dDevice, fileName, tex->m_Rtexture, tex->m_psrv);
      HRESULT hr = DirectX::CreateDDSTextureFromFileEx(m_device.m_pd3dDevice,
        fileName,
        (size_t)0,
        D3D11_USAGE_DEFAULT,
        desc.BindFlags,
        desc.CPUAccessFlags,
        D3D11_RESOURCE_MISC_TEXTURECUBE,
        false,
        (ID3D11Resource**)&tex->m_texture,
        tex->m_ppSRV.data());
      if (SUCCEEDED(hr))
      {
        return SPtr<Texture2D>(tex);
      }
      else
      {
        return nullptr;
      }
    }

    int width, height, channels;
    unsigned char* img = stbi_load(filePath, &width, &height, &channels, 4);
    if (img == NULL) {
      printf("Error in loading the image\n");
      return nullptr;
    }
    // Create texture
    desc.Width = width;
    desc.Height = height;
    

    if (isDDS) {
      desc.MiscFlags = desc.MiscFlags | D3D11_RESOURCE_MISC_TEXTURECUBE;
      desc.ArraySize = 6;
    }
    //else {
    //  desc.MiscFlags = desc.MiscFlags & ~static_cast<uint32>(D3D11_RESOURCE_MISC_TEXTURECUBE);
    //}

    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = img;
    initData.SysMemPitch = width * 4;
    initData.SysMemSlicePitch = width * height * 4;

    HRESULT hr = m_device.m_pd3dDevice->CreateTexture2D(&desc, &initData, &tex->m_texture);
    if (FAILED(hr))
    {
      //coudn't create texture
      return nullptr;
    }
    CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    if (isDDS)
    {
      srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
      srvDesc.TextureCube.MipLevels = desc.MipLevels;
      srvDesc.TextureCubeArray.NumCubes = 1;
    }
    else
    {
      srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    }
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    tex->m_ppSRV.resize(desc.MipLevels);
    hr = m_device.m_pd3dDevice->CreateShaderResourceView(tex->m_texture, &srvDesc, &tex->m_ppSRV[0]);

    if (FAILED(hr))
    {
      //coudn't create texture
      return nullptr;
    }

    return SPtr<Texture2D>(tex);
  }

  void
  DirectX11Api::setRenderTarget(WeakSptr<RenderTarget> rt) {
    D3D11RenderTarget* ret = reinterpret_cast<D3D11RenderTarget*>(rt.lock().get());

    if (nullptr==ret->m_pDepthStencil)
    {
      m_deviceContext.m_pd3dDeviceContext->OMSetRenderTargets(ret->m_pRT.m_pRenderTarget.size(),
                                                              &ret->m_pRT.m_pRenderTarget[0],
                                                              NULL);
    }
    else
    {
      m_deviceContext.m_pd3dDeviceContext->OMSetRenderTargets(ret->m_pRT.m_pRenderTarget.size(),
                                                              &ret->m_pRT.m_pRenderTarget[0],
                                                              ret->m_pDepthStencil);
    }
  }

  void 
  DirectX11Api::Clear(WeakSptr<RenderTarget> rt,
                      const float& r, 
                      const float& g, 
                      const float& b, 
                      const float& a) {
    float color[4] = { r,g,b,a };
    D3D11RenderTarget* tmpRT = reinterpret_cast<D3D11RenderTarget*>(rt.lock().get());
    uint32 numOfTargets = tmpRT->m_pRT.m_pRenderTarget.size();
    for (uint32 i = 0; i < numOfTargets; ++i)
    {
      m_deviceContext.m_pd3dDeviceContext->ClearRenderTargetView(tmpRT->m_pRT.m_pRenderTarget[i],
                                                                 color);
    }
  }

  void 
  DirectX11Api::ClearDepthStencil(WeakSptr<RenderTarget> rt) {
    D3D11RenderTarget* tmpS = reinterpret_cast<D3D11RenderTarget*>(rt.lock().get());
    if (nullptr != tmpS->m_pDepthStencil)
    {
      m_deviceContext.m_pd3dDeviceContext->ClearDepthStencilView(tmpS->m_pDepthStencil,
                                                                 D3D11_CLEAR_DEPTH, 
                                                                 1.0f, 
                                                                 0);
    }
  }

  void 
  DirectX11Api::setVertexBuffer(WeakSptr<VertexBuffer> vertexB) {
    D3D11VertexBuffer* b = reinterpret_cast<D3D11VertexBuffer*>(vertexB.lock().get());
    UINT stride = b->sizeOfStruct;
    UINT offset = 0;
    m_deviceContext.m_pd3dDeviceContext->IASetVertexBuffers(0,
                                                            1,
                                                            &b->m_pVBuf,
                                                            &stride,
                                                            &offset);
  }

  void 
  DirectX11Api::setIndexBuffer(WeakSptr<IndexBuffer> indexB) {
    D3D11IndexBuffer* b = reinterpret_cast<D3D11IndexBuffer*>(indexB.lock().get());
    m_deviceContext.m_pd3dDeviceContext->IASetIndexBuffer(b->m_pIBuf,
                                                          DXGI_FORMAT_R32_UINT,
                                                          0);
  }

  void DirectX11Api::setPrimitiveTopology(const PRIMITIVE_TOPOLOGY_FORMAT::E& format)
  {
    switch (format)
    {
    case PRIMITIVE_TOPOLOGY_FORMAT::UNDEFINED:
      m_deviceContext.m_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED);
      break;
    case PRIMITIVE_TOPOLOGY_FORMAT::POINTLIST:
      m_deviceContext.m_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
      break;
    case PRIMITIVE_TOPOLOGY_FORMAT::LINELIST:
      m_deviceContext.m_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
      break;
    case PRIMITIVE_TOPOLOGY_FORMAT::LINESTRIP:
      m_deviceContext.m_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
      break;
    case PRIMITIVE_TOPOLOGY_FORMAT::TRIANGLELIST:
      m_deviceContext.m_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      break;
    case PRIMITIVE_TOPOLOGY_FORMAT::TRIANGLESTRIP:
      m_deviceContext.m_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
      break;
    default:
      break;  
    }
  }

  void 
  DirectX11Api::updateSubresource(WeakSptr<ConstantBuffer> buffer, void* data) {
    D3D11ConstantBuffer* b = reinterpret_cast<D3D11ConstantBuffer*>(buffer.lock().get());
    m_deviceContext.m_pd3dDeviceContext->UpdateSubresource(b->m_pBuffer, 
                                                           0, 
                                                           NULL, 
                                                           data, 
                                                           0, 
                                                           0);
  }

  void 
  DirectX11Api::setProgramShader(WeakSptr<ProgramShader> programS) {
    D3D11ProgramShader* tmpPS = reinterpret_cast<D3D11ProgramShader*>(programS.lock().get());
    setVertexShader(tmpPS->m_VS);
    setPixelShader(tmpPS->m_PS);
  }

  void 
  DirectX11Api::setVertexShader(WeakSptr<VertexShader> vertexS) {
    D3D11VertexShader* tmpS = reinterpret_cast<D3D11VertexShader*>(vertexS.lock().get());
    m_deviceContext.m_pd3dDeviceContext->VSSetShader(tmpS->m_pdxVS, NULL, 0);
  }

  void DirectX11Api::setPixelShader(WeakSptr<PixelShader> pixelS) {
    D3D11PixelShader* tmpS = reinterpret_cast<D3D11PixelShader*>(pixelS.lock().get());
    m_deviceContext.m_pd3dDeviceContext->PSSetShader(tmpS->m_pdxPS, NULL, 0);
  }

  void 
  DirectX11Api::setGeometryShader(WeakSptr<GeometryShader> geometryS) {
    D3D11GeometryShader* tmpS = reinterpret_cast<D3D11GeometryShader*>(geometryS.lock().get());
    if (nullptr == tmpS)
    {
      m_deviceContext.m_pd3dDeviceContext->GSSetShader(NULL, NULL, 0);
      return;
    }
    m_deviceContext.m_pd3dDeviceContext->GSSetShader(tmpS->m_pdxGS, NULL, 0);
  }

  void 
  DirectX11Api::removeGeometryShader() {
    m_deviceContext.m_pd3dDeviceContext->GSSetShader(NULL, NULL, 0);
  }

  void 
  DirectX11Api::setInputLayout(WeakSptr<InputLayout> input) {
    D3D11InputLayout* in = reinterpret_cast<D3D11InputLayout*>(input.lock().get());
    m_deviceContext.m_pd3dDeviceContext->IASetInputLayout(in->m_pdxInputLayout);
  }

  void 
  DirectX11Api::setConstanBuffer(SPtr<ConstantBuffer> buffer,
                                 int32 bufferSlot, 
                                 uint32 numBuffers) {
    VertexShaderSetConstanBuffer(buffer, bufferSlot, numBuffers);
    PixelShaderSetConstanBuffer(buffer, bufferSlot, numBuffers);
  }

  void 
  DirectX11Api::VertexShaderSetConstanBuffer(WeakSptr<ConstantBuffer> buffer,
                                             int32 bufferSlot, 
                                             uint32 numBuffers) {
    D3D11ConstantBuffer* b = reinterpret_cast<D3D11ConstantBuffer*>(buffer.lock().get());
    m_deviceContext.m_pd3dDeviceContext->VSSetConstantBuffers((UINT)bufferSlot, 
                                                              (UINT)numBuffers, 
                                                              &b->m_pBuffer);
  }

  void 
  DirectX11Api::PixelShaderSetConstanBuffer(WeakSptr<ConstantBuffer> buffer,
                                            int32 bufferSlot, 
                                            uint32 numBuffers) {
    D3D11ConstantBuffer* b = reinterpret_cast<D3D11ConstantBuffer*>(buffer.lock().get());
    m_deviceContext.m_pd3dDeviceContext->PSSetConstantBuffers((UINT)bufferSlot, 
                                                              (UINT)numBuffers, 
                                                              &b->m_pBuffer);
  }


  void 
  DirectX11Api::GeometryShaderSetConstanBuffer(WeakSptr<ConstantBuffer> buffer, 
                                               int32 bufferSlot, 
                                               uint32 numBuffers) {
    D3D11ConstantBuffer* b = reinterpret_cast<D3D11ConstantBuffer*>(buffer.lock().get());
    m_deviceContext.m_pd3dDeviceContext->GSSetConstantBuffers((UINT)bufferSlot, 
                                                              (UINT)numBuffers, 
                                                              &b->m_pBuffer);
  }

  void
  DirectX11Api::SetBonesConstanBuffer() {
    VertexShaderSetConstanBuffer(m_bonesBuffer,3);
    PixelShaderSetConstanBuffer(m_bonesBuffer, 3);
  }

  void 
  DirectX11Api::setShaderResources(WeakSptr<Texture2D> resource,
                                   int32 ResourceSlot, 
                                   uint32 numresources) {
    SPtr<Texture2D> tmpR=resource.lock();
    VertexShaderSetShaderResources(tmpR, ResourceSlot, numresources);
    PixelShaderSetShaderResources(tmpR, ResourceSlot, numresources);
  }

  void 
  DirectX11Api::VertexShaderSetShaderResources(WeakSptr<Texture2D> resource,
                                               int32 ResourceSlot, 
                                               uint32 numresources) {

    D3D11Texture2D* t = reinterpret_cast<D3D11Texture2D*>(resource.lock().get());
    m_deviceContext.m_pd3dDeviceContext->VSSetShaderResources(ResourceSlot, 
                                                              numresources, 
                                                              t->m_ppSRV.data());
  }

  void 
  DirectX11Api::PixelShaderSetShaderResourcesFromRT(WeakSptr<RenderTarget> resouce,
                                                    int32 ResourceSlot,
                                                    uint32 ResourceRTIndex,
                                                    uint32 numresources) {
    D3D11RenderTarget* t = reinterpret_cast<D3D11RenderTarget*>(resouce.lock().get());
    m_deviceContext.m_pd3dDeviceContext->PSSetShaderResources(ResourceSlot, 
                                                              numresources, 
                                                              &t->m_pRT.m_ppSRV[ResourceRTIndex]);
  }

  void
  DirectX11Api::PixelShaderSetShaderResources(WeakSptr<Texture2D> resource,
                                              int32 ResourceSlot, 
                                              uint32 numresources) {
    D3D11Texture2D* t = reinterpret_cast<D3D11Texture2D*>(resource.lock().get());
    if (NULL != t)
    {
      m_deviceContext.m_pd3dDeviceContext->PSSetShaderResources(ResourceSlot,
                                                                numresources,
                                                                t->m_ppSRV.data());
    }
  }

  void 
  DirectX11Api::setSampler(WeakSptr<Sampler> sampler, int32 samplerSlot, uint32 numSamplers) {
    SPtr<Sampler> tmpS = sampler.lock();
    VertexShaderSetSampler(tmpS, samplerSlot, numSamplers);
    PixelShaderSetSampler(tmpS, samplerSlot, numSamplers);
  }

  void 
  DirectX11Api::VertexShaderSetSampler(WeakSptr<Sampler> sampler,
                                       int32 samplerSlot, 
                                       uint32 numSamplers) {
    D3D11Sampler* s = reinterpret_cast<D3D11Sampler*>(sampler.lock().get());
    m_deviceContext.m_pd3dDeviceContext->VSSetSamplers(samplerSlot, 
                                          numSamplers, 
                                          &s->m_pSampler);
  }

  void 
  DirectX11Api::PixelShaderSetSampler(WeakSptr<Sampler> sampler,
                                      int32 samplerSlot, 
                                      uint32 numSamplers) {
    D3D11Sampler* s = reinterpret_cast<D3D11Sampler*>(sampler.lock().get());
    m_deviceContext.m_pd3dDeviceContext->PSSetSamplers(samplerSlot, 
                                          numSamplers, 
                                          &s->m_pSampler);
  }

  void 
  DirectX11Api::DrawIndex(uint32 numIndex) {
    m_deviceContext.m_pd3dDeviceContext->DrawIndexed(numIndex, 0, 0);
    //d3d11DevCon->DrawInstanced(1, numBillboardTreesToDraw, 0, 0);
  }

  void 
  DirectX11Api::DrawInstanced(uint32 VertexCountPerInstance, 
                              uint32 InstanceCount, 
                              uint32 StartVertexLocation, 
                              uint32 StartInstanceLocation) {
    //m_deviceContext.m_pd3dDeviceContext->DrawInstanced(VertexCountPerInstance, 
    //                                                   InstanceCount, 
    //                                                   StartVertexLocation, 
    //                                                   StartInstanceLocation);
    m_deviceContext.m_pd3dDeviceContext->Draw(1,0);
  }

  void
  DirectX11Api::Present() {
    m_swapChain.m_pdxgSwapChain->Present(0,0);
  }

  SPtr<ConstantBuffer> 
  DirectX11Api::createConstantBufferBones() {
    if (nullptr != m_bonesBuffer)
    {
      return m_bonesBuffer;
    }
    m_bonesBuffer = CreateConstantBuffer(sizeof(cbBonesTranform));
    return m_bonesBuffer;
  }

  SPtr<RasterizeState> DirectX11Api::createRasterizeState(RASTERIZER_FILL_MODE::E FillMode, 
                                                          RASTERIZER_CULL_MODE::E CullMode, 
                                                          bool FrontCounterClockwise, 
                                                          int DepthBias, 
                                                          float DepthBiasClamp, 
                                                          float SlopeScaledDepthBias, 
                                                          bool DepthClipEnable, 
                                                          bool ScissorEnable, 
                                                          bool MultisampleEnable, 
                                                          bool AntialiasedLineEnable) {
    CD3D11_RASTERIZER_DESC RSDesc;
    RSDesc.FillMode = D3D11_FILL_MODE(FillMode);
    RSDesc.CullMode = D3D11_CULL_MODE(CullMode);
    RSDesc.FrontCounterClockwise = FrontCounterClockwise;
    RSDesc.DepthBias = DepthBias;
    RSDesc.DepthBiasClamp = DepthBiasClamp;
    RSDesc.SlopeScaledDepthBias = SlopeScaledDepthBias;
    RSDesc.DepthClipEnable = DepthClipEnable;
    RSDesc.ScissorEnable = ScissorEnable;
    RSDesc.MultisampleEnable = MultisampleEnable;
    RSDesc.AntialiasedLineEnable = AntialiasedLineEnable;
    D3D11RasterizeState* newRS = new D3D11RasterizeState();
    HRESULT hr = m_device.m_pd3dDevice->CreateRasterizerState(&RSDesc, &newRS->m_pRasterizeState);
    if (FAILED(hr)) {
      return nullptr;
    }
    SPtr<RasterizeState> sptrRS(newRS);
    return sptrRS;
  }

  void 
  DirectX11Api::setRasterizeState(WeakSptr<RasterizeState> RS) {
    D3D11RasterizeState* pRS = reinterpret_cast<D3D11RasterizeState*>(RS.lock().get());
    m_deviceContext.m_pd3dDeviceContext->RSSetState(pRS->m_pRasterizeState);
  }

  void
  DirectX11Api::onStartUp() {
    createDevice();
  }

  void 
  DirectX11Api::onShutDown() {
    release();
    //for (int32 i = 0; i < m_vRenderTargets.size(); i++)
    //{
    //  m_vRenderTargets[i].release();
    //}
  }

  HRESULT 
  DirectX11Api::CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, 
                                                               ID3D11Device* pD3DDevice, 
                                                               ID3D11InputLayout** pInputLayout) {
    // Reflect shader info
    ID3D11ShaderReflection* pVertexShaderReflection = NULL;
    if (FAILED(D3DReflect(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
    {
      return S_FALSE;
    }

    // Get shader info
    D3D11_SHADER_DESC shaderDesc;
    pVertexShaderReflection->GetDesc(&shaderDesc);

    // Read input layout description from shader info
    std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
    for (uint32 i = 0; i < shaderDesc.InputParameters; i++)
    {
      D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
      pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

      // fill out input element desc
      D3D11_INPUT_ELEMENT_DESC elementDesc;
      elementDesc.SemanticName = paramDesc.SemanticName;
      elementDesc.SemanticIndex = paramDesc.SemanticIndex;
      elementDesc.InputSlot = 0;
      elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
      elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
      elementDesc.InstanceDataStepRate = 0;

      // determine DXGI format
      if (paramDesc.Mask == 1)
      {
        if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
        else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
        else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
      }
      else if (paramDesc.Mask <= 3)
      {
        if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
        else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
        else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
      }
      else if (paramDesc.Mask <= 7)
      {
        if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
        else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
        else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
      }
      else if (paramDesc.Mask <= 15)
      {
        if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
        else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
        else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
      }

      //save element desc
      inputLayoutDesc.push_back(elementDesc);
    }

    // Try to create Input Layout
    HRESULT hr = pD3DDevice->CreateInputLayout(&inputLayoutDesc[0], 
                                               (UINT)inputLayoutDesc.size(), 
                                               pShaderBlob->GetBufferPointer(), 
                                               pShaderBlob->GetBufferSize(), 
                                               pInputLayout);

    //Free allocation shader reflection memory
    pVertexShaderReflection->Release();
    return hr;
  }

  void 
  DirectX11Api::release() {
    SAFE_RELEASE(m_device.m_pd3dDevice);
    SAFE_RELEASE(m_deviceContext.m_pd3dDeviceContext);
    SAFE_RELEASE(m_swapChain.m_pdxgSwapChain);
    DEFAULT_TEXTURE_ERROR->release();
    DEFAULT_TEXTURE_TRANSPARENT->release();
    DEFAULT_TEXTURE_BLACK->release();
    DEFAULT_TEXTURE_WHITE->release();
    DEFAULT_TEXTURE_NORMAL->release();
    DEFAULT_TEXTURE_CHESS->release();
    m_RTV->release();
    for (uint32 i = 0; i < m_renderTargets.size(); i++)
    {
      m_renderTargets[i]->release();
    }
  }

}
