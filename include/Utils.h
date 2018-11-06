/**
 * @Author	Addo White
 * @date	16th February 2017
 * @brief	Some handy dandy static methods for commonly required tasks.
 */

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <math.h>

#define PI    3.1415927f
#define PI180 PI / 180.0f

class Utils {

public:
	/**
	* Get the sin of an angle in degrees.
	* @param The angle in degrees.
	* @return The sin of the angle.
	* @Author Addo White
	*/
	static float sinDegrees(float angle) {
		return sinf(degToRad(angle));
	}
	/**
	* Get the cos of an angle in degrees.
	* @param The angle in degrees.
	* @return The cos of the angle.
	* @Author Addo White
	*/
	static float cosDegrees(float angle) {
		return cosf(degToRad(angle));
	}

	/**
	* Method to convert degrees into radians.
	* @param The angle in degrees.
	* @return The angle in radians.
	* @Author Addo White
	*/
	static float degToRad(float degrees) {
		return degrees * PI180;
	}

	/**
	* Method to convert radians into degrees.
	* @param The angle in radians.
	* @return The angle in degrees.
	* @Author Addo White
	*/
	static float radToDeg(float radians) {
		return radians / PI180;
	}

	/**
	 * Method to split a string by a certain character.
	 * @param inputString the string to be split.
	 * @param delimeter is the character at which the string should be split at each occurance.
	 * @return A vector of strings, in which each entry is the characters between (not including) subsequent occurances of the delimeting character.
	 * @Author Addo White
	 */
	static std::vector<std::string> stringSplit(std::string inputString, char delimeter);

	/**
	 * Method to read text from a file.
	 * @param filePath is the path to the file which will be read.
	 * @return A string representing the contents of the file.
	 * @Author Addo White
	 */
	static std::string readFile(std::string filePath);

	/**
	* @Author	Andrius Murauskas
	* @date	16th March 2017
	*/

	static const std::string CurrentTime();
	static const std::string CurrentDay();

};

#endif // !UTILS_H