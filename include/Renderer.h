/**
 * @brief	A class responsible for rendering drawable entities.
 *
 */

#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "drawable.h"
#include "Text.h"
#include "Sprite.h"
#include "GL/GL.h"
#include "GL/GLU.h"

class Renderer {

public:
	/*!
	* \brief The default constructor.
	* 
	*/
	Renderer(SDL_Window* applicationWindow);

	/*!
	* \brief The default destructor.
	* 
	*/
	~Renderer();


	/*!
	* \brief Clears the render buffer.
	* 
	*/
	void clear();

	/*!
	* \brief Flushes the contents of the render buffer to the window's display buffer.
	* 
	*/
	void flush();

	/*!
	* \author Andrius
	*/
	void render(Text* text);

	/*!
	* \author Andrius
	*/
	void render(Sprite* sprite);

	/*!
	* \brief Render a drawable entity within the scene.
	* \param A pointer to the Drawable to render.
	* 
	*/
	void render(Drawable* drawable);

	/*!
	* \brief Loads a model into video memory from a mesh resource.
	* \param A pointer to the MeshResource to create the model from.
	* 
	*/
	unsigned int loadModel(MeshResource* mesh);

	/*!
	* \brief Loads a texture into video memory from an image resource.
	* \param A pointer to the ImageResource to create the texture from.
	* 
	*/
	unsigned int loadTexture(ImageResource* image);

	/*!
	* \brief Frees a texture from memory.
	* \param The OpenGL ID of the texture to free from video memory.
	* 
	*/
	void freeTexture(GLuint textureId);

	/*!
	* \brief Frees a model from memory.
	* \param The OpenGL ID of the model to free from video memory.
	* 
	*/
	void freeModel(GLuint modelId);

	/*!
	* \brief Frees up memory with the assumption that all resources have already been freed by the resource manager.
	* \param The OpenGL ID of the model to free from video memory.
	* 
	*/
	void dispose();

	/*!
	* \brief Set an entity which should be used to control the camera.
	* \param A pointer to the chosen entity.
	* 
	*/
	void setCameraEntity(Entity* cameraEntity);

	/*!
	* \brief Position the viewpoint at the position and orientation of the camera entity.
	* 
	*/
	void positionCamera();

private:
	/*!
	* \brief A list of all displayLists currently in video memory.
	* 
	*/
	std::vector<GLuint> displayLists;

	/*!
	* \brief A pointer to the window to render to.
	* 
	*/
	SDL_Window* window;

	/*!
	* \brief A pointer to the entity to use as the camera.
	* 
	*/
	Entity* camera;

	/*!
	* \brief Setup the OpenGL viewport and projection matrix for 3D rendering.
	* 
	*/
	void setProjection();

	/*!
	* 
	*/
	void glDisable2D();

	/*!
	*
	*/
	void glEnable2D();

};

/*!
* \brief The pseudo-singleton Renderer instance. Used like a singleton, but the usual singleton restrictions are not enforced. It is a regular class.
* 
*/
extern Renderer* renderer;

#endif // !RENDERER_H
