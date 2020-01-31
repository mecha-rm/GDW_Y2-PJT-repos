// Code edited from: https://codyclaborn.me/tutorials/making-a-basic-fmod-audio-engine-in-c/

#include "AudioEngine.h"

cherry::Implementation::Implementation()
{
	mpStudioSystem = NULL;
	AudioEngine::ErrorCheck(FMOD::Studio::System::create(&mpStudioSystem));
	AudioEngine::ErrorCheck(mpStudioSystem->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL));

	mpSystem = NULL;
	AudioEngine::ErrorCheck(mpStudioSystem->getCoreSystem(&mpSystem));
} 

cherry::Implementation::~Implementation()
{
	AudioEngine::ErrorCheck(mpStudioSystem->unloadAll());
	AudioEngine::ErrorCheck(mpStudioSystem->release());
}

void cherry::Implementation::Update()
{
	std::vector<ChannelMap::iterator> pStoppedChannels;
	for (auto it = mChannels.begin(), itEnd = mChannels.end(); it != itEnd; ++it)
	{
		bool bIsPlaying = false;
		it->second->isPlaying(&bIsPlaying);
		if (!bIsPlaying)
		{
			pStoppedChannels.push_back(it);
		}
	}

	for (auto& it : pStoppedChannels)
	{
		mChannels.erase(it);
	}

	AudioEngine::ErrorCheck(mpStudioSystem->update());
}

cherry::Implementation* implementation = nullptr;

void cherry::AudioEngine::Init()
{
	implementation = new Implementation;
}

void cherry::AudioEngine::Update()
{
	implementation->Update();
}

void cherry::AudioEngine::LoadSound(const std::string& strSoundName, bool b3d, bool bLooping, bool bStream)
{
	auto tFoundIt = implementation->mSounds.find(strSoundName);
	if (tFoundIt != implementation->mSounds.end())
		return;

	FMOD_MODE eMode = FMOD_DEFAULT;
	eMode |= b3d ? FMOD_3D : FMOD_2D;
	eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

	FMOD::Sound* pSound = nullptr;

	AudioEngine::ErrorCheck(implementation->mpSystem->createSound(strSoundName.c_str(), eMode, nullptr, &pSound));
	if (pSound)
	{
		implementation->mSounds[strSoundName] = pSound;
	}
}

void cherry::AudioEngine::UnloadSound(const std::string& strSoundName)
{
	auto tFoundIt = implementation->mSounds.find(strSoundName);
	if (tFoundIt == implementation->mSounds.end())
		return;

	AudioEngine::ErrorCheck(tFoundIt->second->release());
	implementation->mSounds.erase(tFoundIt);
}

int cherry::AudioEngine::PlaySound(const std::string& strSoundName, const glm::vec3& vPosition, float fVolumedB)
{
	int nChannelId = implementation->mnNextChannelId++;
	auto tFoundIt = implementation->mSounds.find(strSoundName);
	if (tFoundIt == implementation->mSounds.end())
	{
		// If the sound cannot be found try to load it
		LoadSound(strSoundName);
		tFoundIt = implementation->mSounds.find(strSoundName);
		if (tFoundIt == implementation->mSounds.end())
		{
			return nChannelId;
		}
	}

	FMOD::Channel* pChannel = nullptr;
	AudioEngine::ErrorCheck(implementation->mpSystem->playSound(tFoundIt->second, nullptr, true, &pChannel));
	if (pChannel)
	{
		FMOD_MODE currMode;
		tFoundIt->second->getMode(&currMode);
		if (currMode & FMOD_3D)
		{
			FMOD_VECTOR position = VectorToFmod(vPosition);
			AudioEngine::ErrorCheck(pChannel->set3DAttributes(&position, nullptr));
		}

		AudioEngine::ErrorCheck(pChannel->setVolume(dbToVolume(fVolumedB)));
		AudioEngine::ErrorCheck(pChannel->setPaused(false));
		implementation->mChannels[nChannelId] = pChannel;

	}

	return nChannelId;

}

void cherry::AudioEngine::SetChannel3dPosition(int nChannelId, const glm::vec3& vPosition)
{
	auto tFoundIt = implementation->mChannels.find(nChannelId);
	if (tFoundIt == implementation->mChannels.end())
		return;

	FMOD_VECTOR position = VectorToFmod(vPosition);
	AudioEngine::ErrorCheck(tFoundIt->second->set3DAttributes(&position, NULL));
}

void cherry::AudioEngine::SetChannelVolume(int nChannelId, float fVolumedB)
{
	auto tFoundIt = implementation->mChannels.find(nChannelId);
	if (tFoundIt == implementation->mChannels.end())
		return;

	AudioEngine::ErrorCheck(tFoundIt->second->setVolume(dbToVolume(fVolumedB)));
}

void cherry::AudioEngine::LoadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags)
{	
	auto tFoundIt = implementation->mBanks.find(strBankName);
	if (tFoundIt != implementation->mBanks.end())
		return;

	FMOD::Studio::Bank* pBank;
	AudioEngine::ErrorCheck(implementation->mpStudioSystem->loadBankFile((strBankName + ".bank").c_str(), flags, &pBank));

	if (pBank)
	{
		implementation->mBanks[strBankName] = pBank;
	}
}

void cherry::AudioEngine::LoadEvent(const std::string& strEventName, const std::string& strEventNumber)
{
	auto tFoundit = implementation->mEvents.find(strEventName);
	if (tFoundit != implementation->mEvents.end())
		return;

	FMOD::Studio::EventDescription* pEventDescription = NULL;
	AudioEngine::ErrorCheck(implementation->mpStudioSystem->getEvent(strEventNumber.c_str(), &pEventDescription));
	if (pEventDescription)
	{
		FMOD::Studio::EventInstance* pEventInstance = NULL;
		AudioEngine::ErrorCheck(pEventDescription->createInstance(&pEventInstance));
		if (pEventInstance) 
		{
			implementation->mEvents[strEventName] = pEventInstance;
		}
	}
}

void cherry::AudioEngine::PlayEvent(const std::string& strEventName)
{
	// If event not found, load the event
	auto tFoundIt = implementation->mEvents.find(strEventName);
	if (tFoundIt == implementation->mEvents.end())
		return;

	tFoundIt->second->start();
}

void cherry::AudioEngine::StopEvent(const std::string& strEventName, bool bImmediate)
{
	auto tFoundIt = implementation->mEvents.find(strEventName);
	if (tFoundIt == implementation->mEvents.end())
		return;

	FMOD_STUDIO_STOP_MODE eMode;
	eMode = bImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
	AudioEngine::ErrorCheck(tFoundIt->second->stop(eMode));
}

bool cherry::AudioEngine::isEventPlaying(const std::string& strEventName) const
{
	auto tFoundIt = implementation->mEvents.find(strEventName);
	if (tFoundIt == implementation->mEvents.end())
		return false;

	FMOD_STUDIO_PLAYBACK_STATE* state = NULL;
	if (tFoundIt->second->getPlaybackState(state) == FMOD_STUDIO_PLAYBACK_PLAYING) //help
	{
		return true;
	}

	return false;
}

void cherry::AudioEngine::GetEventParameter(const std::string& strEventName, const std::string& strParameterName, float* parameter)
{
	auto tFoundIt = implementation->mEvents.find(strEventName);
	if (tFoundIt == implementation->mEvents.end())
		return;

	AudioEngine::ErrorCheck(tFoundIt->second->getParameterByName(strParameterName.c_str(), parameter));
}

void cherry::AudioEngine::SetEventParameter(const std::string& strEventName, const std::string& strParameterName, float fValue)
{
	// checks in the map if the string exists
	auto tFoundIt = implementation->mEvents.find(strEventName);
	if (tFoundIt == implementation->mEvents.end())
		return;
	
	AudioEngine::ErrorCheck(tFoundIt->second->setParameterByName(strParameterName.c_str(), fValue));
}

//// Put new function here
void cherry::AudioEngine::SetEventPosition(const std::string& strEventName, const glm::vec3 vPosition)
{
	//  checks in the map if the string exists
	auto tFoundIt = implementation->mEvents.find(strEventName);
	if (tFoundIt == implementation->mEvents.end())
		return;

	// get the new attribute, set it to be on the position, and then set the new attribute back.

	// temp object - position, velocity, forward, and up.
	FMOD_3D_ATTRIBUTES newAttributes;

	// get attribute from event - sets the object to the position on the parameter. It also checks for errors.
	AudioEngine::ErrorCheck(tFoundIt->second->get3DAttributes(&newAttributes));

	// set the new position
	newAttributes.position = VectorToFmod(vPosition);

	// set new attribute on event
	AudioEngine::ErrorCheck(tFoundIt->second->get3DAttributes(&newAttributes));

	// all fmod functions return a resuly
}

// setting hte velocity
void cherry::AudioEngine::SetEventVelocity(const std::string& strEventName, const glm::vec3 vVelocity)
{
	//  checks in the map if the string exists
	auto tFoundIt = implementation->mEvents.find(strEventName);
	if (tFoundIt == implementation->mEvents.end())
		return;

	// get the new attribute, set it to be on the position, and then set the new attribute back.

	// temp object - position, velocity, forward, and up.
	FMOD_3D_ATTRIBUTES newAttributes;

	// get attribute from event - sets the object to the position on the parameter. It also checks for errors.
	AudioEngine::ErrorCheck(tFoundIt->second->get3DAttributes(&newAttributes));

	// set the new position
	newAttributes.velocity = VectorToFmod(vVelocity);

	// set new attribute on event
	AudioEngine::ErrorCheck(tFoundIt->second->get3DAttributes(&newAttributes));
}

FMOD_VECTOR cherry::AudioEngine::VectorToFmod(const glm::vec3& vPosition)
{
	FMOD_VECTOR fVec;
	fVec.x = vPosition.x;
	fVec.y = vPosition.y;
	fVec.z = vPosition.z;
	return fVec;
}

float cherry::AudioEngine::dbToVolume(float db)
{
	return powf(10.0f, 0.05f * db);
}

float cherry::AudioEngine::VolumeTodb(float volume)
{
	return 20.0f * log10f(volume);
}

void cherry::AudioEngine::SetGlobalParameter(const std::string& strParameterName, float fValue)
{
	cherry::AudioEngine::ErrorCheck(implementation->mpStudioSystem->setParameterByName(strParameterName.c_str(), fValue));

}

int cherry::AudioEngine::ErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		std::cout << "FMOD ERROR: " << FMOD_ErrorString(result) << std::endl;
		return 1;
	}

	// All good
	return 0;
}

void cherry::AudioEngine::Shutdown()
{
	delete implementation;
}

