#pragma once
#include <math.h>
#include <SDL/SDL.h>
#include <unordered_map>
#include <string>
#include "drawable.h"

class Base_App_State
{
private:

protected:
	std::string name;

	/*!
	\brief All the entities which exist in the current game state (some may be drawable)
	*/
	std::unordered_map<unsigned int, Entity*> entities;

	/*!
	\brief The names of all the entities which exist in the current game state
	*/
	std::unordered_map<std::string, Entity*> entityNames;
public:
	Base_App_State(std::string name);

	/*!
	\brief Virtual destructor to make sure that child class destructors are also envoked. Helpful video which demonstrates the problem: www.youtube.com/watch?v=ZiNGWHg5Z-o
	*/
	virtual ~Base_App_State();

	/*!
	\brief Method to retreive the value of the private 'name' member variable.
	*/
	std::string getName() const;

	/*!
	\brief Method to add an entity to the state.
	\param A pointer to the Entity to add.

	*/
	void addEntity(Entity* newEntity);

	/*!
	* \brief Get the entity with the specified id.
	* \return A pointer to the entity with the specified id, or nullptr if none was found.
	*/
	Entity* getEntity(unsigned int entityId);

	/*!
	* \brief Get the entity with the specified name.
	* \return A pointer to the entity with the specified name, or nullptr if none was found.

	*/
	Entity* getEntity(std::string entityName);

	/*!
	\brief Initialise the state.
	*/
	virtual void Init();

	/*!
	\brief Draws everything necessary to the screen.
	\param window is a reference to a render window.
	*/
	virtual void Update(float dt);

	/*!
	\ brief Exit the state and delete anything we might need to.
	*/
	virtual void ExitState();

	/*!
	\ brief Pass any events that have happened on the window.
	\ param SDL_Event
	*/
	virtual void Event(SDL_Event& event);

	/*!
	\brief Updates and renders the entities in the game state.
	\param float deltaTime constant

	*/
	void UpdateEntities(float dt);


};