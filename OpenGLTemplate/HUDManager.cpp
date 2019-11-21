#include "HUDManager.h"
#include "HUD.h"
#include "Camera.h"

CHUDManager::CHUDManager() {
	m_pCamera = NULL;
	m_pHUDs = NULL;
	m_pAmmos = NULL;
	m_pInGameHUD = NULL;
	m_pQuad = NULL;
	m_pMouseCursor = NULL;
	m_pShotEffect = NULL;
	m_pBloodEffect = NULL;
	m_pMenu = NULL;
	m_pEndScreen = NULL;

}
CHUDManager::~CHUDManager() {
	if (m_pHUDs != NULL) {
		for (int i = 0; i < m_pHUDs->size(); i++) {
			delete (*m_pHUDs)[i];
		}
		
	}
	delete m_pHUDs;
	delete m_pInGameHUD;
	delete m_pAmmos;
	delete m_pQuad;
}

#pragma region Initialise
void CHUDManager::Initialise() {
	m_pHUDs = new vector<AHUD*>;
	m_pAmmos = new vector<AHUD*>;
	m_pInGameHUD = new vector<AHUD*>;
	

	m_pQuad = new AHUD;
	m_pQuad->CreateQuad();

	//follow the order to create layer	shot->blood->crosshair->ammo
	m_pBloodEffect = AddHUD("resources\\textures\\blood.png", 0, 0, 0.5 *1.5, 1 * 1.5, false);
	m_pBloodEffect->EnableOpacity();

	m_pShotEffect = AddHUD("resources\\textures\\blood2.png", 0, 0, 0.5, 1, false);
	m_pShotEffect->EnableOpacity();

	m_pMouseCursor = AddHUD("resources\\textures\\crosshair.png", 0, 0, 6, 6.6, true);


	for (int i = 0; i < 6; i++) {
		AHUD *m_pHUD = AddHUD("resources\\textures\\ammo.png", 0.9 - (0.06 *i), -0.75, 0.02, 0.16, true);
		(*m_pAmmos).push_back(m_pHUD);
	}

	m_pMenu = new AHUD;
	m_pMenu->Initialise(glm::vec2(0, 0), glm::vec2(1, 1));
	m_pMenu->Load("resources\\textures\\menu.jpg");
	m_pMenu->Active();
	(*m_pHUDs).push_back(m_pMenu);

	m_pEndScreen = new AHUD;
	m_pEndScreen->Initialise(glm::vec2(0, 0), glm::vec2(1, 1));
	m_pEndScreen->Load("resources\\textures\\endScreen.jpg");
	m_pEndScreen->Disable();
	(*m_pHUDs).push_back(m_pEndScreen);
}
#pragma endregion


AHUD* CHUDManager::AddHUD(string path, float xPos, float yPos, float xScale, float yScale, bool active) {
	AHUD *m_pHUD = new AHUD;
	m_pHUD->Initialise(glm::vec2(xPos, yPos), glm::vec2(xScale, yScale));
	m_pHUD->Load(path);

	if (active == false)
		m_pHUD->Disable();

	(*m_pHUDs).push_back(m_pHUD);
	(*m_pInGameHUD).push_back(m_pHUD);

	return m_pHUD;
}

void CHUDManager::Update(float m_dt) {
	for each (AHUD* widget in *m_pHUDs) {
		widget->Update(m_dt);
	}

	m_pMouseCursor->CopyData(m_pCamera->GetMousePosition());

	if (m_pCamera->UsingScope()) {
		m_pBloodEffect->CopyData(m_pCamera->GetMousePosition());
	}
	else {
		m_pBloodEffect->CopyData(glm::vec2(0, 0));
	}

	
	for (int i = 1; i < 7; i++) {
		if (m_pCamera->GetAmmo() >= i) {
			(*m_pAmmos)[i - 1]->Active();
		}
		else {
			(*m_pAmmos)[i - 1]->Disable();
		}
	}
	
}

#pragma region Getter
AHUD* const CHUDManager::GetQuad() const {
	return m_pQuad;
}

AHUD* const CHUDManager::GetShotEffect() const {
	return m_pShotEffect;
}

AHUD* const CHUDManager::GetBloodEffect() const {
	return m_pBloodEffect;
}

AHUD* const CHUDManager::GetMouseCursor() const {
	return m_pMouseCursor;
}

AHUD* const CHUDManager::GetMenu() const {
	return m_pMenu;
}

AHUD* const CHUDManager::GetEndScreen() const {
	return m_pEndScreen;
}


vector<AHUD*> * const CHUDManager::GetHUDs() const {
	return m_pHUDs;
}

vector<AHUD*> *const CHUDManager::GetAmmoHUD() const {
	return m_pAmmos;
}

vector<AHUD*> *const CHUDManager::GetInGameHUD() const {
	return m_pInGameHUD;
}
#pragma endregion


void CHUDManager::SetCamera(CCamera *cam) {
	m_pCamera = cam;
}