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
    //virtual SPtr<Device>
    //getDevice() const { return nullptr; };
    //
    ///**
    // * @brief get ptr to device context
    // * @return share ptr to device context
    // */
    //virtual SPtr<DeviceContext>
    //getDeviceContex() const { return nullptr; };
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
     * @brief create swap chain
     * @param width is width to depth stencil
     * @param height is is height to depth stencil
     * @return false if couldn´t create
     */
    virtual bool
    createRenderTargetView(uint32, uint32) { return false; };

    /**
     * @brief virtual function to load and save pixel and vertex shade from file
     * @param vertexFilePath is the path to the file of Vertex Shader
     * @param vertexMainFuntion is the name of the main function of the vertex Shader
     * @param vertexShaderVersion is the version of Shader
     * @param pixelFilePath is the path to the file of Pixel Shader
     * @param pixelMainFuntion is the name of the main function of the Pixel Shader
     * @param pixelShaderVersion is the version of Shader
     * @param programS is program shader when save the shader compiled
     * @return false if couldn´t load shader
     */
    virtual SPtr<ProgramShader>
    loadShaderFromFile(const char*, 
                       const char*, 
                       const char*, 
                       const char*, 
                       const char*,
                       const char*) { return nullptr; };

    /**
     * @brief virtual function to load and save vertex shade from file
     * @param vertexFilePath is the path to the file of Vertex Shader
     * @param vertexMainFuntion is the name of the main function of the vertex Shader
     * @param shaderVersion is the version of Shader
     * @param pixelS is the vertex shader when save the shader compiled
     * @return false if couldn´t load shader
     */
    virtual bool
    loadVertexShaderFromFile(const char*, 
                             const char*,
                             const char*,
                             WeakSptr<VertexShader>) { return false; };

    /**
     * @brief virtual function to load and save pixel shade from file
     * @param pixelFilePath is the path to the file of Pixel Shader
     * @param pixelMainFuntion is the name of the main function of the Pixel Shader
     * @param shaderVersion is the version of Shader
     * @param vertexS is the vertex shader when save the shader compiled
     * @return false if couldn´t load shader
     */
    virtual bool
    loadPixelShaderFromFile(const char*, 
                            const char*,
                            const char*,
                            WeakSptr<PixelShader>) { return false; };

    /**
     * @brief set the viewport for the render targets
     * @param vp is a struct with the data
     */
    virtual void
    setViewPort(const ViewPort&) {};

    /**
     * @brief Create VertexBuffer
     * @param numvertex is the number of vertex
     * @param structSize is the size of the struc with the data
     * @param vertex are the vertex
     * @return a share poninter with the new vertex buffer
     */
    virtual SPtr<VertexBuffer>
    createVertexBuffer(int32, 
                       uint32,
                       void*) { return nullptr ;};

    /**
     * @brief Create VertexBuffer
     * @param numindices is the number of index
     * @param index are the index
     * @retun a share pointer with the new index buffer
     */
    virtual SPtr<IndexBuffer>
    createIndexBuffer(int32, uint32*) { return nullptr; };

    /**
     * @brief Create ConstantBuffer
     * @param sizeOfStruct is the size of the struct of buffer
     */
    virtual SPtr<ConstantBuffer>
    CreateConstantBuffer(uint32) { return nullptr; };

    /**
     * @brief Create Sampler linear
     * @return a share pointer with the sampler linear
     */
    virtual SPtr<Sampler>
    CreateSampleLinearState() { return nullptr; };

    /**
     * @brief virtual function to create input layout
     * @param cs is the vertexShader
     * @param elementsInput is a pointer with the elemnts data
     * @param numElements is the number of elementes of the input layout
     */
    virtual SPtr<InputLayout>
    CreateInputLayout(WeakSptr<VertexShader> ,INPUT_LAYOUT_DESC* ,uint32) {
      return nullptr;
    };

    /**
     * @brief virtual function to reflect input layout
     * @param vs is the vertex shader to do reflect
     * @return a shared pointer with the new input layout
     */
    virtual SPtr<InputLayout>
    reflectInputLayout(WeakSptr<VertexShader> ) {return nullptr;};

    /**
     * @brief virtual function to reflect input layout
     * @param filePath is the path with the file
     * @return a shared pointer with the Texture
     */
    virtual SPtr<Texture2D>
    LoadShaderResourceFromFile(char* filePath) { return nullptr; };

    /**
     * @brief function to clear the render target
     * @param rt is the render target to set 
     */
    virtual void
    setRenderTarget(WeakSptr<RenderTarget>) {};

    /**
     * @brief virtual function to clear the render target
     * @param rt is the render target to clear
     * @param r is the red color
     * @param g is the green color
     * @param b is the blue color
     * @param a is the alpha 
     */
    virtual void
    Clear(WeakSptr<RenderTarget>,
          const float&, 
          const float&, 
          const float&, 
          const float&) {};

    /**
     * @brief virtual function to clear the render target
     * @param rt is the render target with the depth stencil to clear 
     */
    virtual void
    ClearDepthStencil(WeakSptr<RenderTarget>) {};

    /**
     * @brief virtual function to set a vertexBuffer
     * @param vertex is the buffer with the vertex data 
     */
    virtual void
    setVertexBuffer(WeakSptr<VertexBuffer> ) {};

    /**
     * @brief virtual function to set a vertexBuffer
     * @param index is the buffer with the index data 
     */
    virtual void
    setIndexBuffer(WeakSptr<IndexBuffer> ) {}

    /**
     * @brief virtual function to set a vertexBuffer
     * @param index is the buffer with the index data 
     */
    virtual void
    setPrimitiveTopology(const PRIMITIVE_TOPOLOGY_FORMAT::E& ) {};

    /**
     * @brief virtual function to update resources
     * @param buffer is the buffer to update 
     * @param data is the data to update the buffer 
     */
    virtual void
    updateSubresource(WeakSptr<ConstantBuffer>, void*)  {};

    /**
     * @brief virtual function to update resources
     * @param program is the program with the shaders to set
     */
    virtual void
    setProgramShader(WeakSptr<ProgramShader> ) {};

    /**
     * @brief virtual function to set VertexShader
     * @param vertexS is the vertex shader to set
     */
    virtual void
    setVertexShader(WeakSptr<VertexShader> ) {};

    /**
     * @brief virtual function to set the inputLayout
     * @param input is the inputLayput to set 
     */
    virtual void
    setInputLayout(WeakSptr<InputLayout> ) {};

    /**
     * @brief virtual function to set PixelShader
     * @param pixelS is the pixel shader to set
     */
    virtual void
    setPixelShader(WeakSptr<PixelShader> ) {};

    /**
     * @brief virtual function to set Constan Buffer in pixel and vertex Shader
     * @param buffer is the buffer to set
     * @param bufferSlot is the slot in the shader
     * @param numBuffers is the number of buffers to set
     */
    virtual void
    setConstanBuffer(SPtr<ConstantBuffer> ,int32 , uint32  = 0) {};

    /**
     * @brief virtual function to set Constan Buffer in vertex Shader
     * @param buffer is the buffer to set
     * @param bufferSlot is the slot in the shader
     * @param numBuffers is the number of buffers to set
     */
    virtual void
    VertexShaderSetConstanBuffer(WeakSptr<ConstantBuffer> ,
                                 int32 , 
                                 uint32  = 1) {};

    /**
     * @brief virtual function to set Constan Buffer in pixel Shader
     * @param buffer is the buffer to set
     * @param bufferSlot is the slot in the shader
     * @param numBuffers is the number of buffers to set
     */
    virtual void
    PixelShaderSetConstanBuffer(WeakSptr<ConstantBuffer>,
                                 int32 , 
                                 uint32  = 1) {};

    /**
     * @brief virtual function to set Shader Resource in pixel and vertex Shader
     * @param resoure is a Texture 2d with the resourse
     * @param ResourceSlot is the slot in the shader
     * @param numresources is the number of resources to set
     */
    virtual void
    setShaderResources(WeakSptr<Texture2D> , int32 , uint32  = 1) {};

    /**
     * @brief virtual function to set Shader Resource in vertex Shader
     * @param resoure is a Texture 2d with the resourse
     * @param ResourceSlot is the slot in the shader
     * @param numresources is the number of resources to set
     */
    virtual void
    VertexShaderSetShaderResources(WeakSptr<Texture2D>,
                                   int32 ,
                                   uint32  = 1) {};

    /**
     * @brief virtual function to set Shader Resource in pixel Shader
     * @param resoure is a Texture 2d with the resourse
     * @param ResourceSlot is the slot in the shader
     * @param numresources is the number of resources to set
     */
    virtual void
    PixelShaderSetShaderResources(WeakSptr<Texture2D>,
                                  int32 ,
                                  uint32  = 1) {};

    /**
     * @brief virtual function to set Sampler in pixel and vertex Shader
     * @param sampler is the sampler to set
     * @param samplerSlot is the slot in the shader
     * @param numSamplers is the number of samplers to set
     */
    virtual void
    setSampler(WeakSptr<Sampler> , int32 , uint32  = 1) {};

    /**
     * @brief virtual function to set Sampler in vertex Shader
     * @param sampler is the sampler to set
     * @param samplerSlot is the slot in the shader
     * @param numSamplers is the number of samplers to set
     */
    virtual void
    VertexShaderSetSampler(WeakSptr<Sampler>, int32 , uint32  = 1) {};

    /**
     * @brief virtual function to set Sampler in pixel Shader
     * @param sampler is the sampler to set
     * @param samplerSlot is the slot in the shader
     * @param numSamplers is the number of samplers to set
     */
    virtual void
    PixelShaderSetSampler(WeakSptr<Sampler>, int32 , uint32  = 1) {};

    /**
     * @brief virtual function to draw the vertx buffer setted with the index buffer setted
     * @param numIdex is of index to draw
     */
    virtual void
    DrawIndex(uint32 ) {};

    /**
     * @brief virtual function to do present to the window
     */
    virtual void
    Present() {};

    void
    setObject(GraphicApi* api) {
      GraphicApi::_instance() = api;
    }

    virtual void* 
    createWindow(uint32  = 600,
                 uint32  = 600 ,
                 const String&  = String("Window"),
                 bool  = false) { return nullptr; };

    virtual bool
    windowIsOpen() { return false; };

    virtual SPtr<RenderTargetView>
    getRenderTargetView() { return nullptr; };

		};

  JD_CORE_EXPORT GraphicApi&
  g_graphicsApi();
}
