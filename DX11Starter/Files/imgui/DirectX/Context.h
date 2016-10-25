#pragma once
#include "imgui\imgui.h"
#include "imgui_impl_dx11.h"

// DirectX
#include <d3d11.h>
#include <d3dcompiler.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
/*
TODO learn how to display resourceview into an image and make an example
TODO UI should display current loaded/saved shaders
TODO UI should display currently loaded/saved framebuffers
TODO UI should display currently loaded/saved models objects
*/
struct ID3D11Device;
struct ID3D11DeviceContext;
namespace NImGui {
	class Context;
	static Context * CURRENT_CONTEXT;
	static void IMGUI_RENDER_CURRENT_CONTEXT(ImDrawData* data);
	struct VERTEX_CONSTANT_BUFFER
	{
		float        mvp[4][4];
	};
	class Context {
	private:
		void CleanupDeviceD3D();
		void CreateRenderTarget();
		void CleanupRenderTarget();
		LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);




		IMGUI_API LRESULT ImGui_ImplDX11_WndProcHandler(HWND, UINT msg, WPARAM wParam, LPARAM lParam);
		void ImGui_ImplDX11_CreateFontsTexture();
		bool    ImGui_ImplDX11_CreateDeviceObjects();
		void    ImGui_ImplDX11_InvalidateDeviceObjects();
		bool    ImGui_ImplDX11_Init(void* hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context);
		void ImGui_ImplDX11_Shutdown();
		void ImGui_ImplDX11_NewFrame();
	protected:
	public:
		IDXGISwapChain*          g_pSwapChain = NULL;
		ID3D11RenderTargetView*  g_mainRenderTargetView = NULL;

		// Data
		INT64                    g_Time = 0;
		INT64                    g_TicksPerSecond = 0;

		HWND                     g_hWnd = 0;
		ID3D11Device*            g_pd3dDevice = NULL;
		ID3D11DeviceContext*     g_pd3dDeviceContext = NULL;
		ID3D11Buffer*            g_pVB = NULL;
		ID3D11Buffer*            g_pIB = NULL;
		ID3D10Blob *             g_pVertexShaderBlob = NULL;
		ID3D11VertexShader*      g_pVertexShader = NULL;
		ID3D11InputLayout*       g_pInputLayout = NULL;
		ID3D11Buffer*            g_pVertexConstantBuffer = NULL;
		ID3D10Blob *             g_pPixelShaderBlob = NULL;
		ID3D11PixelShader*       g_pPixelShader = NULL;
		ID3D11SamplerState*      g_pFontSampler = NULL;
		ID3D11ShaderResourceView*g_pFontTextureView = NULL;
		ID3D11RasterizerState*   g_pRasterizerState = NULL;
		ID3D11BlendState*        g_pBlendState = NULL;
		ID3D11DepthStencilState* g_pDepthStencilState = NULL;
		int                      g_VertexBufferSize = 5000, g_IndexBufferSize = 10000;

		int init(HINSTANCE wc, HWND hwnd,
			ID3D11Device* g_pd3dDevice,
			ID3D11DeviceContext*     g_pd3dDeviceContext,
			IDXGISwapChain*          g_pSwapChain,
			ID3D11RenderTargetView*  g_mainRenderTargetView
		);
		void renderDrawLists(ImDrawData* draw_data);
		virtual void renderSub();
		void render();

	};

}