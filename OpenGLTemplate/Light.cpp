#include "Light.h"

ALight::ALight():m_La(glm::vec3(1)), m_Ld(glm::vec3(1)), m_Ls(glm::vec3(1)), m_exponent(1), m_attenuation(glm::vec3(1,0,0)) {

}

ALight::~ALight() {

}

glm::vec4 ALight::Position() const {
	return glm::vec4(m_position, 1);
}

glm::vec3 ALight::La() const {
	return m_La;
}

glm::vec3 ALight::Ld() const {
	return m_Ld;
}

glm::vec3 ALight::Ls() const {
	return m_Ls;
}

glm::vec3 ALight::Direction() const {
	return m_direction;
}

float ALight::Exponent() const {
	return m_exponent;
}

float ALight::Cutoff() const {
	return m_cutoff;
}

glm::vec3 ALight::Attenuation() const {
	return m_attenuation;
}

void ALight::SetPosition(glm::vec3 p) {
	m_position = p;
}


void ALight::SetLa(glm::vec3 a) {
	m_La = a;
}


void ALight::SetLd(glm::vec3 d) {
	m_Ld = d;
}


void ALight::SetLs(glm::vec3 s) {
	m_Ls = s;
}

void ALight::SetDirection(glm::vec3 d) {
	m_direction = d;
}

void ALight::SetExponent(float e) {
	m_exponent = e;
}

void ALight::SetCutoff(float c) {
	m_cutoff = c;
}

void ALight::SetAttenuation(glm::vec3 a) {
	m_attenuation = a;
}