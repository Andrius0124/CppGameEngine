/**
* @Author	Joseph Radu
* @date		3rd January 2017
* @brief	Entity. Velocity, position and angles.
*
*/

#include "entity.h"

// Declare the static variable
unsigned int Entity::nextNewId;

Entity::Entity(std::string entityName) {
	id = getNewId();
	scriptHandle = -1;
	name = entityName;
}

unsigned int Entity::getNewId()
{
	// Return the value of nextNewId before it is incremented
	return nextNewId++;
}

void Entity::Init()
{

}

void Entity::Update(float dt)
{

}

glm::vec3& Entity::Position()
{
	return transform.position;
}

glm::vec3& Entity::Rotation() {
	return transform.rotation;
}

glm::vec3& Entity::Scale() {
	return transform.scale;
}

glm::vec3& Entity::Direction()
{
	return vec3Direction;
}

glm::vec3& Entity::Velocity()
{
	return physics.velocity;
}

glm::vec3& Entity::Hull()
{
	return physics.hull;
}

void Entity::SetTransform(Transform m_newTransform)
{
	transform = m_newTransform;
}

void Entity::SetPhysics(Physics newPhysics)
{
	physics = newPhysics;
}

void Entity::AddAngle(int m_iAngle)
{
	fAngle += m_iAngle;
}

void Entity::SetAngle(float m_fAngle)
{
	fAngle = m_fAngle;
}

float Entity::Angle()
{
	return fAngle;
}

void Entity::SetTimeScale(float m_fTimeScale)
{
	fTimeScale = m_fTimeScale;
}

float Entity::TimeScale()
{
	return fTimeScale;
}