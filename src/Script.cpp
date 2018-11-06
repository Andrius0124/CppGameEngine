#include "Script.h"

Script::Script(unsigned int m_ownerEntityId, Language m_programmingLanguage, std::string m_sourceCode)
{
	attachedEntityId = m_ownerEntityId;
	language    = m_programmingLanguage;
	textContent = m_sourceCode;
}