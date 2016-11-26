#pragma once
namespace NGame {
	class Context;
	class Entity;
	class Script {	
	public:
		virtual void init(Context& context, Entity& me);
		virtual void update(Context &context, Entity &me, float timeElapsed);
	};
}