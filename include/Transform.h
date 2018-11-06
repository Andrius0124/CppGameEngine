/**
 * @Author	Addo White 
 * @date	9 March 2017
 * @brief	A struct to represent position, orientation and scale in 3D space.
 *
 */

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/vec3.hpp>

struct Transform {

public:
	/*!
	 * \brief A default constructor which takes no arguments.
	 * @Author Addo White
	 */
	Transform() {}

	/*!
	 * \brief A default constructor with initialisation arguments.
	 * @Author Addo White
	 */
	Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl) {
		position = pos;
		rotation = rot;
		scale    = scl;
	}

	/*!
	 * \brief 3-dimensional vectors to represent the position, rotation and scale in 3D space.
	 * @Author Addo White
	 */
	glm::vec3 position, rotation, scale;

};

#endif // !TRANSFORM_H