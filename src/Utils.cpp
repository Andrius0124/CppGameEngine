#include "Utils.h"

#include <time.h>
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<std::string> Utils::stringSplit(std::string m_inputString, char m_delimeter)
{
	// Create the list of strings that we will be returning
	std::vector<std::string> chunks;

	// Create a stringstream with the contents of the inputString
	std::istringstream stream(m_inputString);

	// The current chunk of the whoole string
	std::string currentChunk;

	// Iterate through the string, executing the contents of the loop each time the delimeting character is encountered
	while (std::getline(stream, currentChunk, m_delimeter)) {
		// Add the current substr to the array of strings
		chunks.push_back(currentChunk);
	}

	// Return the list of strings
	return chunks;
}

std::string Utils::readFile(std::string m_filePath)
{
	// This will hold the entire contents of the file
	std::stringstream fileContents;

	// This will hold the current line of text being read
	std::string currentLine;

	// Open the file for reading
	std::ifstream file(m_filePath);

	// If the file could not be opened
	if (file.is_open() == false) {
		// Output an error and return an empty string
		std::cout << "Could not open file \"" << m_filePath << "\"." << std::endl;
		return "";
	}

	// Loop through all the lines in the file
	while (getline(file, currentLine)) {
		// Add the current line of text to the stringstream
		fileContents << currentLine << std::endl;
	}

	// Close the file now that we are done with it
	file.close();

	// Return the contents of the file as a string
	return fileContents.str();
}

/**
* @Author	Andrius Murauskas
* @date	16th March 2017
*/
const std::string Utils::CurrentTime()
{
	time_t     now = time(NULL);
	struct tm  tstruct;
	char       buf[80];
	localtime_s(&tstruct, &now);
	strftime(buf, sizeof(buf), "%H:%M:%S", &tstruct);
	return buf;
}
const std::string Utils::CurrentDay()
{
	time_t     now = time(NULL);
	struct tm  tstruct;
	char       buf[80];
	localtime_s(&tstruct, &now);
	strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);

	return buf;
}