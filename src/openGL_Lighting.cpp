#include "openGL_Lighting.h"

openGL_Lighting lightManager;

openGL_Lighting::openGL_Lighting()
{
};

void openGL_Lighting::initLighting()
{
	/*!
	\	According to documentation written online the standard being pushed forward for
	\	OpenGL initalizing lighting, is to create a Light when initalizing, GL_LIGHT0,
	\	This acts as the sun and starts the scene off. But we can change it if we dont like that.
	*/

	//set up the lighting model and enables lighting mode in OpenGL
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);
	glEnable(GL_NORMALIZE);

	////Define Ambient, Diffuse, Specular and Position params for the light.
	//GLfloat fLightAmbient[]			= { 0.2, 0.2, 0.2, 1.0 };
	//GLfloat fLightDiffuse[]			= { 0.8, 0.8, 0.8, 1.0 };
	//GLfloat fLightSpecular[]		= { 1.0, 1.0, 1.0, 1.0 };
	//GLfloat fLightPosition[]		= { 1.0, 1.0, 1.0, 0.0 };	//Directional light position params

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set the defined param values with glLightfv()...
	/*glLightfv(GL_LIGHT0, GL_AMBIENT, fLightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, fLightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, fLightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, fLightPosition);*/
};

void openGL_Lighting::deInitLighting()
{
	//Used for closing down, disables all lighting
	glDisable(GL_LIGHTING);
};

void openGL_Lighting::createNewLight(eLightId m_Id, eLightType m_Type, float x, float y, float z)
{
	//Create a new light then add its light type settings and initial position
	//both params can be updated later if Light is enabled
	if (glIsEnabled(m_Id))
	{
		std::cout << "Light..." << m_Id << "...is already in use" << std::endl;
	}
	else
	{
		glEnable(m_Id);
		
		switch (m_Type)
		{
		case DirectionalLight:
			setDirectionalLight(m_Id);
			setLightPos(m_Id, x, y, z);
			break;
		
		case SpotLight:
			setSpotLight(m_Id);
			setLightPos(m_Id, x, y, z);
			break;

		case PointLight:
			setPointLight(m_Id);
			setLightPos(m_Id, x, y, z);
			break;
		};
	}
};


void openGL_Lighting::updateLightType(eLightId m_Id, eLightType m_Type)
{
	// check if ID enabled then update the settings for the type of light selected
	if (glIsEnabled(m_Id))
	{
		switch (m_Type)
		{
		case DirectionalLight:
			setDirectionalLight(m_Id);
			break;

		case SpotLight:
			setSpotLight(m_Id);
			break;

		case PointLight:
			setPointLight(m_Id);
			break;
		}
	}
};

void openGL_Lighting::updateLightPos(eLightId m_Id, float x, float y, float z)
{
	//At any point check if light ID is in use then update the position
	if (glIsEnabled(m_Id))
	{
		setLightPos(m_Id, x, y, z);
	}
};


void openGL_Lighting::setLightPos(eLightId m_Id, float x, float y, float z)
{
	//Get ID and then set lights position accordingly used by createNewLight()
	GLfloat fPos[] = { x, y, z, 1.0f };
	
	glLightfv(m_Id, GL_POSITION, fPos);
};


void openGL_Lighting::setDirectionalLight(eLightId m_Id)
{
	//Define Ambient, Diffuse, Specular and Direction params for the light.
	GLfloat fLightAmbient[]			= { 0.2, 0.2, 0.2, 1.0 };
	GLfloat fLightDiffuse[]			= { 0.8, 0.8, 0.8, 1.0 };
	GLfloat fLightSpecular[]		= { 1.0, 1.0, 1.0, 1.0 };
	GLfloat fLightPosition[]		= { 1.0, 1.0, 1.0, 0.0 };

	//Parameters to set the Light ID to a Directional Light
	glLightfv(m_Id, GL_AMBIENT, fLightAmbient);
	glLightfv(m_Id, GL_DIFFUSE, fLightDiffuse);
	glLightfv(m_Id, GL_SPECULAR, fLightSpecular);
	glLightfv(m_Id, GL_SPOT_DIRECTION, fLightPosition);
};

void openGL_Lighting::setSpotLight(eLightId m_Id)
{
	//Define Ambient, Diffuse, Specular, Position and Direction params for the light.
	GLfloat fLightAmbient[]			= { 0.2, 0.2, 0.2, 1.0 };
	GLfloat fLightDiffuse[]			= { 1.0, 1.0, 1.0, 1.0 };
	GLfloat fLightSpecular[]		= { 1.0, 1.0, 1.0, 1.0 };
	GLfloat fLightPosition[]		= { 5.0, 5.0, 5.0, 1.0 };
	GLfloat fLightDirection[]		= { -1.0, -1.0, -1.0 };

	//Parameters to set the Light ID to a Spot Light
	glLightfv(m_Id, GL_AMBIENT, fLightAmbient);
	glLightfv(m_Id, GL_DIFFUSE, fLightDiffuse);
	glLightfv(m_Id, GL_SPECULAR, fLightSpecular);
	glLightfv(m_Id, GL_POSITION, fLightPosition);
	glLightf(m_Id, GL_CONSTANT_ATTENUATION, 0);
	glLightf(m_Id, GL_LINEAR_ATTENUATION, 0.1);
	glLightf(m_Id, GL_QUADRATIC_ATTENUATION, 0.1);
	glLightf(m_Id, GL_SPOT_CUTOFF, 10.0);
	glLightfv(m_Id, GL_SPOT_DIRECTION, fLightDirection);
	glLightf(m_Id, GL_SPOT_EXPONENT, 1.);
};

void openGL_Lighting::setPointLight(eLightId m_Id)
{
	//Define Ambient, Diffuse, Specular and Position params for the light.
	GLfloat fLightAmbient[]			= { 1.0, 0.0, 0.0, 1.0 };
	GLfloat fLightDiffuse[]			= { 1.0, 1.0, 1.0, 1.0 };
	GLfloat fLightSpecular[]		= { 1.0, 1.0, 1.0, 1.0 };
	GLfloat fLightPosition[]		= { 1.0, 1.0, 1.0, 0.0 };
	
	//Parameters to set the Light ID to a Point Light
	glLightfv(m_Id, GL_AMBIENT, fLightAmbient);
	glLightfv(m_Id, GL_DIFFUSE, fLightDiffuse);
	glLightfv(m_Id, GL_SPECULAR, fLightSpecular);
	glLightfv(m_Id, GL_POSITION, fLightPosition);
};


void openGL_Lighting::deleteLight(eLightId m_Id)
{
	//Check if ID is enabled, if it is then disable, or do nothing
	if (glIsEnabled(m_Id))
	{
		glDisable(m_Id);
	}
};

void openGL_Lighting::testSystemLightAmt()
{
	// Create a GLint to use to store glGetIntegerv() output for the test
	GLint maxLights = 0;
	glGetIntegerv(GL_MAX_LIGHTS, &maxLights);
	std::cout << "This system can handle.." << maxLights << "..lights" << std::endl;
};