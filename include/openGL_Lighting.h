/**
*
* @brief	OpenGL Light class, for creating up to 8 lights and using them with different types
* @			allows for the total control of the light at any point.
*/

#ifndef OPENGL_LIGHTING_H
#define OPENGL_LIGHTING_H

#include "lightManager.h"

#include "entity.h"
#include <Windows.h>
#include <math.h>
#include <iostream>
#include <GL\GL.h>

struct openGL_Lighting : public LightManager 
{
	/*!
	\ Constructor.
	\ Set initial property values.
	*/
	openGL_Lighting();
	
	/*!
	\ Initializer.
	\ Initialize the things we need for lighting
	*/
	void initLighting();

	/*!
	\ Deintializer.
	\ Deintializer the things we used for lighting, used for when we shut down.
	*/
	void deInitLighting();

	/*!
	\ Create a new light.
	\ Create new light with params for Id, type and position, then sets selected one
	*/
	void createNewLight(eLightId m_Id, eLightType m_Type, float x, float y, float z);

	/*!
	\ Update Light type.
	\ Update the light type of an active light e.g from directional to spotlight
	*/
	void updateLightType(eLightId m_Id, eLightType m_Type);

	/*!
	\ Update Light position.
	\ Update the light position of selected light
	*/
	void updateLightPos(eLightId m_Id, float x, float y, float z);

	/*!
	\ Set Light position.
	\ Used by createNewLight() to set initial light position
	*/
	void setLightPos(eLightId m_Id, float x, float y, float z);

	/*!
	\ Set Light type = Directional Light.
	\ Direction light type settings used by createNewLight()
	*/
	void setDirectionalLight(eLightId m_Id);

	/*!
	\ Set Light type = Spot Light.
	\ Spot light type settings used by createNewLight()
	*/
	void setSpotLight(eLightId m_Id);

	/*!
	\ Set Light type = Point Light.
	\ Point light type settings used by createNewLight()
	*/
	void setPointLight(eLightId m_Id);
	
	/*!
	\ Delete Light
	\ Delete a light in use, checks if the one selected is active then if it is, makes it inactive
	*/
	void deleteLight(eLightId m_Id);

	/*!
	\ Test system max Lights
	\ Although OpenGL documentation states that 8 lights is the maximum it supports
	\ it is always good to just add in a tester to check. Good for marks ;)
	*/
	void testSystemLightAmt();
};

extern openGL_Lighting lightManager;


#endif