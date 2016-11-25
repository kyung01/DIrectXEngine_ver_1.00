#pragma once
#include <d3d11.h>
namespace NGraphic {
	//mainly used for light 
	class DepthTexture {
	private:
		float m_width, m_height;
		ID3D11DepthStencilView *m_depthView = 0;
		ID3D11ShaderResourceView *m_shaderResourceView = 0;
	public:
		float getWidth();
		float getHeight();
		bool init(ID3D11Device* device, int textureWidth, int textureHeight);
		void clear(ID3D11DeviceContext *context);
		ID3D11DepthStencilView * getDepthStencilView();
		ID3D11ShaderResourceView* getShaderResourceView();

	};
}