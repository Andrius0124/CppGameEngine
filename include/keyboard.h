#pragma once

#include <map>
#include <vector>
#include <iostream>

class Keyboard
{
private:
	std::map<std::string, std::string> vKeyBindings;
public:
	Keyboard();

	/*!
	\brief Add a key binding.
	\param std::string
	\param std::string
	*/
	void AddKeyBinding(std::string binding, std::string key);

	/*!
	\brief Get key binding.
	\param string Binding
	\return string
	*/
	std::string GetKeyBinding(std::string binding);

	/*!
	\brief Check if key is a binding.
	\param string Binding
	\return bool
	*/
	bool IsKeyBinding(std::string binding);

};