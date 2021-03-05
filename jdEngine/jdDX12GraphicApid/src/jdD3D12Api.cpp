#include "jdD3D12Api.h"

namespace jdEngineSDK {
  
  bool
  DirectX12Api::createDevice() {
#if defined(_DEBUG)
    ID3D12Debug* debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
      debugController->EnableDebugLayer();
    }
    m_debugController.reset(debugController);
#endif

    IDXGIFactory4* dxgiFactory;
    HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
    if (FAILED(hr))
      return false;//error
    m_dxgiFactory.reset(dxgiFactory);

    IDXGIAdapter1* adapter;
    for (UINT adapterIndex = 0;
         DXGI_ERROR_NOT_FOUND != m_dxgiFactory->EnumAdapters1(adapterIndex, &adapter);
         ++adapterIndex) {
      DXGI_ADAPTER_DESC1 desc;
      hr = adapter->GetDesc1(&desc);
      if (FAILED(hr))
        continue;

      if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
        // Don't select the Basic Render Driver adapter.
        continue;
      }

      // Check to see if the adapter supports Direct3D 12,
      // but don't create the actual device yet.
      if (SUCCEEDED(D3D12CreateDevice(adapter, 
                                      D3D_FEATURE_LEVEL_11_0,
                                      _uuidof(ID3D12Device), 
                                      nullptr))) {
        break;
      }
    }

    m_adapter.reset(adapter);
#if !defined(NDEBUG)
    if (!adapter) {
      if (FAILED(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&adapter)))) {
        m_adapter.reset();
        //fatal error
        return false;
      }
    }
#endif

    return true;
  }
}