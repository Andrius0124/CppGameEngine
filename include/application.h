#pragma once

#include "manager_app_state.h"
#include <string>
#include <glm\vec2.hpp>
#include "FramerateManager.h"

class Application
{
private:
	/*!
	\ brief A pointer to the game window
	*/
	SDL_Window* window;

	/*!
	\ brief Free all memory when quitting the application.
	\ author Addo White
	*/
	FramerateManager framerateManager;
public:
	Manager_App_State* manager_app_state; //!< Controls the current state of the app.

	/*!
	\ brief Default constructor.
	*/
	Application();

	virtual ~Application();

	/*!
	\brief Initialise the managers.
	*/
	void Init(std::string applicationDefinitionFilePath);

	/*!
	\ brief Computes and renders a single game tick.
	\ author Addo White
	*/
	void Update();

	/*!
	\ brief Pass any events that have happened on the window.
	\ param SDL_Event
	*/
	void Event(SDL_Event& event);

	/*!
	\ brief Create the game window
	\ param glm::ivec2 The 
	\ param std::string
	\ author Addo White
	*/
	bool createWindow(glm::ivec2 windowSize, std::string windowTitle = "");

	/*!
	\ brief Free all memory when quitting the application.
	\ author Addo White
	*/
	void Quit();

	/*!
	\ brief Set the desired framerate of the application.
	\ author Addo White
	*/
	void setFrameRate(unsigned int targetFramerate);

	/*!
	\ brief Get the position of the mouse.
	\ return A vec2 with the x and y component of the mouse position.
	\ author Addo White
	*/
	glm::ivec2 getCursorPosition();

	/*!
	\ brief Set whether to capture relative mouse movement. Capturing the mouse will hide the cursor and report relative mouse movements instead of an absolute position.
	\ param Bool enable or disable this feature.
	\ author Addo White
	*/
	void captureMouse(bool enable);

	/*!
	\ brief Get a pointer to the entity with the specified name.
	\ param The name of the entity to retrieve.
	\ author Addo White
	*/
	Entity* getEntity(std::string entityName);
};

extern Application application;