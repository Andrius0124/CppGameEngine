/**
* @Author	Dale Curry
*
* @brief	This class loads in WAV files in relationship to the core to assign position in 3D sound.
*
*/

#include "openAL_sound.h"


void openAL_sound::initDefaultDevice()
{
	device = alcOpenDevice(NULL);

	if (device == NULL)
	{
		std::cout << "Initialise with DEFAULT ERROR" << std::endl;
	}

	context = alcCreateContext(device, NULL);

	if (context == NULL)
	{
		std::cout << "Initialise with CONTEXT ERROR" << std::endl;
	}
	else
	{
		alcMakeContextCurrent(context);
	}

};

void openAL_sound::setListenerposition(const float x, const float y, const float z)
{
	alListener3f(AL_POSITION, x, y, z);
};

void openAL_sound::setListenerVelocity(const float x, const float y, const float z)
{
	alListener3f(AL_VELOCITY, x, y, z);
};

void openAL_sound::setListenerOrientation(const float atX, const float atY, const float atZ, const float upX, const float upY, const float upZ)
{
	float orientation[6] = { atX, atY, atZ, upX, upY, upZ };

	alListenerfv(AL_ORIENTATION, orientation);
};

openAL_Core* openAL_sound::entityLoadedFromFile(const std::string fp)
{
	return loadWavFile(fp);
};



openAL_Core* openAL_sound::loadWavFile(const std::string fp)
{

	openAL_Core* entity = NULL;


	if (fp.empty())
	{
		std::cout << "file path to WAV error" << std::endl;
	}

	std::ifstream file(fp.c_str(), std::ifstream::binary);

	if (!file.is_open())
	{
		std::cout << "Could not open WAV file" << std::endl;
	}
	else
	{
		char chunkId[5] = "\0";
		unsigned int size = 0;

		//read the header
		file.read(chunkId, 4);
		file.read((char*)&size, 4);

		chunkId[4] = '\0';

		std::cout << "Chunk ID:" << chunkId << std::endl;
		std::cout << "size:" << size << std::endl;

		file.read(chunkId, 4);

		std::cout << "WAV ID:" << chunkId << std::endl;

		file.read(chunkId, 4);
		file.read((char*)&size, 4);

		chunkId[4] = '\0';

		std::cout << "Chunk ID:" << chunkId << std::endl;
		std::cout << "size:" << size << " bytes" << std::endl;

		short tagFormat		= 0;
		short channels		= 0;
		int samplesPerSec	= 0;
		int avgBytesPerSec	= 0;
		short blockAlign	= 0;
		short bitsPerSample = 0;

		file.read((char*)&tagFormat, 2);
		file.read((char*)&channels, 2);
		file.read((char*)&samplesPerSec, 4);
		file.read((char*)&avgBytesPerSec, 4);
		file.read((char*)&blockAlign, 2);
		file.read((char*)&bitsPerSample, 2);

		if (size > 16)
		{
			file.seekg((int)file.tellg() + (size - 16));
		}

		//find out what the format is

		switch(tagFormat)
		{
			case 0x0001:
			{
				std::cout << "PCM Format" << std::endl;
				break;
			}
			case 0x0003:
			{
				std::cout << "IEEE Float Format" << std::endl;
				break;
			}
			case 0x0006:
			{
				std::cout << "8-bit ITU-T G.711 A-law Format" << std::endl;
				break;
			}
			case 0x0007:
			{
				std::cout << "8-bit ITU-T G.711 mi-law Format" << std::endl;
				break;
			}
			default:
			{
				std::cout << "Unknown format" << std::endl;
				break;
			}
		}

		std::cout << "Channels:" << channels << std::endl;
		std::cout << "Samples Per Second:" << samplesPerSec << std::endl;
		std::cout << "Average bytes per second:" << avgBytesPerSec << std::endl;
		std::cout << "Block align:" << blockAlign << std::endl;
		std::cout << "bit per sample:" << bitsPerSample << std::endl;

		file.read(chunkId, 4);
		file.read((char*)&size, 4);

		chunkId[4] = '\0';

		std::cout << "Chunk ID:" << chunkId << std::endl;
		std::cout << "Size:" << size << "bytes" << std::endl;

		unsigned char* data = new unsigned char[size];

		file.read((char*)data, size);

		entity = new openAL_Core(data, size, samplesPerSec, channels);

		delete[] data;
		data = NULL;

		std::cout << "FILE LOAD COMPLETE\n" << std::endl;
	}

	file.close();
	
	return entity;
};


