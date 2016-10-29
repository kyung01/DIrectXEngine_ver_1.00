#pragma once

#include "DXCore.h"
#include "Graphic\SimpleShader.h"
#include <DirectXMath.h>
#include <memory>
#include <list>
#include "WorldMain.h"


#include "imgui\DirectX\Context.h"
#include "Graphic\GraphicMain.h"
#include "Graphic\RenderTexture.h"
#include "Graphic\Camera.h"
#include "Graphic\Mesh.h"

//TODO Draw triangle with different backgrounds onto three different framebuffers
namespace NDirectX {
	/*
	Each GraphicMain should be assinged one scene and one scene only
	*/
	struct RenderContext{
		std::string name; // id to describe the scene
		std::string description; // describe what this scene is
		Graphic::GraphicMain main;
		Graphic::NScene::Scene scene;
	};

	class KContext
		: public DXCore
	{

	private:
		World::WorldMain world;
		std::list<RenderContext> m_renderContexts;

		// Initialization helper methods - feel free to customize, combine, etc.
		void LoadShaders();
		void CreateMatrices();
		void CreateBasicGeometry();

		// Buffers to hold actual geometry data
		//ID3D11Buffer* vertexBuffer;
		//ID3D11Buffer* indexBuffer;

		// Wrappers for DirectX shaders to provide simplified functionality
		Graphic::SimpleVertexShader* vertexShader;
		Graphic::SimpleFragmentShader* pixelShader;

		// The matrices to go from model space to screen space
		DirectX::XMFLOAT4X4 worldMatrix;
		DirectX::XMFLOAT4X4 viewMatrix;
		DirectX::XMFLOAT4X4 projectionMatrix;

		// Keeps track of the old mouse position.  Useful for 
		// determining how far the mouse moved in a single frame.
		POINT prevMousePos;
		std::unique_ptr<Graphic::Mesh>
			triangle, square, diamond;
		void renderMesh(Graphic::Mesh* mesh);
		void renderMesh(Graphic::Mesh mesh);

		Graphic::RenderTexture renderTexture;
	public:
		Graphic::Camera testingCamera;
		~KContext();

		NImGui::Context *m_ui;
		Graphic::GraphicMain m_graphicMain;

		KContext(HINSTANCE hInstance);
		// Overridden setup and game loop methods, which
		// will be called automatically
		void Init();
		void OnResize();
		void Update(float deltaTime, float totalTime);
		void Draw(float deltaTime, float totalTime);

		// Overridden mouse input helper methods
		void OnMouseDown(WPARAM buttonState, int x, int y);
		void OnMouseUp(WPARAM buttonState, int x, int y);
		void OnMouseMove(WPARAM buttonState, int x, int y);
		void OnMouseWheel(float wheelDelta, int x, int y);
	};


}