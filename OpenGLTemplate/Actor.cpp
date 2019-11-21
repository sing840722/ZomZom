#include "Actor.h"
#include "Camera.h"
#include <string.h>


template <typename T>
void DebugLog(T value) {
	std::ostringstream ss;
	ss << value;
	std::string s(ss.str());
	OutputDebugString(s.c_str());
}

//Random lane (left/ middle/ right)
void RandomHelper(int seed, float &lane, float boundary) {
	srand(seed);
	if (rand() % 2 == 0) {
		lane = -boundary;
	}
	else if (rand() % 2 == 1) {
		lane = boundary;
	}
	else {
		lane = boundary;
	}
}

//Random distance
void RandomDistanceHelper(float &distance, int counter, int id, float lane) {
	int max = 750;
	if (id == AActor::Character::tree) {
		max = 850;
	}
	srand(lane + counter + id + distance);
	distance = rand() % max;
}

AActor::AActor(){
}

AActor::~AActor() {

}

// load model 
bool AActor::Load(string model) {
	COpenAssetImportMesh::Load(model);
	return true;
}

void AActor::Initialise(Character character, float camPos) {

	static int counter;
	counter++;

	/////////////////////////////////////////// Default setting if no explicity define
	m_turn = 0.0f;
	m_laneWidth = 2.5f;
	m_scale = 1.0f;
	m_rot = 0.0f;
	m_yOffset = 0.0f;

	m_position = glm::vec3(0, 0, 0);
	m_triggerPosition = glm::vec3(0.0f, 0.0f, 0.0f);

	bIsFinishAnimate = false;
	bRequireCameraShake = false;
	bHasTrigger = false;
	bHasTick = true;
	bIsDynamic = true;
	bIsActive = true;
	bIsVisible = true;
	m_speed = 0.0f;
	m_radius = 2.5;

	m_rotateAround = glm::vec3(0, 1, 0);
	m_angle = 0;

	m_hp = 1;

	///////////////////////////////////////////////////////////
	switch (character)
	{
	case AActor::player:
		id = player;
		m_speed = 0.0075f;
		m_distance = 0;
		//m_matrix = glm::mat4(1);
		break;
	case AActor::face:
		id = face;
		m_speed = 0.0f;
		m_yOffset = 30.f;
		bIsFinishAnimate = true;
		bHasTrigger = true;
		m_radius = 5.25;
		m_hp = 2;
		break;
	case AActor::eyeball:
		id = eyeball;
		m_angle = 0.0f;
		m_speed = 0.0025f;
		m_yOffset = 0.25;
		m_laneWidth = 1.5;
		m_radius = 1.25;
		break;
	case AActor::doll:
		id = doll;
		m_speed = 0.00015f;
		m_yOffset = 2.35f;
		m_radius = 2.35;
		m_hp = 2;
		break;
	case AActor::chinese:
		id = chinese;
		m_speed = 0.0005f;
		m_radius = 2.75;
		m_laneWidth = 0;
		m_yOffset = 2.5;
		break;
	case AActor::tree:
		id = tree;
		bIsActive = false;
		bHasTrigger = false;
		m_speed = 0;
		m_yOffset = 0;
		m_laneWidth = 4.5;
		if (rand() % 2 == 0) {
			m_laneWidth = 9.5;
		}
		break;
	default:
		break;
	}


	srand(counter);
	if (rand() % 2 == 0) {
		m_inLane = -m_laneWidth;
	}
	else if (rand() % 2 == 1) {
		m_inLane = m_laneWidth;
	}
	else {
		m_inLane = m_laneWidth;
	}


	do {
		RandomDistanceHelper(m_distance, counter, id, m_inLane);
		RandomDistanceHelper(m_distance, counter, id, m_inLane);
		RandomDistanceHelper(m_distance, counter, id, m_inLane);
		if (m_distance <= 30)
			m_distance + 120.1;
	} while ( m_distance > 850 || (abs((850 - m_distance) - camPos) < 130 && abs((850 - m_distance) - camPos) > 720));	//generate random distance if it overlaps with player

	bIsActive = true;
	m_hp = 1;
}


void AActor::Update(float m_dt) {
	m_distance += m_speed * m_dt;

	switch (id)
	{
	case player:
		m_inLane = 0;

		break;
	case face:
		m_rotateAround = glm::normalize(glm::vec3(m_matrix[2][0], m_matrix[2][1], m_matrix[2][2]));
		m_angle = -10;
		if (m_yOffset > 29) {
			bIsVisible = false;
		}
		else {
			bIsVisible = true;
		}

		if (!bIsFinishAnimate && m_yOffset > 0) {
			m_yOffset -= 1.751;
		}

		if (m_yOffset < 5.25) {	//finish jump, reset value
			m_yOffset = 5.25;
			bIsFinishAnimate = true;
		}

		break;
	case eyeball:
		m_rotateAround = glm::normalize(glm::vec3(m_matrix[2][0], m_matrix[2][1], m_matrix[2][2]));
		m_angle -= 1.5f * m_dt;
		break;
	case doll:
		m_rotateAround = glm::normalize(glm::vec3(m_matrix[2][0], m_matrix[2][1], m_matrix[2][2]));

		if (m_angle < -20) {
			m_angle = -20;
			bIsFinishAnimate = false;
		}
		if (m_angle > 20) {
			m_angle = 20;
			bIsFinishAnimate = true;
		}
		if (bIsFinishAnimate) {
			m_angle -= 5.0f;
		}
		if (!bIsFinishAnimate) {
			m_angle += 5.0f;
		}
		break;

	case chinese:
		if (!bIsFinishAnimate) {
			m_yOffset += 0.05f;
		}
		else {
			m_yOffset -= 0.05f;
		}
		if (m_yOffset > 3.0) {
			m_yOffset = 3.f;
			bIsFinishAnimate = true;
		}
		if (m_yOffset < 2.5) {
			m_yOffset = 2.5;
			bIsFinishAnimate = false;
		}
		break;
	case tree:
		break;
	default:
		break;
	}
}



void AActor::Render() {
	COpenAssetImportMesh::Render();
}


glm::vec3 & const AActor::GetRoutateAround() {
	return m_rotateAround;
}

void AActor::SetRotateAround(glm::vec3 &rotateAround) {
	m_rotateAround = rotateAround;
}

float & const AActor::GetDistance() {
	return m_distance;
}

void AActor::SetDistance(float distance) {
	m_distance = distance;
}

float & const AActor::GetScale() {
	return m_scale;
}

void AActor::SetScale(float scale) {
	m_scale = scale;
}

float & const AActor::GetSpeed() {
	return m_speed;
}

void AActor::SetSpeed(float speed) {
	m_speed = speed;
}

float & const AActor::GetAngle() {
	return m_angle;
}

void AActor::SetAngle(float angle) {
	m_angle = angle;
}

float & const AActor::GetInLane() {
	return m_inLane;
}

float & const AActor::GetTurn() {
	return m_turn;
}

float & const AActor::GetYOffset() {
	return m_yOffset;
}

bool & const AActor::IsVisible() {
	return bIsVisible;
}

void AActor::SetVisible(bool b) {
	bIsVisible = b;
}

bool & const AActor::HasTrigger() {
	return bHasTrigger;
}

glm::vec3 & const AActor::GetTriggerPosition() {
	return m_triggerPosition;
}

void AActor::SetTriggerPosition(glm::vec3 &p) {
	m_triggerPosition = p;
}

AActor::Character & const AActor::GetID() {
	return id;
}

void AActor::RemoveTrigger() {
	bHasTrigger = false;
}

void AActor::StartAnimation() {
	bIsFinishAnimate = false;
}


bool AActor::CanBeSeen(CCamera *camera) const {
	return (glm::length((m_position - camera->GetPosition())) < 8);
}

bool AActor::CanBeSeen() const {
	return m_yOffset < 29.f;;
}

glm::mat4 &const AActor::GetModelViewMatrix() {
	return m_modelViewMatrix;
}

void AActor::SetModelViewMatrix(glm::mat4 const & const mat, glm::mat4 const *m_projection) {
	m_modelViewMatrix = mat;


}


float & const AActor::GetRadius() {
	return m_radius;
}


void AActor::Active() {
	bIsActive = true;
}

void AActor::Disable() {
	bIsActive = false;
}

bool &const AActor::IsActive() {
	return bIsActive;
}



void AActor::SetHP() {
	m_hp--;
}

int & const AActor::GetHP() {
	return m_hp;
}
