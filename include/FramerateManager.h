/**
* @Author	Addo White
* @date		5th May 2017
* @brief	A class for measuring and controlling the framerate.
*
*/

#ifndef FRAMERATEMANAGER_H
#define FRAMERATEMANAGER_H

#include <Windows.h>
#include <ctime>

class FramerateManager {

public:
	/*!
	 * \brief The default constructor.
	 * \author Addo White
	 */
	FramerateManager();

	/*!
	 * \brief A method to set the desired framerate.
	 * \param The new framerate to stick to.
	 * \author Addo White
	 */
	void setTargetFrameRate(unsigned int newTargetFrameRate);

	/*!
	 * \brief A method to notify the FramerateManager of when a new frame has begun computing.
	 * \author Addo White
	 */
	void notifyFrameStart();

	/*!
	 * \brief Sleeps the main thread until it's time to begin computing the next frame.
	 * \author Addo White
	 */
	void waitForNextFrame();

	/*!
	 * \brief Get the current framerate.
	 * \return Returns an unsigned integer indicating what the framerate is currently measured to be.
	 * \author Addo White
	 */
	unsigned int getCurrentFrameRate();

	/*!
	 * \brief Get the current deltatime value.
	 * \return Returns a float value indicating the ratio to which movement should be sped up to compensate for frames which are taking too long to compute.
	 * \author Addo White
	 */
	float getDeltaTime();

private:

	/*!
	 * \brief A timestamp indicating when the current frame begin computing.
	 * \author Addo White
	 */
	clock_t frameStartTimestamp;

	/*!
	 * \brief A timestamp indicating when the last time the current fps was measured (it's measures once every second).
	 * \author Addo White
	 */
	clock_t secondStartTimeStamp;

	/*!
	 * \brief The framerate the game should run at.
	 * \author Addo White
	 */
	unsigned int targetFramerate;

	/*!
	 * \brief How many millisecond each frame can take to compute given the targetFramerate.
	 * \author Addo White
	 */
	float targetMsPerFrame;

	/*!
	* \brief Stores what the framerate was when it was last calculated.
	* \author Addo White
	*/
	unsigned int currentFrameRate;

	/*!
	* \brief Used to count the number of frames computed in the timespan of a second.
	* \author Addo White
	*/
	unsigned int frameCounter;

	/*!
	* \brief The current deltatime value.
	* \author Addo White
	*/
	float deltaTime;

};

#endif // !FRAMERATEMANAGER_H