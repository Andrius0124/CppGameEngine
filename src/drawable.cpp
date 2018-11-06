/**
* @Author	Joseph Radu
* @date		1st December 2016
* @brief	Drawable for openGL
*
*/

#include "drawable.h"

void Drawable::Init()
{
	vec3Colour = glm::vec3(255, 255, 255);

	fSpeed = 1;
	fTimeScale = 1;
	fAngle = 0;
}

Drawable::Drawable(std::string entityName) : Entity(entityName)
{
	Init();
}

std::vector<glm::vec3>& Drawable::Vertices()
{
	return vertices;
}

void Drawable::Init_Cube()
{
	vertices.clear();
	vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));	// top face
	vertices.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 0.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
	vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));	// front face
	vertices.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
	vertices.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
	vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));	// right face
	vertices.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
	vertices.push_back(glm::vec3(0.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));	// left face
	vertices.push_back(glm::vec3(-1.0f, 0.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
	vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));	// bottom face
	vertices.push_back(glm::vec3(0.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
	vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));	// back face
	vertices.push_back(glm::vec3(-1.0f, 0.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	vertices.push_back(glm::vec3(0.0f, -1.0f, -1.0f));
	std::reverse(vertices.begin(), vertices.end());
}
void Drawable::Init_Plane()
{
	vertices.clear();
	vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));	// top face
	vertices.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 0.0f, -1.0f));
	vertices.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
	std::reverse(vertices.begin(), vertices.end());
}

void Drawable::Draw()
{
	glPushMatrix();

	glTranslatef(transform.position.x, transform.position.y, transform.position.z);
	glRotatef(transform.rotation.z, 0, 0, 1);	// rotate the drawable about the z-axis
	glRotatef(transform.rotation.y, 0, 1, 0);	// rotate the drawable about the y-axis
	glRotatef(transform.rotation.x, 1, 0, 0);	// rotate the drawable about the x-axis
	glColor3f(vec3Colour.x, vec3Colour.y, vec3Colour.z);
	glScalef(transform.scale.x, transform.scale.y, transform.scale.z);

	glBegin(GL_POLYGON);

	for (auto & i : vertices) {
		glVertex3f(i.x, i.y, i.z);
	}
	glEnd();
	glPopMatrix();

}

void Drawable::Update(float dt)
{
}

glm::vec3& Drawable::Colour()
{
	return vec3Colour;
}


/*!
* \brief Get the mesh for this drawable.
* \return A pointer to the MeshResource for this drawable entity.
* \author Addo White
*/
MeshResource* Drawable::getMesh()
{
	return mesh;
}


/*!
* \brief Set the mesh for this drawable.
* \param The mesh to use when rendering this entity.
* \author Addo White
*/
void Drawable::setMesh(MeshResource* newMesh)
{
	mesh = newMesh;
}