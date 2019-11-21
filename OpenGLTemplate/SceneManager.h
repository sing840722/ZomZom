#pragma once

class CCamera;
class CHUDManager;
class CActorManager;
class CAudio;

class CSceneManager{
public:
	enum State {
		inMenu, inGame, inEndSreen
	};

private:
	CCamera *m_pCamera;
	CActorManager *m_pActorManager;
	CHUDManager *m_pHUDManager;
	CAudio *m_pAudio;

	State state;

public:
	CSceneManager();
	~CSceneManager();
	void Initialise();
	void InitialiseLevel();

	void UpdateMenu();
	void UpdateEndScreen();
	void GotoMenu();
	void GotoGame();
	void GotoEndScreen();
	void ClearLevel();


	State GetState();

	//Take reference
	void SetCamera(CCamera *cam);
	void SetActorManager(CActorManager* am);
	void SetHUDManager(CHUDManager *hm);
	void SetAudio(CAudio *audio);
};