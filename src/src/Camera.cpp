#include "Camera.h"


Camera::Camera()
{
	camPosX = 3000;
	minCamPosY = -400;
	camPosY = -400;
}
void Camera::setNewFocusedEntity(const Entity* newFocus)
{
	focusedEntity = newFocus;
}
void Camera::updateCamera(int screenWidth, int screenHeight, float timeStep)
{
	maxPlayerSpeedX = focusedEntity->getMaxVelocityX();
	if (abs(focusedEntity->getVelocityX()) != 0)
	{
		playerSpeedX = abs(focusedEntity->getVelocityX());
	}
	float entityMidPoint =  focusedEntity->mostLeftPoint();
	//if screen is resized then adjust the camera width, height and current camera pos accordingly
	if (camWidth != screenWidth) 
	{
		camPosX -= (screenWidth - camWidth) / 2;
		setMaxCamPosX(maxCamPosX + (camWidth - screenWidth));
		camWidth = screenWidth;
		
	}
	if (camHeight != screenHeight)
	{
		setMaxCamPosY(maxCamPosY + (camHeight - screenHeight));
		camHeight = screenHeight;
	}

	//std::cout << "x: " << (entityMidPoint - camPosX) / camWidth << std::endl;

	//as player moves around move the camera accordingly
	if (1)
	{
		if (focusedEntity->getCameraLean() == "right")
		{

			if (camMovementX != "right")
			{
				
				if ((focusedEntity->mostLeftPoint() - camPosX) > camWidth * 0.53f)
				{
					//set burst true if camera lean and movemenet is not in the same direction
					//so that camera can catch up with the player
					burstX = true;
				}
				camMovementX = "right";
			}
		}
		else if (focusedEntity->getCameraLean() == "left")
		{
			if (camMovementX != "left")
			{
				if ((focusedEntity->mostLeftPoint() - camPosX) < camWidth * 0.47f)
				{
					//set burst true if camera lean and movemenet is not in the same direction
					//so that camera can catch up with the player
					burstX = true;
				}
				camMovementX = "left";
			}
		}

	}
	if (camMovementX == "left" && entityMidPoint - camPosX < camWidth * 0.53f || entityMidPoint - camPosX < camWidth * 0.47f)
	{
		if (entityMidPoint - camPosX < camWidth * 0.5f)
		{

			//first set the cameraSpeed
			if (camSpeedX < playerSpeedX && !burstX)
			{
				camSpeedX += playerSpeedX * 0.06 * timeStep;
			}
			if (camSpeedX > playerSpeedX && !burstX)
			{
				camSpeedX = playerSpeedX;
				reachedMaxSpeedX = true;
			}
			if (burstX)
			{
				if (camSpeedX < maxPlayerSpeedX * 1.7 && !reachedMaxSpeedX)
				{
					camSpeedX += maxPlayerSpeedX * 0.06 * timeStep;
					if (camSpeedX >= maxPlayerSpeedX * 1.7)
					{
						camSpeedX = maxPlayerSpeedX * 1.7;
						reachedMaxSpeedX = true;
					}
				}
				else if (camSpeedX > playerSpeedX)
				{
					camSpeedX -= maxPlayerSpeedX * 0.001 * timeStep;
				}
				else
				{
					camSpeedX = maxPlayerSpeedX;
					burstX = false;
					reachedMaxSpeedX = true;
				}
			}
			//move camera according to the speed and ts
			camPosX -= camSpeedX * timeStep;

		}
		else if(focusedEntity->getVelocityX() == 0)
		{

			camPosX -= camSpeedX * timeStep;
			camSpeedX -= 0.01  * timeStep;
			if (camSpeedX < 0.3)
			{
				camSpeedX = 0.3;
			}
		}
		else if(camFrameDelayX > 0)
		{
			camFrameDelayX--;
		}
		if (camMovementX != "left" && entityMidPoint - camPosX > camWidth * 0.47f)
		{
			camPosX = entityMidPoint - (camWidth * 0.47f);
			camMovementX = "none";
			camSpeedX = 0;
			reachedMaxSpeedX = false;
			burstX = false;
		}
		else if (camMovementX == "left" && entityMidPoint - camPosX > camWidth * 0.53f)
		{
			camPosX = entityMidPoint - (camWidth * 0.53f);
			camMovementX = "none";
			camSpeedX = 0;
			reachedMaxSpeedX = false;
			burstX = false;
		}

	}
	else if (camMovementX == "right" && ((entityMidPoint - camPosX) > (camWidth * 0.47f))  || ((entityMidPoint - camPosX) > (camWidth * 0.53f)) )
	{
		if (entityMidPoint - camPosX > 0.5f * camWidth)
		{
			//first set the cameraSpeed
			if (camSpeedX < playerSpeedX && !burstX)
			{
				camSpeedX += playerSpeedX * 0.06 * timeStep;
			}
			if (camSpeedX > playerSpeedX && !burstX)
			{
				camSpeedX = playerSpeedX;
				reachedMaxSpeedX = true;
			}
			if (burstX)
			{
				if (camSpeedX < playerSpeedX * 1.7 && !reachedMaxSpeedX)
				{
					camSpeedX += playerSpeedX * 0.06 * timeStep;
					if (camSpeedX >= playerSpeedX * 1.7)
					{
						camSpeedX = playerSpeedX * 1.7;
						reachedMaxSpeedX = true;
					}
				}
				else if (camSpeedX > playerSpeedX)
				{
					camSpeedX -= playerSpeedX * 0.001 * timeStep;
				}
				else
				{
					camSpeedX = playerSpeedX;
					burstX = false;
					reachedMaxSpeedX = true;
				}
			}
			//move camera according to the speed and ts
			camPosX += camSpeedX * timeStep;
		}
		else if (focusedEntity->getVelocityX() == 0)
		{
			camPosX += camSpeedX * timeStep;
			camSpeedX -= 0.01 * timeStep;
			if (camSpeedX < 0.3)
			{
				camSpeedX = 0.3;
			}
		}
		else if (camFrameDelayX > 0)
		{
			camFrameDelayX--;
		}
		if (camMovementX != "right" && ((entityMidPoint - camPosX) < (camWidth * 0.53f)))
		{
			camPosX = entityMidPoint - (camWidth * 0.53f);
			camMovementX = "none";
			camSpeedX = 0;
			reachedMaxSpeedX = false;
			burstX = false;
		}
		else if (camMovementX == "right" && ((entityMidPoint - camPosX) < (camWidth * 0.47f)))
		{
			camPosX = entityMidPoint - (camWidth * 0.47f);
			camMovementX = "none";
			camSpeedX = 0;
			reachedMaxSpeedX = false;
			burstX = false;
		}

	}
	/*else if (player1.getVelocityX() == 0)
	{
		camMovementX = "none";
		camSpeedX = 0;
		reachedMaxSpeedX = false;
		burstX = false;
	}*/
	if (camPosX <= minCamPosX)
	{
		camPosX = minCamPosX;
	}
	if (camPosX >= maxCamPosX)
	{
		camPosX = maxCamPosX;
	}

	
	
		
	if (focusedEntity->lowestPoint() >= minCamPosY + camHeight / 2 && focusedEntity->highestPoint() <= maxCamPosY + camHeight / 2)
	{
		if (focusedEntity->getVelocityY() < 0 && focusedEntity->highestPoint() - camPosY < 0.47f * camHeight)
		{
			camPosY += focusedEntity->getVelocityY() * timeStep;
		}
		if (focusedEntity->getVelocityY() > 0 && focusedEntity->highestPoint() - camPosY > 0.53f * camHeight)
		{
			camPosY += focusedEntity->getVelocityY() * timeStep;
		}
		if (focusedEntity->getVelocityY() == 0 && focusedEntity->highestPoint() - camPosY < 0.47f * camHeight)
		{
			camPosY -= 0.5 * timeStep;
		}
		else if (focusedEntity->getVelocityY() == 0 && focusedEntity->highestPoint() - camPosY > 0.53f * camHeight)
		{
			camPosY += 0.5 * timeStep;
		}
	}
	if (camMovementY == "upward" && focusedEntity->highestPoint() - camPosY < camHeight * 0.5f)
	{
		if ((focusedEntity->getVelocityY() < 0))
		{
			if (camSpeedY < 0.4)
			{
				camSpeedY += 0.01 * timeStep;
			}
			if (camSpeedY > 0.4)
			{
				camSpeedY = 0.4;
				reachedMaxSpeedY = true;
			}
			camPosY -= camSpeedY * (timeStep);
		}
		else 
		{
			camPosY -= camSpeedY * (timeStep);
			camSpeedY -= 0.01 * timeStep;
			if (camSpeedY < 0.2)
			{
				camSpeedY = 0.2;
			}
		}
		

	}
	else if (camMovementY == "downward" && ((focusedEntity->lowestPoint() - camPosY) > (camHeight * 0.5)))
	{
		
		if ((focusedEntity->getVelocityY() > 0))
		{

			camPosY += focusedEntity->getVelocityY() * timeStep;
		}
		else 
		{
			camPosY += 0.2 * timeStep;
		}

	}
	else
	{
		camMovementY = "none";
		camSpeedY = 0;
		reachedMaxSpeedY = false;
	}


	if (camPosY <= minCamPosY)
	{
		camPosY = minCamPosY;
	}
	if (camPosY >= maxCamPosY)
	{
		camPosY = maxCamPosY;
	}
}


void Camera::setMaxCamPosX(float newPos)
{
	maxCamPosX = newPos;
}

void Camera::setMaxCamPosY(float newPos)
{
	maxCamPosY = newPos;
}

void Camera::setMinCamPosX(float newPos)
{
	minCamPosX = newPos;
}

void Camera::setMinCamPosY(float newPos)
{
	minCamPosY = newPos;
}

void Camera::setNewCamPosition(float x, float y)
{
	camPosX = x;
	camPosY = y;
}


float Camera::getCamX() const 
{ 
	return camPosX;
}


float Camera::getCamY() const 
{ 
	return camPosY;
}



float Camera::getCamWidth() const 
{ 
	return camWidth;
}



float Camera::getMaxCamPosX() const
{ 
	return maxCamPosX; 
}



float Camera::getMaxCamPosY() const
{ 
	return maxCamPosY; 
}