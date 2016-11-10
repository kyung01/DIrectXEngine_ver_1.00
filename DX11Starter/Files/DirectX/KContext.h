#pragma once

#include "DXCore.h"
#include "Graphic\SimpleShader.h"
#include <DirectXMath.h>
#include <memory>
#include <list>
#include "WorldMain.h"


#include "imgui\DirectX\KContext.h"
#include "Graphic\Assets.h"
#include "Graphic\GraphicMain.h"

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
		NImGui::KContext m_ui;
		World::WorldMain world;

	public:
		std::list<RenderContext> m_renderContexts;
		Graphic::NScene::Camera testingCamera; //TODO delete this?
		~KContext();


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