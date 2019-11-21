#include "camera.h"
#include "gamewindow.h"
#include "Actor.h"



template <typename T>
void DebugLog(T value) {
	std::ostringstream ss;
	ss << value;
	std::string s(ss.str());
	OutputDebugString(s.c_str());
}



// Constructor for camera -- initialise with some default values
CCamera::CCamera()
{
	m_position = glm::vec3(0.0f, 4.5f, 25.0f);
	m_view = glm::vec3(0, 0, 0);
	m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
	m_speed = 0.004f;
	m_distance = 0.0f;
	bIsThirdPerson = false;
	bRPressed = false;
	bCameraShake = false;
	m_strafe = 1;

	RotateViewPoint(M_PI, glm::vec3(0, 1, 0), 1);
	RotateViewPoint(0.5, glm::vec3(1, 0, 0), 1);
	bLeftMousePressed = false;

	m_mousePos = glm::vec2(0, 0);
	m_mouseTime = 0;
	m_ammo = 6;

	bUseScope = false;
	bIsShooting = false;
	bIsFreeMode = false;
	
}

CCamera::~CCamera() {

}
 
// Set the camera at a specific position, looking at the view point, with a given up vector
void CCamera::Set(glm::vec3 &position, glm::vec3 &viewpoint, glm::vec3 &upVector)
{
	m_position = position;
	m_view = viewpoint;
	m_upVector = upVector;

}

void CCamera::SetPosition(glm::vec3 newPos) {
	m_position = newPos;
}

void CCamera::SetPosition(float x, float y, float z) {
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void CCamera::SetOrientedMatrix(glm::mat4 &m) {
	m_matrix = m;
}

void CCamera::ToggleCameraMode() {
	bIsFreeMode = !bIsFreeMode;
}

void CCamera::FreeRotate() {
	int middle_x = GameWindow::SCREEN_WIDTH >> 1;
	int middle_y = GameWindow::SCREEN_HEIGHT >> 1;

	float angle_y = 0.0f;
	float angle_z = 0.0f;

	POINT mouse;
	GetCursorPos(&mouse);

	if (mouse.x == middle_x && mouse.y == middle_y) {
		return;
	}

	//always keep the mouse at the middle of the screen
	SetCursorPos(middle_x, middle_y);

	angle_y = (float)(middle_x - mouse.x) / 1000.0f;
	angle_z = (float)(middle_y - mouse.y) / 1000.0f;

	m_mousePos = glm::vec2(angle_y, angle_z);


	glm::vec3 cross = glm::cross(m_view - m_position, m_upVector);
	glm::vec3 axis = glm::normalize(cross);

	//roate the view port by the amount of angle around the axis
	RotateViewPoint(angle_z, axis);
	RotateViewPoint(angle_y, glm::vec3(0, 1, 0));
}

// Respond to mouse movement
void CCamera::SetViewByMouse()
{  
	if (bIsFreeMode) {
		FreeRotate();
	}
	else {
		
		if (!bUseScope) {
		//get mouse position
		POINT mouse;
		GetCursorPos(&mouse);

		//convert mouse position from screen space to clipping space
		long double nds_x = ((2.0 * mouse.x) / (GameWindow::SCREEN_WIDTH) - 1);
		long double nds_y = 1 - (2.0 * mouse.y) / (GameWindow::SCREEN_HEIGHT);
		glm::vec4 ray_clip = glm::vec4(nds_x, nds_y, -1, 1);

		//convert from clipping space to eye coordinate
		glm::mat4 invProjMat = glm::inverse(m_perspectiveProjectionMatrix);
		glm::vec4 eyeCoords = invProjMat * ray_clip;
		eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1, 0);

		//conver from eye coordinate to world space
		glm::mat4 invViewMat = glm::inverse(m_modelView);
		glm::vec4 rayWorld = invViewMat * eyeCoords;

		//normalise to get direction vector
		glm::vec3 rayDirection = glm::normalize(glm::vec3(rayWorld));

		//set and prevent mouse get out of the scene
		m_mousePos = glm::vec2(nds_x, nds_y);
		if (m_mousePos.x < -1)
		m_mousePos.x = -1;

		if (m_mousePos.x > 1)
		m_mousePos.x = 1;

		if (m_mousePos.y > 1)
		m_mousePos.y = 1;

		if (m_mousePos.y < -1)
		m_mousePos.y = -1;

		}
		else if (bUseScope) {	//comment the function except the code below
			FreeRotate();

			/*
		int middle_x = GameWindow::SCREEN_WIDTH >> 1;
		int middle_y = GameWindow::SCREEN_HEIGHT >> 1;

		float angle_y = 0.0f;
		float angle_z = 0.0f;

		POINT mouse;
		GetCursorPos(&mouse);

		if (mouse.x == middle_x && mouse.y == middle_y) {
		return;
		}

		//always keep the mouse at the middle of the screen
		SetCursorPos(middle_x, middle_y);

		angle_y = (float)(middle_x - mouse.x) / 1000.0f;
		angle_z = (float)(middle_y - mouse.y) / 1000.0f;

		m_mousePos = glm::vec2(angle_y, angle_z);


		glm::vec3 cross = glm::cross(m_view - m_position, m_upVector);
		glm::vec3 axis = glm::normalize(cross);

		//roate the view port by the amount of angle around the axis
		RotateViewPoint(angle_z, axis);
		RotateViewPoint(angle_y, glm::vec3(0, 1, 0));
		*/
		}
		
	}

	
}

glm::vec2 & const CCamera::GetMousePosition()  {
	return m_mousePos;
}

bool CCamera::UsingScope() {
	return bUseScope;
}


glm::vec3 & const CCamera::GetRayDirection() {
	POINT mouse;
	GetCursorPos(&mouse);
	
	//covert mouse position from screen space to clipping
	long double nds_x = ((2.0 * mouse.x) / (GameWindow::SCREEN_WIDTH) - 1);
	long double nds_y = 1 - (2.0 * mouse.y) / (GameWindow::SCREEN_HEIGHT);
	glm::vec4 ray_clip = glm::vec4(nds_x, nds_y, -1, 1);

	//clipping to eye coord
	glm::mat4 invProjMat = glm::inverse(m_perspectiveProjectionMatrix);
	glm::vec4 eyeCoords = invProjMat * ray_clip;
	eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1, 0);

	//eye coord to world
	glm::mat4 invViewMat = glm::inverse(m_modelView);
	glm::vec4 rayWorld = invViewMat * eyeCoords;
	glm::vec3 rayDirection = glm::normalize(glm::vec3(rayWorld));
	m_rayDirection = rayDirection;

	
	return m_rayDirection;
}

bool CCamera::MousePicking(AActor *object) {
	
	POINT mouse;
	GetCursorPos(&mouse);

	glm::vec3 monsterPos = object->GetPosition();
	float radius = object->GetRadius();

	//covert mouse position from screen space to world space
	long double nds_x = ((2.0 * mouse.x) / (GameWindow::SCREEN_WIDTH) - 1);
	long double nds_y = 1 - (2.0 * mouse.y) / (GameWindow::SCREEN_HEIGHT);
	glm::vec4 ray_clip = glm::vec4(nds_x , nds_y, -1, 1);
	glm::mat4 invProjMat = glm::inverse(m_perspectiveProjectionMatrix);
	glm::vec4 eyeCoords = invProjMat * ray_clip;
	eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1, 0);
	glm::mat4 invViewMat = glm::inverse(m_modelView);
	glm::vec4 rayWorld = invViewMat * eyeCoords;
	glm::vec3 rayDirection = glm::normalize(glm::vec3(rayWorld));
	m_rayDirection = rayDirection;

	
	glm::vec3 v = monsterPos - m_position;
	long double a = glm::dot(rayDirection, rayDirection);
	long double b = 2.0 * glm::dot(v, rayDirection);
	long double c = glm::dot(v, v) - radius * radius;
	long double b_squared_minus_4ac = b * b + (-4.0) * a * c;


	//quadratic discrminate
	if (b_squared_minus_4ac == 0)
	{
		// One real root 
		if ((GetKeyState(VK_LBUTTON) & 0x80) != 0) {
			return true;
		}
	}
	else if (b_squared_minus_4ac > 0)
	{
		// Two real roots 
		long double x1 = (-b - sqrt(b_squared_minus_4ac)) / (2.0 * a);
		long double x2 = (-b + sqrt(b_squared_minus_4ac)) / (2.0 * a);

		if (x1 >= 0.0 || x2 >= 0.0) {
			
			if ((GetKeyState(VK_LBUTTON) & 0x80) != 0){

				return true;
			}
		}
			
		if (x1 < 0.0 || x2 >= 0.0) {

			if ((GetKeyState(VK_LBUTTON) & 0x80) != 0) {
				return true;
			}
		}
	}
	
	return false;
}


void CCamera::SetModelViewMatrix(glm::mat4 mat) {
	m_modelView = mat;
}


// Rotate the camera view point -- this effectively rotates the camera since it is looking at the view point
bool CCamera::RotateViewPoint(float fAngle, glm::vec3 &vPoint)
{
	glm::vec3 vView = m_view - m_position;
	glm::mat4 R = glm::rotate(glm::mat4(1), fAngle * 180.0f / (float)M_PI, vPoint);
	glm::vec4 newView = R * glm::vec4(vView, 1);
	m_view = m_position + glm::vec3(newView);

	return true;
}

void CCamera::RotateViewPoint(float fAngle, glm::vec3 &vPoint, int i)
{
	glm::vec3 vView = m_view - m_position;
	glm::mat4 R = glm::rotate(glm::mat4(1), fAngle * 180.0f / (float)M_PI, vPoint);
	glm::vec4 newView = R * glm::vec4(vView, 1);
	m_view = m_position + glm::vec3(newView);
}

// Strafe the camera (side to side motion)
void CCamera::Strafe(double direction)
{
	float speed = (float) (m_speed * direction);

	m_position.x = m_position.x + m_strafeVector.x * speed;
	m_position.z = m_position.z + m_strafeVector.z * speed;

	m_view.x = m_view.x + m_strafeVector.x * speed;
	m_view.z = m_view.z + m_strafeVector.z * speed;
}

// Advance the camera (forward / backward motion)
void CCamera::Advance(double direction)
{
	float speed = (float) (m_speed * direction);

	glm::vec3 view = glm::normalize(m_view - m_position);
	m_position = m_position + view * speed;
	m_view = m_view + view * speed;
}


float Random() {
	float r = ((float)(rand() / (float)(RAND_MAX)) * 2) - 1;	//random between -1.0f to 1.0f;
	return r;
}

float & const CCamera::GetStrafe() {
	return m_strafe;
}

void CCamera::CameraShake() {
	float magnitude = 0.125f;
	
	float x = Random() * magnitude; //random between -1 to 1
	float y = Random() * magnitude;
	m_position += glm::vec3(x, y, 0);
}

void CCamera::RequestCameraShake(float t) {
	m_duration = t;
	bCameraShake = true;
}

const float & const CCamera::GetDistanceBetweenViewPort() {
	return DISTANCE_TO_VIEW_PORT;
}

void CCamera::SetMouseTime(float t) {
	m_mouseTime = t;

}

// Update the camera to respond to mouse motion for rotations and keyboard for translation
void CCamera::Update(double dt)
{
	TranslateByKeyboard(dt);
	SetViewByMouse();

	m_speed = 0.0075f;	//0125
	
	IsRightMouseClicked();


	if (m_distance < 0.1) {
		m_distance = 0.1f;
	}
	m_distance += m_speed * (float)dt;

	m_dt = dt;

	
	if (m_duration > 0) {
		m_duration -= m_dt * 0.01f;
	}

	if (bCameraShake && m_duration > 0 && m_duration < 12.5) {
		CameraShake();
	}
	if (m_duration <= 0) {
		bCameraShake = false;
	}

	//can fire if mouse time > 0
	m_mouseTime -= dt;
}

bool CCamera::TestTrigger(AActor *obj) {
	
	glm::vec3 triggerPos = obj->GetTriggerPosition();
	triggerPos.y = m_position.y;

	if (glm::length(triggerPos - m_position) < 2) { 				//if overlap Trigger
		obj->RemoveTrigger();		//disable trigger
		if (obj->GetID() == AActor::Character::face) {	//fat face
			obj->StartAnimation();
			bCameraShake = true;
			m_duration = 12.5f + (17.5f * m_dt * 0.01f);	//shake duration + (jump time)
			return true;
		}
		else if (obj->GetID() == AActor::Character::coffin) {
			obj->StartAnimation();
			return true;
		}
		else if (obj->GetID() == AActor::Character::umbrella) {
			obj->StartAnimation();
			return true;
		}
	}
	
	return false;
}

int CCamera::GetAmmo() {
	return m_ammo;
}

void CCamera::NoScope() {
	bUseScope = false;
}

bool CCamera::IsLeftMouseClicked() {
	if ((GetKeyState(VK_LBUTTON) & 0x80) && !bLeftMousePressed) {
		bLeftMousePressed = true;
		if (m_mouseTime <= 0) {
			Shoot();
			if (m_ammo < 1) {
				Reload();
			}
		}
		else {
			return false;
		}
		return true;
	}

	if ((GetKeyState(VK_LBUTTON) & 0x8000) == 0 && bLeftMousePressed)
		bLeftMousePressed = false;

	return false;
}

bool CCamera::IsRightMouseClicked() {
	if ((GetKeyState(VK_RBUTTON) & 0x80) && !bRightMousePressed) {
		bRightMousePressed = true;
		bUseScope = !bUseScope;
		return true;
	}

	if ((GetKeyState(VK_RBUTTON) & 0x8000) == 0 && bRightMousePressed)
		bRightMousePressed = false;

	return false;
}


void CCamera::Shoot() {
	m_ammo--;
	m_mouseTime = 250;

}

void CCamera::Reload() {
	m_ammo = 6;
	m_mouseTime = 1250;
}


bool CCamera::IsWalking() {
	if ((GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80) 
		|| (GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80)) {
		bWalking = true;
		return bWalking;
	}
	bWalking = false;
	return bWalking;
}


// Update the camera to respond to key presses for translation
void CCamera::TranslateByKeyboard(double dt)
{
	if(bIsFreeMode)
	m_position.y = 3.5f;


	if (GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80) {
		//m_speed = 0.4;	
		Advance(1.0f *dt);
		
		//m_position.x -= 0.02f * dt;
		//m_position += GetRayDirection() * 0.2f * (float)dt;
		
	}
	if (GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80) {
		//m_speed = 0.4;
		Advance(1.0f *-dt);
		//m_position.x += 0.02f * dt;
	}
	if (GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80) {
		//m_position.z += 0.02f * dt;
		//Strafe(1.0f * dt);
	}
	if (GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80) {
		//m_position.z -= 0.02f * dt;
		//Strafe(1.0f * -dt);
	}

	if (GetKeyState('R') & 0x80 && !bRPressed) {
		bRPressed = true;
		Reload();
	}

	if (GetKeyState('R') & 0x8000 && bRPressed) {
		bRPressed = false;
	}

	if (GetKeyState('1') & 0x80) {
		ToggleCameraMode();
		return;
	}

}


// Return the camera position
glm::vec3 CCamera::GetPosition() const
{
	return m_position;
}

// Return the camera view point
glm::vec3 CCamera::GetView() const
{
	return m_view;
}

// Return the camera up vector
glm::vec3 CCamera::GetUpVector() const
{
	return m_upVector;
}

// Return the camera strafe vector
glm::vec3 CCamera::GetStrafeVector() const
{
	return m_strafeVector;
}

// Return the camera perspective projection matrix
glm::mat4* CCamera::GetPerspectiveProjectionMatrix()
{
	return &m_perspectiveProjectionMatrix;
}

// Return the camera orthographic projection matrix
glm::mat4* CCamera::GetOrthographicProjectionMatrix()
{
	return &m_orthographicProjectionMatrix;
}

// Set the camera perspective projection matrix to produce a view frustum with a specific field of view, aspect ratio, 
// and near / far clipping planes
void CCamera::SetPerspectiveProjectionMatrix(float fov, float aspectRatio, float nearClippingPlane, float farClippingPlane)
{
	m_perspectiveProjectionMatrix = glm::perspective(fov, aspectRatio, nearClippingPlane, farClippingPlane);
}

// The the camera orthographic projection matrix to match the width and height passed in
void CCamera::SetOrthographicProjectionMatrix(int width, int height)
{
	m_orthographicProjectionMatrix = glm::ortho(0.0f, float(width), 0.0f, float(height));
}

// Get the camera view matrix
glm::mat4 CCamera::GetViewMatrix()
{
	return glm::lookAt(m_position, m_view, m_upVector);
}

// The normal matrix is used to transform normals to eye coordinates -- part of lighting calculations
glm::mat3 CCamera::ComputeNormalMatrix(const glm::mat4 &modelViewMatrix)
{
	return glm::transpose(glm::inverse(glm::mat3(modelViewMatrix)));
}

bool & const CCamera::GetCameraMode() {
	return bIsFreeMode;
}



float & const CCamera::GetDistance() {
	return m_distance;
}

void CCamera::SetDistance(float d) {
	m_distance = d;
}

float & const CCamera::GetSpeed() {
	return m_speed;
}