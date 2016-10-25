#pragma once
#include "Graphic\GraphicMain.h"
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