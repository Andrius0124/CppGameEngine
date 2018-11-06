

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <unordered_map>
#include "Resource.h"
#include "Utils.h"

class ResourceManager {

public:

	ResourceManager();


	Resource* getResource(unsigned int id);


	Resource* getResource(std::string name);


	int loadResource(std::string filePath, ResourceType type, std::string resourceName, std::string textureImageName = "");


	bool freeResource(unsigned int id);


	bool freeResource(std::string name);


	bool freeResource(Resource* resource);


	bool freeAllResources();

protected:

	unsigned int getNewResourceId();


	int addResource(Resource* newResource, std::string resourceName);


	ImageResource* loadImageResource(std::string filePath);

	MeshResource* loadMeshResource(std::string filePath, std::string textureImageName = "");


	MeshResource* loadMeshObj(std::string filePath, std::string textureImageName = "");

	MeshResource* loadMeshMd2(std::string filePath, std::string textureImageName = "");


	AudioResource* loadAudioResource(std::string loadAudioResource);

private:

	unsigned int nextUniqueId;


	std::unordered_map<unsigned int, Resource*> resourceIds;


	std::unordered_map<std::string, Resource*> resourceNames;

};


extern ResourceManager resourceManager;

#endif // !RESOURCEMANAGER_H