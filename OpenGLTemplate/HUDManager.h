#pragma once
#include "Common.h"

class AHUD;
class CCamera;
class CHUDManager {
private:
	
	CCamera *m_pCamera;

	vector<AHUD*> *m_pHUDs;
	vector<AHUD*> *m_pAmmos;
	vector<AHUD*> *m_pInGameHUD;

	AHUD *m_pQuad;
	AHUD *m_pMouseCursor;
	AHUD *m_pShotEffect;
	AHUD *m_pBloodEffect;
	AHUD *m_pMenu;
	AHUD *m_pEndScreen;

public:
	CHUDManager();
	~CHUDManager();

	void Initialise();

	//Update every HUD
	void Update(float m_dt);

	AHUD* AddHUD(string path, float xPos, float yPos, float xScale, float yScale, bool active);

	void RemoveHUD();

	//Getter
	vector<AHUD*> *const GetAmmoHUD() const;
	vector<AHUD*> *const GetInGameHUD() const;
	vector<AHUD*>* const GetHUDs() const;
	AHUD* const GetQuad() const;
	AHUD* const GetShotEffect() const;
	AHUD* const GetBloodEffect() const;
	AHUD* const GetMouseCursor() const;
	AHUD* const GetMenu() const;
	AHUD* const GetEndScreen() const;

	void SetCamera(CCamera *cam);
};