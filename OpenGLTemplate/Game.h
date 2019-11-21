#pragma once

#include "Common.h"
#include "GameWindow.h"

// Classes used in game.  For a new class, declare it here and provide a pointer to an object of this class below.  Then, in Game.cpp, 
// include the header.  In the Game constructor, set the pointer to NULL and in Game::Initialise, create a new object.  Don't forget to 
// delete the object in the destructor.   
class CCamera;
class CSkybox;
class CShader;
class CShaderProgram;
class CFreeTypeFont;
class CHighResolutionTimer;
class COpenAssetImportMesh;
class CAudio;
class CCatmullRom;
class AActor;
class AEnemy;
class AObject;
class CPlane;
class AHUD;
class CFrameBufferObject;
class CActorManager;
class CSceneManager;
class CHUDManager;
class ALight;
class ALightHouse;

class Game {
private:
	// Three main methods used in the game.  Initialise runs once, while Update and Render run repeatedly in the game loop.
	void Initialise();
	void Update();
	void Render();
	void RenderMenu();
	void RenderEndScreen();
	void Render3D();
	// Pointers to game objects.  They will get allocated in Game::Initialise()
	vector <CShaderProgram *> *m_pShaderPrograms;
	vector <COpenAssetImportMesh*> *m_pEdges;


	COpenAssetImportMesh *m_pHouse;
	ALightHouse *m_pLightHouse;
	ALightHouse *m_pLightHouse2;
	ALight *m_pDirectionalLight;
	ALight *m_pTorch;
	ALight *m_pHouseLight;


	CAudio *m_pAudio;
	CCamera *m_pCamera;
	CSkybox *m_pSkybox;
	CFreeTypeFont *m_pFtFont;
	CCatmullRom *m_pCatmullRom;
	CPlane *m_pPlanarTerrain;
	CFrameBufferObject *m_pFBO;
	CHighResolutionTimer *m_pHighResolutionTimer;
	
	CActorManager *m_pActorManager;
	CHUDManager *m_pHUDManager;
	CSceneManager *m_pSceneManager;


	// Some other member variables
	bool m_appActive;
	int m_framesPerSecond;
	float m_dt;
	bool bLeftMouseClicked;
	float nearClippingPlane, farClippingPlane;


	AObject* wall;

public:
	Game();
	~Game();
	static Game& GetInstance();
	LRESULT ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param);
	void SetHinstance(HINSTANCE hinstance);
	WPARAM Execute();

	void UpdateCamera();
	void MoveCamera(CCamera* camera);
	void RayCasting();
	void SwitchCameraMode();


private:
	static const int FPS = 60;
	void DisplayFrameRate();
	void GameLoop();
	GameWindow m_gameWindow;
	HINSTANCE m_hInstance;
	int m_frameCount;
	double m_elapsedTime;
	float m_t;
};
