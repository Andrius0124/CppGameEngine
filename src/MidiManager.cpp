#include "MidiManager.h"
#include "Log.h"

MidiManager midiManager;

MidiManager::MidiManager()
{
	// Open the MIDI output port
	if (midiOutOpen(&midiDevice, 0, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR) {
		LOGGER->Log("Failed to open MIDI output port.\n");
	}
}

MidiManager::~MidiManager()
{
	// Stop all midi notes that are playing
	midiOutReset(midiDevice);

	// Close the midi output port
	midiOutClose(midiDevice);
}

void MidiManager::update()
{
	Note note;
	for (unsigned int i = 0; i < currentlyPlayingNotes.size(); ++i) {
		// Get the current note in the loop
		note = currentlyPlayingNotes.at(i);

		// If the note has played for it's full duration
		if ((std::clock() - note.timestamp) / (double)CLOCKS_PER_SEC * 1000 >= note.duration) {
			// Stop playing the note
			channelNoteOff(note.channel, note.pitch);

			// Remove this note from the list of notes that are playing
			currentlyPlayingNotes.erase(currentlyPlayingNotes.begin() + i);

			// Move back one place so that we don't skip the next note
			// Looping backwards would also solve this problem but would be less efficient due to cache misses
			i--;
		}
	}
}

void MidiManager::channelSetVoice(uint32_t m_channelId, uint32_t m_instrumentId)
{
	union {
		DWORD dword;
		unsigned char properties[4] = { 0b11000000, 0, 0, 0 };
	} midiMessage;

	// Decrement 1 from the channel id so that channels start at 0 instead of 1
	m_channelId--;

	// Remove all but the 4 least significant bits
	m_channelId &= 0x0f;

	// Set the midi channel of the message
	midiMessage.properties[0] |= m_channelId;

	// Set the instrument of the message
	midiMessage.properties[1] = m_instrumentId;

	// Send the midi message
	if (midiOutShortMsg(midiDevice, midiMessage.dword) != MMSYSERR_NOERROR) {
		// Log this error
		LOGGER->Log("Failed to change midi instrument for channel %d.\n", m_channelId);
	}
}

void MidiManager::channelNoteOn(uint32_t m_channelId, uint32_t m_pitch, uint32_t m_velocity)
{
	union {
		DWORD dword;
		unsigned char properties[4] = { 0b10010000, 0, 0, 0 };
	} midiMessage;

	// Decrement 1 from the channel id so that channels start at 0 instead of 1
	m_channelId--;

	// Remove all but the 4 least significant bits using a bitmask
	m_channelId &= 0x0f;

	// Set the midi channel of the message
	midiMessage.properties[0] |= m_channelId;

	// Set the pitch of the message
	midiMessage.properties[1] = m_pitch;

	// Set the velocity of the message
	midiMessage.properties[2] = m_velocity;

	// Send the midi message
	if (midiOutShortMsg(midiDevice, midiMessage.dword) != MMSYSERR_NOERROR) {
		// Log this error
		LOGGER->Log("Failed to change midi instrument for channel %d.\n", m_channelId);
	}
}

void MidiManager::channelNoteOff(uint32_t m_channelId, uint32_t m_pitch)
{
	union {
		DWORD dword;
		unsigned char properties[4] = { 0b10000000, 0, 0, 0 };
	} midiMessage;

	// Decrement 1 from the channel id so that channels start at 0 instead of 1
	m_channelId--;

	// Remove all but the 4 least significant bits
	m_channelId &= 0x0f;

	// Set the midi channel of the message
	midiMessage.properties[0] |= m_channelId;

	// Set the pitch of the message
	midiMessage.properties[1] = m_pitch;

	// Send the midi message
	if (midiOutShortMsg(midiDevice, midiMessage.dword) != MMSYSERR_NOERROR) {
		// Log this error
		LOGGER->Log("Failed to change midi instrument for channel %d.\n", m_channelId);
	}
}

void MidiManager::playKey(uint32_t m_channelId, uint32_t m_pitch, uint32_t m_velocity, uint32_t m_duration)
{
	// Start playing the note
	channelNoteOn(m_channelId, m_pitch, m_velocity);

	// Add the note to the list of notes which need to be stopped eventually
	currentlyPlayingNotes.push_back(Note{ m_channelId, m_pitch, m_duration, clock() });
}

void MidiManager::playPercussion(uint32_t m_instrument, uint32_t m_velocity, uint32_t m_duration)
{
	// Start playing the note
	channelNoteOn(10, m_instrument, m_velocity);

	// Add the note to the list of notes which need to be stopped eventually
	currentlyPlayingNotes.push_back(Note{ 10, m_instrument, m_duration, clock() });
}