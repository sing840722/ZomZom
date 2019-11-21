#pragma once

#include "./include/glm/gtc/type_ptr.hpp"
#include "./include/glm/gtc/matrix_transform.hpp"

class AActor;
class CCamera {
private:

	bool bIsThirdPerson;
	bool bUseScope;
	bool bIsShooting;
	float m_duration;
	float m_dt;
	float m_speed;
	float m_distance;
	float m_strafe;
	const float DISTANCE_TO_VIEW_PORT = 1000;
	glm::vec3 m_camForward;
	bool bCameraShake;

	bool bIsLookForward;
	bool bIsLookRight;
	bool bKey1Pressed;

	glm::mat4 m_matrix;
	glm::mat4 m_modelView;
	glm::vec2 m_mousePos;


	bool bLeftMousePressed;
	bool bRightMousePressed;
	bool bRPressed;
	bool bWalking;

	int m_ammo;
	float m_mouseTime;
	glm::vec3 m_rayDirection;

	bool bIsFreeMode;

public:
	bool & const GetCameraMode();
	
	bool IsWalking();


	//reload ammo
	void Reload();

	//fire 
	void Shoot();

	//check if mouse button down
	bool IsLeftMouseClicked();
	bool IsRightMouseClicked();

	//Apply camera shake
	void CameraShake();

	//request camera shake for the amount of time t
	void RequestCameraShake(float t);

	//set current distance on the path
	void SetDistance(float d);

	//check if overlaping a trigger
	bool TestTrigger(AActor *obj);

	
	//set orientated matrix of the camera
	void SetOrientedMatrix(glm::mat4 &m);
	
	//rotate camera
	void RotateViewPoint(float angle, glm::vec3 &viewPoint, int i);
	bool RotateViewPoint(float angle, glm::vec3 &viewPoint);

	//set the modelview matrix
	void SetModelViewMatrix(glm::mat4 mat);

	//set mouse time for time t
	void SetMouseTime(float t);

	//Set view using mouse input
	void SetViewByMouse();

	//enable or disable sniper scope
	void UseScope();
	void NoScope();

	void FreeRotate();
	void ToggleCameraMode();

public:		//default methods
	CCamera();						// Constructor - sets default values for camera position, viewvector, upvector, and speed
	~CCamera();										// Destructor

	//Getter
	glm::vec2 & const GetMousePosition();
	glm::vec3 GetPosition() const;					// Gets the position of the camera centre of projection
	glm::vec3 GetView() const;						// Gets the position of the camera view point
	glm::vec3 GetUpVector() const;					// Gets the camera up vector
	glm::vec3 GetStrafeVector() const;				// Gets the camera strafe vector
	glm::mat4* GetPerspectiveProjectionMatrix();	// Gets the camera perspective projection matrix
	glm::mat4* GetOrthographicProjectionMatrix();	// Gets the camera orthographic projection matrix
	glm::mat4 GetViewMatrix();						// Gets the camera view matrix - note this is not stored in the class but returned using glm::lookAt() in GetViewMatrix()
	int GetAmmo();
	float & const GetSpeed();
	float & const GetDistance();
	float & const GetStrafe();
	glm::vec3 & const GetRayDirection();
	bool UsingScope();
	const float & const GetDistanceBetweenViewPort();

	// Set the camera position, viewpoint, and up vector
	void Set(glm::vec3 &position, glm::vec3 &viewpoint, glm::vec3 &upVector);

	// Ray casting from screen space to world space
	bool MousePicking(AActor *object);

	// Respond to keyboard presses on arrow keys to translate the camera
	void TranslateByKeyboard(double dt);

	// Strafe the camera (move it side to side)
	void Strafe(double direction);

	// Advance the camera (move it forward or backward)
	void Advance(double direction);

	// Update the camera
	void Update(double dt);

	// Set the projection matrices
	void SetPerspectiveProjectionMatrix(float fov, float aspectRatio, float nearClippingPlane, float farClippingPlane);
	void SetOrthographicProjectionMatrix(int width, int height);

	void SetPosition(glm::vec3 newPos);
	void SetPosition(float x, float y, float z);

	glm::mat3 ComputeNormalMatrix(const glm::mat4 &modelViewMatrix);
private:
	glm::vec3 m_position;			// The position of the camera's centre of projection
	glm::vec3 m_view;				// The camera's viewpoint (point where the camera is looking)
	glm::vec3 m_upVector;			// The camera's up vector
	glm::vec3 m_strafeVector;		// The camera's strafe vector

	glm::mat4 m_perspectiveProjectionMatrix;		// Perspective projection matrix
	glm::mat4 m_orthographicProjectionMatrix;		// Orthographic projection matrix
};
