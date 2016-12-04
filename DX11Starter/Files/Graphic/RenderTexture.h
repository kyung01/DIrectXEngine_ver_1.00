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
		int m_width, m_height;
		ID3D11Texture2D* m_renderTargetTexture;
		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11ShaderResourceView* m_shaderResourceView;
	public:
		
		RenderTexture();
		RenderTexture(const RenderTexture&);
		~RenderTexture();

		bool init(ID3D11Device*, int, int);
		void Shutdown();

		void set(ID3D11DeviceContext*, ID3D11DepthStencilView*);
		void clear(ID3D11DeviceContext*, float, float, float, float);
		ID3D11ShaderResourceView* getShaderResourceView();
		ID3D11RenderTargetView * getRenderTargetView();

		D3D11_VIEWPORT viewport;
		int getWidth();
		int getHeight();
	};


}
#endif