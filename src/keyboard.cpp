/**
* @Author	Joseph Radu
* @date		13th January 2017
* @brief	Keyboard for input handling and key bindings!
*
*/

#include "keyboard.h"

Keyboard::Keyboard()
{

}

void Keyboard::AddKeyBinding(std::string binding, std::string key)
{
	vKeyBindings[key] = binding;
}

std::string Keyboard::GetKeyBinding(std::string binding)
{
	return vKeyBindings.at(binding);
}

bool Keyboard::IsKeyBinding(std::string binding)
{
	if (vKeyBindings.find(binding) == vKeyBindings.end()) {
		return false;
	}
	else{
		return true;
	}
}


