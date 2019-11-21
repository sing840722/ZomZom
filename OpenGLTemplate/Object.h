#pragma once
#include "Common.h"
#include "vertexBufferObject.h"
#include "vertexBufferObjectIndexed.h"
#include "Texture.h"
#include "Actor.h"

class AActor;
class AObject: public AActor{
private:
	//control points
	vector<glm::vec3> m_coffinTopVertices;
	vector<glm::vec3> m_coffinBotVertices;
	vector<glm::vec3> m_coffinRightVertices;
	vector<glm::vec3> m_coffinLeftVertices;
	vector<glm::vec3> m_coffinFrontVertices;
	vector<glm::vec3> m_coffinBackVertices;

	vector<glm::vec3> m_umbrellaVertices;
	vector<glm::vec3> m_umbrellaHandleVertices;
	vector<glm::vec3> m_umbrellaBoneFrontVertices;
	vector<glm::vec3> m_umbrellaBoneBackVertices;
	vector<glm::vec3> m_umbrellaBoneLeftVertices;
	vector<glm::vec3> m_umbrellaBoneRightVertices;

	//vao
	GLuint m_vaoCoffinTop;
	GLuint m_vaoCoffinBot;
	GLuint m_vaoCoffinRight;
	GLuint m_vaoCoffinLeft;
	GLuint m_vaoCoffinFront;
	GLuint m_vaoCoffinBack;

	GLuint m_vaoUmbrellaTop;
	GLuint m_vaoUmbrellaHandle;
	GLuint m_vaoUmbrellaBoneFront;
	GLuint m_vaoUmbrellaBoneBack;
	GLuint m_vaoUmbrellaBoneLeft;
	GLuint m_vaoUmbrellaBoneRight;

	//texture
	vector<CTexture> m_textures;
	CTexture m_texture;


	GLuint m_vaoWallLeft;
	GLuint m_vaoWallFront;
	GLuint m_vaoWallRight;
	GLuint m_vaoWallTop;
	GLuint m_vaoWallBot;
	GLuint m_vaoWallBack;
	vector<glm::vec3> m_wallLeftVertices;
	vector<glm::vec3> m_wallFrontVertices;
	vector<glm::vec3> m_wallRightVertices;
	vector<glm::vec3> m_wallTopVertices;
	vector<glm::vec3> m_wallBotVertices;
	vector<glm::vec3> m_wallBackVertices;
public:
	//AObject(const string &model, CCamera *camera, float speed = 0.00015f, glm::vec3 rotateAround = glm::vec3(0.0f, 1.0f, 0.0f), float angle = 0.0f);
	AObject();
	~AObject();
	virtual void Initialise(Character character, float camPos = 880);

	//load vertex and texture
	virtual bool Load(string path);
	virtual void Update(float m_dt);

	void Create(string filename);
	virtual void Render();

	//coffin
	void SetCoffinPoints();
	void CreateCoffinOffsetPoints();
	void CreateCoffin();
	void RenderCoffin();
	
	//umbrella
	void SetUmbrellaPoints();
	void CreateUmbrellaOffsetPoints();
	void CreateUmbrella();
	void RenderUmbrella();

	void CreateWall();
	void RenderWall();


	void CreateObject(AActor::Character id, vector<glm::vec3> &vertices, GLuint &vao, glm::vec3 normal);
private:

};