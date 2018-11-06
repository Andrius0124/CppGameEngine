
#ifndef RESOURCE_H
#define RESOURCE_H

#include <vector>
#include <string>
#include <glm\glm.hpp>

/**
 * An enum for the different types of resources
 * 
 */
enum ResourceType {
	IMAGE,
	MESH,
	ANIMATED_MESH,
	AUDIO
};

/**
 * Interface struct, all resources will inherit this
 * 
 */
struct Resource {

public:
	/**
	* A virtual destructor to make sure that child objects are properly destructed.
	* 
	*/
	virtual ~Resource() {};

	/**
	 * The unique id given to this resource
	 * 
	 */
	unsigned int id;

	/**
	 * The unique name given to this resource
	 * 
	 */
	std::string name;

	/**
	 * The type of resource this is (image, audio, etc)
	 * 
	 */
	ResourceType type;

};

/**
 * A struct for storing image data
 */
struct ImageResource : public Resource {

public:
	/**
	 * Store pixel data in RGBA format with unsigned 8bit integers (0-255)
     * 
	 */
	uint8_t* data;

	/**
	 * Store the number of values in the pixel array
	 * 
	 */
	size_t size;
	
	/**
	 * Store the width and height of the image
	 * 
	 */
	int width, height;

	/**
	 * Constructor.
	 * Set initial property values.
	 * 
	 */
	ImageResource() {
		// Give properties their initial values
		type = ResourceType::IMAGE;
	}

};

/**
 * A struct for storing mesh data
 * 
 */
struct MeshResource : public Resource {

public:
	/**
	 * Store the positions of each vertex in the mesh
	 * 
	 */
	std::vector<glm::vec3> vertexPositions;

	/**
	 * Store the normals of each vertex in the mesh
	 * 
	 */
	std::vector<glm::vec3> vertexNormals;

	/**
	 * Store the texture coordinates of each vertex in the mesh
	 * 
	 */
	std::vector<glm::vec2> vertexTextureCoords;

	/**
	 * Store the indices of the vertex positions which make up a face.
	 * 
	 */
	std::vector<glm::ivec3> faceVertexPositionIndices;

	/**
	 * Store the indices of the vertex normals which make up a face.
	 * 
	 */
	std::vector<glm::ivec3> faceVertexNormalIndices;

	/**
	 * Store the indices of the vertex texture coordinates which make up a face.
	 * 
	 */
	std::vector<glm::ivec3> faceVertexTextureIndices;

	/**
	 * The handle to the OpenGL model for this mesh (when the mesh is loaded into graphics memory, or -1 if not)
	 * 
	 */
	int GLModelHandle;

	/**
	 * The handle to the OpenGL texture for this mesh (when the texture is loaded into graphics memory, or -1 if not)
	 * 
	 */
	int GLTextureHandle;

	/**
	 * Constructor.
	 * Set initial property values.
	 * 
	 */
	MeshResource() {
		// Give properties their initial values
		type = ResourceType::MESH;
		GLModelHandle = -1;
		GLTextureHandle = -1;
	}

};

/**
 * A struct for storing animated meshes
 * 
 */
struct AnimatableMeshResource : public Resource {

public:
	
	/**
	 * Each frame is stored as its own mesh
	 * 
	 */
	std::vector<MeshResource*> keyframes;

	/**
	 * This will store the total number of frames in the animation
	 * 
	 */
	unsigned int numFrames;

	/**
	 * Constructor.
	 * Set initial property values.
	 * 
	 */
	AnimatableMeshResource() {
		// Give properties their initial values
		type = ResourceType::ANIMATED_MESH;
		numFrames = 0;
	}

	/**
	 * A method to retrieve a particular frame of an animated mesh
	 * @return The mesh for the specified frame of the animation.
	 * 
	 */
	MeshResource* getKeyframe(unsigned int frameIndex) {
		return keyframes[frameIndex];
	}

	// The 'frameIndex' argument is a float so that you can retrieve an interpolation between two frames.
	MeshResource* getKeyframe(float frameIndex) {
		// If frameIndex is a whole number
		if (frameIndex == (float)((int)frameIndex)) {
			return getKeyframe(frameIndex);
		}
		
		// TODO: Interpolate here
	}

};

/**
 * A struct for storing audio data.
 * 
 */
struct AudioResource : public Resource {
// TODO: Not yet implemented.
public:
	/**
	 * Constructor.
	 * Set initial property values.
	 * 
	 */
	AudioResource() {
		// Give properties their initial values
		type = ResourceType::AUDIO;
	}

};

#endif // !RESOURCE_H