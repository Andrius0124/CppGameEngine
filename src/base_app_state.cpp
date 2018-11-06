#include "base_app_state.h"
#include "Renderer.h"
#include "ScriptManager.h"
#include "ResourceManager.h"


Base_App_State::Base_App_State(std::string m_name)
{
	name = m_name;
}

Base_App_State::~Base_App_State()
{
	for (auto entityIdPair : entities) {
		delete entityIdPair.second;
	}
	entities.clear();
}

std::string Base_App_State::getName() const
{
	return name;
}

void Base_App_State::addEntity(Entity* m_newEntity)
{
	// Add the entity with it's id to the map
	entities.insert(std::pair<unsigned int, Entity*>(m_newEntity->id, m_newEntity));

	// Add the entity with it's name to the map
	entityNames.insert(std::pair<std::string, Entity*>(m_newEntity->name, m_newEntity));
}

Entity* Base_App_State::getEntity(unsigned int m_entityId)
{
	// Get the entity with the specified id
	std::unordered_map<unsigned int, Entity*>::iterator it = entities.find(m_entityId);

	// If it didn't exist
	if (it == entities.end()) return nullptr;

	// Return a pointer to the entity
	return it->second;
}

Entity* Base_App_State::getEntity(std::string m_entityName)
{
	// Get the entity with the specified id
	std::unordered_map<std::string, Entity*>::iterator it = entityNames.find(m_entityName);

	// If it didn't exist
	if (it == entityNames.end()) return nullptr;

	// Return a pointer to the entity
	return it->second;
}

void Base_App_State::Init()
{
	// Execute the 'load' callback in all loaded scripts
	scriptManager.loadScripts(this);
}

void Base_App_State::Update(float m_deltaTime)
{
	UpdateEntities(m_deltaTime);
}

void Base_App_State::ExitState()
{
	resourceManager.freeAllResources();
	renderer->dispose();
	scriptManager.freeScripts();
}

void Base_App_State::Event(SDL_Event &m_event)
{
	if (m_event.type == SDL_KEYDOWN) {
		scriptManager.keydownScripts(this, SDL_GetKeyName(m_event.key.keysym.sym));
	} else if (m_event.type == SDL_KEYUP) {
		scriptManager.keyupScripts(this, SDL_GetKeyName(m_event.key.keysym.sym));
	}
}

void Base_App_State::UpdateEntities(float m_deltaTime)
{
	// Update all entities in this game state
	for (auto currentEntity : entities) {
		currentEntity.second->Update(m_deltaTime);
	}

	// Execute the 'update' callback in all loaded scripts
	scriptManager.updateScripts(this);

	// Render all drawable entities after they have updated
	for (auto currentEntity : entities) {
		// Attempt to cast the Entity to a Drawable
		Drawable* drawable = dynamic_cast<Drawable*>(currentEntity.second);

		// If it can be drawn
		if (drawable != NULL) {
			// Render it
			renderer->render(drawable);
		}
		// Attempt to cast the Entity to a Text
		Text* text = dynamic_cast<Text*>(currentEntity.second);

		// If it can be drawn
		if (text != NULL) {
			// Render it
			renderer->render(text);
		}
			// Attempt to cast the Entity to a sprite
			Sprite* sprite = dynamic_cast<Sprite*>(currentEntity.second);

		// If it can be drawn
		if (sprite != NULL) {
			// Render it
			renderer->render(sprite);
		}

	}
}
