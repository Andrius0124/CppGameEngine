
/**
* @Author	Andrius Murauskas
* @date		17 April 2016
* @brief	Sprite class 
*
*/

#pragma once
#include "entity.h"
#include <GL\glut.h>
#include "TileSheet.h"
#include <glm\glm.hpp>

class Sprite : public Entity  
{
public:
	Sprite(std::string name);
	void setTextureID( GLuint _textureID);
	GLuint getTextureID();
	int width, height;
	bool animation;
	TileSheet tileTexture;
	void initAnimation();
	void Update(float dt);
	glm::ivec2 Tiles;
protected:
	GLuint textureID;
private:
	int frames = 0;
	void Init();
	
};

