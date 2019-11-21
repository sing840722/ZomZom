#include "LightHouse.h"
#include "Light.h"
#include "Common.h"

ALightHouse::ALightHouse() {
	m_position = glm::vec3(-137.676, 0, -124.156);

	m_pLight = new ALight;
	m_pLight->SetLa(glm::vec3(0));
	m_pLight->SetLd(glm::vec3(0.6,0,0));
	m_pLight->SetLs(glm::vec3(0.85,0,0));

	glm::vec3 lightSrcPosition = m_position;

	lightSrcPosition.y += 65;
	m_pLight->SetPosition(lightSrcPosition);

	m_pLight->SetCutoff(50);
	m_pLight->SetExponent(250);
	m_pLight->SetAttenuation(glm::vec3(1, 0, 0));
}


ALightHouse::~ALightHouse() {
	delete m_pLight;
}

void ALightHouse::Initialise() {
	glm::vec3 lightSrcPosition = m_position;
	lightSrcPosition.y += 65;
	m_pLight->SetPosition(lightSrcPosition);
}


ALight* ALightHouse::GetLight() {
	return m_pLight;
}


void ALightHouse::SetLightDirection(glm::mat3 vNormalMat, glm::vec3 targetLocation) {
	static float t;
	t = t + 0.05;
	glm::vec3 lp = glm::vec3(m_pLight->Position());
	glm::vec3 ttt;
	ttt.x = m_position.x * sin(t);
	ttt.y = 0;
	ttt.z = m_position.x * cos(t/10);


	m_pLight->SetDirection(glm::normalize(vNormalMat *	(ttt - glm::vec3(m_pLight->Position()))));
}


void ALightHouse::SetLightDirection(glm::mat3 vNormalMat) {
	static float t;
	t = t + 0.025;
	glm::vec3 lp = glm::vec3(m_pLight->Position());
	glm::vec3 ttt;
	ttt.x = m_position.x * sin(t);
	ttt.y = 0;
	ttt.z = m_position.x * cos(t / 10);

	m_pLight->SetDirection(glm::normalize(vNormalMat *	(ttt - glm::vec3(m_pLight->Position()))));
}