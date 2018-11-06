#ifdef _WINDOWS
#include <windows.h>
#endif
#include <SDL\SDL.h>
#include "application.h"
#include "Renderer.h"
#include "MidiManager.h"
#include "ScriptManager.h"

// disable implicit float-double casting
#pragma warning(disable:4305)

Application application;

Application::Application()
{
	
}

Application::~Application()
{
	SDL_DestroyWindow(window);
}

void Application::Init(std::string m_applicationDefinitionFilePath)
{
	framerateManager = FramerateManager();
	framerateManager.setTargetFrameRate(60);

	manager_app_state = new Manager_App_State(m_applicationDefinitionFilePath);
	manager_app_state->Init();
}

void Application::Update()
{
	// Clear the screen
	renderer->clear();

	// Position the virtual camera within the scene before rendering
	renderer->positionCamera();

	// Update and render the current scene
	manager_app_state->Update(framerateManager.getDeltaTime()); // From our last process, how much time has passed? Updating everything should be scaled by this.
																// 60fps game runs normally, 120fps game runs twice as fast? This stops that from happening!

	// Now that rendering is finished, flush the contents of the render-buffer to the window-buffer
	renderer->flush();

	// Update the midi manager
	midiManager.update();

	// Wait until it's time to compute the next frame
	framerateManager.waitForNextFrame();

	// Set the deltaTime value to expose to the users scripts
	scriptManager.exposeScriptDeltaTime(framerateManager.getDeltaTime());

	// Set the framerate value to expose to the users scripts
	scriptManager.exposeScriptFramerate(framerateManager.getCurrentFrameRate());

	// Set the framerate value to expose to the users scripts
	scriptManager.exposeScriptMousePosition(getCursorPosition());
}

void Application::Event(SDL_Event& event)
{
	manager_app_state->Event(event);
}

bool Application::createWindow(glm::ivec2 m_windowSize, std::string m_windowTitle)
{
	// Create the game window
	window = SDL_CreateWindow(m_windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_windowSize.x, m_windowSize.y, SDL_WINDOW_OPENGL);

	// If the window wasn't created successfully
	if (window == nullptr) return false;

	// Create the renderer that will render the game to that window
	renderer = new Renderer(window);

	// Return success
	return true;
}

void Application::Quit()
{
	manager_app_state->FreeCurrentState();
	delete manager_app_state;
	delete renderer;
}

void Application::setFrameRate(unsigned int m_targetFramerate)
{
	framerateManager.setTargetFrameRate(m_targetFramerate);
}

glm::ivec2 Application::getCursorPosition()
{
	int mouseX, mouseY;

	if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
		SDL_GetRelativeMouseState(&mouseX, &mouseY);
	} else {
		SDL_GetMouseState(&mouseX, &mouseY);
	}

	return glm::ivec2(mouseX, mouseY);
}

void Application::captureMouse(bool m_enable)
{
	SDL_SetRelativeMouseMode((m_enable) ? SDL_TRUE : SDL_FALSE);
}

Entity* Application::getEntity(std::string m_entityName)
{
	return manager_app_state->getCurrentState()->getEntity(m_entityName);
}