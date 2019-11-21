#include "HUD.h"
#include "gamewindow.h"
#include "Texture.h"


AHUD::AHUD() {
	m_texture = NULL;
	bIsActive = true;
	m_lifeTime = -1;
	bHasOpacity = false;
}

AHUD::~AHUD() {
	delete m_texture;
}

void AHUD::Update(float m_dt) {
	
	
	if (m_lifeTime == -1) {
	
	}
	else {
		if (m_lifeTime > 0) {
			m_lifeTime -= m_dt;
		}

		if (m_lifeTime <= 0) {
			bIsActive = false;
		}
	}
	
}

float AHUD::GetOpacity() {
	if (bHasOpacity)
		return m_lifeTime / 1000;
	else
		return 1;
}

void AHUD::Disable() {

	bIsActive = false;
}

void AHUD::SetLifeTime(float t) {
	m_lifeTime = t;
}

glm::vec2 &const AHUD::GetPosition() {
	return m_position;
}

glm::vec2 &const AHUD::GetScale() {
	return m_scale;
}

void AHUD::BindTexture() {
	m_texture->Bind();
}

void AHUD::CreateQuad() {
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();

	vbo.AddData(&glm::vec2(-1.0, 1), sizeof(glm::vec2));
	vbo.AddData(&glm::vec2(1.0, 1), sizeof(glm::vec2));
	vbo.AddData(&glm::vec2(-1.0, -1), sizeof(glm::vec2));
	vbo.AddData(&glm::vec2(1.0, -1), sizeof(glm::vec2));

	// Upload the VBO to the GPU
	vbo.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
	GLsizei stride = sizeof(glm::vec2);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, 0);
}

void AHUD::Initialise(glm::vec2 pos, glm::vec2 scale) {
	m_position = pos;
	m_scale = scale;
}

void AHUD::Render() {
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void AHUD::Load(string path) {
	m_texture = new CTexture;
	m_texture->Load(path);
	m_texture->SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	m_texture->SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture->SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture->SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void AHUD::CopyData(glm::vec2 pos) {
	m_position = pos;
}

void AHUD::Active() {
	bIsActive = true;
}

bool AHUD::IsActive() {
	return bIsActive;
}


bool AHUD::HasOpacity() {
	return bHasOpacity;
}

void AHUD::DisableOpacity() {
	bHasOpacity = false;
}

void AHUD::EnableOpacity() {
	bHasOpacity = true;
}
