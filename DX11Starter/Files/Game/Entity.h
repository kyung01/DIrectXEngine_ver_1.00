#pragma once
#include <memory>
#include <Graphic\Object.h>
#include <list>
namespace NGame {
	class Entity {
		std::list<std::shared_ptr<NGraphic::NScene::Object>> m_graphicObjects;

		
	};
}