#pragma once
#include "Common.h"

class ALight {
private:
	glm::vec3 m_position;
	glm::vec3 m_La;
	glm::vec3 m_Ld;
	glm::vec3 m_Ls;
	
	glm::vec3 m_direction;
	float m_exponent;
	float m_cutoff;

	glm::vec3 m_Ma;
	glm::vec3 m_Md;
	glm::vec3 m_Ms;
	float shininess;

	glm::vec3 m_attenuation;
	

public:
	ALight();
	~ALight();

	glm::vec4 Position() const;
	glm::vec3 La() const;
	glm::vec3 Ld() const;
	glm::vec3 Ls() const;

	glm::vec3 Direction() const;
	float Exponent() const;
	float Cutoff() const;
	glm::vec3 Attenuation () const;

	void SetPosition(glm::vec3 p);
	void SetLa(glm::vec3 a);
	void SetLd(glm::vec3 d);
	void SetLs(glm::vec3 s);

	void SetDirection(glm::vec3 d);
	void SetExponent(float e);
	void SetCutoff(float c);
	void SetAttenuation(glm::vec3 a);
};