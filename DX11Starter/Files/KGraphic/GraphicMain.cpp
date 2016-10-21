#include "GraphicMain.h"
void Graphic::GraphicMain::processObject(DumbObject obj) {
	if (obj.shader != NULL) {

		Graphic::Shader *shader = shaders[obj.shader->type];
		shader->use(this->matProjection, this->matView);
	}
}


void Graphic::GraphicMain::rendering(Scene scene)
{
	for (auto cam = scene.cams.begin(); cam != scene.cams.end(); cam++) {
		processCamera(*cam);
		for (auto it = scene.objects.begin(); it != scene.objects.end(); it++) {
			//processShader(it->shader);
			processObject(*it);
			//draw the object
		}
	}
	
}

void Graphic::GraphicMain::render(Graphic::Scene scene)
{
	beginRendering();
	rendering(scene);
	endRendering();
}