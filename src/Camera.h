#pragma once
#include <iostream>
#include <string>
#include "Entity.h"

#define SCREEN_HEIGTH 1080
#define SCREEN_W 1280

class Camera
{
public:


	Camera();
	/**
	* Updates the position of camera.
	* 
	* \param screenWidth width of SDL_Window.
	* \param screenHeight height of SDL_Window.
	* \param timeStep time step between frames.
	*/
	void updateCamera(int screenWidth, int screenHeight, float timeStep);

#pragma region setters

	float getCamX() const;
	float getCamY() const;
	float getCamWidth() const;
	float getMaxCamPosX() const;
	float getMaxCamPosY() const;
#pragma endregion



	void setMaxCamPosX(float newPos);
	void setMaxCamPosY(float newPos);
	void setMinCamPosX(float newPos);
	void setMinCamPosY(float newPos);
	void setNewCamPosition(float x, float y);

	/**
	* Sets the current focus of camera to the newFocus
	* \param newFocus Entity object pointer of new camera focus.
	*/
	void setNewFocusedEntity(const Entity* newFocus);

private:

	const Entity* focusedEntity = nullptr;
	int camFrameDelayX = 0, 
		camFrameDelayY= 0;
	float camPosX = 0,
		camPosY = 0,
		camWidth = SCREEN_W,
		camHeight = 960,
		camSpeedX = 0,
		camSpeedY = 0,
		playerSpeedX = 0,
		maxPlayerSpeedX = 0;
	float maxCamPosX = 1365, 
		maxCamPosY = 0, 
		minCamPosY = 0, 
		minCamPosX = 0;
	std::string camMovementX = "none",
		camMovementY = "none";
	bool reachedMaxSpeedX = false,
		reachedMaxSpeedY = false,
		burstX = false;
};

