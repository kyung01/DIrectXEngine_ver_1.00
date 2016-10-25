#pragma once
#include "Context.h"
#include "UIMain.h"
#include "Graphic\GraphicMain.h"
/*
Context class for this project
*/
namespace NImGui{
	class KContext : public Context {
	private:
		UIMain m_uiMain;
	public:
		void init(
			HINSTANCE wc, HWND hwnd,
			ID3D11Device* g_pd3dDevice,
			ID3D11DeviceContext*     g_pd3dDeviceContext,
			IDXGISwapChain*          g_pSwapChain,
			ID3D11RenderTargetView*  g_mainRenderTargetView,
			Graphic::GraphicMain *graphicMain);
		void renderSub() override;
		
	};

}