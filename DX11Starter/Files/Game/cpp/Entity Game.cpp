#include <Game\Context.h>
#include <Game\Entity.h>
#include <Game\Script.h>
using namespace NGame;
Entity::Entity():
	scale(1,1,1)
{
}

Entity Entity::setPos(float x, float y, float z)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
	for (auto it = m_graphicObjects.begin(); it != m_graphicObjects.end(); it++) {
		it->get()->setPos(this->position);
	}
	return *this;
}

void NGame::Entity::setRotation(Quaternion rotation)
{
	this->rotation = rotation;
	for (auto it = m_graphicObjects.begin(); it != m_graphicObjects.end(); it++) {
		it->get()->setRotation(this->rotation);
	}
}

void NGame::Entity::addScript(Context context, std::shared_ptr<Script> script)
{
	script.get()->init(context, *this);
	m_scripts.push_back(script);
}


void Entity::update(Context &context, float timeElapsed)
{

	for (auto it = m_scripts.begin(); it != m_scripts.end(); it++) {
		(**it).update(context, *this, timeElapsed);
	}
}
