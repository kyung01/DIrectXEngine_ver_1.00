#pragma once
#include <d3d11.h>
namespace Graphic {
	//mainly used for light 
	class DepthTexture {
	private:
		float m_width, m_height;
	public:
		float getWidth();
		float getHeight();
		ID3D11DepthStencilView *m_depthView = 0;
		bool init(ID3D11Device* device, int textureWidth, int textureHeight);
	};
}