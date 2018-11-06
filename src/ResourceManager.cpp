#include <fstream>
#include <iostream>
#include "ResourceManager.h"
#include "picoPNG.h"
#include "Renderer.h"
#include "Log.h"

ResourceManager resourceManager = ResourceManager();

ResourceManager::ResourceManager()
{
	// Set the first unique id
	nextUniqueId = 0;
}

unsigned int ResourceManager::getNewResourceId()
{
	return nextUniqueId++;
}

Resource* ResourceManager::getResource(unsigned int m_id)
{
	std::unordered_map<unsigned int, Resource*>::iterator it = resourceIds.find(m_id);
	if (it == resourceIds.end()) return nullptr;
	return it->second;
}

Resource* ResourceManager::getResource(std::string m_name)
{
	std::unordered_map<std::string, Resource*>::iterator it = resourceNames.find(m_name);
	if (it == resourceNames.end()) return nullptr;
	return it->second;
}

bool ResourceManager::freeResource(unsigned int m_id)
{
	return freeResource(getResource(m_id));
}

bool ResourceManager::freeResource(std::string m_name)
{
	return freeResource(getResource(m_name));
}

bool ResourceManager::freeResource(Resource* m_resource)
{
	bool errorOccured = false;

	switch (m_resource->type) {

		case ResourceType::IMAGE:
		{
			ImageResource* image = (ImageResource*)m_resource;
			delete[] image->data;
			break;
		}

		case ResourceType::MESH:
		{
			MeshResource* mesh = (MeshResource*)m_resource;
			if (mesh->GLModelHandle != -1) {
				renderer->freeModel(mesh->GLModelHandle);
			}
			if (mesh->GLTextureHandle != -1) {
				renderer->freeTexture(mesh->GLTextureHandle);
			}
			break;
		}

	}

	auto idIterator = resourceIds.find(m_resource->id);
	if (idIterator == resourceIds.end()) {
		LOGGER->Log("Failed to free resource: No resource found with id %d.", m_resource->id);
		errorOccured = true;
	}

	auto nameIterator = resourceNames.find(m_resource->name);
	if (nameIterator == resourceNames.end()) {
		LOGGER->Log("Failed to free resource: No resource found with name %s.", m_resource->name);
		errorOccured = true;
	}

	resourceIds.erase(idIterator);
	resourceNames.erase(nameIterator);
	delete m_resource;

	return errorOccured;
}

bool ResourceManager::freeAllResources()
{
	bool errorOccured = false;

	while (!resourceIds.empty())
		errorOccured = !freeResource(resourceIds.begin()->second) || errorOccured;

	return errorOccured;
}

int ResourceManager::loadResource(std::string m_filePath, ResourceType m_type, std::string m_resourceName, std::string m_textureImageName)
{

	switch (m_type) {

	case ResourceType::IMAGE:
		return addResource(loadImageResource(m_filePath), m_resourceName);
		break;

	case ResourceType::MESH:
		return addResource(loadMeshResource(m_filePath, m_textureImageName), m_resourceName);
		break;

	case ResourceType::AUDIO:
		return addResource(loadAudioResource(m_filePath), m_resourceName);
		break;

	}

	// If the resource could not be loaded
	// Return negative 1 (an invalid id)
	return -1;

}

int ResourceManager::addResource(Resource* m_newResource, std::string m_resourceName)
{
	// If this resource was not successfully loaded
	if (m_newResource == nullptr) {
		// Return an invalid id
		return -1;
	}

	// Set the name and id of the resource
	m_newResource->id = getNewResourceId();
	m_newResource->name = m_resourceName;

	// Add the resource to the id and name maps
	resourceIds.insert(std::pair<unsigned int, Resource*>(m_newResource->id, m_newResource));
	resourceNames.insert(std::pair<std::string, Resource*>(m_newResource->name, m_newResource));

	// Return the id of the new resource
	return m_newResource->id;
}

ImageResource* ResourceManager::loadImageResource(std::string m_filePath)
{
	// Create the image we will be returning
	ImageResource* image = new ImageResource();

	// Open the file for reading
	std::ifstream file(m_filePath.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

	// Seek to the end of the file
	file.seekg(0, file.end);

	// Get the length of the file
	std::streampos length = file.tellg();

	// If the filesize is 0 there is nothing to load, return a nullptr
	if (length <= 0) {
		std::cout << "Failed to load png image \"" << m_filePath<< "\"." << std::endl;
		return nullptr;
	}

	// Seek to the start of the file again, so that later we start reading from the beginning
	file.seekg(0, file.beg);

	// Create a vector of chars the same size as the file
	std::vector<unsigned char> fileData((size_t)length);

	// Read the file data into the vector
	file.read((char*)&fileData[0], length);

	// Close the file now that we are done with it
	file.close();

	// Decode the image from the data we have loaded
	unsigned long int imageWidth, imageHeight;
	std::vector<unsigned char> imageData;
	int result = picoPNG::decodePNG(imageData, imageWidth, imageHeight, &fileData[0], (unsigned long)fileData.size());

	// If an error occurred
	if (result != 0) {
		std::cout << "Failed to decode png file \"" << m_filePath << "\"." << std::endl;
		return nullptr;
	}

	// Set the length, width, height and imageData of the image
	image->size = imageData.size();
	image->width = imageWidth;
	image->height = imageHeight;
	image->data = new uint8_t[image->size];
	for (unsigned int i = 0; i < image->size; ++i) {
		image->data[i] = imageData[i];
	}

	// Return the image
	return image;
}

MeshResource* ResourceManager::loadMeshResource(std::string m_filePath, std::string m_textureImageName)
{
	// Create the model we will be returning
	MeshResource* mesh;

	// Get the file extension
	std::string fileExt = m_filePath.substr(m_filePath.find_last_of(".") + 1);

	// Use the appropriate method to load the file
	if (fileExt == "obj") {
		mesh = loadMeshObj(m_filePath, m_textureImageName);
	} else if (fileExt == "md2") {
		mesh = loadMeshMd2(m_filePath, m_textureImageName);
	} else {
		// If no loader has been implement for this type of file, log and error
		LOGGER->Log("Failed to load mesh \"%s\": Unsupported file extension \"%s\".", m_filePath, fileExt);
		return nullptr;
	}

	// Load the model into graphics memory with the renderer and store the handle to it in the mesh
	mesh->GLModelHandle = renderer->loadModel(mesh);

	// If an image has already been loaded to use as a texture
	if (m_textureImageName != "") {
		// Load the texture into graphics memory with the renderer and store the handle to it in the mesh
		mesh->GLTextureHandle = renderer->loadTexture((ImageResource*)getResource(m_textureImageName));
	}

	// Return the mesh data
	return mesh;
}

MeshResource* ResourceManager::loadMeshObj(std::string m_filePath, std::string m_textureImageName)
{
	// Create the model we will be returning
	MeshResource* mesh = new MeshResource();

	// This will store the current line of text being read from the file
	std::string currentLine;

	// Open the file for reading
	std::ifstream file(m_filePath);

	// If the file could not be opened, return a null pointer
	if (file.is_open() == false) return nullptr;

	// Iterate over all lines in the file
	while (std::getline(file, currentLine)) {

		// Skip commented lines
		if (currentLine.at(0) == '#') continue;

		// Break the current line up where there are spaces
		std::vector<std::string> chunks = Utils::stringSplit(currentLine, ' ');

		switch (currentLine.at(0)) {

		case 'v':
			switch (currentLine.at(1)) {

			case 'n':
				// If this is a vertex normal
				mesh->vertexNormals.push_back(glm::vec3(
					std::stof(chunks[1]),
					std::stof(chunks[2]),
					std::stof(chunks[3])
				));
				break;

			case 't':
				// If this is a vertex texture coordinate
				mesh->vertexTextureCoords.push_back(glm::vec2(
					std::stof(chunks[1]),
					1.0f - std::stof(chunks[2])
				));
				break;

			default:
				// If this is a vertex position
				mesh->vertexPositions.push_back(glm::vec3(
					std::stof(chunks[1]),
					std::stof(chunks[2]),
					std::stof(chunks[3])
				));
				break;

			}
			break;

		case 'f':
			// If this is a vertex index for a face

			glm::ivec3 faceVertexIndices;
			glm::ivec3 faceNormalIndices;
			glm::ivec3 faceTextureCoordIndices;

			for (unsigned int i = 0; i < 3; ++i) {
				std::vector<std::string> faceComponentStrings = Utils::stringSplit(chunks[i + 1], '/');

				faceVertexIndices[i] = std::stoi(faceComponentStrings[0]);

				if (faceComponentStrings.size() > 2 && faceComponentStrings[1] != "") {
					faceTextureCoordIndices[i] = std::stoi(faceComponentStrings[1]);
				}

				if (faceComponentStrings.size() > 1 && faceComponentStrings[faceComponentStrings.size() - 1] != "") {
					faceNormalIndices[i] = std::stoi(faceComponentStrings[faceComponentStrings.size() - 1]);
				}
			}

			mesh->faceVertexPositionIndices.push_back(faceVertexIndices);
			mesh->faceVertexNormalIndices.push_back(faceNormalIndices);
			mesh->faceVertexTextureIndices.push_back(faceTextureCoordIndices);
			break;

		}

	}

	// Close the file now that we are done with it
	file.close();

	return mesh;
}

MeshResource* ResourceManager::loadMeshMd2(std::string m_filePath, std::string m_textureImageName)
{
	// TODO: Implement this
	return new MeshResource();
}

AudioResource* ResourceManager::loadAudioResource(std::string m_loadAudioResource)
{
	return nullptr;
};