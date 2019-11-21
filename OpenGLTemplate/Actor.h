#pragma once
#include "Common.h"
#include "VertexBufferObject.h"
#include "OpenAssetImportMesh.h"

class CCamera;
class AActor: public virtual COpenAssetImportMesh {
public:
	//actors id
	static enum Character {
		player, face, eyeball, doll, chinese, coffin, umbrella, tree
	};

protected:
	float m_distance;
	float m_speed;
	float m_scale;
	float m_angle;
	float m_turn;
	float m_yOffset;
	float m_laneWidth;
	float m_rot;
	float m_radius;

	bool bIsPlayer;
	bool bIsRotate;
	bool bIsFinishAnimate;
	bool bRequireCameraShake;
	bool bHasTrigger;
	bool bHasTick;
	bool bIsDynamic;
	bool bIsActive;
	bool bIsVisible;
	bool bPlaySound;


	int m_hp;

	glm::vec3 m_triggerPosition;
	glm::vec3 m_rotateAround;
	glm::vec3 m_prevPosition;
	
	float m_inLane;

	glm::mat4 m_modelViewMatrix;

	AActor::Character id;
	


public:
	AActor();
	~AActor();
	
	//initialise actor corresponded character using id, also assigned a random position to the actor that will not overlap with player
	virtual void Initialise(Character character, float camPos = 880);

	//load the model
	virtual bool Load(string path);

	//Update every frame
	virtual void Update(float m_dt);

	//Render
	virtual void Render();

	// Getter
	glm::mat4 &const GetModelViewMatrix();
	glm::vec3 & const GetRoutateAround();
	glm::vec3 & const GetTriggerPosition();
	float & const GetYOffset();
	float & const GetRadius();
	float & const GetDistance();
	float & const GetScale();
	float & const GetSpeed();
	float & const GetAngle();
	float & const GetTurn();
	float & const GetInLane();
	bool & const HasTrigger();
	bool & const IsVisible();
	bool & const IsActive();
	Character & const GetID();
	int & const GetHP();
	bool CanBeSeen(CCamera *camera) const;	//check if actor can be seen by calculate the distance between camera
	bool CanBeSeen() const;		//for face monster, if too y position is too high

	//Setter
	void SetRotateAround(glm::vec3 &rotateAround);
	void SetTriggerPosition(glm::vec3 &p);
	void SetDistance(float distance);
	void SetScale(float scale);
	void SetSpeed(float speed);
	void SetAngle(float angle);
	void SetHP();
	void SetVisible(bool b);
	void SetModelViewMatrix(glm::mat4 const & const mat , glm::mat4 const *m_project);
	void RemoveTrigger();
	void StartAnimation();
	void Active();
	void Disable();

	bool PlaySound();
};