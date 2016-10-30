#include "imgui\DirectX\KContext.h"

using namespace NImGui;

void NImGui::KContext::init(HINSTANCE wc, HWND hwnd,
	ID3D11Device* g_pd3dDevice,
	ID3D11DeviceContext*     g_pd3dDeviceContext,
	IDXGISwapChain*          g_pSwapChain,
	ID3D11RenderTargetView*  g_mainRenderTargetView)
{
	Context::init(wc, hwnd, g_pd3dDevice, g_pd3dDeviceContext, g_pSwapChain, g_mainRenderTargetView);
}

void KContext::renderSub() {
	m_uiMain.render();
}