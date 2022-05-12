#pragma once
//#include "Renderer.h"
#include <chrono>
#include <utility>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "Animation.h"
#include "Math.h"
#define GRAVITY 8/5

enum class EntityType
{
	None,
	Player,
	Enemy,
	Dynamic,
	Static,
	Friendly,
	Unknown
};



class Entity
{
public:
	~Entity();

	//Checks if entity collided in Y or X axises. If so reset related velocity. 
	void normalize();

	/**
	* Moves the Entity with its current velocity.
	* \param timeStep time step between frames.
	*/
	void move(float timeStep);

	//Assign the values of positionX and positionY fields of the objects to the its hitbox's x and y.
	void setRectangleWithPos();

	/**
	* Initialize the Entity object with rectangular hitbox.
	* \param x The x position of the object when it's first created.
	* \param y The y position of the object when it's first created.
	* \param w Hitbox width of the Entity object.
	* \param h Hitbox heigth of the Entity object.
	* \param is_static_body False if object is not static,true otherwise.
	*/
	void Init(float x = 3700, float y = 45, float w =30, float h=100, bool is_static_body = 0);

	void InitCircle(float x, float y, float radius, bool is_static_body);
	void setVelocityX(float velocity);
	void setVelocityY(float velocity);
	Rectangle attack();

	void loadAnimations(SDL_Renderer* const &renderer);

	/**
	* 
	*/
	void setAnimation( Animation* const& animation, AnimType animType);

	void resetAnimation();
	void startsToFall();
	void itIsOnFloor();
	void rightIsWall();
	void leftIsWall();
	void rightIsNotWall();
	void leftIsNotWall();
	void resetXVelocity();
	void setJumpConditionFalse();
	void setFacingLeft();
	void setFacingRight();
	void setOnCeiling();
	void setOnNotCeiling();
	void setWallJumpingTrue();
	void setWallJumpingFalse();
	void startsToJump();
	void collidedFromTop();
	void setType(EntityType type);
	void advanceAnimation(float timeStep);
	void lockMovementY(int lockedDistance);
	void lockMovementX(int lockedDistance);
	void unlockMovementY();
	void unlockMovementX();
	void setRideableWallTrue();
	void setRideableWallFalse();
	void deathEvent();
	void getHit(float xVelocity, float yVelocity, float xLock, float yLock, float timeStep);
	void setNewPosition(const Point2f rect);
	void getPushedX(float xVelocity);
	void getPushedY(float yVelocity);

	//main movement 
	void moveRight(float timeStep);
	void moveLeft(float timeStep);
	void applyGravity(float timeStep);
	void doJump(float timeStep);
	void doWallJump(float timeStep);

	//getters
	float getVelocityX() const;
	float getVelocityY() const;
	float lowestPoint() const;
	float highestPoint() const;
	float mostLeftPoint() const;
	float mostRightPoint() const;
	float getVelocityVectorX() const;
	float getVelocityVectorY() const;
	float getFrameWidth();
	float getFrameHeight();
	float getFrameGap();
	int getLength() const;
	int getWidth() const;
	int getPushConstant() const;
	std::string getCameraLean() const;
	float getFrictionCoef() const;
	float getMaxVelocityX() const;

	SDL_Texture* get_texture() const;
	CollisionShape* getHitboxRect() const;

	//states
	bool isOnFloor() const;
	bool isOnCeiling() const;
	bool isFalling() const;
	bool isWallOnLeft() const;
	bool isWallOnRight() const;
	bool isAbleToJump() const;
	bool isWallRiding() const;
	bool isMovementLockedY() const;
	bool isMovementLockedX() const;
	bool isAbleToAttack() const;
	bool isEntityGotPushedY() const;
	bool isEffectedByGravity() const;

	std::string get_facing() const;
	inline const EntityType get_type() const { return entityType; };
	std::string getAnimationType() const;

private:
	void _goRight(float timeStep);
	void _goLeft(float timeStep);
	void _gravity(float timeStep);
	void _jump(float timeStep);
	void _setWallRidingTrue();
	void _setWallRidingFalse();
	bool _animationComparison(const std::string& candidateAnimationType);
	void _setCameraLeanRight();
	void _setCameraLeanLeft();
	void _decreaseMovementLockY();
	void _decreaseMovementLockX();
private:
	CollisionShape *hitbox = nullptr;
	Animation* currentFrame = nullptr,
		* walkAnimation = nullptr,
		* idleAnimation = nullptr,
		* fallAnimation = nullptr,
		* jumpAnimation = nullptr,
		* attackAnimation = nullptr,
		* landAnimation = nullptr,
		* jumpEndingAnimation = nullptr,
		* turnAnimation = nullptr,
		* wallRideAnimation = nullptr,
		* deathAnimation = nullptr;
	SDL_Surface *entitySurface;
	SDL_Texture *entityTexture;
	
	SDL_Rect textureRect;

	float hitboxWidth = 80, //33,
		hitboxLength = 214,
		jumpLowestPoint,
		jumpHeight,
		lockAmountY,
		yPositionBeforeLock,
		xPositionBeforeLock,
		lockAmountX,
		frictionCoefficient = 1,
		maxVelocityX = 0.5,
		currentAccelerationY = 0;
	float positionX, positionY;
	std::pair<float, float> velocity, normalizedVelocity;
	int speed = 6, 
		delayBetweenFrames = 4,
		movementLockCountY = 0, 
		movementLockCountX = 0,
		pushConstant = 0;

	std::string facing = "right",
				currentAnimationType = "none",
				cameraLean = "right";

	EntityType entityType = EntityType::None;

	bool falling = true,
		wallOnLeft = false,
		wallOnRight = false,
		onFloor = false,
		onCeiling = false,
		gonnaBeOnFloor = false,
		jumpCondition = false,
		collidedInYAxis = false,
		canBeginToJump = false,
		collidedInXAxis = false,
		gravityBool = true,
		wallRiding = false,
		wallJumping = false,
		movementLockY = false,
		movementLockX = false,
		rideableWall = false,
		jumping = false,
		gotPushedX = false,
		gotPushedY = false,
		justGotPushedY = false,
		effectedByGravity = false;
};

