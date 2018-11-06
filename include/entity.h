#pragma once

#include <glm/vec3.hpp> //  glm vec3 type
#include <glm/geometric.hpp> // glm mathematics
#include "Transform.h"
#include "Physics.h"
#include <string>

class Entity
{
private:

	virtual void Init();
protected:
	
	/*!
	 * \brief An object to hold the position, orientation and scale of the Entity in world space.

	 */
	Transform transform;

	Physics physics;

	glm::vec3 vec3Direction;
	glm::vec3 vec3Velocity;
	glm::vec3 vec3Hull;

	float fSpeed; // Used as part of velocity calculation.
	float fTimeScale;
	float fAngle;
public:

	/*!
	 * \brief Default constructor
	 */
	Entity(std::string entityName);

	/*!
	* \brief Virtual destructor to ensure child classes are properly destructed.

	*/
	virtual ~Entity() {};

	/*!
	 * \brief The id of this entity

	 */
	unsigned int id;

	/*!
	* \brief The id of this entity

	*/
	std::string name;

	/*!
	 * \brief An integer handle to a script attached to this entity, or -1 if no script is attached.

	 */
	int scriptHandle;

	/*!
	 * \brief Get a new unique id for an entity.
	 * \return A new entity id. Guaranteed to be unique.

	 */
	static unsigned int getNewId();

	/*!
	 * \brief The id of the next entity to be created.

	 */
	static unsigned int nextNewId;

	/*!
	\ brief Retrieve a reference to the vec3 of the entities' position.
	\ return glm::vec3 Reference
	*/
	glm::vec3& Position();

	/*!
	\ brief Retrieve a reference to the vec3 of the entities' rotation.
	\ return glm::vec3 Reference
	*/
	glm::vec3& Rotation();

	/*!
	\ brief Retrieve a reference to the entity's scale vector
	\ return glm::vec3 Reference
	*/
	glm::vec3& Scale();

	/*!
	\ brief Retrieve a reference to the V3D of the entities' direction.
	\ return glm::vec3 Reference
	*/
	glm::vec3& Direction();

	/*!
	\ brief Retrieve a reference to the vec3 of the entities' velocity.
	\ return glm::vec3 Reference
	*/
	glm::vec3& Velocity();

	/*!
	\ brief Retrieve a reference to the vec3 of the entities' hull.
	\ return glm::vec3 Reference
	*/
	glm::vec3& Hull();

	/*!
	\brief Set the Entity's transform.
	\param newTransform The transform to use for this Entity from now on.
	*/
	void SetTransform(Transform newTransform);

	/*!
	\brief Set size of the entity with a float.
	\param float Size to use.
	*/
	void SetPhysics(Physics newPhysics);

	/*!
	\brief Set size of the entity with a float.
	\param float Size to use.
	*/
	void SetSize(float fSizeIn);

	/*!
	\brief Return size of entity as a float.
	\return float Get size.
	*/
	float Size();

	/*!
	\ brief Add to the angle variable.
	\ param int
	*/
	void AddAngle(int m_iAngle);

	/*!
	\ brief Set the angle variable.
	\ param float
	*/
	void SetAngle(float m_fAngle);

	/*!
	\ brief Return the angle variable.
	\ return float
	*/
	float Angle();

	/*!
	\ brief Set the timescale
	\ param float
	*/
	void SetTimeScale(float m_fTimeScale);

	/*!
	\ brief Return the timescale
	\ return float
	*/
	float TimeScale();

	/*!
	\brief Update the entity.
	\ param float timescale
	*/
	virtual void Update(float m_fTimePassed);

};