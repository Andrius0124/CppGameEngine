/**
* @Author	Andrius Murauskas
* @date	16th April 2017
* @brief	Text to render as UI element 
*/
#pragma once
#include "entity.h"
#include "Resource.h"
#include <GL\glut.h>
class Text : public Entity
{
private:
	void Init();
protected:
	char* msg;
	glm::vec3 color;

public:
	Text(std::string name);
	char* getMsg();
	void setMsg(char* _msg);
	void setColor(float R, float G, float B);
	glm::vec3 getColor();


};

