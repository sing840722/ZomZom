/* 
OpenGL Template for INM376 / IN3005
City University London, School of Mathematics, Computer Science and Engineering
Source code drawn from a number of sources and examples, including contributions from
 - Ben Humphrey (gametutorials.com), Michal Bubner (mbsoftworks.sk), Christophe Riccio (glm.g-truc.net)
 - Christy Quinn, Sam Kellett and others

 For educational use by Department of Computer Science, City University London UK.

 This template contains a skybox, simple terrain, camera, lighting, shaders, texturing

 Potential ways to modify the code:  Add new geometry types, shaders, change the terrain, load new meshes, change the lighting, 
 different camera controls, different shaders, etc.
 
 Template version 5.0a 29/01/2017
 Dr Greg Slabaugh (gregory.slabaugh.1@city.ac.uk) 
*/


#include "game.h"

// Setup includes
#include "HighResolutionTimer.h"
#include "GameWindow.h"

// Game includes
#include "Camera.h"
#include "Skybox.h"
#include "Shaders.h"
#include "FreeTypeFont.h"
#include "Sphere.h"
#include "MatrixStack.h"
#include "OpenAssetImportMesh.h"
#include "Audio.h"
#include "CatmullRom.h"
#include "Actor.h"
#include "Object.h"
#include "Plane.h"
#include "HUD.h"
#include <string.h>
#include "FrameBufferObject.h"
#include "ActorManager.h"
#include "SceneManager.h"
#include "HUDManager.h"
#include "Light.h"
#include "LightHouse.h"

template <typename T>
void DebugLog(T value) {
	std::ostringstream ss;
	ss << value;
	std::string s(ss.str());
	OutputDebugString(s.c_str());
}

// Constructor
Game::Game()
{
	m_dt = 0.0;
	m_framesPerSecond = 0;
	m_frameCount = 0;
	m_elapsedTime = 0.0f;
	nearClippingPlane = 0.5f;
	farClippingPlane = 5000.0f;

	m_pShaderPrograms = NULL;
	m_pEdges = NULL;	//
	m_pAudio = NULL;
	m_pCamera = NULL;
	m_pSkybox = NULL;
	m_pFtFont = NULL;
	m_pCatmullRom = NULL;
	m_pPlanarTerrain = NULL;
	m_pFBO = NULL;	//
	m_pHighResolutionTimer = NULL;
	m_pActorManager = NULL;
	m_pSceneManager = NULL;
	m_pHUDManager = NULL;

	m_pHouse = NULL;
	m_pLightHouse = NULL;
	m_pLightHouse2 = NULL;


	m_pDirectionalLight = NULL;
	m_pTorch = NULL;
	m_pHouseLight = NULL;
	//m_pLight3 = NULL;
}

// Destructor
Game::~Game() 
{ 
	if (m_pShaderPrograms != NULL) {
		for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
			delete (*m_pShaderPrograms)[i];
	}
	delete m_pShaderPrograms;

	if (m_pEdges != NULL) {
		for (unsigned int i = 0; i < m_pEdges->size(); i++) {
			(*m_pEdges)[i]->ClearModelData();
			delete (*m_pEdges)[i];
		}
	}
	delete m_pEdges;

	//game objects	
	delete m_pAudio;
	delete m_pCamera;
	delete m_pSkybox;
	delete m_pFtFont;
	delete m_pCatmullRom;
	delete m_pFBO;
	delete m_pHighResolutionTimer;
	delete m_pActorManager;
	delete m_pSceneManager;
	delete m_pHUDManager;

	delete m_pPlanarTerrain;

	m_pHouse->ClearModelData();
	delete m_pHouse;

	m_pLightHouse->ClearModelData();
	delete m_pLightHouse;
	delete m_pLightHouse2;

	delete m_pDirectionalLight;
	delete m_pTorch;
	delete m_pHouseLight;
	//delete m_pLight3;
}

// Initialisation:  This method only runs once at startup
void Game::Initialise() 
{
	// Set the clear colour and depth
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f);

	//Create objects
	m_pShaderPrograms = new vector <CShaderProgram *>;

	m_pCamera = new CCamera;
	m_pSkybox = new CSkybox;

	m_pEdges = new vector<COpenAssetImportMesh*>;
	m_pCatmullRom = new CCatmullRom;

	//m_pAudio = new CAudio;
	m_pFtFont = new CFreeTypeFont;
	m_pFBO = new CFrameBufferObject;

	m_pActorManager = new CActorManager;
	m_pHUDManager = new CHUDManager;
	m_pSceneManager = new CSceneManager;

	m_pHouse = new COpenAssetImportMesh;
	m_pLightHouse = new ALightHouse;
	m_pLightHouse2 = new ALightHouse;
	m_pDirectionalLight = new ALight;
	m_pTorch = new ALight;
	m_pHouseLight = new ALight;


	RECT dimensions = m_gameWindow.GetDimensions();
	int width = dimensions.right - dimensions.left;
	int height = dimensions.bottom - dimensions.top;

	// Set the orthographic and perspective projection matrices based on the image size
	m_pCamera->SetOrthographicProjectionMatrix(width, height); 
	m_pCamera->SetPerspectiveProjectionMatrix(90.0f, (float) width / (float) height, nearClippingPlane, farClippingPlane);

	// Load shaders
	vector<CShader> shShaders;
	vector<string> sShaderFileNames;
	sShaderFileNames.push_back("mainShader.vert");
	sShaderFileNames.push_back("mainShader.frag");
	sShaderFileNames.push_back("textShader.vert");
	sShaderFileNames.push_back("textShader.frag");
	sShaderFileNames.push_back("trackShader.vert");
	sShaderFileNames.push_back("trackShader.frag");
	sShaderFileNames.push_back("hudShader.vert");
	sShaderFileNames.push_back("hudShader.frag");
	sShaderFileNames.push_back("skyboxShader.vert");
	sShaderFileNames.push_back("skyboxShader.frag");

	for (int i = 0; i < (int) sShaderFileNames.size(); i++) {
		string sExt = sShaderFileNames[i].substr((int) sShaderFileNames[i].size()-4, 4);
		int iShaderType;
		if (sExt == "vert") iShaderType = GL_VERTEX_SHADER;
		else if (sExt == "frag") iShaderType = GL_FRAGMENT_SHADER;
		else if (sExt == "geom") iShaderType = GL_GEOMETRY_SHADER;
		else if (sExt == "tcnl") iShaderType = GL_TESS_CONTROL_SHADER;
		else iShaderType = GL_TESS_EVALUATION_SHADER;
		CShader shader;
		shader.LoadShader("resources\\shaders\\"+sShaderFileNames[i], iShaderType);
		shShaders.push_back(shader);
	}

	// Create the main shader program
	CShaderProgram *pMainProgram = new CShaderProgram;
	pMainProgram->CreateProgram();
	pMainProgram->AddShaderToProgram(&shShaders[0]);
	pMainProgram->AddShaderToProgram(&shShaders[1]);
	pMainProgram->LinkProgram();
	m_pShaderPrograms->push_back(pMainProgram);

	// Create a shader program for fonts
	CShaderProgram *pFontProgram = new CShaderProgram;
	pFontProgram->CreateProgram();
	pFontProgram->AddShaderToProgram(&shShaders[2]);
	pFontProgram->AddShaderToProgram(&shShaders[3]);
	pFontProgram->LinkProgram();
	m_pShaderPrograms->push_back(pFontProgram);

	// Create a shader program for wall
	CShaderProgram *ptrackPrgram = new CShaderProgram;
	ptrackPrgram->CreateProgram();
	ptrackPrgram->AddShaderToProgram(&shShaders[4]);
	ptrackPrgram->AddShaderToProgram(&shShaders[5]);
	ptrackPrgram->LinkProgram();
	m_pShaderPrograms->push_back(ptrackPrgram);

	// Create a shader program for wall
	CShaderProgram *pHUDProgram = new CShaderProgram;
	pHUDProgram->CreateProgram();
	pHUDProgram->AddShaderToProgram(&shShaders[6]);
	pHUDProgram->AddShaderToProgram(&shShaders[7]);
	pHUDProgram->LinkProgram();
	m_pShaderPrograms->push_back(pHUDProgram);

	// Create a shader program for skybox
	CShaderProgram *pSkyboxProgram = new CShaderProgram;
	pSkyboxProgram->CreateProgram();
	pSkyboxProgram->AddShaderToProgram(&shShaders[8]);
	pSkyboxProgram->AddShaderToProgram(&shShaders[9]);
	pSkyboxProgram->LinkProgram();
	m_pShaderPrograms->push_back(pSkyboxProgram);


	// Create the skybox
	m_pSkybox->Create(10);
	m_pFtFont->LoadSystemFont("arial.ttf", 32);
	m_pFtFont->SetShaderProgram(pFontProgram);

	// Create the planar terrain
	m_pPlanarTerrain = new CPlane;
	m_pPlanarTerrain->Create("resources\\textures\\", "stand.jpg", 300.0f, 300.0f, 25.f);

	COpenAssetImportMesh *m_pEdge = new COpenAssetImportMesh;
	m_pEdge->Load("resources\\models\\TryPlane\\trygg.obj");
	(*m_pEdges).push_back(m_pEdge);

	m_pHouse->Load("resources\\models\\House\\house.obj");
	m_pLightHouse->Load("resources\\models\\LightHouse\\Lighthouse.obj");

	//Create Path
	m_pCatmullRom->Create("resources\\textures\\bridge.png");	//track, left track, right track, track top [0]
	m_pCatmullRom->CreateCentreline();
	m_pCatmullRom->CreateOffsetCurves();
	m_pCatmullRom->CreateTrack();

	m_pAudio = new CAudio;
	m_pAudio->Initialise();
	m_pAudio->SetCamera(this->m_pCamera);
	m_pAudio->SetMonster(m_pActorManager->GetPlayer());
	m_pActorManager->SetAudio(this->m_pAudio);
	
	m_pSceneManager->SetAudio(this->m_pAudio);
	m_pSceneManager->Initialise();
	m_pHUDManager->Initialise();
	m_pActorManager->Initialise();
	m_pActorManager->SetCatmullRom(m_pCatmullRom);
	m_pActorManager->SetCamera(m_pCamera);
	m_pActorManager->SetHUDManager(m_pHUDManager);
	m_pActorManager->SetSceneManager(m_pSceneManager);

	m_pFBO->Create(width, height);


	glm::vec3 lhp = m_pLightHouse->GetPosition();
	lhp.x *= -1;
	lhp.z *= -1;
	m_pLightHouse2->SetPosition(lhp);
	m_pLightHouse2->Initialise();

	m_pSceneManager->GotoMenu();
	//m_pSceneManager->GotoGame();




	wall = new AObject;
	wall->CreateWall();

}

#pragma region Rendering
void Game::Render3D() {
	// Clear the buffers and enable depth testing (z-buffering)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Set up a matrix stack
	glutil::MatrixStack modelViewMatrixStack;
	modelViewMatrixStack.SetIdentity();
	modelViewMatrixStack.LookAt(m_pCamera->GetPosition(), m_pCamera->GetView(), m_pCamera->GetUpVector());

	// Use the main shader program 
	CShaderProgram *pMainProgram = (*m_pShaderPrograms)[0];
	pMainProgram->UseProgram();
	pMainProgram->SetUniform("bUseTexture", true);
	pMainProgram->SetUniform("sampler0", 0);
	// Note: cubemap and non-cubemap textures should not be mixed in the same texture unit.  Setting unit 10 to be a cubemap texture.
	int cubeMapTextureUnit = 10;
	pMainProgram->SetUniform("CubeMapTex", cubeMapTextureUnit);

	// Set the projection matrix
	pMainProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
	glm::mat4 viewMatrix = modelViewMatrixStack.Top();
	glm::mat3 viewNormalMatrix = m_pCamera->ComputeNormalMatrix(viewMatrix);
	m_pCamera->SetModelViewMatrix(viewMatrix);


	// Set light and materials in main shader program
	m_pDirectionalLight->SetDirection(glm::normalize(viewNormalMatrix * glm::vec3(0, -1, 0)));
	//m_pDirectionalLight->SetLa(glm::vec3(1));		//remove later
	m_pDirectionalLight->SetLd(glm::vec3(0, 0.0, 0));
	m_pDirectionalLight->SetLs(glm::vec3(0, 0.0, 0));

	pMainProgram->SetUniform("light1.position", viewMatrix*m_pDirectionalLight->Position()); // Position of light source *in eye coordinates*
	pMainProgram->SetUniform("light1.La", m_pDirectionalLight->La());		// Ambient colour of light
	pMainProgram->SetUniform("light1.Ld", m_pDirectionalLight->Ld());		// Diffuse colour of light
	pMainProgram->SetUniform("light1.Ls", m_pDirectionalLight->Ls());		// Specular colour of light
	pMainProgram->SetUniform("light1.direction", m_pDirectionalLight->Direction());
	pMainProgram->SetUniform("light1.attenuation", m_pDirectionalLight->Attenuation());
	
	pMainProgram->SetUniform("material1.Ma", glm::vec3(1.0f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance
	pMainProgram->SetUniform("material1.shininess", 15.0f);		// Shininess material property

	modelViewMatrixStack.Push(); {
		pMainProgram->SetUniform("renderSkybox", true);
		glm::vec3 vEye = m_pCamera->GetPosition();
		modelViewMatrixStack.Translate(vEye);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pSkybox->Render(cubeMapTextureUnit);
		pMainProgram->SetUniform("renderSkybox", false);
	}
	modelViewMatrixStack.Pop();

	// Turn on diffuse + specular materials
	pMainProgram->SetUniform("material1.Ma", glm::vec3(0.5f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.5f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(1.0f));	// Specular material reflectance	



	//Torch
	glm::vec3 pPos = m_pActorManager->GetPlayer()->GetPosition();
	glm::mat3 m = glm::mat3(m_pActorManager->GetPlayer()->GetOrientationMatrix());
	glm::vec3 tPos = glm::vec3(m[0][0], m[0][1], m[0][2]);
	glm::vec3 dir = tPos;
	m_pTorch->SetPosition(pPos);
	m_pTorch->SetDirection(glm::normalize(viewNormalMatrix * dir));


	//Light House
	m_pLightHouse->SetLightDirection(viewNormalMatrix);
	m_pLightHouse2->SetLightDirection(viewNormalMatrix);


	m_pHouseLight->SetLa(glm::vec3(0));
	m_pHouseLight->SetLd(glm::vec3(0));
	m_pHouseLight->SetLs(glm::vec3(0));

	{
		pMainProgram->SetUniform("light1.La", m_pDirectionalLight->La());
		pMainProgram->SetUniform("light1.Ld", m_pDirectionalLight->Ld());
		pMainProgram->SetUniform("light1.Ls", m_pDirectionalLight->Ls());
		pMainProgram->SetUniform("light1.exponent", m_pDirectionalLight->Exponent());
		pMainProgram->SetUniform("light1.cutoff", m_pDirectionalLight->Cutoff());

		pMainProgram->SetUniform("light2.position", viewMatrix* m_pTorch->Position());
		pMainProgram->SetUniform("light2.La", m_pTorch->La());
		pMainProgram->SetUniform("light2.Ld", m_pTorch->Ld());
		pMainProgram->SetUniform("light2.Ls", m_pTorch->Ls());
		pMainProgram->SetUniform("light2.direction", m_pTorch->Direction());
		pMainProgram->SetUniform("light2.exponent", m_pTorch->Exponent());
		pMainProgram->SetUniform("light2.cutoff", m_pTorch->Cutoff());
		pMainProgram->SetUniform("light2.attenuation", m_pTorch->Attenuation());

		pMainProgram->SetUniform("light3.position", viewMatrix* m_pLightHouse->GetLight()->Position());
		pMainProgram->SetUniform("light3.La", m_pLightHouse->GetLight()->La());
		pMainProgram->SetUniform("light3.Ld", m_pLightHouse->GetLight()->Ld());
		pMainProgram->SetUniform("light3.Ls", m_pLightHouse->GetLight()->Ls());
		pMainProgram->SetUniform("light3.direction", m_pLightHouse->GetLight()->Direction());
		pMainProgram->SetUniform("light3.exponent", m_pLightHouse->GetLight()->Exponent());
		pMainProgram->SetUniform("light3.cutoff", m_pLightHouse->GetLight()->Cutoff());
		pMainProgram->SetUniform("light3.attenuation", m_pLightHouse->GetLight()->Attenuation());

		pMainProgram->SetUniform("light4.position", viewMatrix* m_pHouseLight->Position());
		pMainProgram->SetUniform("light4.La", m_pHouseLight->La());
		pMainProgram->SetUniform("light4.Ld", m_pHouseLight->Ld());
		pMainProgram->SetUniform("light4.Ls", m_pHouseLight->Ls());
		pMainProgram->SetUniform("light4.direction", m_pHouseLight->Direction());
		pMainProgram->SetUniform("light4.exponent", m_pHouseLight->Exponent());
		pMainProgram->SetUniform("light4.cutoff", m_pHouseLight->Cutoff());
		pMainProgram->SetUniform("light4.attenuation", m_pHouseLight->Attenuation());

		pMainProgram->SetUniform("light5.position", viewMatrix* m_pLightHouse2->GetLight()->Position());
		pMainProgram->SetUniform("light5.La", m_pLightHouse2->GetLight()->La());
		pMainProgram->SetUniform("light5.Ld", m_pLightHouse2->GetLight()->Ld());
		pMainProgram->SetUniform("light5.Ls", m_pLightHouse2->GetLight()->Ls());
		pMainProgram->SetUniform("light5.direction", m_pLightHouse2->GetLight()->Direction());
		pMainProgram->SetUniform("light5.exponent", m_pLightHouse2->GetLight()->Exponent());
		pMainProgram->SetUniform("light5.cutoff", m_pLightHouse2->GetLight()->Cutoff());
		pMainProgram->SetUniform("light5.attenuation", m_pLightHouse2->GetLight()->Attenuation());
	}



	glDisable(GL_CULL_FACE);
	// Render the Actors///
	//LightHouse
	modelViewMatrixStack.Push(); {
		modelViewMatrixStack.Translate(m_pLightHouse->GetPosition());
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pLightHouse->Render();
	}
	modelViewMatrixStack.Pop();


	//light house 2
	modelViewMatrixStack.Push(); {
		modelViewMatrixStack.Translate(m_pLightHouse2->GetPosition());
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pLightHouse->Render();
	}
	modelViewMatrixStack.Pop();

	//House
	modelViewMatrixStack.Push(); {
		modelViewMatrixStack.Translate(glm::vec3(-60.624828, 0, 25));
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pHouse->Render();
	}
	modelViewMatrixStack.Pop();


	
	for each (AActor* actor in (m_pActorManager->GetActors()))
	{
		AActor::Character id = actor->GetID();
		AActor* model;

		switch (id)
		{
		case AActor::player:
			model = m_pActorManager->GetPlayerModel();
			break;
		case AActor::face:
			model = m_pActorManager->GetFaceMonsterModel();
			break;
		case AActor::eyeball:
			model = m_pActorManager->GetEyeBallMonsterModel();
			break;
		case AActor::doll:
			model = m_pActorManager->GetDollMonsterModel();
			break;
		case AActor::chinese:
			model = m_pActorManager->GetChineseMonsterModel();
			break;
		case AActor::coffin:
			model = m_pActorManager->GetCoffinMonsterModel();
			break;
		case AActor::umbrella:
			model = m_pActorManager->GetUmbrellaMonsterModel();
			break;
		case AActor::tree:
			model = m_pActorManager->GetTreeModel();
			break;
		default:
			continue;
			break;
		}

		if (actor->IsActive() && actor->IsVisible()) {
			modelViewMatrixStack.Push();
			{
				modelViewMatrixStack.Translate(actor->GetPosition());
				modelViewMatrixStack.Scale(actor->GetScale());
				if (actor->GetID() != AActor::coffin) {
					modelViewMatrixStack.Rotate(actor->GetRoutateAround(), actor->GetAngle());
				}
				modelViewMatrixStack *= actor->GetOrientationMatrix();

				if (actor->GetID() == AActor::coffin) {			//only for coffin animation
					if (!actor->HasTrigger()) {
						modelViewMatrixStack.Translate(glm::vec3(1.75, 0, 0));
						modelViewMatrixStack.Rotate(glm::vec3(0, 0, 1), actor->GetAngle());
						modelViewMatrixStack.Translate(glm::vec3(-1.75, 0, 0));
					}
					modelViewMatrixStack.Rotate(glm::vec3(0, 1, 0), 90);
				}

				pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
				pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
				model->Render();
			}
			modelViewMatrixStack.Pop();
		}
	}


	// Change to Track shader program
	CShaderProgram *ptrackProgram = (*m_pShaderPrograms)[2];
	ptrackProgram->UseProgram();
	ptrackProgram->SetUniform("sample0", 0);
	ptrackProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
	ptrackProgram->SetUniform("material1.Ma", glm::vec3(0.5f));	// Ambient material reflectance
	ptrackProgram->SetUniform("material1.Md", glm::vec3(0.5f));	// Diffuse material reflectance
	ptrackProgram->SetUniform("material1.Ms", glm::vec3(1.0f));	// Specular material reflectance

	ptrackProgram->SetUniform("light1.La", m_pDirectionalLight->La());		// Ambient colour of light
	ptrackProgram->SetUniform("light1.Ld", m_pDirectionalLight->Ld());		// Diffuse colour of light
	ptrackProgram->SetUniform("light1.Ls", m_pDirectionalLight->Ls());		// Specular colour of light
	ptrackProgram->SetUniform("light1.direction", m_pDirectionalLight->Direction());
	ptrackProgram->SetUniform("light1.attenuation", m_pDirectionalLight->Attenuation());


	ptrackProgram->SetUniform("light2.position", viewMatrix* m_pTorch->Position());
	ptrackProgram->SetUniform("light2.La", m_pTorch->La());		// Ambient colour of light
	ptrackProgram->SetUniform("light2.Ld", m_pTorch->Ld());		// Diffuse colour of light
	ptrackProgram->SetUniform("light2.Ls", m_pTorch->Ls());		// Specular colour of light
	ptrackProgram->SetUniform("light2.direction", m_pTorch->Direction());
	ptrackProgram->SetUniform("light2.exponent", m_pTorch->Exponent());
	ptrackProgram->SetUniform("light2.cutoff", m_pTorch->Cutoff());
	ptrackProgram->SetUniform("light2.attenuation", m_pTorch->Attenuation());


	ptrackProgram->SetUniform("light3.position", viewMatrix* m_pLightHouse->GetLight()->Position());
	ptrackProgram->SetUniform("light3.La", m_pLightHouse->GetLight()->La());
	ptrackProgram->SetUniform("light3.Ld", m_pLightHouse->GetLight()->Ld());
	ptrackProgram->SetUniform("light3.Ls", m_pLightHouse->GetLight()->Ls());
	ptrackProgram->SetUniform("light3.direction", m_pLightHouse->GetLight()->Direction());
	ptrackProgram->SetUniform("light3.exponent", m_pLightHouse->GetLight()->Exponent());
	ptrackProgram->SetUniform("light3.cutoff", m_pLightHouse->GetLight()->Cutoff());
	ptrackProgram->SetUniform("light3.attenuation", m_pLightHouse->GetLight()->Attenuation());


	ptrackProgram->SetUniform("light4.position", viewMatrix* m_pHouseLight->Position());
	ptrackProgram->SetUniform("light4.La", m_pHouseLight->La());
	ptrackProgram->SetUniform("light4.Ld", m_pHouseLight->Ld());
	ptrackProgram->SetUniform("light4.Ls", m_pHouseLight->Ls());
	ptrackProgram->SetUniform("light4.direction", m_pHouseLight->Direction());
	ptrackProgram->SetUniform("light4.exponent", m_pHouseLight->Exponent());
	ptrackProgram->SetUniform("light4.cutoff", m_pHouseLight->Cutoff());
	ptrackProgram->SetUniform("light4.attenuation", m_pHouseLight->Attenuation());

	ptrackProgram->SetUniform("light5.position", viewMatrix* m_pLightHouse2->GetLight()->Position());
	ptrackProgram->SetUniform("light5.La", m_pLightHouse2->GetLight()->La());
	ptrackProgram->SetUniform("light5.Ld", m_pLightHouse2->GetLight()->Ld());
	ptrackProgram->SetUniform("light5.Ls", m_pLightHouse2->GetLight()->Ls());
	ptrackProgram->SetUniform("light5.direction", m_pLightHouse2->GetLight()->Direction());
	ptrackProgram->SetUniform("light5.exponent", m_pLightHouse2->GetLight()->Exponent());
	ptrackProgram->SetUniform("light5.cutoff", m_pLightHouse2->GetLight()->Cutoff());
	ptrackProgram->SetUniform("light5.attenuation", m_pLightHouse2->GetLight()->Attenuation());


	//Plane
	modelViewMatrixStack.Push(); {
		modelViewMatrixStack.Translate(glm::vec3(0.0f, -0.001f, 0.0f));
		ptrackProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		ptrackProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pPlanarTerrain->Render();
	}
	modelViewMatrixStack.Pop();

	//Edge
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (int i = 0; i < 4; i++) {
		glm::vec3 position;
		float angle;

		switch (i)
		{
		case 0:
			position = glm::vec3(150, -3, 0);
			angle = 90;
			break;
		case 1:
			position = glm::vec3(-150, -3, 0);
			angle = 90;
			break;
		case 2:
			position = glm::vec3(0, -3, 150);
			angle = 0;
			break;
		case 3:
			position = glm::vec3(0, -3, -150);
			angle = 0;
			break;
		default:
			break;
		}

		modelViewMatrixStack.Push(); {
			modelViewMatrixStack.Translate(position);
			modelViewMatrixStack.Rotate(glm::vec3(0, 1, 0), angle);
			modelViewMatrixStack.Scale(15);
			ptrackProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			ptrackProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			(*m_pEdges)[0]->Render();
		}
		modelViewMatrixStack.Pop();
	}

	modelViewMatrixStack.Push(); {
		modelViewMatrixStack.Translate(glm::vec3(-60.624828, 0, 25) - (glm::vec3(0, 0, 1) * 10.0f));
		modelViewMatrixStack.Scale(1);
		ptrackProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		ptrackProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		//wall->RenderWall();
		
	}
	modelViewMatrixStack.Pop();

	modelViewMatrixStack.Push(); {
		modelViewMatrixStack.Translate(glm::vec3(0.0f, 0.0f, 0.0f));
		ptrackProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		ptrackProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pCatmullRom->Render();
	}
	modelViewMatrixStack.Pop();
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}

//Render level
void Game::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutil::MatrixStack modelViewMatrixStack;
	modelViewMatrixStack.SetIdentity();

	CShaderProgram *pHUDProgram = (*m_pShaderPrograms)[3];
	pHUDProgram->UseProgram();
	pHUDProgram->SetUniform("sampler0", 0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Render the FBO of the 3D Scene
	modelViewMatrixStack.Push(); {
		m_pFBO->BindTexture(0);
		modelViewMatrixStack.Rotate(glm::vec3(1, 0, 0), 180.f);
		pHUDProgram->SetUniform("mouseX", m_pCamera->GetMousePosition().x);
		pHUDProgram->SetUniform("mouseY", m_pCamera->GetMousePosition().y);
		pHUDProgram->SetUniform("transformationMatrix", modelViewMatrixStack.Top());
		m_pHUDManager->GetQuad()->Render();
	}
	modelViewMatrixStack.Pop();


	for each (AHUD* widget in *m_pHUDManager->GetInGameHUD())
	{
		if (widget->IsActive()) {
			modelViewMatrixStack.Push(); {
				widget->BindTexture();
				pHUDProgram->SetUniform("opacity", widget->GetOpacity());
				modelViewMatrixStack.Translate(glm::vec3(widget->GetPosition(), 0));
				modelViewMatrixStack.Rotate(glm::vec3(1, 0, 0), 180.f);
				modelViewMatrixStack.Scale(glm::vec3(widget->GetScale(), 0));
				pHUDProgram->SetUniform("transformationMatrix", modelViewMatrixStack.Top());
				m_pHUDManager->GetQuad()->Render();
			}
			modelViewMatrixStack.Pop();
		}
	}

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	DisplayFrameRate();

	SwapBuffers(m_gameWindow.Hdc());
}


void Game::RenderMenu()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutil::MatrixStack modelViewMatrixStack;
	modelViewMatrixStack.SetIdentity();

	CShaderProgram *pHUDProgram = (*m_pShaderPrograms)[3];
	pHUDProgram->UseProgram();
	pHUDProgram->SetUniform("sampler0", 0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	modelViewMatrixStack.Push(); {
		m_pHUDManager->GetMenu()->BindTexture();
		pHUDProgram->SetUniform("opacity", m_pHUDManager->GetMenu()->GetOpacity());
		modelViewMatrixStack.Translate(glm::vec3(m_pHUDManager->GetMenu()->GetPosition(), 0));
		modelViewMatrixStack.Rotate(glm::vec3(1, 0, 0), 180.f);
		modelViewMatrixStack.Scale(glm::vec3(m_pHUDManager->GetMenu()->GetScale(), 0));
		pHUDProgram->SetUniform("transformationMatrix", modelViewMatrixStack.Top());
		m_pHUDManager->GetQuad()->Render();
	}
	modelViewMatrixStack.Pop();


	modelViewMatrixStack.Push(); {
		m_pHUDManager->GetMouseCursor()->BindTexture();
		pHUDProgram->SetUniform("opacity", m_pHUDManager->GetMouseCursor()->GetOpacity());
		modelViewMatrixStack.Translate(glm::vec3(m_pHUDManager->GetMouseCursor()->GetPosition(), 0));
		modelViewMatrixStack.Rotate(glm::vec3(1, 0, 0), 180.f);
		modelViewMatrixStack.Scale(glm::vec3(m_pHUDManager->GetMouseCursor()->GetScale(), 0));
		pHUDProgram->SetUniform("transformationMatrix", modelViewMatrixStack.Top());
		m_pHUDManager->GetQuad()->Render();
	}
	modelViewMatrixStack.Pop();


	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	DisplayFrameRate();

	SwapBuffers(m_gameWindow.Hdc());

}

void Game::RenderEndScreen()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set up a matrix stack
	glutil::MatrixStack modelViewMatrixStack;
	modelViewMatrixStack.SetIdentity();

	// Draw the 2D graphics after the 3D graphics
	CShaderProgram *pHUDProgram = (*m_pShaderPrograms)[3];
	pHUDProgram->UseProgram();
	pHUDProgram->SetUniform("sampler0", 0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	modelViewMatrixStack.Push(); {
		m_pHUDManager->GetEndScreen()->BindTexture();
		pHUDProgram->SetUniform("opacity", m_pHUDManager->GetEndScreen()->GetOpacity());
		modelViewMatrixStack.Translate(glm::vec3(m_pHUDManager->GetEndScreen()->GetPosition(), 0));
		modelViewMatrixStack.Rotate(glm::vec3(1, 0, 0), 180.f);
		modelViewMatrixStack.Scale(glm::vec3(m_pHUDManager->GetEndScreen()->GetScale(), 0));
		pHUDProgram->SetUniform("transformationMatrix", modelViewMatrixStack.Top());
		m_pHUDManager->GetQuad()->Render();
	}
	modelViewMatrixStack.Pop();


	modelViewMatrixStack.Push(); {
		m_pHUDManager->GetMouseCursor()->BindTexture();
		pHUDProgram->SetUniform("opacity", m_pHUDManager->GetMouseCursor()->GetOpacity());
		modelViewMatrixStack.Translate(glm::vec3(m_pHUDManager->GetMouseCursor()->GetPosition(), 0));
		modelViewMatrixStack.Rotate(glm::vec3(1, 0, 0), 180.f);
		modelViewMatrixStack.Scale(glm::vec3(m_pHUDManager->GetMouseCursor()->GetScale(), 0));
		pHUDProgram->SetUniform("transformationMatrix", modelViewMatrixStack.Top());
		m_pHUDManager->GetQuad()->Render();
	}
	modelViewMatrixStack.Pop();


	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	SwapBuffers(m_gameWindow.Hdc());
}
#pragma endregion


#pragma region Camera
void Game::SwitchCameraMode() {
	RECT dimensions = m_gameWindow.GetDimensions();
	int width = dimensions.right - dimensions.left;
	int height = dimensions.bottom - dimensions.top;

	m_pTorch->SetLa(glm::vec3(0));
	m_pTorch->SetLd(glm::vec3(0.7));
	m_pTorch->SetLs(glm::vec3(0.7));
	m_pTorch->SetCutoff(18);
	m_pTorch->SetExponent(500);


	if (m_pCamera->UsingScope()) {
		m_pCamera->SetPerspectiveProjectionMatrix(45.0f - 15, (float)width / (float)height, nearClippingPlane, farClippingPlane);
		m_pActorManager->GetPlayer()->Disable();
		m_pHUDManager->GetMouseCursor()->Active();

		m_pDirectionalLight->SetLa(glm::vec3(0.0, 0.7, 0.0));

		
		m_pTorch->SetLa(glm::vec3(0.0,0.0,0.0));
		m_pTorch->SetLd(glm::vec3(0.0,0.7,0));
		m_pTorch->SetLs(glm::vec3(0.0,0.7,0));
		

		/*	no light (take photo)
		m_pTorch->SetLa(glm::vec3(0));
		m_pTorch->SetLd(glm::vec3(0));
		m_pTorch->SetLs(glm::vec3(0));
		*/
		m_pTorch->SetCutoff(360);
		m_pTorch->SetExponent(0);
		m_pTorch->SetAttenuation(glm::vec3(1, 0, 0));
	}
	else {
		m_pCamera->SetPerspectiveProjectionMatrix(80.0f, (float)width / (float)height, nearClippingPlane, farClippingPlane);
		m_pActorManager->GetPlayer()->Active();
		m_pHUDManager->GetMouseCursor()->Disable();

		m_pDirectionalLight->SetLa(glm::vec3(0.3));

		
		m_pTorch->SetLa(glm::vec3(0));
		m_pTorch->SetLd(glm::vec3(0.7));
		m_pTorch->SetLs(glm::vec3(1));
		
		/*	no light (take photo)
		m_pTorch->SetLa(glm::vec3(0));
		m_pTorch->SetLd(glm::vec3(0));
		m_pTorch->SetLs(glm::vec3(0));
		*/

		m_pTorch->SetCutoff(30);
		m_pTorch->SetExponent(50);
		m_pTorch->SetAttenuation(glm::vec3(1, 1.0, 0));
	}
}


void Game::MoveCamera(CCamera* camera) {
	float distance = camera->GetDistance();

	glm::vec3 pNext;	//pNext - p = tegent = forward vector of p
	glm::vec3 p;

	vector<glm::vec3> path = m_pCatmullRom->GetControlPoints();
	m_pCatmullRom->Sample(distance, p, path);
	m_pCatmullRom->Sample(distance + 1, pNext, path);

	glm::vec3 y = glm::vec3(0, 1, 0);
	glm::vec3 T = glm::normalize(pNext - p);
	glm::vec3 N = glm::normalize(glm::cross(T, y));
	glm::vec3 B = glm::normalize(glm::cross(N, T));

	p.y += 3.5f;
	glm::vec3 d_pos = p - camera->GetPosition();
	camera->SetOrientedMatrix(glm::mat4(glm::mat3(T, B, N)));

	glm::vec3 newView((camera->GetView() + d_pos));
	
	if(!camera->UsingScope())
	newView = p + T;

	camera->Set(p, newView, glm::vec3(0, 1, 0));
}


void Game::RayCasting() {
	if (m_pCamera->GetAmmo() < 1) {
		return;
	}

	if (m_pCamera->IsLeftMouseClicked()) {
		m_pAudio->PlayGunShotSoundEffect();

		if (m_pActorManager->TestRayCasting()) {
			//m_pAudio->PlayKilledSoundEffect();
			return;
		}
		return;
	}
}

void Game::UpdateCamera() {
	
	//MoveCamera(m_pCamera);

	if (!(m_pCamera->GetCameraMode())) {
		MoveCamera(m_pCamera);
		m_pActorManager->GetPlayer()->SetVisible(true);
	}
	else {
		m_pActorManager->GetPlayer()->SetVisible(false);
	}
	
	m_pCamera->Update(m_dt);
	RayCasting();
	SwitchCameraMode();
}
#pragma endregion



// Update method runs repeatedly with the Render method
void Game::Update()
{
	// Update the camera using the amount of time that has elapsed to avoid framerate dependent motion
	UpdateCamera();
	m_pActorManager->Update(m_dt);
	m_pHUDManager->Update(m_dt);
	m_pAudio->Update(m_dt);
}

void Game::DisplayFrameRate()
{
	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;

	// Increase the elapsed time and frame counter
	m_elapsedTime += m_dt;
	m_frameCount++;

	// Now we want to subtract the current time by the last time that was stored
	// to see if the time elapsed has been over a second, which means we found our FPS.
	if (m_elapsedTime > 1000)
    {
		m_elapsedTime = 0;
		m_framesPerSecond = m_frameCount;

		// Reset the frames per second
		m_frameCount = 0;
    }

	if (m_framesPerSecond > 0) {
		// Use the font shader program and render the text
		fontProgram->UseProgram();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_pFtFont->Render(20, height - 20, 20, "FPS: %d", m_framesPerSecond);
		glEnable(GL_DEPTH_TEST);
	}
}

// The game loop runs repeatedly until game over
void Game::GameLoop()
{
	// Variable timer
	m_pHighResolutionTimer->Start();
	
	CSceneManager::State s = m_pSceneManager->GetState();
	switch (s)
	{
	case CSceneManager::inMenu:
		m_pSceneManager->UpdateMenu();
		RenderMenu();
		break;
	case CSceneManager::inGame:
		Update();
		m_pFBO->Bind();
		Render3D();
		m_pFBO->Unbind();
		Render();
		break;
	case CSceneManager::inEndSreen:
		m_pSceneManager->UpdateEndScreen();
		RenderEndScreen();
		break;
	default:
		break;
	}
	

	m_dt = m_pHighResolutionTimer->Elapsed();
}

WPARAM Game::Execute() 
{
	m_pHighResolutionTimer = new CHighResolutionTimer;
	m_gameWindow.Init(m_hInstance);

	if(!m_gameWindow.Hdc()) {
		return 1;
	}

	Initialise();

	m_pHighResolutionTimer->Start();

	
	MSG msg;

	while(1) {													
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
			if(msg.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&msg);	
			DispatchMessage(&msg);
		} else if (m_appActive) {
			GameLoop();
		} 
		else Sleep(200); // Do not consume processor power if application isn't active
	}

	m_gameWindow.Deinit();

	return(msg.wParam);
}

LRESULT Game::ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param) 
{
	LRESULT result = 0;

	switch (message) {


	case WM_ACTIVATE:
	{
		switch(LOWORD(w_param))
		{
			case WA_ACTIVE:
			case WA_CLICKACTIVE:
				m_appActive = true;
				m_pHighResolutionTimer->Start();
				break;
			case WA_INACTIVE:
				m_appActive = false;
				break;
		}
		break;
		}

	case WM_SIZE:
			RECT dimensions;
			GetClientRect(window, &dimensions);
			m_gameWindow.SetDimensions(dimensions);
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(window, &ps);
		EndPaint(window, &ps);
		break;

	case WM_KEYDOWN:
		switch(w_param) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case '1':
			//m_pAudio->PlayEventSound();
			break;
		case VK_F1:
			//m_pAudio->PlayEventSound();
			break;
		case VK_ADD:
			//m_pAudio->AdjustFilter();

			if (m_pAudio->IncreaseVolume()) {}
				//DebugLog("increasing \n");
			break;
		case VK_SUBTRACT:
			//m_pAudio->TurnFilter();
			if (m_pAudio->DecreaseVolume()) {}
				//DebugLog("decreasing \n");
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		result = DefWindowProc(window, message, w_param, l_param);
		break;
	}

	return result;
}

Game& Game::GetInstance() 
{
	static Game instance;

	return instance;
}

void Game::SetHinstance(HINSTANCE hinstance) 
{
	m_hInstance = hinstance;
}

LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	return Game::GetInstance().ProcessEvents(window, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int) 
{
	Game &game = Game::GetInstance();
	game.SetHinstance(hinstance);

	return game.Execute();
}
