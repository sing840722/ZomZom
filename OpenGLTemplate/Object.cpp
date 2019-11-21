#include "Object.h"
#include "gamewindow.h"
#include "Camera.h"
#include <string.h>

template <typename T>
void DebugLog(T value) {
	std::ostringstream ss;
	ss << value;
	std::string s(ss.str());
	OutputDebugString(s.c_str());
}



AObject::AObject(){
	
}

AObject::~AObject() {

}

void AObject::Update(float m_dt) {

	switch (id)
	{
	case coffin:
		if (!bIsFinishAnimate) {
			if (m_angle > -90) {
				m_angle -= 1.75;
			}
			else {
				bIsFinishAnimate = true;
			}
		}
		break;
	case umbrella:
		if (!bIsFinishAnimate) {
			m_angle += 5;
			if (m_yOffset > 0.8) {
				m_yOffset -= 0.015;
			}
			else {
				bIsFinishAnimate = true;
			}
		}
		if (bIsFinishAnimate) {
			m_angle += 5;
			if (m_yOffset < 12.235) {
				m_yOffset += 0.015;
			}
			else {
				bIsFinishAnimate = false;
			}
		}
		break;
	default:
		break;
	}
}

//load vertices
bool AObject::Load(string model) {
	if (model == "coffin") {
		id = coffin;
		SetCoffinPoints();
		CreateCoffinOffsetPoints();
		CreateCoffin();
		Create("resources\\textures\\coffin.jpg");
		Create("resources\\textures\\coffin2.jpg");	
	}
	else if (model == "umbrella") {
		id = umbrella;
		SetUmbrellaPoints();
		CreateUmbrellaOffsetPoints();
		CreateUmbrella();
		Create("resources\\textures\\umbrella.png");
		Create("resources\\textures\\Black.jpg");
	}
	return true;
}

void AObject::Initialise(Character character, float camPos) {
	AActor::Initialise(character, camPos);
	if (character == Character::coffin) {
		id = Character::coffin;
		m_scale = 1.4f;
		m_yOffset = 0.75;
		bHasTrigger = true;
		bIsFinishAnimate = true;
		m_radius = 2.5;
	}
	else if (character == Character::umbrella) {
		id = Character::umbrella;
		bIsFinishAnimate = true;
		m_scale = 1.6f;
		m_yOffset = 12.235;
		m_speed = 0.0015;
		bHasTrigger = true;
		m_radius = 1.5f;
	}
	//set boundary width depth height
}



void AObject::Create(string filename) {
	m_texture.Load(filename);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	m_textures.push_back(m_texture);
}


void AObject::SetCoffinPoints() {
	m_coffinTopVertices.push_back(glm::vec3(0.0, 0.0, -0.2));	//top side
	m_coffinTopVertices.push_back(glm::vec3(0.0, 0.0, -1.2));
	m_coffinTopVertices.push_back(glm::vec3(0.5, 0.0, -1.2));	//right side
	m_coffinTopVertices.push_back(glm::vec3(1.0, 0.0, -0.2));
	m_coffinTopVertices.push_back(glm::vec3(0.5, 0.0, 2.3));
	m_coffinTopVertices.push_back(glm::vec3(0.0, 0.0, 2.3));	//bottom
	m_coffinTopVertices.push_back(glm::vec3(-0.5, 0.0, 2.3));	//left side
	m_coffinTopVertices.push_back(glm::vec3(-1.0, 0.0, -0.2));	
	m_coffinTopVertices.push_back(glm::vec3(-0.5, 0.0, -1.2));
	m_coffinTopVertices.push_back(glm::vec3(0.0, 0.0, -1.2));	//top side
}

void AObject::CreateCoffinOffsetPoints() {
	
	m_coffinBotVertices = m_coffinTopVertices;
	for (int i = 0; i < m_coffinTopVertices.size(); i++) {
		m_coffinBotVertices[i].y = -0.5;

		if (i > 1 && i < 5) {
			m_coffinRightVertices.push_back(m_coffinTopVertices[i]);
			m_coffinRightVertices.push_back(m_coffinBotVertices[i]);
		}
		if (i > 5 && i < 9) {
			m_coffinLeftVertices.push_back(m_coffinTopVertices[i]);
			m_coffinLeftVertices.push_back(m_coffinBotVertices[i]);
		}
		
		if (i > 3 && i < 7) {
			m_coffinFrontVertices.push_back(m_coffinTopVertices[i]);
			m_coffinFrontVertices.push_back(m_coffinBotVertices[i]);
		}

	}
	
	m_coffinBackVertices.push_back(m_coffinTopVertices[m_coffinTopVertices.size()-2]);
	m_coffinBackVertices.push_back(m_coffinBotVertices[m_coffinBotVertices.size()-2]);
	m_coffinBackVertices.push_back(m_coffinTopVertices[m_coffinTopVertices.size()-1]);
	m_coffinBackVertices.push_back(m_coffinBotVertices[m_coffinBotVertices.size()-1]);
	m_coffinBackVertices.push_back(m_coffinTopVertices[2]);
	m_coffinBackVertices.push_back(m_coffinBotVertices[2]);
}

void AObject::CreateCoffin() {
	CreateObject(id, m_coffinTopVertices, m_vaoCoffinTop, glm::vec3(0.0f, 1.0f, 0.0f));
	CreateObject(id, m_coffinBotVertices, m_vaoCoffinBot, glm::vec3(0.0f, -1.0f, 0.0f));
	CreateObject(id, m_coffinRightVertices, m_vaoCoffinRight, glm::vec3(1.0f, 0.0f, 0.0f));
	CreateObject(id, m_coffinLeftVertices,  m_vaoCoffinLeft, glm::vec3(-1.0f, 0.0f, 0.0f));
	CreateObject(id, m_coffinFrontVertices,  m_vaoCoffinFront, glm::vec3(0.0f, 0.0f, 1.0f));
	CreateObject(id, m_coffinBackVertices,  m_vaoCoffinBack, glm::vec3(0.0f, 0.0f, -1.0f));
}


void AObject::RenderCoffin() {

	m_textures[0].Bind();
	glBindVertexArray(m_vaoCoffinTop);
	glDrawArrays(GL_TRIANGLE_FAN, 0, m_coffinTopVertices.size());
	
	glBindVertexArray(m_vaoCoffinBot);
	glDrawArrays(GL_TRIANGLE_FAN, 0, m_coffinBotVertices.size());

	m_textures[1].Bind();
	glBindVertexArray(m_vaoCoffinRight);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_coffinRightVertices.size());

	glBindVertexArray(m_vaoCoffinLeft);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_coffinLeftVertices.size());
	
	glBindVertexArray(m_vaoCoffinFront);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_coffinFrontVertices.size());

	glBindVertexArray(m_vaoCoffinBack);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_coffinBackVertices.size());

}


void AObject::SetUmbrellaPoints() {
	m_umbrellaVertices.push_back(glm::vec3(0.0f, 0.5f, 0.0f));
	m_umbrellaVertices.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
	m_umbrellaVertices.push_back(glm::vec3(0.75f, 0.0f, -0.75f));
	m_umbrellaVertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	m_umbrellaVertices.push_back(glm::vec3(0.75f, 0.0f, 0.75f));
	m_umbrellaVertices.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	m_umbrellaVertices.push_back(glm::vec3(-0.75f, 0.0f, 0.75f));
	m_umbrellaVertices.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
	m_umbrellaVertices.push_back(glm::vec3(-0.75f, 0.0f, -0.75f));
	m_umbrellaVertices.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
}

void AObject::CreateUmbrellaOffsetPoints() {
	//m_umbrellaHandleVertices.push_back()
	for (int i = 0; i < 5; i++) {
		m_umbrellaBoneFrontVertices.push_back(glm::vec3(-0.0125f, -(i*0.1f)*4 +0.5, 0.0125f));
		m_umbrellaBoneFrontVertices.push_back(glm::vec3(0.0125f, -(i*0.1f)*4 +0.5, 0.0125f));

		m_umbrellaBoneBackVertices.push_back(glm::vec3(-0.0125f, -(i*0.1f) * 4 +0.5, -0.0125f));
		m_umbrellaBoneBackVertices.push_back(glm::vec3(0.0125f, -(i*0.1f) * 4 +0.5, -0.0125f));


		m_umbrellaBoneLeftVertices.push_back(glm::vec3(-0.0125f, -(i*0.1f) * 4 +0.5, 0.0125f));
		m_umbrellaBoneLeftVertices.push_back(glm::vec3(-0.0125f, -(i*0.1f) * 4 +0.5, -0.0125f));

		m_umbrellaBoneRightVertices.push_back(glm::vec3(0.0125f, -(i*0.1f) * 4 +0.5, 0.0125f));
		m_umbrellaBoneRightVertices.push_back(glm::vec3(0.0125f, -(i*0.1f) * 4 +0.5, -0.025f));
	}
}

void AObject::CreateUmbrella() {
	CreateObject(id, m_umbrellaVertices, m_vaoUmbrellaTop, glm::vec3(0.0f, 1.0f, 0.0));
	CreateObject(id, m_umbrellaBoneFrontVertices, m_vaoUmbrellaBoneFront, glm::vec3(0.0f, 0.0f, 1.0f));
	CreateObject(id, m_umbrellaBoneBackVertices, m_vaoUmbrellaBoneBack, glm::vec3(0.0f, 0.0f, -1.0f));
	CreateObject(id, m_umbrellaBoneLeftVertices, m_vaoUmbrellaBoneLeft, glm::vec3(-1.0f, 0.0f, 0.0f));
	CreateObject(id, m_umbrellaBoneRightVertices, m_vaoUmbrellaBoneRight, glm::vec3(1.0f, 0.0f, 0.0f));
}

void AObject::RenderUmbrella() {
	m_textures[0].Bind();
	glBindVertexArray(m_vaoUmbrellaTop);
	glDrawArrays(GL_TRIANGLE_FAN, 0, m_umbrellaVertices.size());

	m_textures[1].Bind();
	glBindVertexArray(m_vaoUmbrellaBoneFront);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_umbrellaBoneFrontVertices.size());

	glBindVertexArray(m_vaoUmbrellaBoneBack);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_umbrellaBoneBackVertices.size());
	
	glBindVertexArray(m_vaoUmbrellaBoneLeft);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_umbrellaBoneLeftVertices.size());
	
	glBindVertexArray(m_vaoUmbrellaBoneRight);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_umbrellaBoneRightVertices.size());
}


void AObject::Render() {
	if (id == Character::coffin)
	RenderCoffin();

	if (id == Character::umbrella)
	RenderUmbrella();
}

void AObject::CreateObject(AActor::Character id, vector<glm::vec3> &vertices, GLuint &vao, glm::vec3 normal) {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	float width, height, depth, xOffset, yOffset, zOffset;

	if (id == coffin) {
		width = 2.0f;
		height = 0.5f;
		depth = 3.5f;
		xOffset = 1.0f;
		yOffset = 0.5f;
		zOffset = 1.3f;
	}
	else if (id == umbrella) {
		width = 2.0f;
		height = 0.5f;
		depth = 2.0f;
		xOffset = 1.0f;
		yOffset = 0.0f;
		zOffset = 1.0f;
	}
	else if (id == chinese) {
	
	}
	else {
		return;
	}


	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();
	glm::vec2 texCoord(0.0f, 0.0f);

	for (int i = 0; i < vertices.size(); i++) {

		if (normal.x != 0) {
			texCoord = glm::vec2((vertices[i].y + yOffset) / height * 2, (vertices[i].z + zOffset) / depth);	//side		//left right
		}
		else if (normal.y != 0) {
			texCoord = glm::vec2((vertices[i].x + xOffset) / width, (vertices[i].z + zOffset) / depth);	//top
		}
		else if (normal.z != 0) {	//front/back
			//texCoord = glm::vec2((vertices[i].y + zOffset) / depth * 2, (vertices[i].x + xOffset) / width);	//side
			texCoord = glm::vec2((vertices[i].y + xOffset) / width * 2, (vertices[i].y + yOffset) / height);	//side
		}

		vbo.AddData(&vertices[i], sizeof(glm::vec3));
		vbo.AddData(&texCoord, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));
	}


	// Upload the VBO to the GPU
	vbo.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
		+ sizeof(glm::vec2)));
}

void AObject::CreateWall() {
	Create("resources\\textures\\coffin.jpg");
	{	//left
		m_wallLeftVertices.push_back(glm::vec3(-10.0f, 0.0f, 0.0f));
		m_wallLeftVertices.push_back(glm::vec3(-10.0f, 10.0f, 0.0f));
		m_wallLeftVertices.push_back(glm::vec3(10.0f, 0.0f, 0.0f));
		m_wallLeftVertices.push_back(glm::vec3(10.0f, 10.0f, 0.0f));
	}

	{	//front
		m_wallFrontVertices.push_back(glm::vec3(-10.0f, 0.0f, 0.0f));
		m_wallFrontVertices.push_back(glm::vec3(-10.0f, 10.0f, 0.0f));

		m_wallFrontVertices.push_back(glm::vec3(-10.0f, 0.0f, 15.0f));
		m_wallFrontVertices.push_back(glm::vec3(-10.0f, 10.0f, 15.0f));
	}

	{	//back
		m_wallBackVertices.push_back(glm::vec3(10.0f, 0.0f, 0.0f));
		m_wallBackVertices.push_back(glm::vec3(10.0f, 10.0f, 0.0f));
		m_wallBackVertices.push_back(glm::vec3(10.0f, 0.0f, 15.0f));
		m_wallBackVertices.push_back(glm::vec3(10.0f, 10.0f, 15.0f));
	}

	{	//right
		m_wallRightVertices.push_back(glm::vec3(-10.0f, 0.0f, 15.0f));
		m_wallRightVertices.push_back(glm::vec3(-10.0f, 10.0f, 15.0f));
		m_wallRightVertices.push_back(glm::vec3(10.0f, 0.0f, 15.0f));
		m_wallRightVertices.push_back(glm::vec3(10.0f, 10.0f, 15.0f));
	}

	{	//Top
		m_wallTopVertices.push_back(glm::vec3(-10.0f, 10.0f, 0.0f));
		m_wallTopVertices.push_back(glm::vec3(10.0f, 10.0f, 0.0f));
		m_wallTopVertices.push_back(glm::vec3(-10.0f, 10.0f, 15.0f));
		m_wallTopVertices.push_back(glm::vec3(10.0f, 10.0f, 15.0f));
	}

	{	//Bot
		m_wallBotVertices.push_back(glm::vec3(-10.0f, 0.0f, 0.0f));
		m_wallBotVertices.push_back(glm::vec3(10.0f, 0.0f, 0.0f));
		m_wallBotVertices.push_back(glm::vec3(-10.0f, 0.0f, 15.0f));
		m_wallBotVertices.push_back(glm::vec3(10.0f, 0.0f, 15.0f));
	}



	CreateObject(Character::chinese, m_wallLeftVertices, m_vaoWallLeft, glm::vec3(0, 0, 1));
	CreateObject(Character::chinese, m_wallFrontVertices, m_vaoWallFront, glm::vec3(-1, 0, 0));
	CreateObject(Character::chinese, m_wallRightVertices, m_vaoWallRight, glm::vec3(0, 0, -1));
	CreateObject(Character::chinese, m_wallTopVertices, m_vaoWallTop, glm::vec3(0, 1, 0));
	CreateObject(Character::chinese, m_wallBotVertices, m_vaoWallBot, glm::vec3(0, -1, 0));
	CreateObject(Character::chinese, m_wallBackVertices, m_vaoWallBack, glm::vec3(1, 0, 0));
}


void AObject::RenderWall() {
	m_textures[0].Bind();
	glBindVertexArray(m_vaoWallLeft);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_wallLeftVertices.size());

	glBindVertexArray(m_vaoWallFront);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_wallFrontVertices.size());

	glBindVertexArray(m_vaoWallRight);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_wallRightVertices.size());

	glBindVertexArray(m_vaoWallTop);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_wallTopVertices.size());

	glBindVertexArray(m_vaoWallBack);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_wallBackVertices.size());

	glBindVertexArray(m_vaoWallBot);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_wallBotVertices.size());
}
