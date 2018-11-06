
#ifndef APPSTATELOADER_H
#define APPSTATELOADER_H

#include <string>
#include "TinyXML2/tinyxml2.h"
#include "base_app_state.h"
#include "Transform.h"
#include "Transform.h"
#include <vector>

using namespace tinyxml2;

class AppStateLoader {

public:
	/*!
	 * \brief Get the name and path of all appStates in the game.

	 */
	std::vector<std::pair<std::string, std::string>> loadAppStates(std::string filePath);

	/*!
	 * \brief Read an individual app state file.
	 *
	 * \param stateName is a string defining the name which can abe used to refer to the state once loaded.
	 * \param filePath is a string specifying the path to the state file.
	 * \param error is a reference to a string which will be populated with an error message should anything go wrong.
	 * \return The app state that was loaded.
	 *
	 */
	Base_App_State* loadSingleAppState(std::string stateName, std::string filePath, std::string &error);

private:
	/*!
	 * \brief A method which can be used to log errors that occur when loading.
	 *
	 * \param error A string containing a message describing the error.
	 *
	 */
	void logError(std::string error);

	/*!
	 * \brief Find and load the size and title of the game window
	 *
	 * \param rootNode The root node of the XML document.
	 * \param error A reference to a string which will contain the error message should an error occur.
	 * \return A boolean - False if an error occured, true otherwise.
	 *

	 */
	bool loadWindow(XMLElement* rootNode, std::string &error);

	/*!
	 * \brief Find and load all resouces specified in the XML.
	 *
	 * \param rootNode The root node of the XML document.
	 * \param error A reference to a string which will contain the error message should an error occur.
	 * \return A boolean - False if an error occured, true otherwise.
	 *

	 */
	bool loadResources(XMLElement* rootNode, std::string &error);

	/*!
	 * \brief Find and create all entities specified in the XML.
	 *
	 * \param state A pointer to the state which should contain the entities.
	 * \param rootNode The root node of the XML document.
	 * \param error A reference to a string which will contain the error message should an error occur.
 	 * \return A boolean - False if an error occured, true otherwise.
	 *

	 */
	bool loadEntities(Base_App_State* state, XMLElement* rootNode, std::string &error);

	/*!
	 * \brief Create a transform object from the XML.
	 *
	 * \param entityNode The XML node which specifies the entity.
	 * \param entityName A string containing the name of the entity.
	 * \param error A reference to a string which will contain the error message should an error occur.
	 * \return A Transform object with all the properties that were loaded from the XML.
	 *

	*/
	Transform* loadTransformNode(XMLElement* entityNode, std::string entityName, std::string &error);

	/*!
	* \brief Create a physics object from the XML.
	*
	* \param entityNode The XML node which specifies the entity.
	* \param entityName A string containing the name of the entity.
	* \param error A reference to a string which will contain the error message should an error occur.
	*
	* \return A Physics object with all the properties that were loaded from the XML.
	*/
	Physics* loadPhysicsNode(XMLElement* entityNode, std::string entityName, std::string &error);

	/*!
	 * \brief Add a model to an entity.
	 *
	 * \param drawableEntity A pointer to the drawable entity which will have ownership of the model.
	 * \param entityNode The XML node which specifies the entity.
	 * \param entityName A string containing the name of the entity.
	 * \param error A reference to a string which will contain the error message should an error occur.
	 * \return A boolean - False if an error occured, true otherwise.
	 *
	 */
	bool loadModelNode(Drawable* drawableEntity, XMLElement* entityNode, std::string entityName, std::string &error);

};

#endif // !APPSTATELOADER_H