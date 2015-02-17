#include "Camera.h"
#include <iostream>

Camera::Camera(void)
{
	SetCameraType(LAND_CAM);
	Reset();
	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;
}

Camera::Camera(CAM_TYPE ct)
{
	CameraType = ct;
	Reset();
}

Camera::~Camera(void)
{
}

void Camera::SetCameraType(CAM_TYPE ct) {
	CameraType = ct;
}

void Camera::Reset(void)
{
	Position = Vector3(0.0, 0.0, 0.0);
	Along = Vector3(1.0, 0.0, 0.0);
	Up = Vector3(0.0, 1.0, 0.0);
	Forward = Vector3(0.0, 0.0, -1.0);
}

void Camera::Update() {
	gluLookAt(Position.x, Position.y, Position.z,
		Position.x + Forward.x, Position.y + Forward.y, Position.z + Forward.z,
		0.0f, 1.0f, 0.0f);
}

Vector3 Camera::GetPosition() {
	return Position;
}

void Camera::SetPosition(GLfloat x, GLfloat y, GLfloat z)
{
	this->Position.Set(x, y, z);
}

void Camera::SetDirection(GLfloat x, GLfloat y, GLfloat z)
{
	this->Forward.Set(x, y, z);
}

void Camera::Pitch(GLfloat theta)
{
	Forward.y -= theta;
	if (Forward.y > 3.142f)
		Forward.y = 3.142f;
	else if (Forward.y < -3.142f)
		Forward.y = -3.142f;
}
void Camera::Yaw(GLfloat theta)
{
	Forward.x = sin(-theta);
	Forward.z = -cos(-theta);
}

// Toggle HUD mode
void Camera::SetHUD(bool m_bHUDmode)
{
	if (m_bHUDmode)
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);							// Disable Texture Mapping ( NEW )
	}
	else
	{
		glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
		glEnable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glEnable(GL_DEPTH_TEST);
	}
}

// Get the direction of the camera
Vector3 Camera::GetDirection(void)
{
	return Forward;
}
