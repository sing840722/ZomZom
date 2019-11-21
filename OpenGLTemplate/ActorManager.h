#pragma once
#include "Common.h"

class AActor;
class AObject;
class CCatmullRom;
class CCamera;
class CHUDManager;
class CSceneManager;
class CAudio;
class CActorManager {
private:
	vector <AActor*> *m_pModels;
	vector <AActor*> m_pActors;
	AActor *m_pPlayer;
	
	CCatmullRom *m_pCatmullRom;
	CCamera *m_pCamera;
	CSceneManager *m_pSceneManager;
	CHUDManager *m_pHUDManager;

	float m_dt;

	CAudio *m_pAudio;

	vector<AActor*> m_pDolls;

public:
	CActorManager();
	~CActorManager();

	//add a new player
	void AddPlayer();

	//add a new actor and initlise
	void AddActor(int id, int number, float camPos = 880);

	//remove a single actor
	void RemoveActor(AActor *actor);

	//Update every actor
	void Update(float dt);

	//load model
	void Initialise();

	//Get models
	AActor* const GetPlayerModel() const;
	AActor* const GetFaceMonsterModel() const;
	AActor* const GetEyeBallMonsterModel() const;
	AActor* const GetDollMonsterModel() const;
	AActor* const GetChineseMonsterModel() const;
	AActor* const GetTreeModel() const;
	AActor* const GetCoffinMonsterModel() const;
	AActor* const GetUmbrellaMonsterModel() const;

	//Get player reference
	AActor* const GetPlayer() const;
	
	//Get the managing list
	vector<AActor*> const GetActors() const;

	//Allocate actor on the path
	void MoveObject(AActor *object);

	//Test if trigger
	bool TestTriggers(AActor *actor);

	//Check if the actor has a trigger
	bool HitTestTrigger(AActor *actor);

	//Check if actor has hit/collide with the player
	bool PlayerHitTest(AActor *actor);

	//Check if actor being clicked
	bool TestRayCasting();

	//Place actor on the path
	void PlaceObject(AActor* object);

	//Turn actor to facing at player
	void TurnToCamera(AActor* object);

	//Remove every actor in the game including player (exclude models)
	void ClearActors();

	//Get reference from other class
	void SetCatmullRom(CCatmullRom *c);
	void SetCamera(CCamera *cam);
	void SetHUDManager(CHUDManager *hm);
	void SetSceneManager(CSceneManager *sm);
	void SetAudio(CAudio *audio);
};