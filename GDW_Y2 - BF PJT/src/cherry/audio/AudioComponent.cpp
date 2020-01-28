#include "AudioComponent.h"

// loads the bank
void cherry::AudioComponent::LoadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags)
{
	AudioEngine::LoadBank(strBankName, flags);
	currentBank = strBankName;
}

// gets the audio bank
std::string cherry::AudioComponent::GetCurrentBankName() const { return currentBank; }

// loads an audio event
void cherry::AudioComponent::PlayEvent(const std::string& strEventName)
{
	AudioEngine::PlayEvent(strEventName);
	currentEvent = strEventName;
}

// returns the name of the current event
std::string cherry::AudioComponent::GetCurrentEventName() const { return currentEvent; } 

// loads a sound
int cherry::AudioComponent::PlaySound(const std::string& strSoundName, const glm::vec3& vPosition, float fVolumedB)
{
	currentSound = strSoundName;
	return AudioEngine::PlaySound(strSoundName, vPosition, fVolumedB);
	
}

// returns the name of the current sound.
std::string cherry::AudioComponent::GetCurrentSoundName() const { return currentSound; }
