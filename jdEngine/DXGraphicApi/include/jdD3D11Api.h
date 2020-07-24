/*****************************************************************************/
/**
 * @file    jdD3DApi.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    26/JUN/2020
 * @brief   class for Direct 11 Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once
#include "jdD3D11Prerequisites.h"

#include <jdModulo.h>
#include <jdGraphicApi.h>
#include <SFML/Window.hpp>

#include "jdD3D11Device.h"
#include "jdD3D11DeviceContext.h"
#include "jdD3D11SwapChain.h"
#include "jdD3D11RenderTarget.h"
#include "jdD3D11RenderTargetView.h"
#include "jdD3D11VertexBuffer.h"
#include "jdD3D11IndexBuffer.h"
using namespace jdEngineSDK;

namespace jdEngineSDK {

  class DirectX11Api : public GraphicApi
  {
   public:
    /**
     * @brief Default constructor
     */
    DirectX11Api() = default;

    /**
     * @brief Default destructor
     */
    ~DirectX11Api() override{};

    /**
     * @brief get ptr to device
     * @return share ptr to device
     */
    //SPtr<Device>
    //getDevice() const{
    //  SPtr<Device> tmp(static_cast<Device>(m_ptrDevice));
    //  return tmp;
    //};

    /**
     * @brief get ptr to device context
     * @return share ptr to device context
     */
    //SPtr<DeviceContext>
    //getDeviceContex() const override {
    //  return nullptr;
    //};

    /**
     * @brief get ptr to device Swap Chain
     * @return share ptr to device Swap Chain
     */
    //SPtr<SwapChain>
    //getSwapChain() const override {
    //  return nullptr;
    //};

    /**
     * @brief create device and context
     * @param _dscDevice is a ptr with de creation data
     * @return false if couldn´t create
     */
    bool
    createDevice() override;

    /**
     * @brief create swap chain
     * @param hwind is a handle to window
     * @param format is the format of swap chain
     * @param width is the width of the swap Chain
     * @param height is the height of the swap Chain
     * @return false if couldn´t create
     */
    bool
    createSwapChain(void* hwind,
                    const FORMAT::E& format, 
                    uint32 width, 
                    uint32 height) override;

    /**
     * @brief resize swap chain
     * @param width is the width of the swap Chain
     * @param height is the height of the swap Chain
     */
    void
    resizeSwapChain(uint32 width, uint32 height) override;

    /**
     * @brief create swap chain
     * @param width is width to depth stencil
     * @param height is is height to depth stencil
     * @return false if couldn´t create
     */
    bool
    createRenderTargetView(uint32 width, uint32 height) override;

    /**
     * @brief virtual function to load and save pixel and vertex shade from file
     * @param vertexFilePath is the path to the file of Vertex Shader
     * @param vertexMainFuntion is the name of the main function of the vertex Shader
     * @param vertexShaderVersion is the version of Shader
     * @param pixelFilePath is the path to the file of Pixel Shader
     * @param pixelMainFuntion is the name of the main function of the Pixel Shader
     * @param pixelShaderVersion is the version of Shader
     * @return false if couldn´t load shader
     */
    SPtr<ProgramShader>
    loadShaderFromFile(const char* vertexFilePath,
                       const char* vertexMainFuntion,
                       const char* vertexShaderVersion,
                       const char* pixelFilePath,
                       const char* pixelMainFuntion,
                       const char* pixeñShaderVersion) override;


    /**
     * @brief function to load and save vertex shade from file
     * @param vertexFilePath is the path to the file of Vertex Shader
     * @param vertexMainFuntion is the name of the main function of the vertex Shader
     * @param shaderVersion is the version of Shader
     * @param vertexS is the vertex shader when save the shader compiled
     * @return false if couldn´t load shader
     */
    bool
    loadVertexShaderFromFile(const char* vertexFilePath, 
                             const char* vertexMainFuntion,
                             const char* shaderVersion,
                             WeakSptr<VertexShader> vertexS) override;

    /**
     * @brief function to load and save pixel shade from file
     * @param pixelFilePath is the path to the file of Pixel Shader
     * @param pixelMainFuntion is the name of the main function of the Pixel Shader
     * @param shaderVersion is the version of Shader
     * @param pixelS is the vertex shader when save the shader compiled
     * @return false if couldn´t load shader
     */
    bool
    loadPixelShaderFromFile(const char* pixelFilePath,
                            const char* pixelMainFuntion,
                            const char* shaderVersion,
                            WeakSptr<PixelShader> pixelS) override;

    /**
     * @brief set the viewport for the render targets
     * @param vp is a struct with the data
     */
    void
    setViewPort(const ViewPort& vp) override;

    /**
     * @brief Create VertexBuffer
     * @param numvertex is the number of vertex
     * @param structSize is the size of the struc with the data
     * @param vertex are the vertex
     * @return a share pointer with the new vertex buffer
     */
    SPtr<VertexBuffer>
    createVertexBuffer(int32 numvertex,
                       uint32 structSize,
                       void* vertex) override;

    /**
     * @brief Create VertexBuffer
     * @param numindices is the number of index
     * @param index are the index
     * @retun a share pointer with the new index buffer
     */
    SPtr<IndexBuffer>
    createIndexBuffer(int32 numindices, uint32* index) override;

    /**
     * @brief Create ConstantBuffer
     * @param sizeOfStruct is the size of the struct of buffer
     * @return a share pointer with de new constant buffer
     */
    SPtr<ConstantBuffer>
    CreateConstantBuffer(uint32 sizeOfStruct) override;

    /**
     * @brief Create Sampler linear
     * @return a share pointer with the sampler linear
     */
    SPtr<Sampler>
    CreateSampleLinearState() override;

    /**
     * @brief function to create input layout
     * @param inLayout is the input layout to save in
     * @param elementsInput is the data of input layout
     * @param numElements is the number of elementes of the input layout
     */
    SPtr<InputLayout>
    CreateInputLayout(WeakSptr<VertexShader> vs,
                      INPUT_LAYOUT_DESC* elementsInput, 
                      uint32 numElements) override;

    /**
     * @brief function to reflect input layout
     * @param vs is the vertex shader to do reflect
     * @return a shared pointer with the new input layout
     */
    SPtr<InputLayout>
    reflectInputLayout(WeakSptr<VertexShader> vs) override;

    /**
     * @brief virtual function to reflect input layout
     * @param filePath is the path with the file
     * @return a shared pointer with the Texture
     */
    SPtr<Texture2D>
    LoadShaderResourceFromFile(char* filePath) { return nullptr; };

    /**
     * @brief function to clear the render target
     * @param rt is the render target to set 
     */
    void
    setRenderTarget(WeakSptr<RenderTarget> rt) override;

    /**
     * @brief function to clear the render target
     * @param r is the red color
     * @param g is the green color
     * @param b is the blue color
     * @param a is the alpha 
     */
    void
    Clear(WeakSptr<RenderTarget> rt,
          const float& r = 0, 
          const float& g = 0, 
          const float& b = 0, 
          const float& a = 1) override;

    /**
     * @brief function to clear the depth stencil of the render target
     * @param rt is the render target with the depth stencil to clear 
     */
    void
    ClearDepthStencil(WeakSptr<RenderTarget> rt) override;


    /**
     * @brief function to set a vertexBuffer
     * @param vertex is the buffer with the vertex data 
     */
    void
    setVertexBuffer(WeakSptr<VertexBuffer> vertex) override;

    /**
     * @brief function to set a vertexBuffer
     * @param index is the buffer with the index data 
     */
    void
    setIndexBuffer(WeakSptr<IndexBuffer> index) override;

    /**
     * @brief function to set a vertexBuffer
     * @param index is the buffer with the index data 
     */
    void
    setPrimitiveTopology(const PRIMITIVE_TOPOLOGY_FORMAT::E& format = 
                         PRIMITIVE_TOPOLOGY_FORMAT::TRIANGLELIST) override;

    /**
     * @brief function to update resources
     * @param buffer is the buffer to update 
     * @param data is the data to update the buffer 
     */
    void
    updateSubresource(WeakSptr<ConstantBuffer> buffer, void* data) override;

    /**
     * @brief function to update resources
     * @param program is the program with the shaders to set
     */
    void
    setProgramShader(WeakSptr<ProgramShader> programS) override;

    /**
     * @brief function to set VertexShader
     * @param vertexS is the vertex shader to set
     */
    void
    setVertexShader(WeakSptr<VertexShader> vertexS) override;

    /**
     * @brief function to set PixelShader
     * @param pixelS is the pixel shader to set
     */
    void
    setPixelShader(WeakSptr<PixelShader> pixelS) override;

    /**
     * @brief function to set the inputLayout
     * @param input is the inputLayput to set 
     */
    void
    setInputLayout(WeakSptr<InputLayout> input) override;

    /**
     * @brief virtual function to set Constan Buffer in pixel and vertex Shader
     * @param buffer is the buffer to set
     * @param bufferSlot is the slot in the shader
     * @param numBuffers is the number of buffers to set
     */
    void
    setConstanBuffer(SPtr<ConstantBuffer> buffer,
                     int32 bufferSlot, 
                     uint32 numBuffers = 0) override;

    /**
     * @brief function to set Constan Buffer in vertex Shader
     * @param buffer is the buffer to set
     * @param bufferSlot is the slot in the shader
     * @param numBuffers is the number of buffers to set
     */
    void
    VertexShaderSetConstanBuffer(WeakSptr<ConstantBuffer> buffer,
                                 int32 bufferSlot, 
                                 uint32 numBuffers = 1) override;

    /**
     * @brief function to set Constan Buffer in pixel Shader
     * @param buffer is the buffer to set
     * @param bufferSlot is the slot in the shader
     * @param numBuffers is the number of buffers to set
     */
    void
    PixelShaderSetConstanBuffer(WeakSptr<ConstantBuffer> buffer,
                                 int32 bufferSlot, 
                                 uint32 numBuffers = 1) override;

    /**
     * @brief function to set Shader Resource in pixel and vertex Shader
     * @param resoure is a Texture 2d with the resourse
     * @param ResourceSlot is the slot in the shader
     * @param numresources is the number of resources to set
     */
    void
    setShaderResources(WeakSptr<Texture2D> resource,
                       int32 ResourceSlot, 
                       uint32 numresources = 1) override;

    /**
     * @brief function to set Shader Resource in vertex Shader
     * @param resoure is a Texture 2d with the resourse
     * @param ResourceSlot is the slot in the shader
     * @param numresources is the number of resources to set
     */
    void
    VertexShaderSetShaderResources(WeakSptr<Texture2D> resource,
                                   int32 ResourceSlot,
                                   uint32 numresources = 1) override;

    /**
     * @brief function to set Shader Resource in pixel Shader
     * @param resoure is a Texture 2d with the resourse
     * @param ResourceSlot is the slot in the shader
     * @param numresources is the number of resources to set
     */
    void
    PixelShaderSetShaderResources(WeakSptr<Texture2D> resource,
                                  int32 ResourceSlot,
                                  uint32 numresources = 1) override;

    /**
     * @brief function to set Sampler in pixel and vertex Shader
     * @param sampler is the sampler to set
     * @param samplerSlot is the slot in the shader
     * @param numSamplers is the number of samplers to set
     */
    void
    setSampler(WeakSptr<Sampler> sampler, int32 samplerSlot, uint32 numSamplers = 1) override;

    /**
     * @brief function to set Sampler in vertex Shader
     * @param sampler is the sampler to set
     * @param samplerSlot is the slot in the shader
     * @param numSamplers is the number of samplers to set
     */
    void
    VertexShaderSetSampler(WeakSptr<Sampler> sampler,
                           int32 samplerSlot, 
                           uint32 numSamplers = 1) override;

    /**
     * @brief function to set Sampler in pixel Shader
     * @param sampler is the sampler to set
     * @param samplerSlot is the slot in the shader
     * @param numSamplers is the number of samplers to set
     */
    void
    PixelShaderSetSampler(WeakSptr<Sampler> sampler,
                          int32 samplerSlot, 
                          uint32 numSamplers = 1) override;

    /**
     * @brief function to draw the vertx buffer setted with the index buffer setted
     * @param numIdex is of index to draw
     */
    void
    DrawIndex(uint32 numIdex) override;

    /**
     * @brief do present to the window
     */
    void
    Present() override;

    void* 
    createWindow(uint32 height = 600,
                 uint32 width = 600 ,
                 const String& winName = String("Window"),
                 bool fullScream = false) override;

    bool
    windowIsOpen() override;

    SPtr<RenderTargetView>
    getRenderTargetView() override {
      return SPtr<RenderTargetView>(m_RTV, reinterpret_cast<RenderTargetView*>(m_RTV.get()));
    };

   private:
    /**
     * @brief override the modulo´s function onStartUp
     */
    void
    onStartUp() override;

    /**
     * @brief override the modulo´s function onShutDown
     */
    void
    onShutDown() override;

    HRESULT 
    CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, 
                                                   ID3D11Device* pD3DDevice, 
                                                   ID3D11InputLayout** pInputLayout);

    void
    release();

    /**
     * @brief member device
     */
    D3D11Device m_device;

    /**
     * @brief member device context
     */
    D3D11DeviceContext m_deviceContext;

    /**
     * @brief member swap chain
     */
    D3D11SwapChain m_swapChain;

    /**
     * @brief bool to check if the render target view already created
     */
    bool m_bRenderTargetViewCreated = false;
    
    /**
     * @brief member Render Target View
     */
    SPtr<D3D11RenderTargetView> m_RTV = nullptr;

    sf::WindowBase m_Wind;

    FORMAT::E m_swapChainFormat;
    ViewPort m_viewPort;
  };

  extern "C" JD_PLUGIN_EXPORT GraphicApi *
  createGraphicAPI() {
    DirectX11Api* pDX = new DirectX11Api();
    return pDX;

  }
  
}
