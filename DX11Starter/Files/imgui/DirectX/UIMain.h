#pragma once
#include "Graphic\GraphicMain.h"
#include "Graphic\RenderTexture.h"

namespace NImGui {
	class UIMain {
	private:
		Graphic::GraphicMain * graphicMain = 0;

	protected:
	public:
		static Graphic:: RenderTexture *example_texture;
		void init(Graphic::GraphicMain * graphicMain);
		void render();
	};
}