#pragma once
#include "Graphic\GraphicMain.h"
#include "Graphic\RenderTexture.h"

namespace NImGui {
	class UIMain {
	private:
		Graphic::GraphicMain * graphicMain = 0;

	protected:
	public:
		void init(Graphic::GraphicMain * graphicMain);
		void render();
	};
}