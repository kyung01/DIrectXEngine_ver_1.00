#pragma once
#include <memory>
#include <list>
//#include <Game\Script.h>
//#include <Game\Context.h>
#include <d3d11.h>
#include <SimpleMath.h>
namespace NGraphic {
	namespace NScene{
		class Object;
	}
}
namespace NGame {
	class Script;
	class Context;
	class Entity {
		std::list<std::shared_ptr<Script>> m_scripts;
	public:
		DirectX::SimpleMath::Vector3 position;
		DirectX::SimpleMath::Vector3 scale;
		DirectX::SimpleMath::Quaternion rotation;
		std::list<std::shared_ptr<NGraphic::NScene::Object>> m_graphicObjects;
		Entity();
		void update(Context &context, float timeElapsed);
		Entity setPos(float x, float y, float z);
		void setRotation(DirectX::SimpleMath::Quaternion rotation);
		void addScript(Context context, std::shared_ptr<Script> script);

		
	};
}