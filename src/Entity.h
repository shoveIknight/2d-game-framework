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
	* @param timeStep time step between frames.
	*/
	void move(float timeStep);

	//Assign the values of positionX and positionY fields of the objects to the its hitbox's x and y.
	void setRectangleWithPos();

	/**
	* Initialize the Entity object with rectangular hitbox.
	* @param x The x position of the object when it's first created.
	* @param y The y position of the object when it's first created.
	* @param w Hitbox width of the Entity object.
	* @param h Hitbox heigth of the Entity object.
	* @param is_static_body False if object is not static,true otherwise.
	*/
	void Init(float x = 3700, float y = 45, float w =30, float h=100, bool is_static_body = 0);

	/**
	 * Initialize the Entity object with circular hitbox.
	 * 
	 * @param x The x coordiante of top left corner of the smallest square that contains given circular hitbox.
	 * @param y The y coordiante of top left corner of the smallest square that contains given circular hitbox.
	 * @param radius The radius of circular hitbox.
	 * @param is_static_body False if object is not static,true otherwise.
	 */
	void InitCircle(float x, float y, float radius, bool is_static_body);

	/**
	 * @brief Set the velocity of the object in x axis.
	 * 
	 * @param velocity The velocity to set.
	 */
	void setVelocityX(float velocity);

	
	/**
	 * @brief Set the velocity of the object in y axis.
	 * 
	 * @param velocity The velocity to set.
	 */
	void setVelocityY(float velocity);

	/**
	 * @brief Returns the area which entity damages when attacks.
	 * 
	 * @return Rectangle object of the attacked area.
	 */
	Rectangle attack();



	void loadAnimations(SDL_Renderer* const &renderer);

	/**
	* Sets the given type of animation with given Animation object.
	* @param animation The animation object that you want to set.
	* @param animType The animation type of the entity object you want to set.
	*/
	void setAnimation( Animation* const& animation, AnimType animType);

	void resetAnimation();

	/**
	 * @brief Entity starts to fall.
	 * 
	 */
	void startsToFall();
	/**
	 * @brief Sets entity on floor.
	 * 
	 */
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
	//set currentFrame with next frame of animation 
	void advanceAnimation(float timeStep);

	//Keep y velocioty constant until given distance is taken.
	void lockMovementY(int lockedDistance);
	//Keep y velocioty constant until given distance is taken.
	void lockMovementX(int lockedDistance);

	//unlocks the y veloctiy lock.
	void unlockMovementY();

	///unlocks the x veloctiy lock.
	void unlockMovementX();

	void setRideableWallTrue();
	void setRideableWallFalse();

	//sets currentFrame with deathAnimation
	void deathEvent();
	/**
	 * @brief Called when entity got hit.
	 * 
	 * @param xVelocity The x velocity of response to hit.
	 * @param yVelocity  The x velocity of response to hit.
	 * @param xLock The distance which entity will unable to move in x-axis until the distance is taken.
	 * @param yLock The distance which entity will unable to move in y-axis until the distance is taken.
	 * @param timeStep frame time
	 */
	void gotHit(float xVelocity, float yVelocity, float xLock, float yLock, float timeStep);

	//teleport the entity to given point
	void setNewPosition(const Point2f rect);

	//the Entity got pushed in x axis by another object.
	void getPushedX(float xVelocity);
	
	//the Entity got pushed in y axis by another object.
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

