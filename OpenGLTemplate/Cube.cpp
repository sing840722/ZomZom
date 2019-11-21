#include "Cube.h"

CCube::CCube()
{}
CCube::~CCube()
{
	Release();
}
void CCube::Create(string filename)
{
	m_texture.Load(filename);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	m_vbo.Create();
	m_vbo.Bind();
	// Write the code to add interleaved vertex attributes to the VBO

	std::vector<glm::vec3> v_pos;
	std::vector<glm::vec2> t_coord;

	glm::vec3 p4, p5, p6, p7;
	glm::vec3 n(0, 0, 1);
	p4 = glm::vec3(-100, -100, 1);
	p5 = glm::vec3(100, -100, 1);
	p6 = glm::vec3(-100, 100, 1);
	p7 = glm::vec3(100, 100, 1);

	v_pos.push_back(p4);
	v_pos.push_back(p5);
	v_pos.push_back(p6);
	v_pos.push_back(p7);


	glm::vec2 t4(0, 0);
	glm::vec2 t5(1, 0);
	glm::vec2 t6(0, 1);
	glm::vec2 t7(1, 1);
	
	t_coord.push_back(t4);
	t_coord.push_back(t5);
	t_coord.push_back(t6);
	t_coord.push_back(t7);

	for (int i = 0; i < v_pos.size(); i++) {
		m_vbo.AddData(&v_pos[i], sizeof(glm::vec3));
		m_vbo.AddData(&t_coord[i], sizeof(glm::vec2));
		m_vbo.AddData(&n, sizeof(glm::vec3));

	}

	// Upload data to GPU
	m_vbo.UploadDataToGPU(GL_STATIC_DRAW);
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride,
		(void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
}
void CCube::Render()
{
	glBindVertexArray(m_vao);
	m_texture.Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	// Call glDrawArrays to render each side
}
void CCube::Release()
{
	m_texture.Release();
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release();
}