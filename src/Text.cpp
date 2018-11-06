#include "Text.h"


Text::Text(std::string name) : Entity(name)
{
	Init();
}
void Text::Init()
{
	color = glm::vec3(0.0f,0.5f,0.5f); // default color 

}

char * Text::getMsg()
{
	return msg;
}
void Text::setMsg(char* _msg)
{
	msg = _msg;
}
void Text::setColor(float R, float G, float B)
{
	color = glm::vec3(R,G,B);
}
glm::vec3 Text::getColor()
{
	return color;
}