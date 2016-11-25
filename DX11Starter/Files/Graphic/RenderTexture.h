////////////////////////////////////////////////////////////////////////////////
// Filename: rendertextureclass.h
// from http://www.rastertek.com/dx11tut22.html
////////////////////////////////////////////////////////////////////////////////
#ifndef _RENDERTEXTURECLASS_H_
#define _RENDERTEXTURECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>

namespace NGraphic {

	////////////////////////////////////////////////////////////////////////////////
	// Class name: RenderTextureClass
	////////////////////////////////////////////////////////////////////////////////
	class RenderTexture
	{
	private:
		ID3D11Texture2D* m_renderTargetTexture;
		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11ShaderResourceView* m_shaderResourceView;
	public:
		RenderTexture();
		RenderTexture(const RenderTexture&);
		~RenderTexture();

		bool Initialize(ID3D11Device*, int, int);
		void Shutdown();

		void SetRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*);
		void ClearRenderTarget(ID3D11DeviceContext*, float, float, float, float);
		ID3D11ShaderResourceView* GetShaderResourceView();
		ID3D11RenderTargetView * getRenderTargetView();

		D3D11_VIEWPORT viewport;
	};


}
#endif