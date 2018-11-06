#include "app_state_playing.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "AppStateLoader.h"

App_State_Playing::App_State_Playing() : Base_App_State("app_state_playing")
{
	
}

void App_State_Playing::Init()
{

	std::ifstream ii("data/keybindings.json");
	ii >> j;

	for (json::iterator it = j.begin(); it != j.end(); ++it) {
		keyboard.AddKeyBinding(it.key(), it.value());
	}

}

void App_State_Playing::DrawCube(float xPos, float yPos, float zPos)
{
	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);
	glBegin(GL_POLYGON);
	glVertex3f(0.0f, 0.0f, 0.0f);	// top face
	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);	// front face
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);	// right face
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);	// left face
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);	// bottom face
	glVertex3f(0.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);	// back face
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(0.0f, -1.0f, -1.0f);
	glEnd();
	glPopMatrix();
}

void App_State_Playing::Update(float dt)
{
	UpdateEntities(dt);
	DrawCube(0, 0, -6);
}

void App_State_Playing::Event(SDL_Event& event)
{

		if (event.type == SDL_KEYDOWN){
			// If this key is a binding, we may do special actions with it such as player control
			if (keyboard.IsKeyBinding(SDL_GetKeyName(event.key.keysym.sym)))
			{
				std::string sBinding = "";
				sBinding = keyboard.GetKeyBinding(SDL_GetKeyName(event.key.keysym.sym)); // Returns the binding in relation to what key is pressed.

				if (sBinding == "MOVEMENT_ROBOT_FORWARD"){
					std::cout << "Forward!" << std::endl;

				}else if(sBinding == "MOVEMENT_ROBOT_BACKWARD"){
					std::cout << "Back it up!" << std::endl;
				}
			}

			// Some keys will not need binding.
			switch (event.key.keysym.sym)
			{
			case SDLK_w:
				break;
			case SDLK_UP:
				break;
			case SDLK_a:
				break;
			case SDLK_LEFT:
				break;
			case SDLK_s:
				break;
			case SDLK_DOWN:
				break;
			case SDLK_d:
				break;
			case SDLK_RIGHT:
				break;
			case SDLK_ESCAPE:
				break;
			}

		}
		else if (event.type == SDL_QUIT){

		}

}

void App_State_Playing::TogglePause() {
	if (bPaused) {
		bPaused = false;
	}
	else {
		bPaused = true;
	}
}

void App_State_Playing::ExitState()
{
}
