#pragma once
#include <windows.h>									// Header File For The Windows Library
#include "./include/fmod_studio/fmod.hpp"
#include "./include/fmod_studio/fmod_errors.h"
#include "Common.h"
#include "Camera.h"
#include "House.h";
#include "Actor.h"
#include "CircularBuffer.h"

class CAudio
{
public:
	CAudio();
	~CAudio();
	bool Initialise();

	//Load and play gun shot effect
	bool LoadGunShotSoundEffect();
	bool PlayGunShotSoundEffect();

	//Load and play house music (music box)
	bool LoadHouseMusic();
	bool PlayHouseMusic();

	//Load and play purify sound effect (monster dead sound effect)
	bool LoadKilledSoundEffect();
	bool PlayKilledSoundEffect(glm::vec3 &p, glm::vec3 &v = glm::vec3(0));

	//Load and play music stream (background music)
	bool LoadMusicStream(char *filename);
	bool PlayMusicStream();
	
	//Load and play roar sound effect (gian face monster sound)
	bool LoadFaceMonsterSound();
	bool PlayFaceMonsterSound(glm::vec3 &p, glm::vec3 &v = glm::vec3(0));

	//Change volume
	bool IncreaseVolume();
	bool DecreaseVolume();
	
	//Update
	void Update(float dt);

	//Set refernece of actor
	void SetMonster(AActor *actor);
	void SetCamera(CCamera *cam);

	//Create DSPs
	bool CreateDSP();

	//Analysis signal
	void FAnalysis();

	//Change filter coeffient overtime
	void FilterOverTime(float dt);

	//Toggle filter
	void DynamicFilter();

	//Create house geometry
	void CreateHouse();
	
	//Create wall with given vertices at house location
	void CreateWall(FMOD_VECTOR* vertices);

private:
	//Convert glm vector to fmod vector
	FMOD_VECTOR ToFmodVector(glm::vec3 &v);

	//error check
	void FmodErrorCheck(FMOD_RESULT result);

	float m_volume;

	FMOD_RESULT result;
	FMOD::System *m_FmodSystem;	// the global variable for talking to FMOD
	FMOD::Sound *m_eventSound;

	FMOD::Sound *m_music;
	FMOD::Channel* m_musicChannel;

	FMOD::Sound *m_gunShot;
	FMOD::Channel* m_gunShotChannel;

	FMOD::Sound *m_houseMusic;
	FMOD::Channel* m_houseChannel;

	FMOD::Sound *m_killed;
	FMOD::Channel *m_killedChannel;

	FMOD::Sound *m_roar;
	FMOD::Channel *m_roarChannel;

	CCamera *m_pCamera;
	AActor *m_pActor;

	FMOD::DSP *m_dsp;
	FMOD::DSP *m_roarDSP;
	FMOD::DSP *m_fft;
	FMOD::DSP *m_echo;
	FMOD::DSP *m_lowPass;
	FMOD::DSP *m_highPass;

	float m_dt;

	glm::vec3 housePosition;

	FMOD::ChannelGroup **mainChannelGroup;
};
