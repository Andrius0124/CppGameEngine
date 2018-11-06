/**
* @Author	Dale Curry
*
* @brief
*/

#ifndef OPENAL_SOUND_H
#define OPENAL_SOUND_H


#include <iostream>
#include <fstream>
#include <Windows.h>
#include "openAL_Core.h"
#include "al.h"
#include "alc.h" 
#include "alut.h"


class openAL_sound
{
public:

	/*!
	\ Initialise OpenAL Default device
	\ Sets OpenAL device/context to the current device/context in order to initialise and be used.
	*/
	void initDefaultDevice();

	/*!
	\ 3D Listener position
	\ sets the listeners position in the 3D space x,y,z co-ordinates.
	*/
	void setListenerposition(const float x, const float y, const float z);

	/*!
	\ Velocity position
	\ sets the velocity's position in the 3D space x,y,z co-ordinates.
	*/
	void setListenerVelocity(const float x, const float y, const float z);

	/*!
	\ Orientation position
	\ sets the orientation's position in the 3D space x,y,z co-ordinates.
	*/
	void setListenerOrientation(const float atX, const float atY, const float atZ, const float upX, const float upY, const float upZ);

	/*!
	\ Load from file
	\ function passes through the private function for loading a wav file.
	*/
	openAL_Core* entityLoadedFromFile(const std::string fp);

private:
	/*!
	\ Load Wav
	\ here we create buffers for the loaded wav file and read the data chunks of
	\ of that file which determines the files Format, Channels, Sample rate
	\ Byte rate, Alignment and Bit rate.
	\
	\ Wav file break down information link ( http://soundfile.sapp.org/doc/WaveFormat/ )
	*/
	openAL_Core* loadWavFile(const std::string fp);

	/*!
	\ OpenAL device object
	*/
	ALCdevice* device;
	
	/*!
	\ OpenAL context object
	*/
	ALCcontext* context;

};

#endif