/**
* @Author	Dale Curry
* @date
* @brief
*/

#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H

class LightManager
{
public:
	
	enum eLightId
	{
		Light0,
		Light1,
		Light2,
		Light3,
		Light4,
		Light5,
		Light6,
		Light7,
	};

	enum eLightType
	{
		DirectionalLight,
		SpotLight,
		PointLight
	};

	virtual void initLighting() = 0;
	virtual void deInitLighting() = 0;
	virtual void createNewLight(eLightId m_Id, eLightType m_Type, float x, float y, float z) = 0;
	virtual void updateLightType(eLightId m_Id, eLightType m_Type) = 0;
	virtual void updateLightPos(eLightId m_Id, float x, float y, float z) = 0;
	virtual void setLightPos(eLightId m_Id, float x, float y, float z) = 0;

	virtual void deleteLight(eLightId m_Id) = 0;

};



#endif
