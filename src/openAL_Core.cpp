/**
* @Author	Dale Curry
*
* @brief	The core of OpenAL used for positioning 3D sound and passing states for the sound to be played.
*/

#include "openAL_Core.h"



openAL_Core::openAL_Core(unsigned char* data, const unsigned int size, const unsigned int frequency, const short channels) :
	bufferId(0), sourceId(0), soundState(eSoundState::stopSound)
{
	alGenBuffers(1, &bufferId);
	alBufferData(bufferId, (channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, data, size, frequency);

	alGenSources(1, &sourceId);
	alSourcei(sourceId, AL_BUFFER, bufferId);

};


openAL_Core::openAL_Core(const openAL_Core &src)
{
};


void openAL_Core::deInitSound()
{
	alDeleteSources(1, &sourceId);
	alDeleteBuffers(1, &bufferId);
};




////////////////////////////////////////////////////////////////////
//BEGINNING OF STATE FUNCTIONS
////////////////////////////////////////////////////////////////////
void openAL_Core::play(const bool isPlaying)
{
	if (soundState != eSoundState::playSound)
	{
		soundState = eSoundState::playSound;

		alSourcei(sourceId, AL_LOOPING, isPlaying);
		alSourcePlay(sourceId);
	}
};


void openAL_Core::pause()
{
	if (soundState != eSoundState::pauseSound)
	{
		soundState = eSoundState::pauseSound;

		alSourcePause(sourceId);
	}
};


void openAL_Core::stop()
{
	if (soundState != eSoundState::stopSound)
	{
		soundState = eSoundState::stopSound;

		alSourceStop(sourceId);
	}
};


void openAL_Core::rewind()
{
	if (soundState != eSoundState::rewindSound)
	{
		soundState = eSoundState::rewindSound;

		alSourceStop(sourceId);
	}
};
////////////////////////////////////////////////////////////////////
//END OF STATE FUNCTIONS
////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////
//BEGINNING OF POSITIONING / DIRECTION / VELOCITY / PITCH / GAIN
////////////////////////////////////////////////////////////////////
void openAL_Core::setAudioPosition(const float x, const float y, const float z)
{
	alSource3f(sourceId, AL_POSITION, x, y, z);
};


void openAL_Core::setAudioDirection(const float x, const float y, const float z)
{
	float direction[3] = { x, y, z };

	alSourcefv(sourceId, AL_DIRECTION, direction);
};


void openAL_Core::setAudioVelocity(const float x, const float y, const float z)
{
	alSource3f(sourceId, AL_VELOCITY, x, y, z);
};


void openAL_Core::setAudioPitch(const float pitch)
{
	alSourcef(sourceId, AL_PITCH, pitch);
};


void openAL_Core::setAudioGain(const float gain)
{
	alSourcef(sourceId, AL_GAIN, gain);
};
////////////////////////////////////////////////////////////////////
//END OF POSITIONING / DIRECTION / VELOCITY / PITCH / GAIN
////////////////////////////////////////////////////////////////////