#include <GL\glut.h>
#include "Renderer.h"
#include "Utils.h"

Renderer* renderer = nullptr;

Renderer::Renderer(SDL_Window* applicationWindow) {
	window = applicationWindow;
	camera = nullptr;

	// Set the version of OpenGL.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	// Create the OpenGL rendering context
	SDL_GLContext context = SDL_GL_CreateContext(window);

	// Disable vsync
	SDL_GL_SetSwapInterval(0);

	// If it failed
	if (context == NULL) {
		std::cout << "SDL context creation has failed:" << SDL_GetError() << std::endl;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_TEXTURE_2D);

	setProjection();
}

Renderer::~Renderer() {
	// Free all the display lists from graphics memory
	for (unsigned int displayList : displayLists) {
		glDeleteLists(displayList, 1);
	}
}

void Renderer::setCameraEntity(Entity* cameraEntity) {
	camera = cameraEntity;
}

void Renderer::positionCamera() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (camera != nullptr) {
		glm::vec3 cameraPosition = camera->Position();
		glm::vec3 cameraRotation = camera->Rotation();

		// Use the assigned camera transform to transform the world
		// To give the illusion of camera movement
		float verticalDampening = cosf(Utils::degToRad(cameraRotation.x));
		gluLookAt(
			cameraPosition.x, cameraPosition.y, cameraPosition.z,

			// Camera orientation
			cameraPosition.x + sinf(Utils::degToRad(cameraRotation.y)) * 10 * verticalDampening,
			cameraPosition.y + sinf(Utils::degToRad(cameraRotation.x)) * 10,
			cameraPosition.z + cosf(Utils::degToRad(cameraRotation.y)) * 10 * verticalDampening,

			0, 1, 0
		);
	}
}

void Renderer::setProjection() {
	// Get the current width and height of the window (it may have been resized or fullscreened)
	int windowWidth, windowHeight;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);


	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(90.0f, (GLfloat)windowWidth / (GLfloat)windowHeight, 0.01f, 1000000.0f);
}
void Renderer::render(Text* text)
{
	glEnable2D();

	glLoadIdentity();

	glTranslatef(text->Position().x, text->Position().y, text->Position().z);
	glRotatef(180, 1, 0, 0);	//fliping text
	glRotatef(text->Rotation().z, 0, 0, 1);	// Rotate the text around the z-axis
	glRotatef(text->Rotation().y, 0, 1, 0);	// Rotate the text around the y-axis
	glRotatef(text->Rotation().x, 1, 0, 0);	// Rotate the text around the x-axis
	glScalef(text->Scale().x, text->Scale().y, text->Scale().z);
	glColor3f(text->getColor().x, text->getColor().y, text->getColor().z);

	char *c;
		for (c = text->getMsg(); *c != '\0'; c++)
		{
			glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *c);
		}
		glDisable2D();
}
void Renderer::glEnable2D()

{

	GLint iViewport[4];

	// Get a copy of the viewport

	glGetIntegerv(GL_VIEWPORT, iViewport);



	// Save  copy of the projection matrix so that we can restore it


	glMatrixMode(GL_PROJECTION);

	glPushMatrix();

	glLoadIdentity();



	// Set up the orthographic projection

	glOrtho(iViewport[0], iViewport[0] + iViewport[2],

		iViewport[1] + iViewport[3], iViewport[1], -60, 60);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glLoadIdentity();



	// Make sure depth testing and lighting are disabled for 2D rendering until

	// we are finished rendering in 2D

	glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT);

	glDisable(GL_DEPTH_TEST);

	glDisable(GL_LIGHTING);

}
void Renderer::glDisable2D()

{

	glPopAttrib();

	glMatrixMode(GL_PROJECTION);

	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

	glPopMatrix();

}

void Renderer::render(Sprite* sprite)
{
	glEnable2D();

	glEnable(GL_TEXTURE_2D);

	// Set the primitive color to white
	glColor4f(1.0f, 1.0f, 1.0f,1.0f);



	// Bind the texture to the polygon
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, sprite->getTextureID());

	glTranslatef(sprite->Position().x, sprite->Position().y, sprite->Position().z);

	glRotatef(sprite->Rotation().z, 0, 0, 1);

	glScalef(sprite->Scale().x, sprite->Scale().y, sprite->Scale().z);
	glBegin(GL_POLYGON);
	printf(" %f   ,  %f   ,  %f  ,  %f   \n", sprite->tileTexture.getUVS().x, sprite->tileTexture.getUVS().y, sprite->tileTexture.getUVS().z, sprite->tileTexture.getUVS().w);
	glTexCoord2f(sprite->tileTexture.getUVS().x+ sprite->tileTexture.getUVS().z, sprite->tileTexture.getUVS().y+ sprite->tileTexture.getUVS().w);
	glVertex2f( -sprite->width/2, -sprite->height / 2);
	glTexCoord2f(sprite->tileTexture.getUVS().x, sprite->tileTexture.getUVS().y+ sprite->tileTexture.getUVS().w);
	glVertex2f( sprite->width - sprite->width / 2, -sprite->height / 2);
	glTexCoord2f(sprite->tileTexture.getUVS().x, sprite->tileTexture.getUVS().y);
	glVertex2f( sprite->width - sprite->width/2,  sprite->height - sprite->height / 2);
	glTexCoord2f(sprite->tileTexture.getUVS().x+ sprite->tileTexture.getUVS().z, sprite->tileTexture.getUVS().y);
	glVertex2f(-sprite->width / 2,  sprite->height - sprite->height / 2);
	glEnd();
	glDisable2D();
}
void Renderer::render(Drawable* drawable) {
	glMatrixMode(GL_MODELVIEW);

	MeshResource* mesh = drawable->getMesh();
	if (mesh != nullptr && mesh->GLModelHandle != -1) {
		glm::vec3 entityPosition = drawable->Position();
		glm::vec3 entityRotation = drawable->Rotation();
		glm::vec3 entityScale    = drawable->Scale();
		glm::vec3 entityColour   = drawable->Colour();

		glPushMatrix();

		glTranslatef(entityPosition.x, entityPosition.y, entityPosition.z);
		glRotatef(entityRotation.z, 0, 0, 1);	// Rotate the drawable around the z-axis
		glRotatef(entityRotation.y, 0, 1, 0);	// Rotate the drawable around the y-axis
		glRotatef(entityRotation.x, 1, 0, 0);	// Rotate the drawable around the x-axis
		glColor3f(entityColour.r, entityColour.g, entityColour.b);
		glScalef(entityScale.x, entityScale.y, entityScale.z);
		// Select the texture before drawing the mesh
		glBindTexture(GL_TEXTURE_2D, mesh->GLTextureHandle);

		glCallList(mesh->GLModelHandle);

		glBindTexture(GL_TEXTURE_2D, 0);

		glPopMatrix();
	}
}

unsigned int Renderer::loadModel(MeshResource* mesh) {
	// Create a displayList
	GLuint displayList = glGenLists(1);

	// Start the displayList
	glNewList(displayList, GL_COMPILE);

	// Start the primitive
	glBegin(GL_TRIANGLES);

	glm::ivec3 positionIndices, uvIndices, normalIndices;
	glm::vec3 position, normal;
	glm::vec2 uv;
	bool hasUVs = mesh->vertexTextureCoords.size() > 0;
	bool hasNormals = mesh->vertexNormals.size() > 0;
	for (unsigned int i = 0; i < mesh->faceVertexPositionIndices.size(); ++i) {
		positionIndices = mesh->faceVertexPositionIndices[i];
		uvIndices = mesh->faceVertexTextureIndices[i];
		normalIndices = mesh->faceVertexNormalIndices[i];

		if (hasUVs) {
			uv = mesh->vertexTextureCoords[uvIndices.x - 1];
			glTexCoord2f(uv.x, uv.y);
		}
		if (hasNormals) {
			normal = mesh->vertexNormals[normalIndices.x - 1];
		}
		position = mesh->vertexPositions[positionIndices.x - 1];
		glVertex3f(position.x, position.y, position.z);

		if (hasUVs) {
			uv = mesh->vertexTextureCoords[uvIndices.y - 1];
			glTexCoord2f(uv.x, uv.y);
		}
		if (hasNormals) {
			normal = mesh->vertexNormals[normalIndices.y - 1];
		}
		position = mesh->vertexPositions[positionIndices.y - 1];
		glVertex3f(position.x, position.y, position.z);

		if (hasUVs) {
			uv = mesh->vertexTextureCoords[uvIndices.z - 1];
			glTexCoord2f(uv.x, uv.y);
		}
		if (hasNormals) {
			normal = mesh->vertexNormals[normalIndices.z - 1];
		}
		position = mesh->vertexPositions[positionIndices.z - 1];
		glVertex3f(position.x, position.y, position.z);
	}

	// End the primitive
	glEnd();

	// End the displayList
	glEndList();

	// Return the displayList
	displayLists.push_back(displayList);

	// Return the displaylist handle
	return displayList;
}

unsigned int Renderer::loadTexture(ImageResource* image) {
	// Create a new OpenGL texture
	GLuint textureHandle;
	glGenTextures(1, &textureHandle);

	// Bind the new texture
	glBindTexture(GL_TEXTURE_2D, textureHandle);

	// Allow the texture to wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Enabled linear interpolation
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Create a new array for the colour data
	float* colourData = new float[image->size];

	// Get the number of pixels in the image
	int pixelCount = image->size / 4;

	// Iterate over the colour data and convert all the colour values from integer range 0-255 to float range 0-1
	// The Image struct data is stored in RGBA format and we need to pass it to glTexImage2D() as GL_RGB, so we also drop the alpha channel in this step
	for (unsigned int i = 0; i < pixelCount; ++i) {
		colourData[i * 4    ] = (float)image->data[i * 4    ] / 255.0f; // RED
		colourData[i * 4 + 1] = (float)image->data[i * 4 + 1] / 255.0f; // GREEN
		colourData[i * 4 + 2] = (float)image->data[i * 4 + 2] / 255.0f; // BLUE
		colourData[i * 4 + 3] = (float)image->data[i * 4 + 3] / 255.0f;	// ALPHA
	}

	// Load the image data into the OpenGL texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_FLOAT, colourData);

	delete[] colourData;

	// Return the texture handle
	return textureHandle;
}

void Renderer::clear() {
	// Clear the render buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::flush() {
	// Flush the render buffer to the window after drawing
	SDL_GL_SwapWindow(window);
}

void Renderer::freeTexture(GLuint textureId) {
	glDeleteTextures(1, &textureId);
}

void Renderer::freeModel(GLuint modelId) {
	glDeleteLists(modelId, 1);
}

void Renderer::dispose() {
	displayLists.clear();
}
