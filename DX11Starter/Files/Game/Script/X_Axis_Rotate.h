#pragma once
#include <Game\Script.h>
#include <d3d11.h>
#include <SimpleMath.h>
namespace NGame {
	namespace NScript {
		class X_Axis_Rotate :public Script {
		protected:
			float progress;
			float progressDir;
			DirectX::SimpleMath::Quaternion rotationInit;
		public:
			void init(Context& context, Entity& me) override;
			void update(Context &context, Entity &me, float timeElapsed) override;

		};
	}
}