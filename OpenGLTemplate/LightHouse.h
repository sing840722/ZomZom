#pragma once
#include "Common.h"
#include "OpenAssetImportMesh.h"

class ALight;
class ALightHouse : public COpenAssetImportMesh{
private:
	ALight *m_pLight;
	
public:
	ALightHouse();
	~ALightHouse();

	void Initialise();
	ALight* GetLight();

	//set light direction to follow a target
	void SetLightDirection(glm::mat3 vNormalMat, glm::vec3 targetLocation);

	//set light direction following a pattern
	void SetLightDirection(glm::mat3 vNormalMat);

};