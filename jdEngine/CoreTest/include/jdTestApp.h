#pragma once
#include <jdGraphicApi.h>
#include <jdBaseApp.h>
#include <jdVector2.h>
#include <jdVector3.h>
#include <jdVector4.h>
#include <jdMatrix4.h>
#include <jdPoint.h>
using namespace jdEngineSDK; 

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

class testApp : public BaseApp
{
 public:
		testApp();

		testApp(const JDPoint& winPos, 
				      const JDPoint& winSize, 
				      const String& winTitle, 
				      bool fullScreen);

		~testApp() {};
 private:

		void
		onCreate() override;

  void
  onRender() override;

		CBNeverChanges m_neverChanges;
		CBChangeOnResize m_changeOnResize;
		CBChangesEveryFrame m_changeEveryFrame;

		SPtr<RenderTargetView> m_rtv = nullptr;
		SPtr<ProgramShader> m_progShader = nullptr;
		Vector<INPUT_LAYOUT_DESC> m_inLayoutElements;
		SPtr<InputLayout> m_inLayOut = nullptr;
		SPtr<VertexBuffer> m_vertexB = nullptr;
		SPtr<IndexBuffer> m_indexB = nullptr;
		SPtr<ConstantBuffer> m_neverChangeB = nullptr;
		SPtr<ConstantBuffer> m_changeOnResizeB = nullptr;
		SPtr<ConstantBuffer> m_changeEveryFrameB = nullptr;
};
