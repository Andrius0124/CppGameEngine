#pragma once

/**
* @brief	Manager of application states.
*
*/

#include "app_state_playing.h"
#include <unordered_map>
#include <string>

class Manager_App_State
{
private:
	/*!
	\ A dictionary containing the names of all the states in the game and the path to the xml file defining them.
	*/
	std::unordered_map<std::string, std::string> unloadedStates;

	/*!
	\ The name of a state which has been queued to load.
	*/
	std::string stateWaitingToLoad;

	std::unordered_map<std::string, Base_App_State*> states; //!< Store states against their unique name.
	Base_App_State* currentAppState; //!< Current state that is handled.
	std::string currentAppStateName; //!< Name of current state that is handled.
	std::string firstStateName; //!< Name of the first state ever added.
	std::string applicationDefinitionFilePath; //!< The path to the file which contains the application definition.
public:
	/*!
	\ brief Constructor.
	*/
	Manager_App_State(std::string applicationDefinitionFilePath);

	/*!
	\ brief Get the state which should be current at the beginning of the game e.g. SplashScreen/MainMenu
	\ return The AppState to start the game with.
	*/
	Base_App_State* getFirstState();

	/*!
	\ brief Get the name of the state which should be current at the beginning of the game e.g. SplashScreen/MainMenu
	\ return A string containing the name of the AppState to start the game with.
	
	*/
	std::string getFirstStateName();

	/*!
	\ brief Get the current state.
	\ return A pointer to the currently actife AppState.
	
	*/
	Base_App_State* getCurrentState();

	/*!
	\ brief Switch to a different state at the next game tick.
	\ param The name of the state to switch to.
	
	*/
	void RequestStateChange(std::string newCurrentState);

	/*!
	\ brief Free the memory for the currently active state.
	
	*/
	void FreeCurrentState();

	/*!
	\ brief Switch state to use.
	\ param The name of the state to switch to.
	
	*/
	void StateSwitch(std::string newCurrentState);

	/*!
	\ brief Reload the currently active state from disk.
	
	*/
	void RequestReloadState();

	/*!
	\ brief Pass any events that have happened on the window.
	\ param SDL_Event
	*/
	void Event(SDL_Event& event);

	/*!
	\ brief Initialise state provided.
	\ param state to be initialised.
	*/
	void Init();

	/*!
	\ brief Update state.
	\ param float timescale
	*/
	void Update(float dt);

	/*!
	\ brief Get a pointer to the appState with the specified name.
	\ param string The name of the state to fetch.
	\ return A pointer to the AppState with the specified name, or nullptr if it wasn't found.
	
	*/
	Base_App_State* getStateByName(std::string stateName);

	/*!
	\ brief Get the path to the definition file of the state with the specified name.
	\ param string The name of the state.
	\ return string The path to the AppState with the specified name.
	
	*/
	std::string getStatePath(std::string stateName);
};
