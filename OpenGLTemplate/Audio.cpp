#pragma region Headers
#include "Audio.h"
#include "stdafx.h"
#include "math.h"
#include <iostream>
#include <array>
#pragma endregion

#pragma comment(lib, "lib/fmod_vc.lib")
#define min(a, b) (((a) < (b)) ? (a) : (b));
#define max(a, b) (((a) > (b)) ? (a) : (b));

#pragma region Global vars
//averaging filter
vector<float> filter =
{ 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f };
//copy the unflipped averaging filter
vector<float> filtCopy = filter;
CircularBuffer *cBuffer = new CircularBuffer(filter.size());

//for flanger effect	
CircularBuffer *roarCBuffer = new CircularBuffer(500);
#pragma endregion

//convolution for a given index in a sample group
float conv(CircularBuffer *c, vector<float> &b, int sample) {
	float sum = 0.0f;
	int j = 0;
	
	//dot product between the selected sample and filter coefficient
	for (int i = sample - (b.size()-1); i <= sample; i++, j++) {
		sum += c->AtPosition(i) * b[j];
	}

	return sum;
}

#pragma region DSP Callbacks
// DSP callback
///////////////////////AVERAGING////////////////////////////////
///////////////////////coefficient change over time/////////////
FMOD_RESULT F_CALLBACK
DSPCallback(FMOD_DSP_STATE *dsp_state, float *inbuffer, float *outbuffer, unsigned int length, int inchannels, int *outchannels)
{
	FMOD::DSP *thisdsp = (FMOD::DSP *)dsp_state->instance;
	//mixing the filtered sound and original sound by 1:1
	float bValue = 0.5f;

	//for every sample every channel
	for (int samp = 0; samp < length; samp++) {
		for (int chan = 0; chan < *outchannels; chan++) {
			//the number of inchannels usally will be the same as outchannels, therefore this could be the same as (samp ** outchannesl) + chan
			int n = (samp * inchannels) + chan;
			//store the sample in circular buffer
			cBuffer->Put(n, inbuffer[n]);

			outbuffer[n] =		//output sound equals to 
				inbuffer[n] * bValue +	//half of the original signal
				conv(cBuffer, filter, n) * (1 - bValue);	//mixing with half of the filtered signal
		}
	}

	return FMOD_OK;
}

// DSP callback for Roar Sound Effect
///////////////////////////////FLANGER///////////////////////////////////
FMOD_RESULT F_CALLBACK
RoarDSPCallback(FMOD_DSP_STATE *dsp_state, float *inbuffer, float *outbuffer, unsigned int length, int inchannels, int *outchannels)
{
	FMOD::DSP *thisdsp = (FMOD::DSP *)dsp_state->instance;
	//mixing the original and delay signal with enhanced amptitude
	float bValue = 2.0f;

	//delay between 441 to 0 sample 
	int range = 441 / 2;

	//rate of changing delay
	int sweep_freq = 1;
	int fs = 44100;

	for (int samp = 0; samp < length; samp++) {
		for (int chan = 0; chan < *outchannels; chan++) {
			int n = (samp * inchannels) + chan;
			outbuffer[n] =	//output sound equals to 
				roarCBuffer->AtPosition(n) * bValue +	//original signal with enhanced amptitude
				roarCBuffer->AtPosition(n - (441 + round(range * sin(2 * M_PI * n * sweep_freq / fs)))) * (1 -bValue);	//mixing with delayed signal with enhanced amptitude

			roarCBuffer->Put(n, inbuffer[n]);	//store the output to circular buffer for reuse

		}
	}
	return FMOD_OK;
}
#pragma endregion

CAudio::CAudio()
{
	m_FmodSystem = NULL;
	mainChannelGroup = NULL;

	m_eventSound = NULL;

	m_gunShot = NULL;
	m_gunShotChannel = NULL;

	m_houseMusic = NULL;
	m_houseChannel = NULL;

	m_killed = NULL;
	m_killedChannel = NULL;

	m_roar = NULL;
	m_roarChannel = NULL;

	m_music = NULL;
	m_musicChannel = NULL;

	m_dsp = NULL;
	m_fft = NULL;
	m_echo = NULL;
	m_highPass = NULL;
	m_roarDSP = NULL;
	m_lowPass = NULL;

	m_pCamera = NULL;
	m_pActor = NULL;

	housePosition = glm::vec3(-60.624828, 0, 25);

	//filp the filter coefficient
	std::reverse(filter.begin(), filter.end());
	std::reverse(filtCopy.begin(), filtCopy.end());
}

CAudio::~CAudio()
{
	
}

#pragma region Initialise
bool CAudio::Initialise()
{
	m_volume = 1.0f;

	// Create an FMOD system
	result = FMOD::System_Create(&m_FmodSystem);
	FmodErrorCheck(result);
	if (result != FMOD_OK)
		return false;


	// Initialise the system
	result = m_FmodSystem->init(32, FMOD_INIT_NORMAL, 0);
	FmodErrorCheck(result);
	if (result != FMOD_OK)
		return false;

	//set doppler effect
	float doppler = 1;
	float distFactor = 1;
	float distRolloff = 1;

	m_FmodSystem->set3DSettings(doppler, distFactor, distRolloff);

	//create dsp (echo, low pass, custom dsp etc)
	CreateDSP();

	//load all the audio
	LoadHouseMusic();	//little girl singing
	LoadGunShotSoundEffect();	//gun shot
	LoadMusicStream("resources\\audio\\bgm.mp3");	//backgorund music
	LoadKilledSoundEffect();	//play whenever a monster is dead	//purifying sound effect
	LoadFaceMonsterSound();		//play whenever the giant face monster has jump off from the sky //Roar sound effect

	PlayMusicStream();	//background music

	CreateHouse();	//create the geometry for house model at the center of the map

	return true;
}
#pragma endregion

#pragma region Load Audio
//Load gun shot sound effect
bool CAudio::LoadGunShotSoundEffect() {
	result = m_FmodSystem->createSound("resources\\audio\\gunshot.mp3", NULL, 0, &m_gunShot);
	FmodErrorCheck(result);
	if (result != FMOD_OK)
		return false;

	return true;
}

// Load the music stream for house (music box). inc occlusion
bool CAudio::LoadHouseMusic()
{
	//Create a music stream that will loop
	result = m_FmodSystem->createStream("resources\\audio\\house3.mp3", NULL | FMOD_LOOP_NORMAL, 0, &m_houseMusic);

	FmodErrorCheck(result);

	if (result != FMOD_OK)
		return false;

	return true;
}

//load purify sound effect
bool CAudio::LoadKilledSoundEffect() {
	result = m_FmodSystem->createSound("resources\\audio\\killed2.mp3", NULL, 0, &m_killed);

	FmodErrorCheck(result);

	if (result != FMOD_OK)
		return false;

	return true;
}

//load roar sound effect
bool CAudio::LoadFaceMonsterSound() {
	result = m_FmodSystem->createSound("resources\\audio\\roar2.mp3", NULL, 0, &m_roar);

	FmodErrorCheck(result);

	if (result != FMOD_OK)
		return false;

	return true;
}

//Load background music
bool CAudio::LoadMusicStream(char *filename)
{
	result = m_FmodSystem->createStream(filename, NULL | FMOD_LOOP_NORMAL, 0, &m_music);
	FmodErrorCheck(result);


	if (result != FMOD_OK)
		return false;

	return true;
}
#pragma endregion

#pragma region Play Audio
//Play gun shot sound effect
bool CAudio::PlayGunShotSoundEffect() {
	result = m_FmodSystem->playSound(m_gunShot, NULL, false, &m_gunShotChannel);
	FmodErrorCheck(result);

	//apply low pass filter. filter bypass toggle by right click (use sniper scope or not)
	m_gunShotChannel->addDSP(0, m_lowPass);

	if (result != FMOD_OK)
		return false;
	return true;
}

//Play music box 
bool CAudio::PlayHouseMusic()
{
	result = m_FmodSystem->playSound(m_houseMusic, NULL, false, &m_houseChannel);
	FmodErrorCheck(result);

	if (result != FMOD_OK)
		return false;

	m_houseChannel->setVolume(0.75f);

	FMOD_VECTOR position =
		ToFmodVector(glm::vec3(-60.624828, 5, 25));

	FMOD_VECTOR velocity =
		ToFmodVector(glm::vec3(0, 0, 0));

	//set 3d mode and position
	m_houseChannel->setMode(FMOD_3D);
	m_houseChannel->set3DAttributes(&position, &velocity);

	//apply occlusion effect
	result = m_houseChannel->set3DOcclusion(0.9f, 0.5f);

	if (result != FMOD_OK)
		return false;
	return true;
}

//play purify sound effect, called when a monster is shot dead. (position and velocity of the monster will be passed in from actor manager )
bool CAudio::PlayKilledSoundEffect(glm::vec3 &p, glm::vec3 &v) {
	result = m_FmodSystem->playSound(m_killed, NULL, false, &m_killedChannel);
	FmodErrorCheck(result);
	if (result != FMOD_OK)
		return false;

	m_killedChannel->setVolume(0.5f);

	FMOD_VECTOR position =
		ToFmodVector(p);

	FMOD_VECTOR velocity =
		ToFmodVector(v);

	//3d setting
	m_killedChannel->setMode(FMOD_3D);
	m_killedChannel->set3DAttributes(&position, &velocity);

	return true;
}

//player roar sound effect
//Take position and velocity of the monster as parameter. By default, velocity will be 0 if not passed in
bool CAudio::PlayFaceMonsterSound(glm::vec3 &p, glm::vec3 &v) {
	result = m_FmodSystem->playSound(m_roar, NULL, false, &m_roarChannel);
	FmodErrorCheck(result);

	FMOD_VECTOR position =
		ToFmodVector(p);

	FMOD_VECTOR velocity =
		ToFmodVector(v);

	//apply roar dsp (flanger effect)
	m_roarChannel->addDSP(0, m_roarDSP);

	//high volume (Because occlusion is applied)
	m_roarChannel->setVolume(1.0f);

	//3D setting
	m_roarChannel->setMode(FMOD_3D);
	m_roarChannel->set3DAttributes(&position, &velocity);

	//Occlusion
	m_roarChannel->set3DOcclusion(0.25f, 0.1f);

	if (result != FMOD_OK)
		return false;
	return true;
}

//Play background music
bool CAudio::PlayMusicStream()
{
	result = m_FmodSystem->playSound(m_music, NULL, false, &m_musicChannel);
	FmodErrorCheck(result);

	//high vlume because both custom dsp and high pass filter reduce a lot of amptitude
	m_musicChannel->setVolume(1.0f);

	//apply high pass filter before custom dsp
	m_musicChannel->addDSP(0, m_highPass);

	//apply averaging filter (but filter coefficient change overtime)
	m_musicChannel->addDSP(0, m_dsp);

	if (result != FMOD_OK)
		return false;
	return true;
}
#pragma endregion

#pragma region Adjust volume
//increase volume
bool CAudio::IncreaseVolume() {
	m_volume += 0.1f;
	m_volume = min(m_volume, 1);
	//increase volume for every channel
	//result = (*mainChannelGroup)->setVolume(m_volume);

	if (result != FMOD_OK)
		return false;
	return true;
}

//decrease volume
bool CAudio::DecreaseVolume() {
	m_volume -= 0.1f;
	m_volume = max(0, m_volume);
	//decrease volume for every channel
	//result = (*mainChannelGroup)->setVolume(m_volume);

	if (result != FMOD_OK)
		return false;
	return true;
}
#pragma endregion

#pragma region Dynamic Filters
//Filter switch on/off by right click (using sniper scope or not)
void CAudio::DynamicFilter() {
	//if using sniper scope, apply low pass filter and increase volume
	if (m_pCamera->UsingScope()) {
		m_lowPass->setBypass(false);
		m_gunShotChannel->setVolume(0.15f);
	}
	//if not using sniper scope, remove low pass filter and descrease volume
	else {
		m_lowPass->setBypass(true);
		m_gunShotChannel->setVolume(0.05f);
	}
}

//Changing the filter coefficient overtime
void CAudio::FilterOverTime(float dt) {
	static float t = 0.0f;
	t += dt * 0.0025;
	for (int i = 0; i < filter.size(); i++) {
		if (i % 2 == 0) {
			filter[i] = filtCopy[i] * cos(t);
		}
		else
		{
			filter[i] = filtCopy[i] * sin(t);
		}
	}
}
#pragma endregion

#pragma region Update
void CAudio::Update(float dt)
{
	//get delta time from main game class
	m_dt = dt;

	//change filter coefficient over time
	FilterOverTime(dt);

	//listen for right click to switch between filter
	DynamicFilter();

	//set up listener(player/camera)
	m_FmodSystem->set3DListenerAttributes(0, &ToFmodVector(m_pCamera->GetPosition()), NULL, NULL, &ToFmodVector(glm::vec3(0, 1, 0)));

	//update system
	m_FmodSystem->update();
}
#pragma endregion

#pragma region Fmod Helper functions
//convert a glm vector to fmod vector 
FMOD_VECTOR CAudio::ToFmodVector(glm::vec3 &v) {
	FMOD_VECTOR f_vector = FMOD_VECTOR();
	f_vector.x = v.x;
	f_vector.y = v.y;
	f_vector.z = v.z;
	return f_vector;
}

// Check for error
void CAudio::FmodErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK) {
		const char *errorString = FMOD_ErrorString(result);
		// MessageBox(NULL, errorString, "FMOD Error", MB_OK);
		// Warning: error message commented out -- if headphones not plugged into computer in lab, error occurs
	}
}

void CAudio::FAnalysis() {
	m_FmodSystem->createDSPByType(FMOD_DSP_TYPE_FFT, &m_fft);
	m_fft->setParameterInt(FMOD_DSP_FFT_WINDOWSIZE, 1024);
	m_fft->setParameterInt(FMOD_DSP_FFT_WINDOWTYPE, FMOD_DSP_FFT_WINDOW_HANNING);

	FMOD_DSP_PARAMETER_FFT *fftData;
	//m_fft->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void **)fftData);
}
#pragma endregion

#pragma region Setter/ Create polygon/ Create DSPs
bool CAudio::CreateDSP() {
	// Create the DSP for background music
	//	averaging filter with filter coefficient changing overtime
	{
		FMOD_DSP_DESCRIPTION dspdesc;
		memset(&dspdesc, 0, sizeof(dspdesc));

		strncpy_s(dspdesc.name, "My first DSP unit", sizeof(dspdesc.name));
		dspdesc.numinputbuffers = 1;
		dspdesc.numoutputbuffers = 1;
		dspdesc.read = DSPCallback;

		result = m_FmodSystem->createDSP(&dspdesc, &m_dsp);
		FmodErrorCheck(result);

		if (result != FMOD_OK)
			return false;
	}

	// Create the DSP for Roar sound effect (face monster sound)
	//	Flanger effect
	{
		FMOD_DSP_DESCRIPTION dspdesc;
		memset(&dspdesc, 0, sizeof(dspdesc));

		strncpy_s(dspdesc.name, "My second DSP unit", sizeof(dspdesc.name));
		dspdesc.numinputbuffers = 1;
		dspdesc.numoutputbuffers = 1;
		dspdesc.read = RoarDSPCallback;

		result = m_FmodSystem->createDSP(&dspdesc, &m_roarDSP);
		FmodErrorCheck(result);

		if (result != FMOD_OK)
			return false;
	}

	{
		//create an echo filter
		result = m_FmodSystem->createDSPByType(FMOD_DSP_TYPE_ECHO, &m_echo);
		FmodErrorCheck(result);

		if (result != FMOD_OK)
			return false;

		//create a high pass filter
		result = m_FmodSystem->createDSPByType(FMOD_DSP_TYPE_HIGHPASS, &m_highPass);
		FmodErrorCheck(result);

		if (result != FMOD_OK)
			return false;

		//create a low pass filter
		result = m_FmodSystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &m_lowPass);
		FmodErrorCheck(result);

		if (result != FMOD_OK)
			return false;
	}

	return true;
}

void CAudio::CreateHouse() {
	//define all the vertices for a cube 
	vector<FMOD_VECTOR*> polys;

	FMOD_VECTOR wallPoly[4];
	wallPoly[0] = ToFmodVector(glm::vec3(-10, 0, 0));
	wallPoly[1] = ToFmodVector(glm::vec3(-10, 10, 0));
	wallPoly[2] = ToFmodVector(glm::vec3(10, 0, 0));
	wallPoly[3] = ToFmodVector(glm::vec3(10, 10, 0));

	FMOD_VECTOR fWallPoly[4];
	fWallPoly[0] = ToFmodVector(glm::vec3(-10.0f, 0.0f, 0.0f));
	fWallPoly[1] = ToFmodVector(glm::vec3(-10.0f, 10.0f, 0.0f));
	fWallPoly[2] = ToFmodVector(glm::vec3(-10.0f, 0.0f, 15.0f));
	fWallPoly[3] = ToFmodVector(glm::vec3(-10.0f, 10.0f, 15.0f));

	FMOD_VECTOR rWallPoly[4];
	rWallPoly[0] = ToFmodVector(glm::vec3(-10.0f, 0.0f, 15.0f));
	rWallPoly[1] = ToFmodVector(glm::vec3(-10.0f, 10.0f, 15.0f));
	rWallPoly[2] = ToFmodVector(glm::vec3(10.0f, 0.0f, 15.0f));
	rWallPoly[3] = ToFmodVector(glm::vec3(10.0f, 10.0f, 15.0f));

	FMOD_VECTOR tWallPoly[4];
	tWallPoly[0] = ToFmodVector(glm::vec3(-10.0f, 10.0f, 0.0f));
	tWallPoly[1] = ToFmodVector(glm::vec3(10.0f, 10.0f, 0.0f));
	tWallPoly[2] = ToFmodVector(glm::vec3(-10.0f, 10.0f, 15.0f));
	tWallPoly[3] = ToFmodVector(glm::vec3(10.0f, 10.0f, 15.0f));

	FMOD_VECTOR dWallPoly[4];
	dWallPoly[0] = ToFmodVector(glm::vec3(-10.0f, 0.0f, 0.0f));
	dWallPoly[1] = ToFmodVector(glm::vec3(10.0f, 0.0f, 0.0f));
	dWallPoly[2] = ToFmodVector(glm::vec3(-10.0f, 0.0f, 15.0f));
	dWallPoly[3] = ToFmodVector(glm::vec3(10.0f, 0.0f, 15.0f));

	FMOD_VECTOR bWallPoly[4];
	bWallPoly[0] = ToFmodVector(glm::vec3(-10.0f, 0.0f, 0.0f));
	bWallPoly[1] = ToFmodVector(glm::vec3(10.0f, 0.0f, 0.0f));
	bWallPoly[2] = ToFmodVector(glm::vec3(-10.0f, 0.0f, 15.0f));
	bWallPoly[3] = ToFmodVector(glm::vec3(10.0f, 0.0f, 15.0f));

	polys.push_back(wallPoly);
	polys.push_back(fWallPoly);
	polys.push_back(rWallPoly);
	polys.push_back(tWallPoly);
	polys.push_back(dWallPoly);
	polys.push_back(bWallPoly);

	//create polygons
	CreateWall(wallPoly);
	CreateWall(fWallPoly);
	CreateWall(rWallPoly);
	CreateWall(tWallPoly);
	CreateWall(dWallPoly);
	CreateWall(bWallPoly);
}

//Craete Poplygon that take up to 4 vertices (a plane)
void CAudio::CreateWall(FMOD_VECTOR* vertices) {
	static int polyIndex;
	FMOD::Geometry *geometry;
	m_FmodSystem->createGeometry(1, 4, &geometry);
	geometry->addPolygon(0.9f, 0.5f, TRUE, 4, vertices, &polyIndex);
	FMOD_VECTOR wallPosition;
	wallPosition = ToFmodVector(glm::vec3(housePosition) - (glm::vec3(0, 0, 1) * 10.0f));
	geometry->setPosition(&wallPosition);
	geometry->setActive(TRUE);
	polyIndex++;
}

//set camera reference
void CAudio::SetCamera(CCamera *cam) {
	m_pCamera = cam;
}

//set actor refernece
void CAudio::SetMonster(AActor *actor) {
	this->m_pActor = actor;
}
#pragma endregion