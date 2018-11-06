#ifndef OPENAL_CORE_H
#define OPENAL_CORE_H


#include <iostream>
#include <Windows.h>
#include "entity.h"
#include "al.h"
#include "alc.h" 
#include "alut.h"


enum eSoundState
{
	playSound = 0,
	stopSound = 1,
	pauseSound = 2,
	rewindSound = 3
};

class openAL_Core : public Entity
{
public:

	/*!
	\ Constructor - new instance
	\ new instance of the class
	\ *data* stores the audio content
	\ *size* stores the size of the audio content
	\ *frequency* stores the audios frequency
	\ *channels* stores the channels amount that audio can be used for
	*/
	openAL_Core(unsigned char* data, const unsigned int size, const unsigned int frequency, const short channels);

	/*!
	\ copy of the other instance
	\ reference to the other instance to be used when copying
	*/
	openAL_Core(const openAL_Core &src);

	/*!
	\ Deintializer.
	\ Deintializer the things we used for sound, used for when we shut down.
	*/
	void deInitSound();

	/*!
	\ Play
	\ starts playing the audio file
	*/
	void play(const bool isPlaying);

	/*!
	\ Stop
	\ stops playing the audio file
	*/
	void stop();

	/*!
	\ Pause
	\ Pauses the playing audio file
	*/
	void pause();

	/*!
	\ Rewind
	\ rewinds the audio position to the beginning
	*/
	void rewind();

	/*!
	\ Set Audio Position
	\ sets the audio position with x,y,z co-ordinates
	*/
	void setAudioPosition(const float x, const float y, const float z);

	/*!
	\ Set Audio Direction
	\ sets the audio direction with x,y,z co-ordinates
	*/
	void setAudioDirection(const float x, const float y, const float z);

	/*!
	\ Set Audio Velocity
	\ sets velocity to the audio to determine distance x,y,z co-ordinates
	*/
	void setAudioVelocity(const float x, const float y, const float z);

	/*!
	\ Set Audio Pitch
	\ sets pitch to the audio adds for effect
	*/
	void setAudioPitch(const float pitch);

	/*!
	\ Set Audio Gain
	\ sets gain to the audio
	*/
	void setAudioGain(const float gain);

	 


private:

	ALuint bufferId;

	ALuint sourceId;

	eSoundState soundState;
};


#endif 

