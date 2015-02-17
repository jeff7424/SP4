#pragma once

#include "Vector3.h"
#include <GL/glut.h>
#include "Map.h"

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 672

class Camera
{
public:
	enum CAM_TYPE { LAND_CAM, AIR_CAM };

	Camera(void);
	Camera(CAM_TYPE ct);
	~Camera(void);

	void SetCameraType(CAM_TYPE ct);
	void Reset(void);
	void Update();
	Vector3 GetPosition();
	// Get the direction of the camera
	Vector3 GetDirection(void);
	void SetPosition(GLfloat x, GLfloat y, GLfloat z);
	void SetDirection(GLfloat x, GLfloat y, GLfloat z);

	void Pitch(GLfloat theta);
	void Yaw(GLfloat theta);

	// Toggle HUD mode
	void SetHUD(bool m_bHUDmode);

	int width;
	int height;

private:
	CAM_TYPE CameraType;
	Vector3 Position;
	Vector3 Along;
	Vector3 Up;
	Vector3 Forward;

	//public:
	//	
};
