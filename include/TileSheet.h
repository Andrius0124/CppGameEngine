
#pragma once
/**
* @Author	Andrius Murauskas
* @date		1st May 2017
* @brief	TileSheet class for Sprite class
			
*
*/

#include<glm\glm.hpp>
#include "Resource.h"
#include "ResourceManager.h"
class TileSheet
{
public:

	void init(GLuint _Texture,  glm::ivec2 tileDims) {
		texture = _Texture;
		dims = tileDims;
	}
	// returns new uvs coordinates   
	glm::vec4 getUVS() {
	
		int xTile = index % dims.x;
		int yTile = index / dims.x;
		glm::vec4 uvs;

		uvs.x = xTile / (float)dims.x;
		uvs.y = yTile / (float)dims.y;
		uvs.z = 1.0f / dims.x;
		uvs.w = 1.0f / dims.y;

		return uvs;

	}

	GLuint  texture;
	glm::ivec2  dims;
	int index = 1;
private:
	
};

