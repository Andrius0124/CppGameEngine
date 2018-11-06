/**
* @Author	Joseph Radu
* @date		1st November 2016
* @brief	Manager of application states.
*
*/

#include "manager_app_state.h"
#include "AppStateLoader.h"
#include "Log.h"

Manager_App_State::Manager_App_State(std::string m_applicationDefinitionFilePath)
{
	applicationDefinitionFilePath = m_applicationDefinitionFilePath;
	currentAppState = nullptr;
	stateWaitingToLoad = "";
}

Base_App_State* Manager_App_State::getFirstState()
{
	return getStateByName(firstStateName);
}

std::string Manager_App_State::getFirstStateName()
{
	return firstStateName;
}

Base_App_State* Manager_App_State::getCurrentState()
{
	return currentAppState;
}

void Manager_App_State::RequestStateChange(std::string m_newCurrentState)
{
	stateWaitingToLoad = m_newCurrentState;
}

void Manager_App_State::FreeCurrentState()
{
	if (currentAppState != nullptr) {
		currentAppState->ExitState();
		delete currentAppState;
	}
}

void Manager_App_State::RequestReloadState()
{
	RequestStateChange(currentAppStateName);
}

void Manager_App_State::StateSwitch(std::string m_newCurrentStateName)
{
	FreeCurrentState();

	// Remember the name of the new current state
	currentAppStateName = m_newCurrentStateName;

	// Load the specified appState from it's definition file
	std::string errorString;
	AppStateLoader loader;
	currentAppState = loader.loadSingleAppState(m_newCurrentStateName, getStatePath(m_newCurrentStateName), errorString);

	// If the appState was not loaded successfully
	if (currentAppState == nullptr) {
		// Record the error that occured.
		LOGGER->Log("Failed to load appState \"" + m_newCurrentStateName + "\": " + errorString);
	}

	// Initialise the new current state
	if (currentAppState != nullptr) currentAppState->Init();
}

void Manager_App_State::Event(SDL_Event& m_event)
{
	if (currentAppState != nullptr) currentAppState->Event(m_event);
}

void Manager_App_State::Init()
{
	// Load all states from file
	AppStateLoader loader;
	std::vector<std::pair<std::string, std::string>> appStates = loader.loadAppStates(applicationDefinitionFilePath);

	firstStateName = "";
	std::string fallbackFirstState = "";

	for (unsigned int i = 0; i < appStates.size(); ++i) {
		// Get the current state in the loop
		std::pair<std::string, std::string> state = appStates[i];

		// If this item contains the name of the starting appState
		if (state.first == "_FIRST_STATE_") {
			firstStateName = state.second;
		} else {
			unloadedStates.insert(state);
			if (fallbackFirstState == "") {
				fallbackFirstState = state.first;
			}
		}
	}

	// If no first state was specified by the user, use the first state that was added when loading the file
	if (firstStateName == "") firstStateName = fallbackFirstState;

	// Make the first state active
	StateSwitch(getFirstStateName());
}

void Manager_App_State::Update(float m_deltaTime)
{
	if (stateWaitingToLoad != "") {
		StateSwitch(stateWaitingToLoad);
		stateWaitingToLoad = "";
	}
	if (currentAppState != nullptr) currentAppState->Update(m_deltaTime);
}

Base_App_State* Manager_App_State::getStateByName(std::string m_stateName)
{
	// Attempt to get the state with the specified name
	std::unordered_map<std::string, Base_App_State*>::iterator it = states.find(m_stateName);

	// If one couldn't be found
	if (it == states.end()) {
		LOGGER->Log("Failed to get state \"", m_stateName, "\". State does not exist.");
		return nullptr;
	}

	// Return a pointer to the matching state for the specified name
	return it->second;
}

std::string Manager_App_State::getStatePath(std::string m_stateName)
{
	// Attempt to get the state with the specified name
	std::unordered_map<std::string, std::string>::iterator it = unloadedStates.find(m_stateName);

	// If one couldn't be found
	if (it == unloadedStates.end()) {
		LOGGER->Log("Failed to get path of state \"", m_stateName, "\". State not specified in game definition file.");
		return nullptr;
	}

	// Return a pointer to the matching state for the specified name
	return it->second;
}