#pragma once

// FMOD
#include <fmod/fmod_studio.hpp>
#include <fmod/fmod.hpp>
#include <fmod/fmod_errors.h>

// Standard Library
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>
#include <fstream>

#include <glm/glm.hpp>

namespace cherry
{
	struct Implementation
	{
		/*
		- Initializes and shuts down FMOD
		- Holds instances of of both the Studio and core system objects
		- Also holds a map of all the sounds and events we've played
		*/

		Implementation();
		~Implementation();

		void Update();

		// System
		FMOD::Studio::System* mpStudioSystem;
		FMOD::System* mpSystem;

		// GUIDs
		typedef std::map<std::string, std::string> GUIDMap;

		// Banks
		typedef std::map<std::string, FMOD::Studio::Bank*> BankMap;

		// Events
		typedef std::map<std::string, FMOD::Studio::EventInstance*> EventMap;


		// Channels
		int mnNextChannelId;
		typedef std::map<int, FMOD::Channel*> ChannelMap;

		// Sounds
		typedef std::map<std::string, FMOD::Sound*> SoundMap;


		GUIDMap mGUIDs;
		BankMap mBanks;
		EventMap mEvents;
		ChannelMap mChannels;
		SoundMap mSounds;

	};

	class AudioEngine
	{
	public:

		//// TODO: Singleton Stuff Here
		static AudioEngine& GetInstance() // creates an audio engine instance.
		{
			static AudioEngine* singleton = new AudioEngine();
			return *singleton;
		}

		// Logistics
		void Init();
		void LoadGUIDs(); // Called by init
		void Update();
		void Shutdown();
		static int ErrorCheck(FMOD_RESULT result);

		// Banks
		void LoadBank(const std::string& strBankName);
		void UnloadAllBanks();

		// Events
		void LoadEvent(const std::string& strEventName);
		void PlayEvent(const std::string& strEventName);
		void StopEvent(const std::string& strEventName, bool bImmediate = false);

		void SetEventPosition(const std::string& strEventName, const glm::vec3 vPosition);
		bool isEventPlaying(const std::string& strEventName) const;

		// Parameters
		void GetEventParameter(const std::string& strEventName, const std::string& strEventParameter, float* parameter);
		void SetEventParameter(const std::string& strEventName, const std::string& strParameterName, float fValue);
		void SetGlobalParameter(const std::string& strParameterName, float fValue);

		// Listeners
		void SetListenerPosition(const glm::vec3& vPosition);
		void SetListenerOrientation(const glm::vec3& vUP, const glm::vec3& vForward);

		// Helpers
		float dbToVolume(float db);
		float VolumeTodb(float volume);
		FMOD_VECTOR VectorToFmod(const glm::vec3& vPosition);

	private:
		//// TODO: Constructor here
		// needed to make sure the singleton is the only way to make an object.
		AudioEngine() = default;
	};
}