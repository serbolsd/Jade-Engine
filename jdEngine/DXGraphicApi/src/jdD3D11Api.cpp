#include "jdD3D11Api.h"

#include <SFML/Window.hpp>

#include "jdD3D11ProgramShader.h"
#include "jdD3D11ConstantBuffer.h"
#include "jdD3D11Sampler.h"
#include "jdD3D11InputLayout.h"


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

    return true;
  }

  void 
  DirectX11Api::resizeSwapChain(uint32 width, uint32 height) {
    m_deviceContext.m_pd3dDeviceContext->OMSetRenderTargets(0, 0, 0);

    // Release all outstanding references to the swap chain's buffers.
    SAFE_RELEASE(m_RTV.get()->m_pRT.m_pRenderTarget);
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

    hr = m_device.m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_RTV->m_pRT.m_pRenderTarget);
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
    HRESULT hr = m_swapChain.m_pdxgSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (FAILED(hr))
      return false;

    hr = m_device.m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_RTV->m_pRT.m_pRenderTarget);
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
                                                            &m_RTV->m_pRT.m_pRenderTarget,
                                                            m_RTV->m_pDepthStencil);
    return true;
  }

  SPtr<ProgramShader>
  DirectX11Api::loadShaderFromFile(const char* vertexFilePath, 
                                   const char* vertexMainFuntion, 
                                   const char* vertexShaderVersion,
                                   const char* pixelFilePath, 
                                   const char* pixelMainFuntion, 
                                   const char* pixelShaderVersion) {

    D3D11ProgramShader* newProg = new D3D11ProgramShader();
    SPtr<VertexShader> vs(loadVertexShaderFromFile(vertexFilePath,
                          vertexMainFuntion, 
                          vertexShaderVersion));
    SPtr<D3D11VertexShader> dvs(vs, reinterpret_cast<D3D11VertexShader*>(vs.get()));
    newProg->m_VS = dvs;

    SPtr<PixelShader> ps(loadPixelShaderFromFile(pixelFilePath,
                         pixelMainFuntion,
                         pixelShaderVersion));
    SPtr<D3D11PixelShader> dps(ps, reinterpret_cast<D3D11PixelShader*>(ps.get()));
    newProg->m_PS = dps;
    SPtr<ProgramShader> np(newProg);
    return np;
  }

  SPtr<VertexShader> 
  DirectX11Api::loadVertexShaderFromFile(const char* vertexFilePath,
                                         const char* vertexMainFuntion,
                                         const char* shaderVersion) {
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

    D3DCompile(vertexShader.c_str(),
      vertexShader.length(),
      nullptr,
      nullptr,
      nullptr,
      vertexMainFuntion,
      shaderVersion,
      D3DCOMPILE_ENABLE_STRICTNESS,
      0,
      &tmpS->m_pVBlob,
      &pErroMsg);

    if (pErroMsg)
    {
      SIZE_T msgSize = pErroMsg->GetBufferSize();
      std::string msg;
      msg.resize(msgSize);
      memcpy(&msg[0], pErroMsg->GetBufferPointer(), msgSize);
      std::string toBox = "Couldn't read the vertex shader for: \n" + msg;
      MessageBox(NULL, toBox.c_str(), "Error to read shader", MB_ICONERROR | MB_OK);
      std::cout << toBox.c_str() << std::endl;

      SAFE_RELEASE(pErroMsg);
      SAFE_RELEASE(tmpS->m_pVBlob);

      OutputDebugStringA(msg.c_str());
      std::cout << "can't compile vertex shader\n" << std::endl;
      return nullptr;
    }
    else
    {
      m_device.m_pd3dDevice->CreateVertexShader(tmpS->m_pVBlob->GetBufferPointer(),
        tmpS->m_pVBlob->GetBufferSize(),
        NULL,
        &tmpS->m_pdxVS);
    }


    VertexShaderStream.close();
    SAFE_RELEASE(pErroMsg);
    SPtr<VertexShader> ns(tmpS);
    return ns;
  }

  SPtr<PixelShader> 
  DirectX11Api::loadPixelShaderFromFile(const char* pixelFilePath, 
                                        const char* pixelMainFuntion, 
                                        const char* shaderVersion) {
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
      return false;
    }
    auto FrameShader = FragmentShaderCode;
    //Microsoft::WRL::ComPtr<ID3DBlob> blob;
    D3DCompile(FragmentShaderCode.c_str(),
      FragmentShaderCode.length(),
      nullptr,
      nullptr,
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
      MessageBox(NULL, toBox.c_str(), "Error to read shader", MB_ICONERROR | MB_OK);
      std::cout << toBox.c_str() << std::endl;
      SAFE_RELEASE(pErroMsg);
      SAFE_RELEASE(tmpS->m_pPBlob);
      OutputDebugStringA(msg.c_str());
      std::cout << "can't compile pixel shader\n" << std::endl;

      return false;
    }
    else
    {
      m_device.m_pd3dDevice->CreatePixelShader(tmpS->m_pPBlob->GetBufferPointer(),
                                               tmpS->m_pPBlob->GetBufferSize(),
                                               NULL,
                                               &tmpS->m_pdxPS);
    }

    FragmentShaderStream.close();
    SAFE_RELEASE(pErroMsg);

    SPtr<PixelShader> ns(tmpS);
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

  void 
  DirectX11Api::setRenderTarget(WeakSptr<RenderTarget> rt) {
    D3D11RenderTarget* ret = reinterpret_cast<D3D11RenderTarget*>(rt.lock().get());

    if (nullptr==ret->m_pDepthStencil)
    {
      m_deviceContext.m_pd3dDeviceContext->OMSetRenderTargets(1,
                                                              &ret->m_pRT.m_pRenderTarget,
                                                              NULL);
    }
    else
    {
      m_deviceContext.m_pd3dDeviceContext->OMSetRenderTargets(1,
                                                              &ret->m_pRT.m_pRenderTarget,
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
    D3D11RenderTarget* tmpS = reinterpret_cast<D3D11RenderTarget*>(rt.lock().get());
    m_deviceContext.m_pd3dDeviceContext->ClearRenderTargetView(tmpS->m_pRT.m_pRenderTarget, 
                                                               color);
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
                                                              &t->m_ppSRV[0]);
  }

  void 
  DirectX11Api::PixelShaderSetShaderResources(WeakSptr<Texture2D> resource,
                                              int32 ResourceSlot, 
                                              uint32 numresources) {
    D3D11Texture2D* t = reinterpret_cast<D3D11Texture2D*>(resource.lock().get());
    m_deviceContext.m_pd3dDeviceContext->PSSetShaderResources(ResourceSlot, 
                                                              numresources, 
                                                              &t->m_ppSRV[0]);
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
  }

  void
  DirectX11Api::Present() {
    m_swapChain.m_pdxgSwapChain->Present(0,0);
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
    m_RTV->release();
  }

}
