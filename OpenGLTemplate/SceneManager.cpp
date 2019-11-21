#include "SceneManager.h"
#include "Camera.h"
#include "HUDManager.h"
#include "ActorManager.h"
#include "Actor.h"
#include "HUD.h"
#include "Audio.h"


template <typename T>
void DebugLog(T value) {
	std::ostringstream ss;
	ss << value;
	std::string s(ss.str());
	OutputDebugString(s.c_str());
}



CSceneManager::CSceneManager() {
	m_pCamera = NULL;
	m_pActorManager = NULL;
	m_pHUDManager = NULL;
	m_pAudio = NULL;
}

CSceneManager::~CSceneManager() {
}

void CSceneManager::Initialise() {
	state = inMenu;
	//GotoGame();
}

void CSceneManager::InitialiseLevel() {
	m_pCamera->SetDistance(0);
	m_pCamera->Reload();
	
	m_pActorManager->AddPlayer();

	//adjust number of actor
	m_pActorManager->AddActor(AActor::Character::tree, 225);
	m_pActorManager->AddActor(AActor::Character::chinese, 8);
	m_pActorManager->AddActor(AActor::Character::doll, 6);
	//m_pActorManager->AddActor(AActor::Character::eyeball, 8);
	m_pActorManager->AddActor(AActor::Character::face, 5);
	//m_pActorManager->AddActor(AActor::Character::coffin, 12);
	//m_pActorManager->AddActor(AActor::Character::umbrella, 6);
	
	m_pHUDManager->GetBloodEffect()->Disable();
	m_pHUDManager->GetBloodEffect()->SetLifeTime(-1);
}


void CSceneManager::ClearLevel() {
	m_pCamera->SetMouseTime(0);
	m_pActorManager->ClearActors();
	m_pHUDManager->GetBloodEffect()->Disable();
}

void CSceneManager::UpdateMenu() {
	m_pCamera->SetViewByMouse();
	m_pHUDManager->GetMouseCursor()->CopyData(m_pCamera->GetMousePosition());
	if (m_pCamera->IsLeftMouseClicked() && m_pHUDManager->GetMenu()->IsActive()) {	//if in menu
		GotoGame();
	}
}

void CSceneManager::UpdateEndScreen() {
	m_pCamera->SetViewByMouse();
	m_pHUDManager->GetMouseCursor()->CopyData(m_pCamera->GetMousePosition());
	if (m_pCamera->IsLeftMouseClicked() && m_pHUDManager->GetEndScreen()->IsActive()) {	//if in end screen
		GotoMenu();
	}
}


#pragma region ChanageSceen
void CSceneManager::GotoMenu() {
	m_pCamera->SetMouseTime(0);
	ClearLevel();
	m_pHUDManager->GetEndScreen()->Disable();
	m_pHUDManager->GetMenu()->Active();
	m_pHUDManager->GetBloodEffect()->SetLifeTime(-1);
	
	state = inMenu;
}

void CSceneManager::GotoGame() {
	m_pCamera->NoScope();
	m_pCamera->SetMouseTime(0);
	m_pHUDManager->GetMenu()->Disable();
	m_pHUDManager->GetBloodEffect()->SetLifeTime(-1);
	InitialiseLevel();
	state = inGame;
	m_pAudio->PlayHouseMusic();
}

void CSceneManager::GotoEndScreen() {
	m_pCamera->SetMouseTime(0);
	ClearLevel();
	m_pHUDManager->GetMenu()->Disable();
	m_pHUDManager->GetEndScreen()->Active();
	m_pHUDManager->GetBloodEffect()->SetLifeTime(-1);
	state = inEndSreen;
}
#pragma endregion




CSceneManager::State CSceneManager::GetState() {
	return state;
}

#pragma region Setter
void CSceneManager::SetCamera(CCamera *cam) {
	m_pCamera = cam;
}

void CSceneManager::SetActorManager(CActorManager *am) {
	m_pActorManager = am;
}

void CSceneManager::SetHUDManager(CHUDManager *hm) {
	m_pHUDManager = hm;
}

void CSceneManager::SetAudio(CAudio* audio) {
	m_pAudio = audio;
}
#pragma endregion