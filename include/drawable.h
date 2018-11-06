#pragma once


#ifdef _WINDOWS
#include <windows.h>
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "entity.h"
#include <vector>
#include <iostream>
#include <string>
#include "Resource.h"

class Drawable : public Entity
{
private:
	void Init();
protected:
	/*!
	\brief The mesh to render for this entity (or nullptr if this entity should have no mesh).

	*/
	MeshResource* mesh;

	std::vector<glm::vec3> vertices; // A vector of V3D vertices (storage of polygons essentially).
	glm::vec3 vec3Colour;
public:
	/*!
	\brief Default constructor.
	*/
	Drawable(std::string entityName);

	/*!
	\brief Create a cube using the vertices.
	*/
	void Init_Cube();

	/*!
	\brief Create a plane using the vertices.
	*/
	void Init_Plane();

	/*!
	\brief Get the mesh used for this drawable.

	*/
	MeshResource* getMesh();

	/*!
	\brief Set the mesh to used for this drawable.

	*/
	void setMesh(MeshResource* newMesh);

	/*!
	\brief
	*/
	glm::vec3& Colour();


	/*!
	\ brief Return a reference to the vector of V3D vertices.
	\ return V3D Reference
	*/
	std::vector<glm::vec3>& Vertices();

	/*!
	\brief Draw function.
	*/
	void Draw();

	/*!
	\brief Entity update function.
	\ param float timescale
	*/
	void Update(float dt);
};