/*****************************************************************************/
/**
 * @file    GraphicApi.h
 * @author  Sergio Diaz (idv17c.sdiaz@uartesdigitales.edu.mx)
 * @date    26/JUN/2020
 * @brief   Virtual class for any Graphic Api
 *
 *
 *
 * @bug     No known bugs.
 */
/*****************************************************************************/
#pragma once
#include "jdPrerequisitesCore.h"

#include "jdDevice.h"
#include "jdDeviceContext.h"
#include "jdSwapChain.h"
#include "jdViewPort.h"
#include "jdProgramShader.h"
#include "jdVertexBuffer.h"
#include "jdIndexBuffer.h"
#include "jdConstantBuffer.h"
#include "jdSampler.h"
#include "jdRenderTarget.h"
#include "jdRenderTargetView.h"
#include "jdTexture2D.h"
#include "jdInputLayout.h"
#include "jdRasterizeState.h"


namespace PRIMITIVE_TOPOLOGY_FORMAT {
  enum E
  {
    UNDEFINED = 0,
    POINTLIST,
    LINELIST,
    LINESTRIP,
    TRIANGLELIST,
    TRIANGLESTRIP,
    NUMOFTOPLOGYS
  };
}

namespace INPUT_CLASSIFICATION {
  enum E
  {
    D3D11_INPUT_PER_VERTEX_DATA = 0,
    D3D11_INPUT_PER_INSTANCE_DATA = 1
  };
}

namespace jdEngineSDK {


  struct INPUT_LAYOUT_DESC {
    String semanicName;
    uint32 SemanticIndex;
    FORMAT::E format;
    uint32 InputSlot;
    uint32 AlignedByteOffset;
    INPUT_CLASSIFICATION::E InputSlotClass;
    uint32 InstanceDataStepRate;
  };


  class JD_CORE_EXPORT GraphicApi : public Module<GraphicApi>
  {
   public:
    /**
     * @brief Default constructor
     */
     GraphicApi() {
       GraphicApi::onStartUp();
     };

    /**
     * @brief Default destructor
     */
    virtual ~GraphicApi() {};

    /**
     * @brief get ptr to device
     * @return share ptr to device 
     */
    virtual void*
    getDevice() { return nullptr; };
    
    /**
     * @brief get ptr to device context
     * @return share ptr to device context
     */
    virtual void*
    getDeviceContex() { return nullptr; };
    //
    ///**
    // * @brief get ptr to device Swap Chain
    // * @return share ptr to device Swap Chain
    // */
    //virtual SPtr<SwapChain>
    //getSwapChain() const { return nullptr; };

    /**
     * @brief create device and context
     * @param _dscDevice is a ptr with de creation data
     */
    virtual bool
    createDevice() { return false; };
    
    /**
     * @brief create swap chain
     * @param hwind is a handle to window
     * @param format is the format of swap chain
     * @param width is the width of the swap Chain
     * @param height is the height of the swap Chain
     * @return false if couldn´t create
     */
    virtual bool
    createSwapChain(void* /*hwind*/, 
                    const FORMAT::E& /*format*/, 
                    uint32 /*width*/, 
                    uint32 /*height*/) {
      return false; 
    };

    /**
     * @brief resize swap chain
     * @param width is the width of the swap Chain
     * @param height is the height of the swap Chain
     */
    virtual void
    resizeSwapChain(uint32 /*width*/, uint32 /*height*/) {};

    /**
     * @brief create RenderTargetView
     * @param width is width to depth stencil
     * @param height is is height to depth stencil
     * @return false if couldn´t create
     */
    virtual bool
    createRenderTargetView(uint32 /*width*/, uint32 /*height*/) { return false; };

    /**
     * @brief create swap chain
     * @param width is width to depth stencil
     * @param height is is height to depth stencil
     * @return false if couldn´t create
     */
    virtual SPtr<RenderTarget>
    createRenderTarget(uint32 /*width*/, 
                       uint32 /*height*/, 
                       uint32 /*mipLevels*/ = 1, 
                       bool /*Depth*/ = false) {
      return nullptr; 
    };

    /**
     * @brief virtual function to load and save pixel and vertex shade from file
     * @param vertexFilePath is the path to the file of Vertex Shader
     * @param vertexMainFuntion is the name of the main function of the vertex Shader
     * @param vertexShaderVersion is the version of Shader
     * @param pixelFilePath is the path to the file of Pixel Shader
     * @param pixelMainFuntion is the name of the main function of the Pixel Shader
     * @param pixelShaderVersion is the version of Shader
     * @return shared ptr with the program shader created
     */
    virtual SPtr<ProgramShader>
    loadShaderFromFile(const char* /*vertexFilePath*/ , 
                       const char* /*vertexMainFuntion*/, 
                       const char* /*vertexShaderVersio*/, 
                       const char* /*pixelFilePath*/, 
                       const char* /*pixelMainFuntion*/,
                       const char* /*pixelShaderVersion*/) { return nullptr; };

    /**
     * @brief virtual function to load and save vertex shade from file
     * @param vertexFilePath is the path to the file of Vertex Shader
     * @param vertexMainFuntion is the name of the main function of the vertex Shader
     * @param shaderVersion is the version of Shader
     * @return a shared ptr with the vertex shader created
     */
    virtual SPtr<VertexShader>
    loadVertexShaderFromFile(const char* /*vertexFilePath*/, 
                             const char* /*vertexMainFuntion*/,
                             const char* /*shaderVersion*/) { return nullptr; };

    /**
     * @brief virtual function to load and save pixel shade from file
     * @param pixelFilePath is the path to the file of Pixel Shader
     * @param pixelMainFuntion is the name of the main function of the Pixel Shader
     * @param shaderVersion is the version of Shader
     * @return a shared ptr with the pixel shader created
     */
    virtual SPtr<PixelShader>
    loadPixelShaderFromFile(const char* /*pixelFilePath*/, 
                            const char* /*pixelMainFuntion*/,
                            const char* /*shaderVersion*/) { return nullptr; };

    /**
     * @brief set the viewport for the render targets
     * @param vp is a struct with the data
     */
    virtual void
    setViewPort(const ViewPort& /*vp*/) {};

    /**
     * @brief Create VertexBuffer
     * @param numvertex is the number of vertex
     * @param structSize is the size of the struc with the data
     * @param vertex are the vertex
     * @return a share poninter with the new vertex buffer
     */
    virtual SPtr<VertexBuffer>
    createVertexBuffer(int32 /*numvertex*/, 
                       uint32 /*structSize*/,
                       void* /*vertex*/) { return nullptr ;};

    /**
     * @brief Create VertexBuffer
     * @param numindices is the number of index
     * @param index are the index
     * @retun a share pointer with the new index buffer
     */
    virtual SPtr<IndexBuffer>
    createIndexBuffer(int32 /*numindices*/, uint32* /*index*/) { return nullptr; };

    /**
     * @brief Create Default Textures
     */
    virtual void
    createDefaultTextures() {};

    /**
     * @brief Create ConstantBuffer
     * @param sizeOfStruct is the size of the struct of buffer
     */
    virtual SPtr<ConstantBuffer>
    CreateConstantBuffer(uint32 /*sizeOfStruct*/) { return nullptr; };

    /**
     * @brief Create Sampler linear
     * @return a share pointer with the sampler linear
     */
    virtual SPtr<Sampler>
    CreateSampleLinearState() { return nullptr; };

    /**
     * @brief virtual function to create input layout
     * @param vs is the vertexShader
     * @param elementsInput is a pointer with the elemnts data
     * @param numElements is the number of elementes of the input layout
     */
    virtual SPtr<InputLayout>
    CreateInputLayout(WeakSptr<VertexShader> /*vs*/,
                      INPUT_LAYOUT_DESC* /*elementsInput*/,
                      uint32 /*numElements*/) {
      return nullptr;
    };

    /**
     * @brief virtual function to reflect input layout
     * @param vs is the vertex shader to do reflect
     * @return a shared pointer with the new input layout
     */
    virtual SPtr<InputLayout>
    reflectInputLayout(WeakSptr<VertexShader> /*vs*/) {return nullptr;};

    /**
     * @brief virtual function to reflect input layout
     * @param filePath is the path with the file
     * @return a shared pointer with the Texture
     */
    virtual SPtr<Texture2D>
    LoadShaderResourceFromFile(const char* /*filePath*/, bool /*isCubeMap*/) { 
      return nullptr; 
    };

    /**
     * @brief function to clear the render target
     * @param rt is the render target to set 
     */
    virtual void
    setRenderTarget(WeakSptr<RenderTarget> /*rt*/) {};

    /**
     * @brief virtual function to clear the render target
     * @param rt is the render target to clear
     * @param r is the red color
     * @param g is the green color
     * @param b is the blue color
     * @param a is the alpha 
     */
    virtual void
    Clear(WeakSptr<RenderTarget> /*rt*/,
          const float& /*r*/, 
          const float& /*g*/, 
          const float& /*b*/, 
          const float& /*a*/) {};

    /**
     * @brief virtual function to clear the render target
     * @param rt is the render target with the depth stencil to clear 
     */
    virtual void
    ClearDepthStencil(WeakSptr<RenderTarget> /*rt*/) {};

    /**
     * @brief virtual function to set a vertexBuffer
     * @param vertexB is the buffer with the vertex data 
     */
    virtual void
    setVertexBuffer(WeakSptr<VertexBuffer> /*vertexB*/) {};

    /**
     * @brief virtual function to set a vertexBuffer
     * @param indexB is the buffer with the index data 
     */
    virtual void
    setIndexBuffer(WeakSptr<IndexBuffer> /*indexB*/) {}

    /**
     * @brief virtual function to set a vertexBuffer
     * @param format is the format of primitive topology to set
     */
    virtual void
    setPrimitiveTopology(const PRIMITIVE_TOPOLOGY_FORMAT::E& /*format*/) {};

    /**
     * @brief virtual function to update resources
     * @param buffer is the buffer to update 
     * @param data is the data to update the buffer 
     */
    virtual void
    updateSubresource(WeakSptr<ConstantBuffer> /*buffer*/, void* /*data*/)  {};

    /**
     * @brief virtual function to update resources
     * @param program is the program with the shaders to set
     */
    virtual void
    setProgramShader(WeakSptr<ProgramShader> /*program*/ ) {};

    /**
     * @brief virtual function to set VertexShader
     * @param vertexS is the vertex shader to set
     */
    virtual void
    setVertexShader(WeakSptr<VertexShader> /*vertexS*/) {};

    /**
     * @brief virtual function to set the inputLayout
     * @param input is the inputLayput to set 
     */
    virtual void
    setInputLayout(WeakSptr<InputLayout> /*input*/) {};

    /**
     * @brief virtual function to set PixelShader
     * @param pixelS is the pixel shader to set
     */
    virtual void
    setPixelShader(WeakSptr<PixelShader> /*pixelS*/) {};

    /**
     * @brief virtual function to set Constan Buffer in pixel and vertex Shader
     * @param buffer is the buffer to set
     * @param bufferSlot is the slot in the shader
     * @param numBuffers is the number of buffers to set
     */
    virtual void
    setConstanBuffer(SPtr<ConstantBuffer> /*buffer*/,
                     int32 /*bufferSlot*/, 
                     uint32 /*numBuffers*/ = 0) {};

    /**
     * @brief virtual function to set Constan Buffer in vertex Shader
     * @param buffer is the buffer to set
     * @param bufferSlot is the slot in the shader
     * @param numBuffers is the number of buffers to set
     */
    virtual void
    VertexShaderSetConstanBuffer(WeakSptr<ConstantBuffer> /*buffer*/,
                                 int32 /*bufferSlot*/, 
                                 uint32 /*numBuffers*/ = 1) {};

    /**
     * @brief virtual function to set Constan Buffer in pixel Shader
     * @param buffer is the buffer to set
     * @param bufferSlot is the slot in the shader
     * @param numBuffers is the number of buffers to set
     */
    virtual void
    PixelShaderSetConstanBuffer(WeakSptr<ConstantBuffer> /*buffer*/,
                                 int32 /*bufferSlot*/, 
                                 uint32 /*numBuffers*/ = 1) {};

    virtual void
    SetBonesConstanBuffer() {};

    /**
     * @brief virtual function to set Shader Resource in pixel and vertex Shader
     * @param resoure is a Texture 2d with the resourse
     * @param ResourceSlot is the slot in the shader
     * @param numresources is the number of resources to set
     */
    virtual void
    setShaderResources(WeakSptr<Texture2D> /*resoure*/, 
                       int32 /*ResourceSlot*/, 
                       uint32 /*numresources*/ = 1) {};

    /**
     * @brief virtual function to set Shader Resource in vertex Shader
     * @param resoure is a Texture 2d with the resourse
     * @param ResourceSlot is the slot in the shader
     * @param numresources is the number of resources to set
     */
    virtual void
    VertexShaderSetShaderResources(WeakSptr<Texture2D> /*resoure*/,
                                   int32 /*ResourceSlot*/,
                                   uint32 /*numresources*/ = 1) {};

    /**
     * @brief virtual function to set Shader Resource in pixel Shader
     * @param resoure is a Texture 2d with the resourse
     * @param ResourceSlot is the slot in the shader
     * @param numresources is the number of resources to set
     */
    virtual void
    PixelShaderSetShaderResources(WeakSptr<Texture2D> /*resoure*/,
                                  int32 /*ResourceSlot*/,
                                  uint32 /*numresources*/ = 1) {};

    /**
     * @brief virtual function to set Sampler in pixel and vertex Shader
     * @param sampler is the sampler to set
     * @param samplerSlot is the slot in the shader
     * @param numSamplers is the number of samplers to set
     */
    virtual void
    setSampler(WeakSptr<Sampler> /*sampler*/, 
               int32 /*samplerSlot*/, 
               uint32 /*numSamplers*/ = 1) {};

    /**
     * @brief virtual function to set Sampler in vertex Shader
     * @param sampler is the sampler to set
     * @param samplerSlot is the slot in the shader
     * @param numSamplers is the number of samplers to set
     */
    virtual void
    VertexShaderSetSampler(WeakSptr<Sampler> /*sampler*/, 
                           int32 /*samplerSlot*/, 
                           uint32 /*numSamplers*/ = 1) {};

    /**
     * @brief virtual function to set Sampler in pixel Shader
     * @param sampler is the sampler to set
     * @param samplerSlot is the slot in the shader
     * @param numSamplers is the number of samplers to set
     */
    virtual void
    PixelShaderSetSampler(WeakSptr<Sampler> /*sampler*/, 
                          int32 /*samplerSlot*/, 
                          uint32 /*numSamplers*/ = 1) {};

    /**
     * @brief virtual function to draw the vertx buffer setted with the index buffer setted
     * @param numIdex is of index to draw
     */
    virtual void
    DrawIndex(uint32 /*numIdex*/) {};

    /**
     * @brief virtual function to do present to the window
     */
    virtual void
    Present() {};

    /**
     * @brief to set the api objetc selected
     */
    void
    setObject(GraphicApi* api) {
      GraphicApi::_instance() = api;
    }

    /**
     * @brief get render target view
     * @return a shared_ptr with the render target view
     */
    virtual SPtr<RenderTargetView>
    getRenderTargetView() { return nullptr; };
    
    /**
     * @brief get default error texture
     */
    SPtr<Texture2D>
    getDefaultTextureError() { return DEFAULT_TEXTURE_ERROR; };

    /**
     * @brief get default trasparent texture
     */
    SPtr<Texture2D>
    getDefaultTextureTransparent() { return DEFAULT_TEXTURE_TRANSPARENT; };

    /**
     * @brief get default black texture
     */
    SPtr<Texture2D>
    getDefaultTextureBlack() { return DEFAULT_TEXTURE_BLACK; };

    /**
     * @brief get default white texture
     */
    SPtr<Texture2D>
    getDefaultTextureWhite() { return DEFAULT_TEXTURE_WHITE; };

    /**
     * @brief get default normal texture
     */
    SPtr<Texture2D>
    getDefaultTextureNormal() { return DEFAULT_TEXTURE_NORMAL; };

    /**
     * @brief get default chess texture
     */
    SPtr<Texture2D>
    getDefaultTextureChess() { return DEFAULT_TEXTURE_CHESS; };

    /**
     * @brief to create constant buffer for bones
     * @note only call with the mesh
     */
    virtual SPtr<ConstantBuffer>
    createConstantBufferBones() { return nullptr; };

    /**
     * @brief to get constant buffer for bones
     * @note only call with the mesh
     */
    virtual SPtr<ConstantBuffer>
    getConstantBufferBones() { return nullptr; };

    virtual SPtr<RasterizeState>
    createRasterizeState(RASTERIZER_FILL_MODE::E /*FillMode*/ = 
                                                       RASTERIZER_FILL_MODE::D3D11_FILL_SOLID,
                         RASTERIZER_CULL_MODE::E /*CullMode*/ = 
                                                        RASTERIZER_CULL_MODE::D3D11_CULL_BACK,
                         bool /*FrontCounterClockwise*/ = false,
                         int /*DepthBias*/ = 0,
                         float /*DepthBiasClamp*/ = 0.0f,
                         float /*SlopeScaledDepthBias*/ = 0.0f,
                         bool /*DepthClipEnable*/ = true,
                         bool /*ScissorEnable*/ = false,
                         bool /*MultisampleEnable*/ = false,
                         bool /*AntialiasedLineEnable*/ = false) { 
      return nullptr; 
    };

    virtual void
    setRasterizeState(WeakSptr<RasterizeState> RS) {};

   protected:

    /**
     * @brief default error texture
     */
    SPtr<Texture2D> DEFAULT_TEXTURE_ERROR;

    /**
     * @brief default transparent texture
     */
    SPtr<Texture2D> DEFAULT_TEXTURE_TRANSPARENT;

    /**
     * @brief default black texture
     */
    SPtr<Texture2D> DEFAULT_TEXTURE_BLACK;

    /**
     * @brief default white texture
     */
    SPtr<Texture2D> DEFAULT_TEXTURE_WHITE;

    /**
     * @brief default normal texture
     */
    SPtr<Texture2D> DEFAULT_TEXTURE_NORMAL;

    /**
     * @brief default chess texture
     */
    SPtr<Texture2D> DEFAULT_TEXTURE_CHESS;

		};

  JD_CORE_EXPORT GraphicApi&
  g_graphicsApi();
}
