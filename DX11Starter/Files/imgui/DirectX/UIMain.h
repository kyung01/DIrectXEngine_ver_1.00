#pragma once
#include "Graphic\GraphicMain.h"
#include "rendertextureclass.h"
namespace NImGui {
	class UIMain {
	private:
		Graphic::GraphicMain * graphicMain = 0;

	protected:
	public:
		static RenderTextureClass *example_texture;
		void init(Graphic::GraphicMain * graphicMain);
		void render();
	};
}