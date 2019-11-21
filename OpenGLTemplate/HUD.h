#pragma once
#include "Common.h"
#include "VertexBufferObject.h"
#include "OpenAssetImportMesh.h"


class CTexture;
class AButton;

class AHUD {
protected:
	CTexture *m_texture;
	glm::vec2 m_position;
	glm::vec2 m_scale;
	bool bIsActive;
	bool bHasOpacity;
	float m_lifeTime;
	double m_opacity;
private:
	GLuint m_vao;
public:
	AHUD();
	~AHUD();

	glm::vec2 &const GetPosition();
	glm::vec2 &const GetScale();

	
	void BindTexture();

	void CreateQuad();
	void Render();
	void Load(string path);
	void Initialise(glm::vec2 pos, glm::vec2 scale);
	void CopyData(glm::vec2 pos);
	
	bool IsActive();
	void Active();
	void Disable();
	void SetLifeTime(float t);
	void Update(float m_dt);

	float GetOpacity();
	bool HasOpacity();
	void EnableOpacity();
	void DisableOpacity();
};