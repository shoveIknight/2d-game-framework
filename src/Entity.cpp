#include "Entity.h"
#include <math.h>
#include <iostream>
#include <assert.h>



void Entity::Init(float x, float y, float w, float h, bool is_static_body)
{
	hitbox = new Rectangle(x, y, w, h);
	positionX = x;
	positionY = y;
	hitbox->x = x;
	hitbox->y = y;
	hitbox->w = w;
	hitbox->h = h;
	setRectangleWithPos();
	velocity.first = 0;
	velocity.second = 0;
	if(!is_static_body)
	{
		startsToFall();
		normalize();
		pushConstant = 0;
		effectedByGravity = true;
	}
	else if (is_static_body)
	{
		effectedByGravity = false;
		pushConstant = INT_MAX;
	}
}
void Entity::InitCircle(float x, float y, float radius, bool is_static_body)
{
	hitbox = new Circle(x, y, radius);
	positionX = x;
	positionY = y;
	hitbox->x = x;
	hitbox->y = y;
	hitbox->w = 2*radius;
	hitbox->h = 2*radius;
	setRectangleWithPos();
	velocity.first = 0;
	velocity.second = 0;
	if (!is_static_body)
	{
		startsToFall();
		normalize();
		pushConstant = 0;
		effectedByGravity = true;
	}
	else if (is_static_body)
	{
		pushConstant = INT_MAX;
		effectedByGravity = false;
	}
}

void Entity::loadAnimations(SDL_Renderer* const &renderer)
{
	/*Animation* head = new Animation;
	head->setFrame("circle1.png", renderer, 0, 0);
	head->setNextAnimation(head);
	setAnimation(head, WallRideAnim);
	setAnimation(head, TurnAnim);
	setAnimation(head, JumpEndingAnim);
	setAnimation(head, LandAnim);
	setAnimation(head, FallAnim);
	setAnimation(head, JumpAnim);
	setAnimation(head, AttackAnim);
	setAnimation(head, IdleAnim);
	setAnimation(head, WalkAnim);
	return;*/



	{
		Animation* head = new Animation("newrun1.png", renderer, 50, 50);

		Animation* newAnim1 = new Animation("newrun2.png", renderer, 50, 50);
		head->setNextAnimation(newAnim1);

		Animation* newAnim2 = new Animation("newrun3.png", renderer, 50, 50);
		newAnim1->setNextAnimation(newAnim2);

		Animation* newAnim3 = new Animation("newrun4.png", renderer, 50, 50);
		newAnim2->setNextAnimation(newAnim3);

		Animation* newAnim4 = new Animation("newrun5.png", renderer, 50, 50);
		newAnim3->setNextAnimation(newAnim4);

		Animation* newAnim5 = new Animation("newrun6.png", renderer, 50, 50);
		newAnim4->setNextAnimation(newAnim5);

		Animation* newAnim6 = new Animation("newrun7.png", renderer, 50, 50);
		newAnim5->setNextAnimation(newAnim6);

		Animation* newAnim7 = new Animation("newrun8.png", renderer, 50, 50);
		newAnim6->setNextAnimation(newAnim7);

		newAnim7->setNextAnimation(head);

		setAnimation(head, WalkAnim);
	}
	{
		
		Animation* head = new Animation;
		head->setFrame("newidle1.png", renderer, 50, 50);

		Animation* newAnim1 = new Animation;
		newAnim1->setFrame("newidle2.png", renderer, 49, 51);
		head->setNextAnimation(newAnim1);

		Animation* newAnim2 = new Animation;
		newAnim2->setFrame("newidle3.png", renderer, 49, 51);
		newAnim1->setNextAnimation(newAnim2);

		Animation* newAnim3 = new Animation;
		newAnim3->setFrame("newidle4.png", renderer, 49, 51);
		newAnim2->setNextAnimation(newAnim3);

		Animation* newAnim4 = new Animation;
		newAnim4->setFrame("newidle5.png", renderer, 49, 51);
		newAnim3->setNextAnimation(newAnim4);

		Animation* newAnim5 = new Animation;
		newAnim5->setFrame("newidle6.png", renderer, 48, 52);
		newAnim4->setNextAnimation(newAnim5);

		newAnim5->setNextAnimation(head);

		setAnimation(head, IdleAnim);

	}

	{

		Animation* head = new Animation;
		head->setFrame("attack_01.png", renderer,31,0);

		Animation* newAnim1 = new Animation;
		newAnim1->setFrame("attack_02.png", renderer, 31,0);
		head->setNextAnimation(newAnim1);

		Animation* newAnim2 = new Animation;
		newAnim2->setFrame("attack_03.png", renderer,0 ,59);
		newAnim1->setNextAnimation(newAnim2);

		Animation* newAnim3 = new Animation;
		newAnim3->setFrame("attack_04.png", renderer, 0,58);
		newAnim2->setNextAnimation(newAnim3);

		Animation* newAnim4 = new Animation;
		newAnim4->setFrame("attack_05.png", renderer, 0,57);
		newAnim3->setNextAnimation(newAnim4);

		Animation* newAnim5 = new Animation;
		newAnim5->setFrame("attack_06.png", renderer, 0,57);
		newAnim4->setNextAnimation(newAnim5);

		newAnim5->setNextAnimation(nullptr);

		setAnimation(head, AttackAnim);

	}

	{
		Animation* head = new Animation;
		head->setFrame("newjump1.png", renderer, 50, 70);

		Animation* newAnim1 = new Animation;
		newAnim1->setFrame("newjump2.png", renderer, 51, 71);
		head->setNextAnimation(newAnim1);

		Animation* newAnim2 = new Animation;
		newAnim2->setFrame("newjump3.png", renderer, 49, 71);
		newAnim1->setNextAnimation(newAnim2);

		newAnim2->setNextAnimation(newAnim1);

		
		setAnimation(head, JumpAnim);

	}

	{
		Animation* head = new Animation;
		head->setFrame("newfall1.png", renderer, 50, 70);

		Animation* newAnim1 = new Animation;
		newAnim1->setFrame("newfall2.png", renderer, 49, 71);
		head->setNextAnimation(newAnim1);

		Animation* newAnim2 = new Animation;
		newAnim2->setFrame("newfall3.png", renderer, 49, 71);
		newAnim1->setNextAnimation(newAnim2);

		Animation* newAnim3 = new Animation;
		newAnim3->setFrame("newfall4.png", renderer, 49, 71);
		newAnim2->setNextAnimation(newAnim3);

		newAnim3->setNextAnimation(head);

		setAnimation(head, FallAnim);

	}


	{
		Animation* head = new Animation;
		head->setFrame("newland1.png", renderer, 50, 70);

		Animation* newAnim1 = new Animation;
		newAnim1->setFrame("newland2.png", renderer, 49, 71);
		head->setNextAnimation(newAnim1);

		Animation* newAnim2 = new Animation;
		newAnim2->setFrame("newland3.png", renderer, 49, 71);
		newAnim1->setNextAnimation(newAnim2);

		newAnim2->setNextAnimation(nullptr);

		setAnimation(head, LandAnim);

	}
	{

		Animation* head = new Animation;
		head->setFrame("newjump3.png", renderer, 50, 70);

		Animation* newAnim1 = new Animation;
		newAnim1->setFrame("newjump4.png", renderer, 49, 70);
		head->setNextAnimation(newAnim1);

		Animation* newAnim2 = new Animation;
		newAnim2->setFrame("newjump5.png", renderer, 49, 71);
		newAnim1->setNextAnimation(newAnim2);


		newAnim2->setNextAnimation(fallAnimation);

		setAnimation(head, JumpEndingAnim);

	}


	{		
		
		Animation* head = new Animation;
		head->setFrame("newmirrored_idle1.png", renderer, 50, 70);

		Animation* newAnim1 = new Animation;
		newAnim1->setFrame("newidle1.png", renderer, 49, 70);
		head->setNextAnimation(newAnim1);

		newAnim1->setNextAnimation(walkAnimation);

		setAnimation(head, TurnAnim);

	}
	{
		Animation* head = new Animation;
		head->setFrame("newwallride1.png", renderer, 1, 50);

		Animation* newAnim1 = new Animation;
		newAnim1->setFrame("newwallride2.png", renderer, 1, 51);
		head->setNextAnimation(newAnim1);

		Animation* newAnim2 = new Animation;
		newAnim2->setFrame("newwallride3.png", renderer, 1, 49);
		newAnim1->setNextAnimation(newAnim2);

		Animation* newAnim3 = new Animation;
		newAnim3->setFrame("newwallride4.png", renderer, 1, 46);
		newAnim2->setNextAnimation(newAnim3);

		newAnim3->setNextAnimation(newAnim1);

		setAnimation(head, WallRideAnim);
	}
}

void Entity::setAnimation(Animation* const &animation, AnimType animType)
{
	switch (animType)
	{
		case IdleAnim: 
			idleAnimation = animation;
			break;
		case WalkAnim:
			walkAnimation = animation;
			break;
		case FallAnim:
			fallAnimation = animation;
			break;
		case AttackAnim:
			attackAnimation = animation;
			break;
		case JumpAnim:
			jumpAnimation = animation;
			break;
		case LandAnim:
			landAnimation = animation;
			break;
		case JumpEndingAnim:
			jumpEndingAnimation = animation;
			break;
		case TurnAnim:
			turnAnimation = animation;
			break;
		case WallRideAnim:
			wallRideAnimation = animation;
			break;
	}
}

void Entity::startsToFall()
{
	if (entityType == EntityType::Player 
	|| entityType == EntityType::Enemy 
	|| entityType == EntityType::Friendly)
	{
		falling = true;
		gravityBool = true;
		onFloor = false;
		gonnaBeOnFloor = false;
		jumpCondition = false;
	}

}

void Entity::startsToJump()
{
	falling = true;
	gravityBool = false;
	onFloor = false;
	gonnaBeOnFloor = false;
}

void Entity::collidedFromTop()
{
	onCeiling = true;
	if (isMovementLockedY())
	{
		unlockMovementY();
	}
	jumpCondition = false;
	jumping = false;
	//setJumpConditionFalse();
}

void Entity::setType(EntityType type)
{
	entityType = type;
	if (type == EntityType::Dynamic)
	{
		pushConstant = 1;
		normalizedVelocity.second = -0.1f;
	}
	if (type == EntityType::Enemy)
	{
		pushConstant = 0;
		//normalizedVelocity.first = 1.0f;
		//hitbox->h += 100;
	}
}

void Entity::moveRight(float timeStep)
{
	if (isWallOnRight() )
	{
		if (isOnFloor())
		{
			setFacingRight();
			_setCameraLeanRight();
		}
		else if (getVelocityY() > 0 && rideableWall)
		{
			_setWallRidingTrue();
			setFacingLeft();
		}
	}
	if (!isWallOnRight() && !isMovementLockedX())
	{

		_goRight(timeStep);
		setFacingRight();
		_setCameraLeanRight();
	}
	if (walkAnimation != nullptr && _animationComparison("walkAnimation") && get_facing() == "right")
	{
		currentFrame = walkAnimation;
		currentAnimationType = "walkAnimation";

	}
	else if (turnAnimation != nullptr && get_facing() == "left" && _animationComparison("walkAnimation"))
	{
		currentFrame = turnAnimation;
		currentAnimationType = "turnAnimation";

	}
}

void Entity::moveLeft(float timeStep)
{
	if (isWallOnLeft())
	{
		if (isOnFloor())
		{
			setFacingLeft();
			_setCameraLeanLeft();
		}
		else if( getVelocityY() > 0 && rideableWall)
		{
			_setWallRidingTrue();
			setFacingRight();
		}
	}
	else if (!isWallOnLeft() && !isMovementLockedX())
	{
		_goLeft(timeStep);
		setFacingLeft();
		_setCameraLeanLeft();
	}
	if (walkAnimation != nullptr && _animationComparison("walkAnimation") && get_facing() == "left")
	{
		currentFrame = walkAnimation;
		currentAnimationType = "walkAnimation";
	}
	else if (turnAnimation != nullptr && get_facing() == "right" && _animationComparison("walkAnimation"))
	{
		currentFrame = turnAnimation;
		currentAnimationType = "turnAnimation";

	}
}

void Entity::applyGravity(float timeStep)
{
	if (!effectedByGravity)
	{
		return;
	}
	if (isMovementLockedX())
	{
		_decreaseMovementLockX();
		std::cout << movementLockCountX << std::endl;
	}
	if (isMovementLockedY())
	{
		_decreaseMovementLockY();
		//return;
	}
	else if (!isOnFloor())
	{

		_gravity(timeStep);

	}
}

void Entity::doJump(float timeStep)
{
	if (isWallRiding() && !wallJumping && !jumping)
	{
		doWallJump(timeStep);
	}
	else if (!wallJumping)
	{
		if (isOnFloor())
		{
			jumping = true;
			jumpLowestPoint = lowestPoint();
			jumpHeight = 250;
		}
		if ((jumpLowestPoint - lowestPoint() > jumpHeight))
		{
			startsToFall();
		}
		if (isAbleToJump())
		{
			startsToJump();
			_jump(timeStep);
		}
	}
	else if (wallJumping)
	{
		if ((jumpLowestPoint - lowestPoint() > jumpHeight))
		{
			startsToFall();
		}
	}
	
}

void Entity::doWallJump(float timeStep)
{
	if (get_facing() == "left")
	{
		velocity.first = -maxVelocityX;
		normalizedVelocity.first = -maxVelocityX;
		//_goLeft(timeStep);
	}
	if (get_facing() == "right")
	{
		velocity.first = maxVelocityX;
		normalizedVelocity.first = maxVelocityX;
		//_goRight(timeStep);
	}
	lockMovementY(200);
	jumpLowestPoint = lowestPoint();
	jumpHeight = 200;
	lockMovementX(75);
	startsToJump();
	_jump(timeStep);
	_setWallRidingFalse();
	setWallJumpingTrue();
	if (get_facing() == "left")
	{
		leftIsNotWall();
	}
	if (get_facing() == "right")
	{
		rightIsNotWall();
	}
}

void Entity::itIsOnFloor()
{
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	if (get_type() == EntityType::Enemy);
		//__debugbreak();
	falling = false;
	gravityBool = false;
	gonnaBeOnFloor = true;
	_setWallRidingFalse();
	if (isEntityGotPushedY() && getVelocityY() < 0)
	{
		onFloor = true;
	}
	if (currentAnimationType != "landAnimation" && !this->isOnFloor() && currentAnimationType != "attackAnimation" && currentAnimationType != "idleAnimation")
	{
		currentFrame = landAnimation;
		currentAnimationType = "landAnimation";
	}
	else if(entityType == EntityType::Player && !keyboard[SDL_SCANCODE_UP])
	{
		jumpCondition = true;
		setWallJumpingFalse();
	}
	
}

void Entity::rightIsWall()
{
	wallOnRight = true;
	if (normalizedVelocity.first > 0)
	{
		movementLockX = false;
	}
}

void Entity::leftIsWall()
{
	wallOnLeft = true;
	if (normalizedVelocity.first < 0)
	{
		movementLockX = false;
	}
}

Entity::~Entity()
{
	delete hitbox;
}

void Entity::normalize()
{
	if (entityType == EntityType::Dynamic)
	{
		//normalizedVelocity.first = 0.03f;
	}
		
	if (collidedInXAxis)
	{
		normalizedVelocity.first = 0;
		collidedInXAxis = false;
	}
	if (collidedInYAxis)
	{
		normalizedVelocity.second = 0;
		collidedInYAxis = false;
	}
}

void Entity::move(float timeStep)
{
	if (gonnaBeOnFloor)
	{
		positionY += normalizedVelocity.second * timeStep;
		onFloor = true;
		gonnaBeOnFloor = false;
		normalizedVelocity.second = 0;
		velocity.second = 0;
	}
	else if (!onFloor)
	{
		positionY += (normalizedVelocity.second - (currentAccelerationY * 0.5)) * timeStep;
	}
	positionX += normalizedVelocity.first * timeStep;
	setRectangleWithPos();
	if (collidedInXAxis)
	{
		normalizedVelocity.first = 0;
		collidedInXAxis = false;
	}
	else if (gotPushedX)
	{
		normalizedVelocity.first = 0;
		gotPushedX = false;
	}
	if (collidedInYAxis )
	{
		normalizedVelocity.second = 0;
		collidedInYAxis = false;
	}
	else if (gotPushedY)
	{
		normalizedVelocity.second = 0;
		gotPushedY = false;
	}
	resetXVelocity();
	//normalize();
}


void Entity::setRectangleWithPos()
{
	hitbox->x = positionX;
	hitbox->y = positionY;
}

void Entity::leftIsNotWall()
{
	wallOnLeft = false;
	if (get_facing() == "right")
	{
		_setWallRidingFalse();
	}
}

void Entity::rightIsNotWall()
{
	wallOnRight = false;
	if (get_facing() == "left")
	{
		_setWallRidingFalse();
	}
}

float Entity::getVelocityX() const
{
	return normalizedVelocity.first;
}

float Entity::getVelocityY() const
{
	return normalizedVelocity.second;
}

float Entity::lowestPoint() const
{
	return positionY + hitbox->h;
}

float Entity::highestPoint() const
{
	return positionY;
}

float Entity::mostLeftPoint() const
{
	return positionX;
}

float Entity::mostRightPoint() const
{
	return positionX + hitbox->w;
}

int Entity::getLength() const
{
	return hitbox->h;
}

int Entity::getWidth() const
{
	return hitbox->w;
}

int Entity::getPushConstant() const
{
	return pushConstant;
}

std::string Entity::getCameraLean() const
{
	return cameraLean;
	//returns whether camera is leaned to right or left
}

float Entity::getFrictionCoef() const
{
	return frictionCoefficient;
}

float Entity::getMaxVelocityX() const
{
	return maxVelocityX;
}

void Entity::setVelocityX(float velocity)
{
	normalizedVelocity.first = velocity;
	collidedInXAxis = true;
}

void Entity::setVelocityY(float velocity)
{
	normalizedVelocity.second = velocity;
	currentAccelerationY = 0;
	collidedInYAxis = true;
}

void Entity::_goRight(float timeStep)
{
	velocity.first = maxVelocityX;
	normalizedVelocity.first = maxVelocityX;
}

void Entity::_goLeft(float timeStep)
{
	velocity.first = -maxVelocityX;
	normalizedVelocity.first = -maxVelocityX;
}

void Entity::_gravity(float timeStep)
{
	
	if (gravityBool)
	{
		if (get_type() == EntityType::Enemy)
		{
			pushConstant = 0;
			normalizedVelocity.first = .1f;
			//hitbox->h += 100;
		}

		if (normalizedVelocity.second < 0)
		{
			if (currentAnimationType != "jumpEndingAnimation" && currentAnimationType != "attackAnimation")
			{
				currentFrame = jumpEndingAnimation;
				currentAnimationType = "jumpEndingAnimation";
				normalizedVelocity.second = 0;
			}
			if (abs(normalizedVelocity.second) <= 0.0025)
			{

				normalizedVelocity.second = 0;
				currentAccelerationY = 0;
			}
			if (normalizedVelocity.second < -0.1)
			{
				normalizedVelocity.second += 0.005 * timeStep;
				currentAccelerationY = 0.005;
			}
			else
			{
				normalizedVelocity.second += 0.0005 * timeStep;
				currentAccelerationY = 0.0005;
			}

		}
		else if (!isWallRiding())
		{
			if (_animationComparison("fallAnimation"))
			{
				currentFrame = fallAnimation;
				currentAnimationType = "fallAnimation";
			}
			if (normalizedVelocity.second > 0.1)
			{
				normalizedVelocity.second += 0.005 * timeStep;
				currentAccelerationY = 0.005;
			}
			else
			{
				normalizedVelocity.second += 0.0005 * timeStep;
				currentAccelerationY = 0.0005;
			}

			if (normalizedVelocity.second > 1)
			{
				normalizedVelocity.second = 1;
				currentAccelerationY = 0;
			}
		}
		else if (isWallRiding())
		{
			if (normalizedVelocity.second == 0)
			{
				normalizedVelocity.second = 0.1;
				currentAccelerationY = 0;
			}
			normalizedVelocity.second += 0.0125 * timeStep;
			currentAccelerationY = 0.0125;
			if (normalizedVelocity.second > 0.4)
			{
				normalizedVelocity.second = 0.4;
				currentAccelerationY = 0;
			}
		}


	}
		
}

float Entity::getVelocityVectorX() const
{
	return velocity.first;
}

float Entity::getVelocityVectorY() const
{
	return velocity.second;
}

float Entity::getFrameWidth()
{
	return currentFrame->getFrameWidth();
}

float Entity::getFrameHeight()
{
	return currentFrame->getFrameHeight();
}

float Entity::getFrameGap()
{
	return currentFrame->getHitboxAndFrameGap(get_facing());
}

void Entity::_jump(float timeStep)
{
	if (_animationComparison("jumpAnimation"))
	{
		currentAnimationType = "jumpAnimation";
		currentFrame = jumpAnimation;
	}
	velocity.second = -1 ;// *(timeStep / 10);
	normalizedVelocity.second = -1;// *(timeStep / 10);
	//velocity.second -= 4;
}

void Entity::_setWallRidingTrue()
{
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	wallRiding = true;
	if (keyboard[SDL_SCANCODE_UP])
	{
		//jumpCondition = false;
	}
	else
	{
		setWallJumpingFalse();
	}
	if (currentAnimationType != "wallRideAnimation")
	{
		currentFrame = wallRideAnimation;
		currentAnimationType = "wallRideAnimation";
	}

}

void Entity::_setWallRidingFalse()
{
	wallRiding = false;
	if (getVelocityY() > 0 && _animationComparison("fallAnimation"))
	{
		currentFrame = fallAnimation;
		currentAnimationType = "fallAnimation";
	}
	if ( !isOnFloor()) // if(getVelocityY() != 0)
	{
		jumpCondition = false;
	}
	if (get_facing() == "left")
	{
		_setCameraLeanLeft();
	}
	else if (get_facing() == "right")
	{
		_setCameraLeanRight();
	}
}

bool Entity::_animationComparison(const std::string& candidateAnimationType)
{
	if ( candidateAnimationType == "walkAnimation" && 
		(0 
			|| currentAnimationType == "idleAnimation" 
			|| (isOnFloor() && currentAnimationType == "fallAnimation") ))
			
	{
		return true;
	}
	else if (candidateAnimationType == "jumpAnimation" && 
			(0 
			|| currentAnimationType == "idleAnimation" 
			|| currentAnimationType == "walkAnimation" 
			|| currentAnimationType == "turnAnimation" 
			|| (currentAnimationType == "wallRideAnimation") // && !isWallRiding())
			|| currentAnimationType == "fallAnimation" 
			|| currentAnimationType == "landAnimation"))
	{
		return true;
	}
	else if (candidateAnimationType == "fallAnimation" &&
			(0
			|| currentAnimationType == "idleAnimation" 
			|| currentAnimationType == "walkAnimation" 
			|| currentAnimationType == "turnAnimation"
			|| currentAnimationType == "jumpAnimation"
			|| (currentAnimationType == "wallRideAnimation" && !isWallRiding() )  ) )
	{
		return true;
	}
	else if (candidateAnimationType == "attackAnimation" && 
			(0
			|| currentAnimationType == "idleAnimation" 
			|| currentAnimationType == "walkAnimation" 
			|| currentAnimationType == "jumpAnimation" 
			|| currentAnimationType == "fallAnimation" 
			|| currentAnimationType == "landAnimation"
			|| currentAnimationType == "turnAnimation"
			|| currentAnimationType == "jumpEndingAnimation" ))
	{
		return true;
	}
	else if ((candidateAnimationType == "landAnimation") && 
			(0 
			|| currentAnimationType == "fallAnimation" 
			|| currentAnimationType == "idleAnimation"))
	{
		return true;
	}
	else if (currentAnimationType == "none")
	{
		return true;
	}
	return false;
}

void Entity::_setCameraLeanRight()
{
	cameraLean = "right";
}

void Entity::_setCameraLeanLeft()
{
	cameraLean = "left";
}

void Entity::_decreaseMovementLockY()
{
	if (abs(yPositionBeforeLock - positionY) > lockAmountY)
	{
		std::cout << yPositionBeforeLock << " " << positionY << std::endl;
		unlockMovementY();
	}
}

void Entity::_decreaseMovementLockX()
{
	if (abs(xPositionBeforeLock - positionX) > lockAmountX)
	{
		unlockMovementX();
	}
}

SDL_Texture* Entity::get_texture() const
{
	return (currentFrame) ? currentFrame->getCurrentFrame() : entityTexture;
}

CollisionShape* Entity::getHitboxRect() const
{
	return hitbox;
}

bool Entity::isOnFloor() const
{
	return onFloor;
}

bool Entity::isOnCeiling() const
{
	return onCeiling;
}

bool Entity::isFalling() const
{
	return falling;
}

bool Entity::isWallOnLeft() const
{
	return wallOnLeft;
}

bool Entity::isWallOnRight() const
{
	return wallOnRight;
}

bool Entity::isAbleToJump() const
{
	return jumpCondition;
}

bool Entity::isWallRiding() const
{
	return wallRiding;
}

bool Entity::isMovementLockedY() const
{
	return movementLockY;
}

bool Entity::isMovementLockedX() const
{
	return movementLockX;
}

bool Entity::isAbleToAttack() const
{
	if (currentAnimationType == "attackAnimation")
	{
		return false;
	}
	return true;
}

bool Entity::isEntityGotPushedY() const
{
	return gotPushedY;
}

bool Entity::isEffectedByGravity() const
{
	return effectedByGravity;
}

std::string Entity::get_facing() const
{
	return facing;
}


std::string Entity::getAnimationType() const
{
	return currentAnimationType;
}

Rectangle Entity::attack()
{
	Rectangle attackHitbox;
	if (_animationComparison("attackAnimation"))
	{
		currentAnimationType = "attackAnimation";
		if (attackAnimation != nullptr)
		{
			currentFrame = attackAnimation;
		}
	}
	if (facing == "left")
	{
		attackHitbox.x -= attackHitbox.w;
	}
	else
	{
		attackHitbox.x += attackHitbox.w;
	}
	return attackHitbox;
}

void Entity::resetXVelocity()
{
	if (!isMovementLockedX())
	{
		const Uint8* keyboard = SDL_GetKeyboardState(NULL);

		normalizedVelocity.first = 0;
		if (!(keyboard[SDL_SCANCODE_RIGHT] || keyboard[SDL_SCANCODE_LEFT]) && normalizedVelocity.second == 0 && currentAnimationType != "attackAnimation" && currentAnimationType != "idleAnimation" && currentAnimationType != "landAnimation")
		{
			currentAnimationType = "idleAnimation";
			currentFrame = idleAnimation;
		}
	}

}

void Entity::resetAnimation()
{
	if (normalizedVelocity.second < 0 && currentAnimationType != "attackAnimation")
	{
	currentAnimationType = "jumpAnimation";
	currentFrame = jumpAnimation;
	}
	else if (normalizedVelocity.second > 0 && currentAnimationType != "attackAnimation")
	{
	currentAnimationType = "fallAnimation";
	currentFrame = fallAnimation;
	}
	else if (normalizedVelocity.first != 0 && currentAnimationType != "attackAnimation")
	{
		currentAnimationType = "walkAnimation";
		currentFrame = walkAnimation;
	}
	else
	{
		currentAnimationType = "idleAnimation";
		currentFrame = idleAnimation;
	}
}
void Entity::setJumpConditionFalse()
{
	if (!isMovementLockedY())
	{
		jumpCondition = false;
		jumping = false;
		if (!isOnFloor());
		gravityBool = true;
	}

}

void Entity::setFacingLeft()
{
	facing = "left";
}

void Entity::setFacingRight()
{
	facing = "right";
}

void Entity::setOnCeiling()
{
	onCeiling = true;//setJumpConditionFalse();
	unlockMovementY();
}

void Entity::setOnNotCeiling()
{
	onCeiling = false;
}

void Entity::setWallJumpingTrue()
{
	wallJumping = true;
}

void Entity::setWallJumpingFalse()
{
	wallJumping = false;
}

void Entity::advanceAnimation(float timeStep)
{
	if (currentAnimationType == "idleAnimation")
	{
		if (1);
	}
	if (currentFrame && !delayBetweenFrames)
	{
		currentFrame = currentFrame->getNextAnimation();
		if (currentFrame == nullptr)
		{
			currentAnimationType = "none";
			resetAnimation();
		}
		delayBetweenFrames = (int)90/timeStep;
	}
	else if(currentFrame)
	{
		delayBetweenFrames -= 1;
	}
	
}

void Entity::lockMovementY(int lockedDistance)
{
	yPositionBeforeLock = positionY;
	lockAmountY = lockedDistance;
	movementLockY = true;
}

void Entity::lockMovementX(int lockedDistance)
{
	xPositionBeforeLock = positionX;
	lockAmountX = lockedDistance;
	movementLockX = true;
}

void Entity::unlockMovementY()
{
	movementLockY = false;
	if (!isOnFloor())
	{
		startsToFall();
	}
	
}

void Entity::unlockMovementX()
{
	movementLockX = false;
	resetXVelocity();
}

void Entity::setRideableWallTrue()
{
	rideableWall = true;
}

void Entity::setRideableWallFalse()
{
	rideableWall = false;
}

void Entity::deathEvent()
{
	currentAnimationType = "deathAnimation";
	currentFrame = deathAnimation;
}

void Entity::gotHit(float xVelocity, float yVelocity, float xLock, float yLock, float timeStep)
{
	if (xVelocity != 0)
	{
		velocity.first = xVelocity * (timeStep / 10);
		normalizedVelocity.first = xVelocity * (timeStep / 10);
	}


	if (yVelocity != 0)
	{
		velocity.second = yVelocity * (timeStep / 10);
		normalizedVelocity.second = yVelocity * (timeStep / 10);
	}

	if (xLock > 0)
	{
		lockMovementX(int(xLock) );
	}
	if (yLock > 0)
	{
		lockMovementY(int(yLock) );
	}
	
	
	
}

void Entity::setNewPosition(const Point2f rect)
{
	hitbox->x = rect.x;
	hitbox->y = rect.y;
	positionX = rect.x;
	positionY = rect.y;
}

void Entity::getPushedX(float xVelocity)
{
	normalizedVelocity.first = xVelocity;
	gotPushedX = true;
}

void Entity::getPushedY(float yVelocity)
{
	if (jumping)
		return;
	normalizedVelocity.second = yVelocity;
	gotPushedY = true;
}
