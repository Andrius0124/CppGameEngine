#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>

#include "base_app_state.h"
#include "keyboard.h"
#include "json.hpp"
#include "drawable.h"

class App_State_Playing : public Base_App_State
{
private:

	using json = nlohmann::json; // JSON reader.

	bool bPaused;

	Keyboard keyboard;
	json j;

	int iCameraSelected;

	GLuint image;

	// Bespoke Functions
	void TogglePause();

	void DrawCube(float xPos, float yPos, float zPos);


public:
	/*!
	\ brief Default constructor..
	*/
	App_State_Playing();

	/*!
	\ brief Initialise state.
	*/
	void Init();

	/*!
	\ brief Update state.
	\ param float timescale
	*/
	void Update(float dt);

	/*!
	\ brief Exit the state and delete anything we might need to.
	*/
	void ExitState();

	/*!
	\ brief Pass any events that have happened on the window.
	\ param SDL_Event
	*/
	void Event(SDL_Event& event);

};