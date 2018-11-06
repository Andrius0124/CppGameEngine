#include "Sprite.h"

Sprite::Sprite(std::string name) : Entity(name)
{
	Init();
}

void Sprite::Init()
{

	
}

void Sprite::setTextureID(GLuint _textureID)
{
	textureID = _textureID;
}
GLuint Sprite::getTextureID()
{
	return textureID;
}
void Sprite::initAnimation()
{
	tileTexture.init(textureID, Tiles);

}
void Sprite::Update(float dt)
{
	frames++;

	if (tileTexture.texture != 0 &&  frames >250)  // TO DO : change frames to deltTime  
	{
		

		if (tileTexture.index < tileTexture.dims.x *tileTexture.dims.y)
		{
			tileTexture.index++;
			frames = 0;
		}
		else
		{
			tileTexture.index = 0;
			frames = 0;

		}
	}
}