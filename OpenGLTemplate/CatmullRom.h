#pragma once
#include "Common.h"
#include "vertexBufferObject.h"
#include "vertexBufferObjectIndexed.h"
#include "Texture.h"


class CCatmullRom
{
public:
	CCatmullRom();
	~CCatmullRom();

	void CreateCentreline();
	void RenderCentreline();

	void CreateOffsetCurves();
	void RenderOffsetCurves();

	void CreateTrack();
	void RenderTrack();

	void CreateWall();
	void RenderWall();

	int CurrentLap(float d); // Return the currvent lap (starting from 0) based on distance along the control curve.

	bool Sample(float d, glm::vec3 &p, vector<glm::vec3> &controlPts, glm::vec3 &up = glm::vec3(0, 0, 0)); // Return a point on the centreline based on a certain distance along the control curve.
	void Create(string filename);

	void CreateStand();
	void RenderStand();

	void Render();

	void CreateObject(vector<glm::vec3> &vertices, unsigned int &counter, GLuint &vao, glm::vec3 normala = glm::vec3(0.0f, 0.0f, 1.0f));

	vector<glm::vec3> & const GetControlPoints();
	vector<glm::vec3> & const GetEnemyPath();


	void RenderInnerLeft();
	void RenderInnerRight();
	void RenderLeftWallTop();
	void RenderRightWallTop();
	void RenderLeftWallFront();
	void RenderRightWallFront();
private:

	void SetControlPoints();
	void ComputeLengthsAlongControlPoints();
	void UniformlySampleControlPoints(int numSamples);
	glm::vec3 Interpolate(glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3, float t);

	vector<float> m_distances;
	CTexture m_texture;
	vector<CTexture> m_textures;

	const int N_SAMPLE_RATE;
	const int N_CONTROL_POINTS;



private:			//Points and vao and counter and path
	GLuint m_vaoCentreline;
	GLuint m_vaoLeftOffsetCurve;
	GLuint m_vaoRightOffsetCurve;
	GLuint m_vaoTrack;
	GLuint m_vaoLeftWall;
	GLuint m_vaoRightWall;
	GLuint m_vaoLeftStand;
	GLuint m_vaoRightStand;
	GLuint m_vaoLeftInnerWall;
	GLuint m_vaoRightInnerWall;
	GLuint m_vaoLeftWallTop;
	GLuint m_vaoRightWallTop;
	GLuint m_vaoTrackBot;
	GLuint m_vaoTrackLeft;
	GLuint m_vaoTrackRight;
	
	GLuint m_vaoLeftWallFront;
	GLuint m_vaoRightWallFront;

	vector<glm::vec3> m_controlPoints;		// Control points, which are interpolated to produce the centreline points
	vector<glm::vec3> m_controlUpVectors;	// Control upvectors, which are interpolated to produce the centreline upvectors
	vector<glm::vec3> m_centrelinePoints;	// Centreline points
	vector<glm::vec3> m_centrelineUpVectors;// Centreline upvectors
	vector<glm::vec3> m_enemyPath;

	vector<glm::vec3> m_leftOffsetPoints;	// Left offset curve points
	vector<glm::vec3> m_rightOffsetPoints;	// Right offset curve points

	vector<glm::vec3> m_trackOffsetPoints;
	vector<glm::vec3> m_trackBotOffsetPoints;

	vector<glm::vec3> m_leftWallOffsetPoints;	//outer left wall
	vector<glm::vec3> m_rightWallOffsetPoints;	//outer right wall

	vector<glm::vec3> m_leftInnerWallOffsetPoints;	//inner left wall
	vector<glm::vec3> m_rightInnerWallOffsetPoints;	//inner right wall

	vector<glm::vec3> m_leftStandOffsetPoints;
	vector<glm::vec3> m_rightStandOffsetPoints;

	vector<glm::vec3> m_leftWallTopOffsetPoints;
	vector<glm::vec3> m_rightWallTopOffsetPoints;

	vector<glm::vec3> m_trackLeftOffsetPoints;
	vector<glm::vec3> m_trackRightOffsetPoints;

	vector<glm::vec3> m_leftWallFrontOffsetPoints;
	vector<glm::vec3> m_rightWallFrontOffsetPoints;

	unsigned int m_trackVertexCount;
	unsigned int m_leftWallVertexCount;				// Number of vertices in the track VBO
	unsigned int m_rightWallVertexCount;
	unsigned int m_leftStandVertexCount;
	unsigned int m_rightStandVertexCount;
	unsigned int m_leftInnerWallVertexCount;
	unsigned int m_rightInnerWallVertexCount;
	unsigned int m_leftWallTopVertexCount;
	unsigned int m_rightWallTopVertexCount;
	unsigned int m_trackBotVertexCount;
	unsigned int m_trackLeftVertexCount;
	unsigned int m_trackRightVertexCount;
	unsigned int m_leftWallFrontVertexCount;
	unsigned int m_rightWallFrontVertexCount;
};
