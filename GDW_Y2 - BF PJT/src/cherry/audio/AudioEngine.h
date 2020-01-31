// Code edited from: https://codyclaborn.me/tutorials/making-a-basic-fmod-audio-engine-in-c/

#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENGINE_H_

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

#include <glm/glm.hpp>

//using namespace std;


#endif


namespace cherry
{
	struct Implementation
	{
		/*
		- Initializes and shuts down FMOD
		- Holds instances of of both the Studio and low-level system objects
		- Also holds a map of all the sounds and events we've played
		*/

		Implementation();
		~Implementation();

		void Update();

		FMOD::Studio::System* mpStudioSystem;
		FMOD::System* mpSystem;

		int mnNextChannelId;

		typedef std::map<std::string, FMOD::Sound*> SoundMap;
		typedef std::map<int, FMOD::Channel*> ChannelMap;
		typedef std::map<std::string, FMOD::Studio::EventInstance*> EventMap;
		typedef std::map<std::string, FMOD::Studio::Bank*> BankMap;

		BankMap mBanks;
		EventMap mEvents;
		SoundMap mSounds;
		ChannelMap mChannels;

	};

	class AudioEngine
	{
	public:
		static void Init();
		static void Update();
		static void Shutdown();
		static int ErrorCheck(FMOD_RESULT result);

		virtual void LoadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
		virtual void LoadEvent(const std::string& strEventName, const std::string& strEventNumber);
		virtual void LoadSound(const std::string& strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false);
		
		void UnloadSound(const std::string& strSoundName);
		//void Set3dListenerAndOrientation(const Vector3& vPos = Vector3{ 0, 0, 0 }, float  fVolumedB = 0.0f);
		virtual int PlaySound(const std::string& strSoundName, const glm::vec3& vPos = glm::vec3{ 0, 0, 0 }, float fVolumedB = 0.0f);
		virtual void PlayEvent(const std::string& strEventName);
		//void StopChannel(int nChannelId);
		void StopEvent(const std::string& strEventName, bool bImmediate = false);
		void GetEventParameter(const std::string& strEventName, const std::string& strEventParameter, float* parameter);
		void SetEventParameter(const std::string& strEventName, const std::string& strParameterName, float fValue);
		void SetEventPosition(const std::string& strEventName, const glm::vec3 vPosition);
		void SetEventVelocity(const std::string& strEventName, const glm::vec3 vVelocity);
		//void StopAllChannels();
		void SetChannel3dPosition(int nChannelId, const glm::vec3& vPosition);
		void SetChannelVolume(int nChannelId, float fVolumedb);
		//bool isPlaying(int nChannelId) const;
		bool isEventPlaying(const std::string& strEventName) const;
		float dbToVolume(float db);
		float VolumeTodb(float volume);
		FMOD_VECTOR VectorToFmod(const glm::vec3& vPosition);
		void SetGlobalParameter(const std::string& strParameterName, float fValue);

	private:

	protected:

	};
}