#pragma once
#include "Graphic\GraphicMain.h"
#include "Graphic\RenderTexture.h"

namespace NImGui {
	class UIMain {
	private:
		Graphic::GraphicMain * graphicMain = 0;
		void render(std::map<Graphic::RENDER_TYPE,std::shared_ptr<Graphic::RenderTexture>> renderTexutres);
		void render(std::map<Graphic::RENDER_TYPE, std::shared_ptr<Graphic::DepthTexture>> depthTextures);
		void render(std::map<int, Graphic::DepthTexture*> textures);
	protected:
	public:
		void init(Graphic::GraphicMain * graphicMain);
		void render();
	};
}