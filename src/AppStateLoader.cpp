#include <iostream>
#include <glm\vec2.hpp>
#include "AppStateLoader.h"
#include "Resource.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Application.h"
#include "ScriptManager.h"
#include "Utils.h"
#include "Log.h"

using namespace tinyxml2;

std::vector<std::pair<std::string, std::string>> AppStateLoader::loadAppStates(std::string m_filePath)
{
	// The list of game states we will be returning
	std::vector<std::pair<std::string, std::string>> appStates;

	// Load the XML file
	XMLDocument document;
	if (document.LoadFile(m_filePath.c_str()) != 0) {
		logError("Could not open file for reading \"" + m_filePath + "\", does the file exist?");
		return appStates;
	}

	XMLElement* rootNode = document.FirstChildElement("Game");

	std::string loadWindowError;
	if (!loadWindow(rootNode, loadWindowError)) {
		logError("Error loading AppState from file \"" + m_filePath + "\": " + loadWindowError);
		return appStates;
	}

	// Get the root node which contains all the game states
	XMLElement* gameStateListNode = rootNode->FirstChildElement("GameStates");
	if (rootNode == nullptr) {
		logError("\"GameStates\" node not found in file \"" + m_filePath + "\".");
		return appStates;
	}

	const char* firstGameStateName = gameStateListNode->Attribute("firstState");

	// Add the name of the first gameState which should be loaded to the dictionary
	appStates.push_back(std::pair<std::string, std::string>("_FIRST_STATE_", firstGameStateName));

	// Get the first gamestate node
	XMLElement* gameStateNode = gameStateListNode->FirstChildElement("GameState");

	const char* gameStateName;
	const char* gameStateFilePath;

	// Loop until there are no more gamestates
	while (gameStateNode != nullptr) {

		// Get the name of the gamestate
		gameStateName = gameStateNode->Attribute("name");
		if (gameStateName == nullptr) {
			logError("Error loading AppState from file \"" + m_filePath + "\": name attribute missing from \"GameState\" node.");
			return appStates;
		}

		// Get the path to the gamestate xml file
		gameStateFilePath = gameStateNode->Attribute("path");
		if (gameStateFilePath == nullptr) {
			logError("Error loading AppState from file \"" + m_filePath + "\": path attribute missing from \"GameState\" node with name \"" + gameStateName + "\".");
			return appStates;
		}

		// Add the successfully loaded state to the list of states
		appStates.push_back(std::pair<std::string, std::string>(gameStateName, gameStateFilePath));

		// Get the next gamestate node
		gameStateNode = gameStateNode->NextSiblingElement("GameState");
	}

	// Return the list of loaded states
	return appStates;
}

Base_App_State* AppStateLoader::loadSingleAppState(std::string m_stateName, std::string m_filePath, std::string &m_error)
{
	Base_App_State* newState = new Base_App_State(m_stateName);

	// Load the XML file
	XMLDocument document;
	if (document.LoadFile(m_filePath.c_str()) != 0) {
		logError("Could not open file for reading \"" + m_filePath + "\", does the file exist?");
		return nullptr;
	}

	// Get the root node which contains all the game states
	XMLElement* rootNode = document.FirstChildElement("GameState");
	if (rootNode == nullptr) {
		logError("\"GameState\" node not found in file \"" + m_filePath + "\".");
		return nullptr;
	}

	std::string resourcesError, entitiesError;

	// Load the resources, log an error if it failed
	if (!loadResources(rootNode, resourcesError)) {
		m_error = "Error loading resources from file \"" + m_filePath + "\": " + resourcesError;
		return nullptr;
	}

	// Load the entities, log an error if it failed
	if (!loadEntities(newState, rootNode, entitiesError)) {
		m_error = "Error loading entites from file \"" + m_filePath + "\": " + entitiesError;
		return nullptr;
	}

	return newState;
}

bool AppStateLoader::loadWindow(XMLElement* m_rootNode, std::string &m_error)
{
	// Get the window node
	XMLElement* windowNode = m_rootNode->FirstChildElement("Window");

	// If no window node could be found
	if (windowNode == nullptr) {
		m_error = "Missing window node.";
		return false;
	}

	// Get the desired width and height of the window from the XML
	int windowWidth, windowHeight;
	windowNode->QueryIntAttribute("width" , &windowWidth);
	windowNode->QueryIntAttribute("height", &windowHeight);

	// Get the desired window title from the XML
	const char* windowTitle = windowNode->Attribute("title");

	// Create a window with the desired dimensions and title
	if (!application.createWindow(glm::ivec2(windowWidth, windowHeight), std::string(windowTitle))) {
		m_error = "Failed to create window: " + std::string(SDL_GetError());
		return false;
	}

	// Return success
	return true;
}

bool AppStateLoader::loadResources(XMLElement* m_rootNode, std::string &m_error)
{
	// Get the node with the list of resources
	XMLElement* resourceListNode = m_rootNode->FirstChildElement("Resources");

	// Get the first resource
	XMLElement* resourceNode = resourceListNode->FirstChildElement();
	// Loop until there are no more resources
	while (resourceNode != nullptr) {

		// Get the name of the xml node
		std::string resourceTypeString = resourceNode->Name();

		// Get the resource type
		ResourceType resourceType;
		if (resourceTypeString == "Image") {
			resourceType = ResourceType::IMAGE;
		} else if (resourceTypeString == "Mesh") {
			resourceType = ResourceType::MESH;
		} else if (resourceTypeString == "Audio") {
			resourceType = ResourceType::AUDIO;
		}

		// Get the node with the resource name
		XMLElement* resourceNameNode = resourceNode->FirstChildElement("Name");
		if (resourceNameNode == nullptr) {
			m_error = "\"Name\" node not found in \"" + resourceTypeString + "\" node.";
			return false;
		}

		// Get the node with the resource path
		XMLElement* resourcePathNode = resourceNode->FirstChildElement("Path");
		if (resourcePathNode == nullptr) {
			m_error = "\"Path\" node not found in \"" + resourceTypeString + "\" node.";
			return false;
		}

		// Load the resource
		resourceManager.loadResource(resourcePathNode->GetText(), resourceType, resourceNameNode->GetText());

		// Get the next resource
		resourceNode = resourceNode->NextSiblingElement();
	}

	return true;
}

bool AppStateLoader::loadEntities(Base_App_State* m_state, XMLElement* m_rootNode, std::string &m_error)
{
	// Get the node with the list of resources
	XMLElement* entityListNode = m_rootNode->FirstChildElement("Entities");

	// Get the first resource
	XMLElement* entityNode = entityListNode->FirstChildElement("Entity");

	// The name of this entity
	const char* entityName;

	// Load all the plain entities and drawables
	while (entityNode != nullptr) {

		// Get the name of this entity
		entityName = entityNode->Attribute("name");

		// If there was no name specified for this entity
		if (entityName == nullptr) {
			m_error = "\"Name\" attribute missing from \"Entity\" node.";
			return false;
		}

		// Get the node with the resource name
		XMLElement* transformNode = entityNode->FirstChildElement("Transform");
		if (transformNode == nullptr) {
			m_error = "\"Transform\" node not found in entity \"" + std::string(entityName) + "\".";
			return false;
		}

		// Create the new entity
		Entity* newEntity;

		// Get the node with the mesh and image resource names
		XMLElement* modelNode = entityNode->FirstChildElement("Model");

		std::string modelError, transformError, physicsError;

		// If the entity has a model, it should be drawable
		if (modelNode == nullptr) {
			newEntity = new Entity(entityName);
		} else {
			newEntity = new Drawable(entityName);

			// Attempt to set up the requested model and texture for the entity
			if (!loadModelNode((Drawable*)newEntity, entityNode, entityName, modelError)) {
				m_error = "Error loading model for entity \"" + std::string(entityName) + "\": " + modelError;
				return false;
			}
		}

		XMLElement* scriptNode = entityNode->FirstChildElement("Script");
		if (scriptNode != nullptr) {
			std::string scriptSourceCode;

			const char* scriptPath = scriptNode->Attribute("src");
			if (scriptPath != nullptr) {
				scriptSourceCode += Utils::readFile(scriptPath);
			}

			const char* textContent = scriptNode->GetText();
			if (textContent != nullptr) {
				scriptSourceCode += "\n" + std::string(textContent);
			}

			newEntity->scriptHandle = scriptManager.addScript(new Script(newEntity->id, Script::Language::LUA, scriptSourceCode));
		}

		XMLElement* cameraNode = entityNode->FirstChildElement("Camera");
		if (cameraNode != nullptr) {
			renderer->setCameraEntity(newEntity);
		}

		// Attempt to load the transform from the XML, set the error description of there was an issue
		Transform* entityTransform = loadTransformNode(entityNode, entityName, transformError);
		if (entityTransform == nullptr) {
			m_error = "Error loading transform for entity \"" + std::string(entityName) + "\": " + transformError;
			return false;
		}

		// Set the transform of the entity to the loaded transform
		newEntity->SetTransform(*entityTransform);

		delete entityTransform;

		Physics* entityPhysics = loadPhysicsNode(entityNode, entityName, physicsError);
		if (entityPhysics != nullptr) {
			//error = "Physics not found for entity \"" + std::string(entityName) + "\": " + physicsError;
			//return false;
			newEntity->SetPhysics(*entityPhysics);
		}

		delete entityPhysics;

		// Add the loaded entity to the state
		m_state->addEntity(newEntity);

		// Get the next resource
		entityNode = entityNode->NextSiblingElement("Entity");
	}

	// Load all the text entities
	XMLElement* TextNode = entityListNode->FirstChildElement("Text");
	while (TextNode != nullptr)
	{
		// Get the name of this entity
		entityName = TextNode->Attribute("name");

		// If there was no name specified for this entity
		if (entityName == nullptr) {
			m_error = "\"Name\" attribute missing from \"Entity\" node.";
			return false;
		}

		// Get the node with the resource name
		XMLElement* transformNode = TextNode->FirstChildElement("Transform");
		if (transformNode == nullptr) {
			m_error = "\"Transform\" node not found in text entity \"" + std::string(entityName) + "\".";
			return false;
		}

		Entity* newEntity = new Text(entityName);
		const char * msg = TextNode->Attribute("String");
		char * _msg = _strdup(msg);


		if (msg != nullptr)
			((Text*)newEntity)->setMsg(_msg);
		XMLElement* ColorNode = TextNode->FirstChildElement("Color");
		// get color values if set in xml
		if (ColorNode != nullptr)
		{
			float R, G, B;
			R = ColorNode->FloatAttribute("R");
			G = ColorNode->FloatAttribute("G");
			B = ColorNode->FloatAttribute("B");
			((Text*)newEntity)->setColor(R, G, B);

		}

		std::string transformError;

		// Attempt to load the transform from the XML, set the error description of there was an issue
		Transform* entityTransform = loadTransformNode(TextNode, entityName, transformError);
		if (entityTransform == nullptr) {
			m_error = "Error loading transform for entity \"" + std::string(entityName) + "\": " + transformError;
			return false;
		}

		// Set the transform of the entity to the loaded transform
		newEntity->SetTransform(*entityTransform);

		delete entityTransform;

		// Add the loaded entity to the state
		m_state->addEntity(newEntity);

		TextNode = TextNode->NextSiblingElement("Text");
	}

	// Load all the sprite entities
	XMLElement* SpriteNode = entityListNode->FirstChildElement("Sprite");
	if (SpriteNode != nullptr)
	{
		// Get the name of this entity
		entityName = SpriteNode->Attribute("name");

		// If there was no name specified for this entity
		if (entityName == nullptr) {
			m_error = "\"Name\" attribute missing from \"Entity\" node.";
			return false;
		}

		// Get the node with the resource name
		XMLElement* transformNode = SpriteNode->FirstChildElement("Transform");
		if (transformNode == nullptr) {
			m_error = "\"Transform\" node not found in sprite entity \"" + std::string(entityName) + "\".";
			return false;
		}

		Entity* newEntity = new Sprite(entityName);
		const char* imageName = SpriteNode->Attribute("texture");
		ImageResource* loadedImage = (ImageResource*)resourceManager.getResource(imageName);
		if (loadedImage == nullptr) {
			m_error = "\"Sprite\" node in entity \"" + std::string(entityName) + "\" attempt to use image which does not exist \"" + imageName + "\".";
			return false;
		}
		((Sprite*)newEntity)->setTextureID(renderer->loadTexture(loadedImage));
		((Sprite*)newEntity)->width = SpriteNode->IntAttribute("width");
		((Sprite*)newEntity)->height = SpriteNode->IntAttribute("height");
		if (SpriteNode->BoolAttribute("animation")) {
			((Sprite*)newEntity)->animation = SpriteNode->BoolAttribute("animation");
			((Sprite*)newEntity)->Tiles.x = SpriteNode->IntAttribute("tiles.x");
			((Sprite*)newEntity)->Tiles.y = SpriteNode->IntAttribute("tiles.y");

			((Sprite*)newEntity)->initAnimation();

		}

		std::string transformError;

		// Attempt to load the transform from the XML, set the error description of there was an issue
		Transform* entityTransform = loadTransformNode(SpriteNode, entityName, transformError);
		if (entityTransform == nullptr) {
			m_error = "Error loading transform for entity \"" + std::string(entityName) + "\": " + transformError;
			return false;
		}

		// Set the transform of the entity to the loaded transform
		newEntity->SetTransform(*entityTransform);

		delete entityTransform;

		// Add the loaded entity to the state
		m_state->addEntity(newEntity);

		SpriteNode = SpriteNode->NextSiblingElement("Sprite");
	}

	return true;
}

Transform* AppStateLoader::loadTransformNode(XMLElement* m_entityNode, std::string m_entityName, std::string &m_error)
{
	// Get the node with the resource name
	XMLElement* transformNode = m_entityNode->FirstChildElement("Transform");
	if (transformNode == nullptr) {
		m_error = "\"Transform\" node not found in entity \"" + std::string(m_entityName) + "\".";
		return nullptr;
	}

	// Get the node with the entity's position
	XMLElement* positionNode = transformNode->FirstChildElement("Position");
	if (positionNode == nullptr) {
		m_error = "\"Position\" node not found in \"Transform\" node of entity \"" + std::string(m_entityName) + "\"";
		return nullptr;
	}

	// Get the node with the entity's position
	XMLElement* rotationNode = transformNode->FirstChildElement("Rotation");
	if (positionNode == nullptr) {
		m_error = "\"Position\" node not found in \"Transform\" node of entity \"" + std::string(m_entityName) + "\"";
		return nullptr;
	}

	// Get the node with the entity's position
	XMLElement* scaleNode = transformNode->FirstChildElement("Scale");
	if (positionNode == nullptr) {
		m_error = "\"Position\" node not found in \"Transform\" node of entity \"" + std::string(m_entityName) + "\"";
		return nullptr;
	}

	// Create the transform that will be returned
	Transform* newTransform = new Transform();

	// Read the position attributes from the position node
	positionNode->QueryFloatAttribute("x", &newTransform->position.x);
	positionNode->QueryFloatAttribute("y", &newTransform->position.y);
	positionNode->QueryFloatAttribute("z", &newTransform->position.z);

	// Read the rotation attributes from the rotation node
	rotationNode->QueryFloatAttribute("x", &newTransform->rotation.x);
	rotationNode->QueryFloatAttribute("y", &newTransform->rotation.y);
	rotationNode->QueryFloatAttribute("z", &newTransform->rotation.z);
	
	// Read the scale attributes from the scale node
	scaleNode->QueryFloatAttribute("x", &newTransform->scale.x);
	scaleNode->QueryFloatAttribute("y", &newTransform->scale.y);
	scaleNode->QueryFloatAttribute("z", &newTransform->scale.z);

	// Return the loaded transform
	return newTransform;
}

Physics * AppStateLoader::loadPhysicsNode(XMLElement * entityNode, std::string entityName, std::string & error)
{
	// Get the node with the resource name
	XMLElement* physicsNode = entityNode->FirstChildElement("Physics");
	if (physicsNode == nullptr) {
		// Entity doesn't support physics
		//error = "\"Physics\" node not found in entity \"" + std::string(entityName) + "\".";
		return nullptr;
	}

	// Get the node with the entity's position
	XMLElement* velocityNode = physicsNode->FirstChildElement("Velocity");
	if (velocityNode == nullptr) {
		error = "\"Velocity\" node not found in \"Physics\" node of entity \"" + std::string(entityName) + "\"";
		return nullptr;
	}

	// Get the node with the entity's position
	XMLElement* hullNode = physicsNode->FirstChildElement("Hull");
	if (hullNode == nullptr) {
		error = "\"Hull\" node not found in \"Physics\" node of entity \"" + std::string(entityName) + "\"";
		return nullptr;
	}

	// Create the transform that will be returned
	Physics* newPhysics = new Physics();

	// Read the position attributes from the position node
	velocityNode->QueryFloatAttribute("x", &newPhysics->velocity.x);
	velocityNode->QueryFloatAttribute("y", &newPhysics->velocity.y);
	velocityNode->QueryFloatAttribute("z", &newPhysics->velocity.z);

	// Read the rotation attributes from the rotation node
	hullNode->QueryFloatAttribute("x", &newPhysics->hull.x);
	hullNode->QueryFloatAttribute("y", &newPhysics->hull.y);
	hullNode->QueryFloatAttribute("z", &newPhysics->hull.z);

	// Return the loaded transform
	return newPhysics;
}

bool AppStateLoader::loadModelNode(Drawable* m_drawableEntity, XMLElement* m_entityNode, std::string m_entityName, std::string &m_error)
{
	// Get the node with the mesh and image resource names
	XMLElement* modelNode = m_entityNode->FirstChildElement("Model");

	// If the entity has a model, it should be drawable
	if (modelNode == nullptr) {
		m_error = "\"Model\" node not found in entity \"" + std::string(m_entityName) + "\".";
		return false;
	}

	// Get the names of the mesh and image resources to use for the model and texture
	const char* meshName  = modelNode->Attribute("mesh");
	const char* imageName = modelNode->Attribute("texture");

	MeshResource* loadedMesh = (MeshResource*)resourceManager.getResource(meshName);
	if (loadedMesh == nullptr) {
		m_error = "\"Model\" node in entity \"" + std::string(m_entityName) + "\" attempt to use mesh which does not exist \"" + meshName + "\".";
		return false;
	}

	if (imageName != NULL) {
		ImageResource* loadedImage = (ImageResource*)resourceManager.getResource(imageName);
		if (loadedImage == nullptr) {
			m_error = "\"Model\" node in entity \"" + std::string(m_entityName) + "\" attempt to use image which does not exist \"" + imageName + "\".";
			return false;
		}

		// Create a texture from the image resource
		if (loadedMesh->GLTextureHandle == -1) loadedMesh->GLTextureHandle = renderer->loadTexture(loadedImage);
	}

	// Set the entity's mesh
	m_drawableEntity->setMesh(loadedMesh);
	
	return true;
}

void AppStateLoader::logError(std::string m_error)
{
	LOGGER->Log(m_error);
}