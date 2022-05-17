#include "enemy.h"
#include <iostream>

void Enemy::Init(float x, float y, SDL_Renderer* const& renderer)
{
	main_object->Init(x, y);
	main_object->setType(EntityType::Enemy);
	main_object->loadAnimations(renderer);
}

void Enemy::tempCircleTest(float x, float y, float radius, SDL_Renderer* const& renderer)
{
	main_object->InitCircle(x, y, radius, 0);
	main_object->setType(EntityType::Enemy);
	Animation* head = new Animation;
	head->setFrame("circle1.png", renderer, 0, 0);
	head->setNextAnimation(head);
	main_object->setAnimation(head, WallRideAnim);
	main_object->setAnimation(head, TurnAnim);
	main_object->setAnimation(head, JumpEndingAnim);
	main_object->setAnimation(head, LandAnim);
	main_object->setAnimation(head, FallAnim);
	main_object->setAnimation(head, JumpAnim);
	main_object->setAnimation(head, AttackAnim);
	main_object->setAnimation(head, IdleAnim);
	main_object->setAnimation(head, WalkAnim);
	//main_object->loadAnimations(renderer);
}

void Enemy::updateEnemy(float timeStep, const CollisionShape& playerHitbox)
{
	//main_object->resetXVelocity();
	main_object->normalize();
	if(main_object->mostLeftPoint() < 600)
	{
		//main_object->doJump(timeStep);
	}
	
	if (main_object->mostLeftPoint() < 3600)
	{
		main_object->moveRight(timeStep);
	}
}

void Enemy::applyGravity(float timeStep)
{
	main_object->applyGravity(timeStep);
}

Entity& Enemy::getEntity()
{
	return *main_object;
}

void Enemy::move(float timeStep)
{
	main_object->move(timeStep);
}

void Enemy::gotHit(int damage,const  std::string& direction, float timeStep)
{
	health -= damage;
	if (health <= 0) 
	{
		main_object->deathEvent();
	}
	if (direction == "left")
	{
		main_object->gotHit(5, 0, 20, 0, timeStep);
	}
	else if (direction == "right")
	{
		main_object->gotHit(-5, 0, 20, 0, timeStep);
	}
	

}
/*
bool Enemy::isOnFloor() const
{
	return main_object->isOnFloor();
}

bool Enemy::isFalling() const
{
	return main_object->isFalling();
}

bool Enemy::isWallOnLeft() const
{
	return main_object->isWallOnLeft();
}

bool Enemy::isWallOnRight() const
{
	return main_object->isWallOnRight();
}

float Enemy::getVelocityX() const
{
	return main_object->getVelocityX();
}

float Enemy::getVelocityY() const
{
	return main_object->getVelocityY();
}

float Enemy::lowestPoint() const
{
	return main_object->lowestPoint();
}

float Enemy::highestPoint() const
{
	return main_object->highestPoint();
}

float Enemy::mostLeftPoint() const
{
	return main_object->mostLeftPoint();
}

float Enemy::mostRightPoint() const
{
	return main_object->mostRightPoint();
}

float Enemy::getVelocityVectorX() const
{
	return main_object->getVelocityVectorX();
}

float Enemy::getVelocityVectorY() const
{
	return main_object->getVelocityVectorY();
}*/

Entity* Enemy::getEntityPointer()
{
	return main_object;
}

CollisionShape* Enemy::getHitboxRect()
{
	return main_object->getHitboxRect();
}

bool Enemy::isDied()
{
	if (health <= 0)
	{
		return true;
	}
	return false;
}

bool Enemy::shouldBeDeleted()
{

	//Check if death animation is over and health is below zero.
	if (1 && health <= 0)
	{
		return true;
	}
	return false;
}

