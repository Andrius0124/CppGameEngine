#include "FramerateManager.h"
#include <stdio.h>

FramerateManager::FramerateManager()
{
	frameCounter = 0;
	deltaTime = 0;
	frameStartTimestamp = secondStartTimeStamp = clock();
}

void FramerateManager::setTargetFrameRate(unsigned int m_newTargetFrameRate)
{
	// Set the target framerate
	targetFramerate = m_newTargetFrameRate;

	// Calculate the maximum number of milliseconds each frame is allowed to take
	targetMsPerFrame = 1000.0f / (float)targetFramerate;
}

unsigned int FramerateManager::getCurrentFrameRate()
{
	return currentFrameRate;
}

float FramerateManager::getDeltaTime()
{
	return deltaTime;
}

void FramerateManager::notifyFrameStart()
{
	// Record a timestamp of when we started computing this frame
	frameStartTimestamp = clock();

	// Increment the counter of frames
	frameCounter++;

	// If a full second has passed since we last updated the fps
	if (clock() - secondStartTimeStamp > CLOCKS_PER_SEC) {
		// Record the framerate
		currentFrameRate = frameCounter;

		// Reset the frame counter
		frameCounter = 0;

		// Record a timestamp at the start of next second
		secondStartTimeStamp = clock();
	}
}

void FramerateManager::waitForNextFrame()
{
	// Measure how long it took to compute the most recent frame
	float frameTime = (clock() - frameStartTimestamp) / (float)CLOCKS_PER_SEC * 1000.0f;

	// How much spare time do we have to sleep?
	int sleepTime = targetMsPerFrame - frameTime;

	// If we have time to sleep
	if (sleepTime > 0) {
		// Sleep until the next frame
		Sleep(sleepTime);
	}

	// Calculate how much faster things should move (in relation to a framerate of 60fps)
	deltaTime = ((frameTime > targetMsPerFrame) ? frameTime : targetMsPerFrame) / 16.6666f;

	// We have now finished sleeping and it is time for the next frame
	// Record the timestamp
	notifyFrameStart();
}