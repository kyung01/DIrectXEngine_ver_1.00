#include "KContext.h"

using namespace NImGui;

void NImGui::KContext::init(HINSTANCE wc, HWND hwnd,
	ID3D11Device* g_pd3dDevice,
	ID3D11DeviceContext*     g_pd3dDeviceContext,
	IDXGISwapChain*          g_pSwapChain,
	ID3D11RenderTargetView*  g_mainRenderTargetView,
	Graphic::GraphicMain *graphicMain)
{
	Context::init(wc, hwnd, g_pd3dDevice, g_pd3dDeviceContext, g_pSwapChain, g_mainRenderTargetView);
	m_uiMain.init(graphicMain);
}

void KContext::renderSub() {
	m_uiMain.render();
}