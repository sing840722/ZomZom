#include "ActorManager.h"
#include "Actor.h"
#include "Object.h"
#include "CatmullRom.h"
#include "Camera.h"
#include "SceneManager.h"
#include "HUDManager.h"
#include "HUD.h"
#include "Audio.h"

template <typename T>
void DebugLog(T value) {
	std::ostringstream ss;
	ss << value;
	std::string s(ss.str());
	OutputDebugString(s.c_str());
}

CActorManager::CActorManager() {
	m_pModels = NULL;
	m_pPlayer = NULL;

	//do not delete
	m_pCatmullRom = NULL;
	m_pCamera = NULL;
	m_pHUDManager = NULL;
	m_pSceneManager = NULL;
}

CActorManager::~CActorManager() {
	ClearActors();

	if (m_pModels != NULL) {
		for (int i = 0; i < m_pModels->size(); i++) {
			(*m_pModels)[i]->COpenAssetImportMesh::ClearModelData();
			delete (*m_pModels)[i];
		}
	}
	delete m_pModels;
}

#pragma region Initialise
void CActorManager::Initialise() {
	m_pModels = new vector<AActor*>;

	AActor *m_pPlayerModel = new AActor;
	m_pPlayerModel->Load("resources\\models\\Sniper\\sniper.obj");
	(*m_pModels).push_back(m_pPlayerModel);
	
	AActor *m_pFaceMonsterModel = new AActor;
	m_pFaceMonsterModel->Load("resources\\models\\Face\\face.obj");			//face monster rep
	(*m_pModels).push_back(m_pFaceMonsterModel);

	AActor *m_pEyeballMonsterModel = new AActor;
	m_pEyeballMonsterModel->Load("resources\\models\\Eyeball\\eyeball.obj");
	(*m_pModels).push_back(m_pEyeballMonsterModel);

	AActor *m_pChineseMonsterModel = new AActor;
	m_pChineseMonsterModel->Load("resources\\models\\Chinese\\zombie.obj");
	(*m_pModels).push_back(m_pChineseMonsterModel);

	AActor *m_pDollMonsterModel = new AActor;
	m_pDollMonsterModel->Load("resources\\models\\DollHead\\Creepy_Doll_Head.obj");
	(*m_pModels).push_back(m_pDollMonsterModel);

	AActor *m_pTreeModel = new AActor;
	m_pTreeModel->Load("resources\\models\\Tree\\tree.obj");
	(*m_pModels).push_back(m_pTreeModel);

	AObject *m_pCoffinMonsterModel = new AObject;
	m_pCoffinMonsterModel->Load("coffin");
	(*m_pModels).push_back(m_pCoffinMonsterModel);

	AObject *m_pUmbrellaMonsterModel = new AObject;
	m_pUmbrellaMonsterModel->Load("umbrella");
	(*m_pModels).push_back(m_pUmbrellaMonsterModel);

}
#pragma endregion



void CActorManager::Update(float dt) {
	m_dt = dt;

	for (int i = 0; i < m_pActors.size(); i++) {
		MoveObject(m_pActors[i]);
		TurnToCamera(m_pActors[i]);
		m_pActors[i]->Update(m_dt);
		TestTriggers(m_pActors[i]);

		/*
		//comment this to disable hit test collision
		if (PlayerHitTest(m_pActors[i])) {
			m_pSceneManager->GotoEndScreen();
			return;
		}
		*/	
	}

}


void CActorManager::RemoveActor(AActor *actor) {

}

//add player actor 
void CActorManager::AddPlayer() {
	m_pPlayer = new AActor;
	m_pPlayer->Initialise(AActor::player);
	m_pPlayer->SetDistance(0);
	m_pActors.push_back(m_pPlayer);
}

//clear every actor in the list
void CActorManager::ClearActors() {
	for (std::vector<AActor* >::iterator it = m_pActors.begin(); it != m_pActors.end(); ++it)
	{
		delete (*it);
	}
	m_pActors.clear();
}


#pragma region GetRenderModel
AActor* const CActorManager::GetPlayerModel() const {
	return (*m_pModels)[0];
}
AActor* const CActorManager::GetFaceMonsterModel() const {
	return (*m_pModels)[1];
}
AActor* const CActorManager::GetEyeBallMonsterModel() const {
	return (*m_pModels)[2];
}
AActor* const CActorManager::GetChineseMonsterModel() const {
	return (*m_pModels)[3];
}
AActor* const CActorManager::GetDollMonsterModel() const {
	return (*m_pModels)[4];
}
AActor* const CActorManager::GetTreeModel() const {
	return (*m_pModels)[5];
}
AActor* const CActorManager::GetCoffinMonsterModel() const {
	return (*m_pModels)[6];
}
AActor* const CActorManager::GetUmbrellaMonsterModel() const {
	return (*m_pModels)[7];
}
#pragma endregion


AActor* const CActorManager::GetPlayer() const {
	return m_pPlayer;
}

vector<AActor*> const CActorManager::GetActors() const {
	return m_pActors;
}

#pragma region Method to Loop

bool CActorManager::PlayerHitTest(AActor *actor) {
	if (m_pPlayer->GetDistance() < 5)
		return false;

	if (actor == m_pPlayer || actor->GetID() == AActor::tree)
		return false;

	if (!actor->IsActive())
		return false;

	//if monster is very close to camera(player), damange the player
	if (glm::length(m_pPlayer->GetPosition() - actor->GetPosition()) < 6) {
		m_pCamera->RequestCameraShake(4);
		actor->Disable();  //each monster can only hit player once
		if (!m_pHUDManager->GetBloodEffect()->IsActive()) {
			m_pHUDManager->GetBloodEffect()->CopyData(m_pCamera->GetMousePosition());
			m_pHUDManager->GetBloodEffect()->SetLifeTime(2500);
			m_pHUDManager->GetBloodEffect()->Active();
		}
		else {	//if already injured
			return true;	
		}
	}

	return false;
}

void CActorManager::MoveObject(AActor* object) {
	AActor::Character id = object->GetID();
	if (id == AActor::tree) return;

	float distance = object->GetDistance();

	vector<glm::vec3> path = m_pCatmullRom->GetEnemyPath();


	if(id == AActor::player)
		path = m_pCatmullRom->GetControlPoints();

	glm::vec3 pNext;	//pNext - p = tegent = forward vector of p
	glm::vec3 p;

	m_pCatmullRom->Sample(distance, p, path);
	m_pCatmullRom->Sample(distance + 1, pNext, path);

	glm::vec3 y = glm::vec3(0, 1, 0);
	glm::vec3 T = glm::normalize(pNext - p);
	glm::vec3 N = glm::normalize(glm::cross(T, y));
	glm::vec3 B = glm::normalize(glm::cross(N, T));

	//Set Trigger point before additional factor add to the position
	if (object->HasTrigger()) {
		glm::vec3 t_position;
		
		if(id == AActor::face)
			m_pCatmullRom->Sample(distance + 30, t_position, path);
		else if(id == AActor::coffin)
			m_pCatmullRom->Sample(distance + 10, t_position, path);
		else
			m_pCatmullRom->Sample(distance + 40, t_position, path);
		object->SetTriggerPosition(t_position);
	}

	float inLane = object->GetInLane();
	p += (N * inLane);						//stay left/ middle/ right base on the value inLane
	p += (N * object->GetTurn());			//change horizontal(N) position of player
	p += (B * object->GetYOffset());		//change vertical(B) position of the actor

	object->SetPosition(p);
	
	//if the actor is face monster or chinese zombie or doll, look at player
	if (glm::length(p - m_pCamera->GetPosition())  > 0.5) {
		if (id == AActor::Character::face || id == AActor::Character::chinese || id == AActor::Character::doll) {
			T = glm::normalize(m_pCamera->GetPosition() - p);
			N = glm::cross(T, B);
		}
	}

	if (id == AActor::player) {	//if this is player
		T = m_pCamera->GetRayDirection();	//follow the mouse
		N = glm::cross(T, B);
	}

	object->SetOrientationMatrix(glm::mat4(glm::mat3(T, B, N)));
}

bool CActorManager::TestTriggers(AActor* actor) {
	if (actor == m_pPlayer)
		return false;

	if (actor->HasTrigger()) {
		HitTestTrigger(actor);
	}

	return false;
}

bool CActorManager::HitTestTrigger(AActor *obj) {
	glm::vec3 triggerPos = obj->GetTriggerPosition();
	triggerPos.y = m_pPlayer->GetPosition().y;	//do not consider vertical distance
	if (glm::length(triggerPos - m_pPlayer->GetPosition()) < 5) { 				//if overlap Trigger
		obj->RemoveTrigger();		//disable trigger
		obj->StartAnimation();
		if (obj->GetID() == AActor::Character::face) {
			//m_pCamera->RequestCameraShake(12.5f + (17.5f * m_dt * 0.01f));
			m_pCamera->RequestCameraShake(12.5f + (17.5f * m_dt * 0.01f));
			m_pAudio->PlayFaceMonsterSound(obj->GetPosition() 
				,glm::vec3(0.0f, -100, 0)
				);
			m_pAudio->SetMonster(obj);
			return true;
		}
		return true;
	}
}


bool CActorManager::TestRayCasting() {
	for (int i = 0; i < m_pActors.size(); i++) {
		if ((m_pActors)[i]->GetID() != AActor::player &&	//do not test for player and tree
			(m_pActors)[i]->GetID() != AActor::tree) {

			if (m_pCamera->MousePicking((m_pActors)[i])) {	//if successfuly hit, activiate shoot effect
				m_pHUDManager->GetShotEffect()->CopyData(m_pCamera->GetMousePosition());
				m_pHUDManager->GetShotEffect()->Active();
				m_pHUDManager->GetShotEffect()->SetLifeTime(750);
				(m_pActors)[i]->SetHP();

				if ((m_pActors)[i]->GetHP() < 1) {	//if monster has less than 1 hp, kill the monster
					
					m_pAudio->PlayKilledSoundEffect((m_pActors)[i]->GetPosition());

					AddActor((m_pActors)[i]->GetID(), 1, m_pCamera->GetDistance());
					delete (m_pActors)[i];
					m_pActors.erase(m_pActors.begin() + i);
				}
				return true;
			}
		}
	}
	return false;
}

//place object on the path when it is initilised
void CActorManager::PlaceObject(AActor* object) {
	float distance = object->GetDistance();
	AActor::Character id = object->GetID();
	vector<glm::vec3> path = m_pCatmullRom->GetEnemyPath();

	glm::vec3 pNext;	//pNext - p = tegent = forward vector of p
	glm::vec3 p;

	m_pCatmullRom->Sample(distance, p, path);
	m_pCatmullRom->Sample(distance + 1, pNext, path);

	glm::vec3 y = glm::vec3(0, 1, 0);
	glm::vec3 T = glm::normalize(pNext - p);
	glm::vec3 N = glm::normalize(glm::cross(T, y));
	glm::vec3 B = glm::normalize(glm::cross(N, T));

	//Set Trigger point before additional factor add to the position
	if (object->HasTrigger()) {
		glm::vec3 t_position;
		m_pCatmullRom->Sample(distance + 50, t_position, path);
		object->SetTriggerPosition(t_position);
	}

	float inLane = object->GetInLane();
	p += (N * inLane);						//stay left/ middle/ right base on the value inLane
	p += (N * object->GetTurn());			//change horizontal(N) position of player
	p += (B * object->GetYOffset());		//change vertical(B) position of the actor

	object->SetPosition(p);
}


//This function is for tree only, since tree does not move. it will be wasteful to calculate the new position for the group of tree in every frame
//just calculate the oreientation matrix
void CActorManager::TurnToCamera(AActor* object) {
	AActor::Character id = object->GetID();
	if (id == AActor::tree) {}
	else { return; }

	glm::vec3 p = object->GetPosition();
	glm::mat4 m_matrix = object->GetOrientationMatrix();
	glm::vec3 T = glm::vec3(m_matrix[0][0], m_matrix[0][1], m_matrix[0][2]);
	glm::vec3 B = glm::vec3(m_matrix[1][0], m_matrix[1][1], m_matrix[1][2]);
	glm::vec3 N = glm::vec3(m_matrix[2][0], m_matrix[2][1], m_matrix[2][2]);//right

	if (glm::length(object->GetPosition() - m_pCamera->GetPosition())  > 5.5) {
		if (id == AActor::Character::tree) {
			T = glm::normalize(m_pCamera->GetPosition() - p);
			N = glm::cross(T, B);
			object->SetOrientationMatrix(glm::mat4(glm::mat3(T, B, N)));
		}
	}
}

#pragma endregion


void CActorManager::AddActor(int id, int number, float camPos) {
	AActor::Character monster = static_cast<AActor::Character>(id);
	AActor *m_pActor;

	for (int i = 0; i < number; i++) {
		if (monster == AActor::coffin || monster == AActor::umbrella)	//derived class of AActor
			m_pActor = new AObject;
		else
			m_pActor = new AActor;

		m_pActor->Initialise(monster, camPos);
		PlaceObject(m_pActor);
		(m_pActors).push_back(m_pActor);
		//m_pDolls.push_back(m_pActor);
		//m_pAudio->PlayDollMoveSoundEffect();
	}

	//if (id == AActor::doll) {
		//m_pAudio->SetDolls(this->m_pDolls);
	//}
}


#pragma region Setter Function
void CActorManager::SetCatmullRom(CCatmullRom *c) {
	m_pCatmullRom = c;
}

void CActorManager::SetCamera(CCamera *cam) {
	m_pCamera = cam;
}

void CActorManager::SetHUDManager(CHUDManager *hm) {
	m_pHUDManager = hm;
	m_pHUDManager->SetCamera(this->m_pCamera);
}

void CActorManager::SetSceneManager(CSceneManager *sm) {
	m_pSceneManager = sm;
	m_pSceneManager->SetActorManager(this);
	m_pSceneManager->SetCamera(this->m_pCamera);
	m_pSceneManager->SetHUDManager(this->m_pHUDManager);
}

void CActorManager::SetAudio(CAudio *audio) {
	this->m_pAudio = audio;
}

#pragma endregion

