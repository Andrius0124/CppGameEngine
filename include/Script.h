

#ifndef SCRIPT_H
#define SCRIPT_H

#include <string>

struct Script {

	enum Language { LUA };


	Script(unsigned int ownerEntityId, Language programmingLanguage, std::string sourceCode);


	std::string textContent;


	Language language;


	unsigned int attachedEntityId;

};

#endif // !SCRIPT_H