#pragma once
#include <vector>
#include "Entity.h"

class Enemy
{
public:
	/**
	* Init the Enemy object.
	* \param x x position of the object when it first created
	* \param y y position of the object when it first created
	* \param renderer The renderer where you want to render the object afterwards.
	*/
	void Init(float x, float y, SDL_Renderer* const& renderer);

	void tempCircleTest(float x, float y, float radius, SDL_Renderer* const& renderer);

	/**
	* Main update function to execute enemy logic.
	* \param timeStep time step between frames.
	* \param playerHitbox CollisionShape of enemey hitbox.
	*/
	void updateEnemy(float timeStep, const CollisionShape& playerHitbox);
	void applyGravity(float timeStep);
	Entity& getEntity();

	/**
	* Virtual function for Entity::Move look at Entity::Move for more detailed information.
	* \param timeStep time step between frames.
	*/
	void move(float timeStep);

	/**
	* Response for getting hit.
	* \param damage Damage dealt by the hit.
	* \param direction Direction of the hit( i.e. where it comes from)
	* \param timeStep time step between frames.
	*/
	void gotHit(int damage, const  std::string& direction, float timeStep);


	Entity* getEntityPointer();
	CollisionShape* getHitboxRect();

	/**
	* Checks if enemy ran out of health points.
	* 
	* \returns Returns true if enemy ran out of health points. False otherwise.
	*/
	bool isDied();

	/**
	* Check if enemy should be deleted on next update or not.
	*
	* \returns Returns true if enemy ran out of health points and death animation is over. False otherwise.
	*/
	bool shouldBeDeleted();

private:

private:
	Entity *main_object = new Entity;
	std::string type = "none";
	std::vector<std::pair<float, float>> patrolingRoute;
	float baseSpeed;
	int waitingTimeAtEachPoint;
	std::string currentState;
	int health = 3000;
};

