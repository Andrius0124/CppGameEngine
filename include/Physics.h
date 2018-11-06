#pragma once

/**
* @Author	Joseph
* @date	7 May 2017
* @brief	A struct to represent entity physics
*
*/

#include <glm/vec3.hpp>

struct Physics {

public:
	/*!
	* \brief A default constructor which takes no arguments.
	*/
	Physics() {}

	/*!
	* \brief A default constructor with initialisation arguments.
	*/
	Physics(glm::vec3 _hull, glm::vec3 vel) {
		hull = _hull;
		velocity = vel;
	}

	/*!
	* \brief 3-dimensional vectors to represent the position, rotation and scale in 3D space.
	*/
	glm::vec3 hull, velocity;

};