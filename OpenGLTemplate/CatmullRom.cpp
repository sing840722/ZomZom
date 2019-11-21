#include "CatmullRom.h"
#define _USE_MATH_DEFINES
#include <math.h>

CCatmullRom::CCatmullRom():N_SAMPLE_RATE(6500), N_CONTROL_POINTS(65)
{
	//initialize vertex counter
	m_trackVertexCount = 0;
	m_leftWallVertexCount = 0;
	m_rightWallVertexCount = 0;
	m_leftStandVertexCount = 0;
	m_rightStandVertexCount = 0;
	m_leftInnerWallVertexCount = 0;
	m_rightInnerWallVertexCount = 0;
	m_leftWallTopVertexCount = 0;
	m_rightWallTopVertexCount = 0;
	m_trackBotVertexCount = 0;
	m_trackLeftVertexCount = 0;
	m_trackRightVertexCount = 0;
	m_leftWallFrontVertexCount = 0;
	m_rightWallFrontVertexCount = 0;
}

CCatmullRom::~CCatmullRom()
{}

void CCatmullRom::Create(string filename) {
	m_texture.Load(filename);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	m_textures.push_back(m_texture);
}

// Perform Catmull Rom spline interpolation between four points, interpolating the space between p1 and p2
glm::vec3 CCatmullRom::Interpolate(glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3, float t)
{
	
	float t2 = t * t;
	float t3 = t2 * t;

	glm::vec3 a = p1;
	glm::vec3 b = 0.5f * (-p0 + p2);
	glm::vec3 c = 0.5f * (2.0f*p0 - 5.0f*p1 + 4.0f*p2 - p3);
	glm::vec3 d = 0.5f * (-p0 + 3.0f*p1 - 3.0f*p2 + p3);

	return a + b*t + c*t2 + d*t3;

}


void CCatmullRom::SetControlPoints()
{
	m_controlPoints.push_back(glm::vec3(-39.43, 0, 9.26));
	m_controlPoints.push_back(glm::vec3(-37.89, 0, 13.22));
	m_controlPoints.push_back(glm::vec3(-36.75, 0, 16.13));
	m_controlPoints.push_back(glm::vec3(-34.38, 0, 19.88));
	m_controlPoints.push_back(glm::vec3(-31.72, 0, 22.1));
	m_controlPoints.push_back(glm::vec3(-29.15, 0, 23.59));
	m_controlPoints.push_back(glm::vec3(-25.79, 0, 24.92));
	m_controlPoints.push_back(glm::vec3(-21.74, 0, 26.35));
	m_controlPoints.push_back(glm::vec3(-18.09, 0, 27.44));
	m_controlPoints.push_back(glm::vec3(-14.39, 0, 27.78));
	m_controlPoints.push_back(glm::vec3(-10, 0, 28));
	m_controlPoints.push_back(glm::vec3(-5.65, 0, 27.73));
	m_controlPoints.push_back(glm::vec3(-2.49, 0, 26.89));
	m_controlPoints.push_back(glm::vec3(-0.71, 0, 25.61));
	m_controlPoints.push_back(glm::vec3(2.55, 0, 24.77));
	m_controlPoints.push_back(glm::vec3(6.25, 0, 23.73));
	m_controlPoints.push_back(glm::vec3(8.57, 0, 21.31));
	m_controlPoints.push_back(glm::vec3(11.19, 0, 18.4));
	m_controlPoints.push_back(glm::vec3(12.77, 0, 15.54));
	m_controlPoints.push_back(glm::vec3(14.45, 0, 12.25));
	m_controlPoints.push_back(glm::vec3(16.72, 0, 10.25));
	m_controlPoints.push_back(glm::vec3(19.53, 0, 8.63));
	m_controlPoints.push_back(glm::vec3(23.38, 0, 8.03));
	m_controlPoints.push_back(glm::vec3(27.19, 0, 7.34));
	m_controlPoints.push_back(glm::vec3(31.38, 0, 6.45));
	m_controlPoints.push_back(glm::vec3(35.33, 0, 5.66));
	m_controlPoints.push_back(glm::vec3(38.79, 0, 4.38));
	m_controlPoints.push_back(glm::vec3(42.74, 0, 2.4));
	m_controlPoints.push_back(glm::vec3(44.81, 0, -2.63));
	m_controlPoints.push_back(glm::vec3(45.11, 0, -7.52));
	m_controlPoints.push_back(glm::vec3(44.71, 0, -12.51));
	m_controlPoints.push_back(glm::vec3(42, 0, -16));
	m_controlPoints.push_back(glm::vec3(39.44, 0, -18.83));
	m_controlPoints.push_back(glm::vec3(34.79, 0, -20.7));
	m_controlPoints.push_back(glm::vec3(30.79, 0, -22.28));
	m_controlPoints.push_back(glm::vec3(26.64, 0, -23.32));
	m_controlPoints.push_back(glm::vec3(22.45, 0, -23.52));
	m_controlPoints.push_back(glm::vec3(18.15, 0, -23.57));
	m_controlPoints.push_back(glm::vec3(14.2, 0, -23.67));
	m_controlPoints.push_back(glm::vec3(10.94, 0, -23.67));
	m_controlPoints.push_back(glm::vec3(7.14, 0, -23.76));
	m_controlPoints.push_back(glm::vec3(3.73, 0, -23.86));
	m_controlPoints.push_back(glm::vec3(0.18, 0, -23.57));
	m_controlPoints.push_back(glm::vec3(-3.72, 0, -23.47));
	m_controlPoints.push_back(glm::vec3(-6.98, 0, -23.72));
	m_controlPoints.push_back(glm::vec3(-10.64, 0, -23.57));
	m_controlPoints.push_back(glm::vec3(-14.63, 0, -23.32));
	m_controlPoints.push_back(glm::vec3(-18.73, 0, -21.64));
	m_controlPoints.push_back(glm::vec3(-21.45, 0, -19.77));
	m_controlPoints.push_back(glm::vec3(-22.97, 0, -16.44));
	m_controlPoints.push_back(glm::vec3(-24.88, 0, -13.44));
	m_controlPoints.push_back(glm::vec3(-28, 0, -14));
	m_controlPoints.push_back(glm::vec3(-29.94, 0, -18.33));
	m_controlPoints.push_back(glm::vec3(-30, 0, -22));
	m_controlPoints.push_back(glm::vec3(-31.22, 0, -26.04));
	m_controlPoints.push_back(glm::vec3(-33.99, 0, -29.15));
	m_controlPoints.push_back(glm::vec3(-37.45, 0, -28.6));
	m_controlPoints.push_back(glm::vec3(-40.31, 0, -27.07));
	m_controlPoints.push_back(glm::vec3(-42.64, 0, -23.64));
	m_controlPoints.push_back(glm::vec3(-42.9, 0, -19.45));
	m_controlPoints.push_back(glm::vec3(-42.99, 0, -14.99));
	m_controlPoints.push_back(glm::vec3(-43.08, 0, -11.6));
	m_controlPoints.push_back(glm::vec3(-43.08, 0, -7.59));
	m_controlPoints.push_back(glm::vec3(-42.67, 0, -2.73));
	m_controlPoints.push_back(glm::vec3(-42.19, 0, 1.06));
	m_controlPoints.push_back(glm::vec3(-41.03, 0, 4.8));


	//scale all the vertex 
	float scale = 3.0f;
	for (int i = 0; i < m_controlPoints.size(); i++) {		m_controlPoints[i].x *= scale;		m_controlPoints[i].y *= scale;		m_controlPoints[i].z *= scale;	}
}

vector<glm::vec3> & const CCatmullRom::GetControlPoints() {
		return m_controlPoints;
}

//reserve the order of control point to get opposition direction of the curve
vector<glm::vec3> & const CCatmullRom::GetEnemyPath() {
	m_enemyPath = m_controlPoints;
	std::reverse(m_enemyPath.begin(), m_enemyPath.end());
	return m_enemyPath;
}

// Determine lengths along the control points, which is the set of control points forming the closed curve
void CCatmullRom::ComputeLengthsAlongControlPoints()
{
	int M = (int)m_controlPoints.size();

	float fAccumulatedLength = 0.0f;
	m_distances.push_back(fAccumulatedLength);
	for (int i = 1; i < M; i++) {
		fAccumulatedLength += glm::distance(m_controlPoints[i - 1], m_controlPoints[i]);
		m_distances.push_back(fAccumulatedLength);
	}

	// Get the distance from the last point to the first
	fAccumulatedLength += glm::distance(m_controlPoints[M - 1], m_controlPoints[0]);
	m_distances.push_back(fAccumulatedLength);
}


// Return the point (and upvector, if control upvectors provided) based on a distance d along the control polygon
bool CCatmullRom::Sample(float d, glm::vec3 &p, vector<glm::vec3> &controlPts, glm::vec3 &up)
{
	if (d < 0)
		return false;

	int M = (int)controlPts.size();
	if (M == 0)
		return false;


	float fTotalLength = m_distances[m_distances.size() - 1];

	// The the current length along the control polygon; handle the case where we've looped around the track
	float fLength = d - (int)(d / fTotalLength) * fTotalLength;

	// Find the current segment
	int j = -1;
	for (int i = 0; i < (int)m_distances.size(); i++) {
		if (fLength >= m_distances[i] && fLength < m_distances[i + 1]) {
			j = i; // found it!
			break;
		}
	}

	if (j == -1)
		return false;

	// Interpolate on current segment -- get t
	float fSegmentLength = m_distances[j + 1] - m_distances[j];
	float t = (fLength - m_distances[j]) / fSegmentLength;
	
	// Get the indices of the four points along the control polygon for the current segment
	int iPrev = ((j - 1) + M) % M;
	int iCur = j;
	int iNext = (j + 1) % M;
	int iNextNext = (j + 2) % M;


	// Interpolate to get the point (and upvector)
	p = Interpolate(controlPts[iPrev], controlPts[iCur], controlPts[iNext], controlPts[iNextNext], t);
	if (controlPts.size() == controlPts.size())
		up = glm::normalize(Interpolate(controlPts[iPrev], controlPts[iCur], controlPts[iNext], controlPts[iNextNext], t));


	return true;
}



// Sample a set of control points using an open Catmull-Rom spline, to produce a set of iNumSamples that are (roughly) equally spaced
void CCatmullRom::UniformlySampleControlPoints(int numSamples)
{
	glm::vec3 p, up;

	// Compute the lengths of each segment along the control polygon, and the total length
	ComputeLengthsAlongControlPoints();
	float fTotalLength = m_distances[m_distances.size() - 1];

	// The spacing will be based on the control polygon
	float fSpacing = fTotalLength / numSamples;

	// Call PointAt to sample the spline, to generate the points
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, m_controlPoints,up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);
	}


	// Repeat once more for truly equidistant points
	m_controlPoints = m_centrelinePoints;
	m_controlUpVectors = m_centrelineUpVectors;
	m_centrelinePoints.clear();
	m_centrelineUpVectors.clear();
	m_distances.clear();
	ComputeLengthsAlongControlPoints();
	fTotalLength = m_distances[m_distances.size() - 1];
	fSpacing = fTotalLength / numSamples;
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, m_controlPoints, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);
	}


}


void CCatmullRom::CreateCentreline()
{
	// Call Set Control Points
	SetControlPoints();
	// Call UniformlySampleControlPoints with the number of samples required
	UniformlySampleControlPoints(N_SAMPLE_RATE);
}


void CCatmullRom::CreateOffsetCurves()
{
	float trackWidth = 70.0f;
	float trackHeight = 0.4f;
	//float wallHeight = 1.0f;
	float wallHeight = 10.0f;
	//float wallWidth = 4.5f;
	float wallWidth = 19.f;
	float standLength = 90.0f;

	for (int i = 0; i < m_centrelinePoints.size() - 1; i++) {
		glm::vec3 p = m_centrelinePoints[i];
		glm::vec3 pNext = m_centrelinePoints[i + 1];
		
		glm::vec3 up(0.0f, 1.0f, 0.0f);
		glm::vec3 t = pNext - p;
		glm::vec3 n = glm::cross(t, up);

		//Calculate left and right offset points of the track 
		glm::vec3 l = p - (trackWidth / 2) * n;
		glm::vec3 r = p + (trackWidth / 2) * n;
		glm::vec3 track_left_bot = l - (trackHeight * up);
		glm::vec3 track_right_bot = r - (trackHeight * up);


		//Calculate left and right offset points of the outter wall
		glm::vec3 left_wall_bot = l;
		glm::vec3 left_wall_top = left_wall_bot + (wallHeight * up);
		glm::vec3 right_wall_bot = r;
		glm::vec3 right_wall_top = right_wall_bot + (wallHeight * up);


		//Calculate left and right inner wall
		glm::vec3 left_inner_wall_bot = left_wall_bot + (wallWidth * n);
		glm::vec3 left_inner_wall_top = left_wall_top + (wallWidth * n);
		glm::vec3 right_inner_wall_bot = right_wall_bot - (wallWidth *n);
		glm::vec3 right_inner_wall__top = right_wall_top - (wallWidth * n);


		//Calculate left and right offset points of the stand (Bottom side)
		glm::vec3 left_stand_bot = l - (standLength * up);
		glm::vec3 right_stand_bot = r - (standLength * up);


		//Track offset point
		m_trackOffsetPoints.push_back(l);						//top side/surface of the track
		m_trackOffsetPoints.push_back(r);
		m_trackBotOffsetPoints.push_back(track_left_bot);		//bottom side
		m_trackBotOffsetPoints.push_back(track_right_bot);
		m_trackLeftOffsetPoints.push_back(l);					//left side
		m_trackLeftOffsetPoints.push_back(track_left_bot);		//right side
		m_trackRightOffsetPoints.push_back(r);
		m_trackRightOffsetPoints.push_back(track_right_bot);


		//Wall offset points 
		m_leftWallOffsetPoints.push_back(l);
		m_leftWallOffsetPoints.push_back(left_wall_top);
		m_rightWallOffsetPoints.push_back(r);
		m_rightWallOffsetPoints.push_back(right_wall_top);


		//Inner Wall offset points
		m_leftInnerWallOffsetPoints.push_back(left_inner_wall_bot);
		m_leftInnerWallOffsetPoints.push_back(left_inner_wall_top);
		m_rightInnerWallOffsetPoints.push_back(right_inner_wall_bot);
		m_rightInnerWallOffsetPoints.push_back(right_inner_wall__top);

		//Wall front side offeset points
		m_leftWallFrontOffsetPoints.push_back(l);
		m_leftWallFrontOffsetPoints.push_back(left_inner_wall_bot);
		m_leftWallFrontOffsetPoints.push_back(left_wall_top);
		m_leftWallFrontOffsetPoints.push_back(left_inner_wall_top);
		//m_rightWallFrontOffsetPoints

		//Wall top offset points
		m_rightWallTopOffsetPoints.push_back(right_wall_top);
		m_leftWallTopOffsetPoints.push_back(left_wall_top);
		m_leftWallTopOffsetPoints.push_back(left_inner_wall_top);
		m_rightWallTopOffsetPoints.push_back(right_inner_wall__top);


		//Stand offset points
		m_leftStandOffsetPoints.push_back(track_left_bot);
		m_leftStandOffsetPoints.push_back(left_stand_bot);
		m_rightStandOffsetPoints.push_back(track_right_bot);
		m_rightStandOffsetPoints.push_back(right_stand_bot);

	}
}

void CCatmullRom::CreateObject(vector<glm::vec3> &vertices, unsigned int &counter, GLuint &vao, glm::vec3 normal) {
	float ratio = N_SAMPLE_RATE / N_CONTROL_POINTS;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();
	glm::vec2 texCoord(0.0f, 0.0f);

	// Generate a VAO called m_vaoTrack and a VBO to get the offset curve points and indices on the graphics card
	for (int i = 0; i < vertices.size(); i++) {
		glm::vec3 v = vertices[i];
		if (i % 2 == 0) {	//bot || left
			if (i == 0) {
				texCoord = glm::vec2(0.0f, 0.0f);
			}
			else {
				texCoord = glm::vec2(0.0f, i/ratio);
			}
		}
		else if (i % 2 == 1) {	//top || right
			if (i == 1) {
				texCoord = glm::vec2(1, 0.0f);
			}
			else {
				texCoord = glm::vec2(1, i/ratio);
			}
		}
		counter++;
		vbo.AddData(&v, sizeof(glm::vec3));
		vbo.AddData(&texCoord, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));
	}

	//Link back to the start point
	for (int i = 0; i < 2; i++) {
		glm::vec3 v = vertices[i];
		if (i % 2 == 0) {	//bot 

			texCoord = glm::vec2(0.0f, 1.0f);

		}
		else if (i % 2 == 1) {	//top
			texCoord = glm::vec2(1.0f, 1.0f);
		}
		counter++;
		vbo.AddData(&v, sizeof(glm::vec3));
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

void CCatmullRom::Render() {
	//RenderCentreline();
	RenderTrack();
	RenderWall();
	RenderStand();
}

void CCatmullRom::CreateTrack()
{
	//Main part of the track (path)
	CreateObject(m_trackOffsetPoints, m_trackVertexCount, m_vaoTrack, glm::vec3(0.0f, 1.0f, 0.0f));
	CreateObject(m_trackBotOffsetPoints, m_trackBotVertexCount, m_vaoTrackBot, glm::vec3(0.0f, -1.0f, 0.0f));
	CreateObject(m_trackLeftOffsetPoints, m_trackLeftVertexCount, m_vaoTrackLeft, glm::vec3(0.0f, 0.0f, -1.0f));
	CreateObject(m_trackRightOffsetPoints, m_trackRightVertexCount, m_vaoTrackRight, glm::vec3(0.0f, 0.0f, 1.0f));


	CreateWall();
	//CreateStand();
}


void CCatmullRom::RenderCentreline()
{
	// Bind the VAO m_vaoCentreline and render it
	glBindVertexArray(m_vaoCentreline);
	glDrawArrays(GL_LINE_STRIP, 0, m_centrelinePoints.size());
	//glDrawArrays(GL_POINTS, 0, m_centrelinePoints.size());

}

void CCatmullRom::RenderTrack()
{
	//glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(m_vaoTrack);
	m_textures[0].Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_trackVertexCount);
	
	glDisable(GL_BLEND);
	
	
	//uncomment to draw down face of the track
	/*
	glBindVertexArray(m_vaoTrackBot);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_trackBotVertexCount);


	m_textures[3].Bind();
	glBindVertexArray(m_vaoTrackLeft);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_trackLeftVertexCount);

	glBindVertexArray(m_vaoTrackRight);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_trackRightVertexCount);
	*/

	glEnable(GL_CULL_FACE);

}

int CCatmullRom::CurrentLap(float d)
{
	return (int)(d / m_distances.back());
}


void CCatmullRom::CreateWall() {
	glm::vec3 faceUp(0.0f, 1.0f, 0.0f);
	glm::vec3 faceLeft(0.0f, 0.0f, -1.0f);
	glm::vec3 faceRight(0.0f, 0.0f, 1.0f);

	//outer wall left and right
	CreateObject(m_leftWallOffsetPoints, m_leftWallVertexCount, m_vaoLeftWall, faceLeft);
	CreateObject(m_rightWallOffsetPoints, m_rightWallVertexCount, m_vaoRightWall, faceRight);

	//inner wall left and right
	CreateObject(m_leftInnerWallOffsetPoints, m_leftInnerWallVertexCount, m_vaoLeftInnerWall, faceRight);
	CreateObject(m_rightInnerWallOffsetPoints, m_rightInnerWallVertexCount, m_vaoRightInnerWall, faceLeft);

	//top of the wall left and right
	CreateObject(m_leftWallTopOffsetPoints, m_leftWallTopVertexCount, m_vaoLeftWallTop, faceUp);
	CreateObject(m_rightWallTopOffsetPoints, m_rightWallTopVertexCount, m_vaoRightWallTop, faceUp);

	//Front of the wall left and right
	//CreateObject(m_leftWallFrontOffsetPoints, m_leftWallFrontVertexCount, m_vaoLeftWallFront, glm::vec3(1.0f, 0.0f, 0.0f));
}

template <typename T>
void DebugLog(T value) {
	std::ostringstream ss;
	ss << value;
	std::string s(ss.str());
	OutputDebugString(s.c_str());
}

void CCatmullRom::RenderWall() {
	/*
	//Render Outer Wall left and right
	glBindVertexArray(m_vaoLeftWall);
	//m_textures[4].Bind();
	glDisable(GL_CULL_FACE);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, m_leftWallVertexCount);
	glEnable(GL_CULL_FACE);

	glBindVertexArray(m_vaoRightWall);
	//m_textures[4].Bind();
	glDisable(GL_CULL_FACE);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, m_rightWallVertexCount);
	glEnable(GL_CULL_FACE);


	//RenderInnerLeft();
	//RenderInnerRight();
	//RenderLeftWallTop();
	//RenderRightWallTop();
	RenderLeftWallFront();
	*/
}

void CCatmullRom::RenderInnerLeft() {
	/*
	//Render Inner Wall left and right
	glBindVertexArray(m_vaoLeftInnerWall);
	m_textures[1].Bind();
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_leftInnerWallVertexCount);
	glEnable(GL_CULL_FACE);
	*/
}

void CCatmullRom::RenderInnerRight() {
	/*
	glBindVertexArray(m_vaoRightInnerWall);
	m_textures[1].Bind();
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_rightInnerWallVertexCount);
	glEnable(GL_CULL_FACE);
	*/
}

void CCatmullRom::RenderLeftWallTop() {
	/*
	//Render Top of the Wall left and right
	glBindVertexArray(m_vaoLeftWallTop);
	m_textures[1].Bind();
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_leftWallTopVertexCount);
	glEnable(GL_CULL_FACE);
	*/
}

void CCatmullRom::RenderRightWallTop(){
	/*
	glBindVertexArray(m_vaoRightWallTop);
	m_textures[1].Bind();
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_rightWallTopVertexCount);
	glEnable(GL_CULL_FACE);
	*/
}

void CCatmullRom::RenderLeftWallFront() {
	/*
	//m_textures[4].Bind();
	glBindVertexArray(m_vaoLeftWallFront);
	glDisable(GL_CULL_FACE);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 100);
	//glDrawArrays(GL_TRIANGLE_STRIP, 300, 100);
	glEnable(GL_CULL_FACE);
	*/
}

void CCatmullRom::CreateStand() {
	CreateObject(m_leftStandOffsetPoints, m_leftStandVertexCount, m_vaoLeftStand, glm::vec3(0.0f, 0.0f, -1.0f));
	CreateObject(m_rightStandOffsetPoints, m_rightStandVertexCount, m_vaoRightStand, glm::vec3(0.0f, 0.0f, 1.0f));
}


void CCatmullRom::RenderStand() {
	/*
	float intersectionPt = 5000.0f;
	glBindVertexArray(m_vaoLeftStand);
	m_textures[2].Bind();
	glDisable(GL_CULL_FACE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, intersectionPt);
	glDrawArrays(GL_TRIANGLE_STRIP, 6000, m_leftStandVertexCount - 6000);
	glEnable(GL_CULL_FACE);

	glBindVertexArray(m_vaoRightStand);
	m_textures[2].Bind();
	glDisable(GL_CULL_FACE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, intersectionPt);
	glDrawArrays(GL_TRIANGLE_STRIP, 6000, m_rightStandVertexCount - 6000);
	glEnable(GL_CULL_FACE);
	*/
	/*
	glDisable(GL_CULL_FACE);
	glBindVertexArray(m_vaoLeftStand);
	m_textures[2].Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_leftStandVertexCount);

	glBindVertexArray(m_vaoRightStand);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_rightStandVertexCount);
	glEnable(GL_CULL_FACE);
	*/
}

