#include <sol.hpp>

#include "Log.h"
#include "ScriptManager.h"
#include "MidiManager.h"
#include "application.h"
#include "manager_app_state.h"
#include "Utils.h"
#include "Text.h"


// Create a script manager accessible from global scope
ScriptManager scriptManager;

ScriptManager::ScriptManager()
{
	// The id of the first script is always zero
	nextNewScriptId = 0;

	deltaTime = 0;
}

void ScriptManager::loadScripts(Base_App_State* m_appState)
{
	for (auto scriptPair : scripts) {
		executeScript(m_appState, ScriptEvent::LOAD, scriptPair.first);
	}
}

void ScriptManager::updateScripts(Base_App_State* m_appState)
{
	for (auto scriptPair : scripts) {
		executeScript(m_appState, ScriptEvent::UPDATE, scriptPair.first);
	}
}

void ScriptManager::keydownScripts(Base_App_State* m_appState, std::string m_key)
{
	for (auto scriptPair : scripts) {
		executeScript(m_appState, ScriptEvent::KEYDOWN, scriptPair.first, m_key);
	}
}

void ScriptManager::keyupScripts(Base_App_State* m_appState, std::string m_key)
{
	for (auto scriptPair : scripts) {
		executeScript(m_appState, ScriptEvent::KEYUP, scriptPair.first, m_key);
	}
}

unsigned int ScriptManager::addScript(Script* m_newScript)
{
	// Get a new unique id for the script
	unsigned int scriptId = getNewScriptId();

	// Add the id and script to the map
	scripts.insert(std::pair<unsigned int, Script*>(scriptId, m_newScript));

	// Get the script ready for first use
	prepareScript(m_newScript);

	// Return the id of the script
	return scriptId;
}

bool ScriptManager::prepareScript(Script* m_script)
{
	switch (m_script->language) {

	case Script::Language::LUA:
		return prepareLUAScript(m_script);

	default:
		return false;

	}
}

bool ScriptManager::prepareLUAScript(Script* m_script)
{
	// Create a new LUA state for this script
	sol::state* luaState = new sol::state();

	// Import some standard LUA libraries
	luaState->open_libraries(sol::lib::base);

	// Bind some C++ functions to make them available to the LUA script
	luaState->set_function("midiSetInstrument", &MidiManager::channelSetVoice, &midiManager);
	luaState->set_function("midiOn"           , &MidiManager::channelNoteOn  , &midiManager);
	luaState->set_function("midiOff"          , &MidiManager::channelNoteOff , &midiManager);
	luaState->set_function("midiNote"         , &MidiManager::playKey        , &midiManager);
	luaState->set_function("midiPercussion"   , &MidiManager::playPercussion , &midiManager);
	luaState->set_function("changeScene"      , &Manager_App_State::RequestStateChange, application.manager_app_state);
	luaState->set_function("reloadScene"      , &Manager_App_State::RequestReloadState, application.manager_app_state);
	luaState->set_function("setFPS"           , &Application::setFrameRate, &application);
	luaState->set_function("captureMouse"     , &Application::captureMouse, &application);
	luaState->set_function("getEntity"        , &ScriptManager::getEntityInfoForScript, this);
	luaState->set_function("setText"          , &ScriptManager::setTextValue, this);
	luaState->set_function("sqrt"             , sqrtf);

	// Bind some math functions
	luaState->set_function("sin", Utils::sinDegrees);
	luaState->set_function("cos", Utils::cosDegrees);

	// Add the new LUA state to the dictionary of states and scripts
	luaStates.insert(std::pair<Script*, sol::state*>(m_script, luaState));

	// Execute the script to define all variables and functions
	luaState->do_string(m_script->textContent);

	return true;
}

sol::table ScriptManager::getEntityInfoForScript(std::string m_entityName)
{
	// Get a pointer to the entity with the specified name
	Entity* entity = application.getEntity(m_entityName);

	// Get the position, rotation and scale of the entity
	glm::vec3 entityPosition = entity->Position();
	glm::vec3 entityRotation = entity->Rotation();
	glm::vec3 entityScale = entity->Scale();
	glm::vec3 entityHull = entity->Hull();
	glm::vec3 entityVelocity = entity->Velocity();

	// Create the table to hold all the tables
	return currentLuaState->create_table_with(
		"position", currentLuaState->create_table_with(
			"x", entityPosition.x,
			"y", entityPosition.y,
			"z", entityPosition.z
		),
		"rotation", currentLuaState->create_table_with(
			"x", entityRotation.x,
			"y", entityRotation.y,
			"z", entityRotation.z
		),
		"scale", currentLuaState->create_table_with(
			"x", entityScale.x,
			"y", entityScale.y,
			"z", entityScale.z
		),
		"hull", currentLuaState->create_table_with(
			"x", entityHull.x,
			"y", entityHull.y,
			"z", entityHull.z
		),
		"velocity", currentLuaState->create_table_with(
			"x", entityVelocity.x,
			"y", entityVelocity.y,
			"z", entityVelocity.z
		)
	);
}

void ScriptManager::setTextValue(std::string m_entityName, std::string m_value)
{
	// Get the entity with the specified name
	Entity* entity = application.getEntity(m_entityName);
	if (entity == nullptr) {
		LOGGER->Log("Failed to get Text entity \"" + m_entityName + "\": No entity was found with that name.");
		return;
	}

	// Attempt to cast the Entity to a Text Entity
	Text* text = dynamic_cast<Text*>(entity);

	// If an entity was found but it's not a Text entity
	if (text == NULL) {
		LOGGER->Log("Failed to get Text entity \"" + m_entityName + "\": The entity with that name is not a Text entity.");
		return;
	}

	// Get the length of the char array in bytes (including the null terminator)
	int messageLength = strlen(m_value.c_str()) + 1;

	// Create the char array to pass to the setMsg() method
	char* message = new char[messageLength];

	// Copy the contents of the 'value' string into the char array
	strcpy_s(message, messageLength, m_value.c_str());

	// Pass the new char array to the Text entity
	text->setMsg(message);
}

unsigned int ScriptManager::getNewScriptId()
{
	// Return the value of nextNewScriptId before it is incremented
	return nextNewScriptId++;
}

void ScriptManager::executeScript(Base_App_State* m_appState, ScriptEvent m_scriptEvent, unsigned int m_scriptId, std::string m_callbackData)
{
	// Get the script to execute
	Script* script = getScript(m_scriptId);

	sol::state* luaState = luaStates[script];

	currentLuaState = luaState;

	// If it couldn't be found
	if (script == nullptr) {
		LOGGER->Log("Couldn't execute script with id %d: script does not exist.", m_scriptId);

		return;
	}

	// Get the entity this script is attached to
	Entity* entity = m_appState->getEntity(script->attachedEntityId);

	if (entity == NULL) return;

	// Set the transform information for the entity the script is attached to
	luaState->create_named_table("position", "x", entity->Position().x, "y", entity->Position().y, "z", entity->Position().z);
	luaState->create_named_table("rotation", "x", entity->Rotation().x, "y", entity->Rotation().y, "z", entity->Rotation().z);
	luaState->create_named_table("scale", "x", entity->Scale().x, "y", entity->Scale().y, "z", entity->Scale().z);
	luaState->create_named_table("velocity", "x", entity->Velocity().x, "y", entity->Velocity().y, "z", entity->Velocity().z);
	luaState->create_named_table("hull", "x", entity->Hull().x, "y", entity->Hull().y, "z", entity->Hull().z);

	// Provide the mouse position to the LUA script
	luaState->create_named_table("mouse", "x", mousePosition.x, "y", mousePosition.y);

	// Add the current deltaTime
	luaState->set("deltaTime", deltaTime);

	// Add the current framerate
	luaState->set("fps", framerate);

	switch (m_scriptEvent) {

		case ScriptEvent::LOAD:
			callScriptFunction(luaState, "load");
			break;

		case ScriptEvent::UPDATE:
			callScriptFunction(luaState, "update");
			break;

		case ScriptEvent::KEYDOWN:
			callScriptFunction(luaState, "keypress", m_callbackData, true);
			break;

		case ScriptEvent::KEYUP:
			callScriptFunction(luaState, "keypress", m_callbackData, false);
			break;

	}

	// Get the transform information from the script
	sol::table entityPosition = luaState->get<sol::table>("position");
	sol::table entityRotation = luaState->get<sol::table>("rotation");
	sol::table entityScale    = luaState->get<sol::table>("scale");

	// Apply the transform to the entity
	entity->SetTransform(Transform(
		glm::vec3(entityPosition["x"], entityPosition["y"], entityPosition["z"]),
		glm::vec3(entityRotation["x"], entityRotation["y"], entityRotation["z"]),
		glm::vec3(entityScale["x"], entityScale["y"], entityScale["z"])
	));

	// Get the transform information from the script
	sol::table entityHull = luaState->get<sol::table>("hull");
	sol::table entityVelocity = luaState->get<sol::table>("velocity");

	// Apply the transform to the entity
	entity->SetPhysics(Physics(
		glm::vec3(entityHull["x"], entityHull["y"], entityHull["z"]),
		glm::vec3(entityVelocity["x"], entityVelocity["y"], entityVelocity["z"])
	));

}

Script* ScriptManager::getScript(unsigned int m_scriptId)
{
	// Find the script with the given id
	std::unordered_map<unsigned int, Script*>::iterator it = scripts.find(m_scriptId);

	// If it couldn't be found
	if (it == scripts.end()) return nullptr;

	// Return a pointer to the script
	return it->second;
}


bool ScriptManager::callScriptFunction(sol::state* m_luaState, std::string m_functionName, std::string m_callbackData, bool m_flag, int m_arg1, int m_arg2)
{
	// Get a reference to the script function
	sol::protected_function luaFunction = m_luaState->get<sol::protected_function>(m_functionName);

	// Function does not exist, in some cases this is fine (pre-defined functions are not mandatory and can be added as needed but should be called automatically if it exists)
	if (!luaFunction.valid()) return false;

	if (m_callbackData == "") {
		// Call the function
		auto result = luaFunction(m_arg1, m_arg2);

		// If everything is hunky-dory
		if (result.valid()) return true;

		sol::error error = result;

		// Print the error and return false
		LOGGER->Log(error.what());
	} else if (m_arg1 != -1 || m_arg2 != -1) {
		// Call the function
		auto result = luaFunction(m_callbackData, m_arg1, m_arg2);

		// If everything is hunky-dory
		if (result.valid()) return true;

		sol::error error = result;

		// Print the error and return false
		LOGGER->Log(error.what());
	} else {
		// Call the function
		auto result = luaFunction(m_callbackData, m_flag);

		// If everything is hunky-dory
		if (result.valid()) return true;

		sol::error error = result;

		// Print the error and return false
		LOGGER->Log(error.what());
	}

	return false;
}

bool ScriptManager::freeScript(unsigned int m_scriptId)
{
	// Get the script with the specified id
	auto scriptIt = scripts.find(m_scriptId);

	// If the script could not be found
	if (scriptIt == scripts.end()) {
		// Log the error
		LOGGER->Log("Failed to free script: No script found with id %d.", m_scriptId);

		// Return failure
		return false;
	}

	// Get a pointer to the script
	Script* script = scriptIt->second;

	// Remove the script from the dictionary
	scripts.erase(scriptIt);

	// Get the LUA state for this script
	auto stateIt = luaStates.find(script);

	// Delete the script pointer
	delete script;

	// If the LUA state could not be found
	if (stateIt == luaStates.end()) {
		// Log the error
		LOGGER->Log("Failed to free LUA state: No state found for script with id %d.", m_scriptId);

		// Return failure
		return false;
	}

	// Delete the LUA state
	delete stateIt->second;

	// Remove the LUA state from the dictionary
	luaStates.erase(stateIt);

	// Return success
	return true;
}

bool ScriptManager::freeScripts()
{
	bool errorOccured = false;

	while (!scripts.empty()) {
		errorOccured = !freeScript(scripts.begin()->first) || errorOccured;
	}

	return errorOccured;
}

void ScriptManager::exposeScriptDeltaTime(float scriptDeltaTime)
{
	deltaTime = scriptDeltaTime;
}

void ScriptManager::exposeScriptFramerate(unsigned int scriptFramerate)
{
	framerate = scriptFramerate;
}

void ScriptManager::exposeScriptMousePosition(glm::ivec2 mousePos)
{
	mousePosition = mousePos;
}
