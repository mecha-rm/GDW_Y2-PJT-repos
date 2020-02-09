// used for including audio functionality
#pragma once

#include "AudioEngine.h"

namespace cherry
{
	// class
	class AudioComponent : public cherry::AudioEngine
	{
	public:
		// the audio component
		AudioComponent() = default;

		// loads the bank. Use FMOD_STUDIO_LOAD_BANK_NORMAL for the local flag
		void LoadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags) override;

		// gets the bank for the audio component
		std::string GetCurrentBankName() const;

		// loads the event
		void PlayEvent(const std::string& strEventName) override;

		// returns the current event name
		std::string GetCurrentEventName() const;

		// plays a sound
		int PlaySound(const std::string& strSoundName, const glm::vec3& vPos = glm::vec3{ 0, 0, 0 }, float fVolumedB = 0.0f) override;

		// returns the current sound name
		std::string GetCurrentSoundName() const;

	private:
		// the curret bank being used
		std::string currentBank = "";

		// the current event
		std::string currentEvent = "";

		// the current sound
		std::string currentSound = "";

	protected:
	};
}