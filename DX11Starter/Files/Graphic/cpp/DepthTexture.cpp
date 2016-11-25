#include "Graphic/DepthTexture.h"
using namespace NGraphic;

float DepthTexture::getWidth()
{
	return m_width;
}

float DepthTexture::getHeight()
{
	return m_height;
}
void DepthTexture::clear(ID3D11DeviceContext* deviceContext )  {

	deviceContext->ClearDepthStencilView(m_depthView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
bool DepthTexture::init(ID3D11Device * device, int width, int height)
{
	m_width = width;
	m_height = height;

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	//depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//R24G8_TYPELESS 
	//depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;//DXGI_FORMAT_R8G8B8A8_UNORM
	depthStencilDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	/*
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//depthStencilDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;// | D3D11_BIND_SHADER_RESOURCE;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	*/

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Texture2DMS.UnusedField_NothingToDefine = 0;
	descDSV.Texture2D.MipSlice = 0;
	descDSV.Flags = 0; // not read only

	ID3D11Texture2D* depthBufferTexture;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = -1;



	HRESULT result;
	result = device->CreateTexture2D(&depthStencilDesc, 0, &depthBufferTexture); 	
	if (FAILED(result))
	{
		return false;
	}
	result = device->CreateDepthStencilView(depthBufferTexture, &descDSV, &m_depthView);
	if (FAILED(result))
	{
		return false;
	}
	depthBufferTexture->Release();
	result = device->CreateShaderResourceView(depthBufferTexture, &shaderResourceViewDesc, &m_shaderResourceView);
	if (FAILED(result))
	{
		return false;
	}



	return true;
}

ID3D11DepthStencilView * DepthTexture::getDepthStencilView() {
	return m_depthView;
}
ID3D11ShaderResourceView* DepthTexture::getShaderResourceView() {
	return m_shaderResourceView;

}