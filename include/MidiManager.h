/**
* @brief	A class for playing midi sounds.
*
*/

#ifndef MIDIMANAGER_H
#define MIDIMANAGER_H

#include <Windows.h>
#include <mmsystem.h>
#include <stdint.h>
#include <vector>
#include <ctime>

class MidiManager {

public:
	/*!
	* \brief The default constructor.
	* 
	*/
	MidiManager();

	/*!
	* \brief The default destructor.
	* 
	*/
	~MidiManager();

	/*!
	* \brief Checks when notes have finished playing and stops them.
	* 
	*/
	void update();

	/*!
	* \brief Set the instrument to use for the specified channel.
	* \param The channel to set the instrument for.
	* \param The instrument to use on the channel.
	* 
	*/
	void channelSetVoice(uint32_t channelId, uint32_t instrumentId);

	/*!
	* \brief Start playing a note on the specified channel at the specified pitch and volume.
	* \param The channel on which to play the note.
	* \param The pitch to play the note at.
	* \param The volume to play the note at.
	* 
	*/
	void channelNoteOn(uint32_t channelId, uint32_t pitch, uint32_t velocity);

	/*!
	* \brief Stop playing a note on the specified channel at the specified pitch.
	* \param The channel on which to stop playing the note.
	* \param The pitch at which the note is already playing.
	* 
	*/
	void channelNoteOff(uint32_t channelId, uint32_t pitch);

	/*!
	* \brief Play a note for a specified duration and stop it automatically.
	* \param The channel on which to play the note.
	* \param The pitch to play the note at.
	* \param The volume to play the note at.
	* \param The duration of the note.
	* 
	*/
	void playKey(uint32_t channelId, uint32_t pitch, uint32_t velocity, uint32_t duration);

	/*!
	* \brief Play a percussive note for a specified duration and stop it automatically.
	* \param The channel on which to play the note.
	* \param The volume to play the note at.
	* \param The duration of the note.
	* 
	*/
	void playPercussion(uint32_t instrument, uint32_t velocity, uint32_t duration);

private:

	/*!
	* \brief A data structure used to communicate with the MIDI api.
	* 
	*/
	struct Note {
		uint32_t channel;
		uint32_t pitch;
		uint32_t duration;
		clock_t  timestamp;
	};

	/*!
	* \brief The MIDI output device to play sounds with.
	* 
	*/
	HMIDIOUT midiDevice;


	/*!
	* \brief A list of all currently playing notes which should be automatically stopped after their duration expires.
	* 
	*/
	std::vector<Note> currentlyPlayingNotes;

};


/*!
* \brief The pseudo-singleton MidiManager instance. Used like a singleton, but the usual singleton restrictions are not enforced. It is a regular class.
* 
*/
extern MidiManager midiManager;

#endif // !MIDIMANAGER_H
