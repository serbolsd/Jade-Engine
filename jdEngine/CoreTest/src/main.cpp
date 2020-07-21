#include <Windows.h>
#include <jdGraphicApi.h>
#include <jdVector2.h>
#include <jdVector3.h>
#include <jdVector4.h>
#include <jdMatrix4.h>
#include <jdPoint.h>
using namespace jdEngineSDK;
using graphApi = GraphicApi * (*)();

struct SimpleVertex
{
		JDVector4 pos;
		JDVector2 textcoord;
};

struct CBNeverChanges
{
		JDMatrix4 mView;
};

struct CBChangeOnResize
{
		JDMatrix4 mProjection;
};

struct CBChangesEveryFrame
{
		JDMatrix4 mWorld;
		JDVector4 vMeshColor;
};

CBNeverChanges g_neverChanges;
CBChangeOnResize g_changeOnResize;
CBChangesEveryFrame g_changeEveryFrame;

//graphApi g_apiMan;
int main()
{
		JDPoint windSize = {640,480};
  HINSTANCE hin = LoadLibraryExA("jdDXGraphicApid.dll", nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

		if (!hin)
		{
				return 1;
		}

		auto createGraphicsApi = reinterpret_cast<graphApi>(GetProcAddress(hin, "createGraphicAPI"));
		if (!createGraphicsApi)
		{
				return 2;
		}
		GraphicApi::startUp();
		GraphicApi* gsApi = createGraphicsApi();
		g_graphicsApi().setObject(gsApi);

		String winName("windowTest");
		void* windHan(g_graphicsApi().createWindow(windSize.y, windSize.x, winName, false));

		g_graphicsApi().createDevice();
		g_graphicsApi().createSwapChain(windHan,FORMAT::E::FORMAT_R8G8B8A8_UNORM, windSize.x, windSize.y);
		g_graphicsApi().createRenderTargetView(windSize.x, windSize.y);
		SPtr<RenderTargetView> rtv = g_graphicsApi().getRenderTargetView();

		ViewPort vp;
		vp.Width = windSize.x;
		vp.Height = windSize.y;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.m_topLeftX = 0;
		vp.m_topLeftY = 0;
		g_graphicsApi().setViewPort(vp);

		g_graphicsApi().setRenderTarget(rtv);

		SPtr<ProgramShader>progShader=
		g_graphicsApi().loadShaderFromFile("data/shader/Tutorial072.fx",
				                                 "VS", 
				                                 "vs_5_0",
				                                 "data/shader/Tutorial072.fx",
				                                 "PS", 
				                                 "ps_5_0");
		g_graphicsApi().setProgramShader(progShader);
		
		Vector<INPUT_LAYOUT_DESC> layout;
		layout.resize(2);
		
		layout[0] = { "POSITION", 0, FORMAT::E::FORMAT_R32G32B32A32_FLOAT, 0, 0, INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 };
		layout[1] = { "TEXCOORD", 0, FORMAT::E::FORMAT_R32G32_FLOAT, 0, 16, INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 };

		SPtr<InputLayout> inLayOut =
				g_graphicsApi().CreateInputLayout(progShader.get()->getVertexShader(),&layout[0],2);
		//g_graphicsApi().reflectInputLayout(progShader.get()->getVertexShader());

		g_graphicsApi().setInputLayout(inLayOut);

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

		SPtr<VertexBuffer> vertexB =
		g_graphicsApi().createVertexBuffer(24,sizeof(SimpleVertex), &vertices);
		g_graphicsApi().setVertexBuffer(vertexB);

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

		SPtr<IndexBuffer> indexB =
		g_graphicsApi().createIndexBuffer(36, indices);
		g_graphicsApi().setIndexBuffer(indexB);

		SPtr<ConstantBuffer> neverChangeB =
		g_graphicsApi().CreateConstantBuffer(sizeof(CBNeverChanges));

		SPtr<ConstantBuffer> changeOnResizeB =
		g_graphicsApi().CreateConstantBuffer(sizeof(CBChangeOnResize));

		SPtr<ConstantBuffer> changeEveryFrameB =
		g_graphicsApi().CreateConstantBuffer(sizeof(CBChangesEveryFrame));

		JDVector4 Eye(0.0f, 3.0f, -6.0f, 0.0f);
		JDVector4 At(0.0f, 1.0f, 0.0f, 0.0f);
		JDVector4 Up(0.0f, 1.0f, 0.0f, 0.0f);
		g_neverChanges.mView = createViewMatrix(Eye,At,Up);
		g_neverChanges.mView.transpose();
		
		g_changeOnResize.mProjection = createProjectionPerspectiveMatrix(Math::HALF_PI/2, windSize.x, windSize.y, 0.01f, 100.0f);
		g_changeOnResize.mProjection.transpose();
		JDMatrix4 world;
		world.identity();
		JDVector4 color(1,0,0,1);
		g_changeEveryFrame.mWorld = world;
		g_changeEveryFrame.vMeshColor = color;

		g_graphicsApi().updateSubresource(neverChangeB, &g_neverChanges);
		g_graphicsApi().updateSubresource(changeOnResizeB, &g_changeOnResize);
		g_graphicsApi().updateSubresource(changeEveryFrameB, &g_changeEveryFrame);

		//g_graphicsApi().setConstanBuffer(neverChangeB,0);
		g_graphicsApi().VertexShaderSetConstanBuffer(neverChangeB,0);
		g_graphicsApi().PixelShaderSetConstanBuffer(neverChangeB,0);
		//g_graphicsApi().setConstanBuffer(changeOnResizeB,1);
		g_graphicsApi().VertexShaderSetConstanBuffer(changeOnResizeB,1);
		g_graphicsApi().PixelShaderSetConstanBuffer(changeOnResizeB,1);
		//g_graphicsApi().setConstanBuffer(changeEveryFrameB,2);
		g_graphicsApi().VertexShaderSetConstanBuffer(changeEveryFrameB,2);
		g_graphicsApi().PixelShaderSetConstanBuffer(changeEveryFrameB,2);

		SPtr<Sampler> samplerLineal =
		g_graphicsApi().CreateSampleLinearState();
		g_graphicsApi().setSampler(samplerLineal, 0);

		g_graphicsApi().setPrimitiveTopology(PRIMITIVE_TOPOLOGY_FORMAT::TRIANGLELIST);
		float rot = 0.0f;
	
		while (g_graphicsApi().windowIsOpen())
		{
	  	g_graphicsApi().Clear(rtv,0,1,0,1);
				g_graphicsApi().ClearDepthStencil(rtv);
				//rot += 0.0001;
				////g_changeEveryFrame.mWorld.rotateY(Radian(rot));
				//
				g_graphicsApi().updateSubresource(changeEveryFrameB, &g_changeEveryFrame);
				g_graphicsApi().VertexShaderSetConstanBuffer(changeEveryFrameB, 2);
				g_graphicsApi().PixelShaderSetConstanBuffer(changeEveryFrameB, 2);

				g_graphicsApi().setVertexBuffer(vertexB);
				g_graphicsApi().setIndexBuffer(indexB);
				
				g_graphicsApi().DrawIndex(36);
				g_graphicsApi().Present();
		}

		g_graphicsApi().shutDown();
		//rtv.reset();
		//progShader.reset();
		return 0;
}