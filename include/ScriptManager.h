#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include <sol.hpp>
#include <unordered_map>
#include "Script.h"
#include "base_app_state.h"

class ScriptManager {

public:
	enum ScriptEvent { LOAD, UPDATE, KEYDOWN, KEYUP };


	ScriptManager();


	unsigned int addScript(Script* newScript);


	void executeScript(Base_App_State* appState, ScriptEvent scriptEvent, unsigned int scriptId, std::string callbackData = "");

	void loadScripts(Base_App_State* appState);


	void updateScripts(Base_App_State* appState);


	void keydownScripts(Base_App_State* appState, std::string key);


	void keyupScripts(Base_App_State* appState, std::string key);


	bool freeScript(unsigned int scriptId);


	bool freeScripts();


	void exposeScriptDeltaTime(float scriptDeltaTime);


	void exposeScriptFramerate(unsigned int scriptFramerate);


	void exposeScriptMousePosition(glm::ivec2 mousePos);

private:

	unsigned int getNewScriptId();


	unsigned int nextNewScriptId;


	Script* getScript(unsigned int scriptId);


	bool prepareScript(Script* script);


	bool prepareLUAScript(Script* script);


	sol::table getEntityInfoForScript(std::string entityName);


	void setTextValue(std::string entityName, std::string value);


	bool callScriptFunction(sol::state* luaState, std::string functionName, std::string callbackData = "", bool flag = false, int arg1 = -1, int arg2 = -1);


	std::unordered_map<unsigned int, Script*> scripts;


	std::unordered_map<Script*, sol::state*> luaStates;


	float deltaTime;


	float framerate;


	glm::ivec2 mousePosition;


	sol::state* currentLuaState;

};


extern ScriptManager scriptManager;

#endif // !SCRIPTMANAGER_H
