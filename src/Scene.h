#pragma once
#include <algorithm>
#include <fstream>
#include <limits>
#include <iostream>
#include <vector>
#include "Entity.h"
#include "Enemy.h"
#include "Camera.h"
#include "SceneEditor.h"
#include "Tree.h"

struct ParallaxLayers
{
	SDL_Texture* farBackground = nullptr,
		* mainBackground = nullptr,
		* closeBackground = nullptr,
		* foreGround = nullptr;
};

class Scene : public SceneEditor, public Camera
{
public:
	/**
	 * @brief Adds new rectangular static object to the scene represented by first recorded point and point given as parameter.
	 * 
	 * @param secondPoint The other point that will be used to construct the rectangle.
	 */
	void newStaticEntity(Point2f secondPoint);
	void newCircleEntity(int secondX, int secondY);
	/**
	* @brief Adds the given Entity to scene's bodies.
	*/
	void addCollisionBody(Entity* newBody);
	// @brief add new Enemy to scene's enemies.
	void addEnemy(Enemy* newEnemy);

	// @brief deletes selected scene Entity.
	void deleteSelected();

	// @brief deletes the entity given as parameter
	// @param entityToBeDeleted the entity you want to delete from the scene.
	void deleteEntity(Entity* entityToBeDeleted);

	/**
	* @brief delete the given enemy from scene's enemies.
	* 
	* @param enemyToBeDeleted the entity you want to delete from the scene's enemies.
	*/
	void deleteEnemy(Enemy* enemyToBeDeleted);

	/**
	* @brief Adds a new gate that teleports the player to the level(scene) that paired with uint newGate.second when collided with rectangle newGate.first.
	* 
	* @param newGate Rectangle unsigned int pair. 
	*/
	void addGate(std::pair<Rectangle, unsigned int> newGate);

	//split all of the static bodies in the scene into 16 different chunks(vectors) to minimize comparison when checking collisions.
	void splitCollisionBodies();

	/**
	* @brief Add new dynamic entity to scene.
	* 
	* @param newDynamicEntity pointer to the entity you want to add to the scene.
	*/
	void addToDynamicBodies(Entity* newDynamicEntity);

	/**
	* @brief Find the corresponding gate from given scene ID.
	* 
	* @param prevSceneID The ID of previous scene that player teleported from.
	* 
	* @return Returns Point2f pair first Point2f corresponds to new position of the cam and the scond Point2f is the point that player will start in the new scenes.
	*/
	std::pair<Point2f, Point2f > findGateIn(unsigned int prevSceneID);

	/**
	* @brief Prepares the position of Cam and returns the new hitbox rectangle for player.
	* 
	* @param prevSceneID previous scene's ID as unsigned int type.
	* @param playerRect hitbox rectangle of the player.
	* 
	* @return Returns the new rectangle for the player.
	*/
	Rectangle setTheScene(unsigned int prevSceneID, Rectangle playerRect);

	/**
	* Checks if scene change needs to happen.
	* @param hitboxRect Hitbox rectangle of the player.
	* 
	* @return Returns the ID of the new scene if scene change should happen, returns 0 otherwise.
	*/
	unsigned int checkSceneChange(const Rectangle& hitboxRect);


	unsigned int getSceneId();
	std::string getName();
	void setName(std::string);
	Node* getRoot();
	int getSize();

	std::vector<Entity*>::iterator getCollisionBodiesBegin();
	std::vector<Entity*>::iterator getCollisionBodiesEnd();

	std::vector<Entity*> bodiesAtArea(Rectangle);

	std::vector<Enemy*>::iterator getEnemiesBegin();
	std::vector<Enemy*>::iterator getEnemiesEnd();

	std::vector<std::pair<Rectangle, unsigned int>>::iterator gatesOutBegin();
	std::vector<std::pair<Rectangle, unsigned int>>::iterator gatesOutEnd();

	std::vector<std::pair<unsigned int, std::pair<Point2f, Point2f > >>::iterator gatesInBegin();
	std::vector<std::pair<unsigned int, std::pair<Point2f, Point2f > >>::iterator gatesInEnd();

	std::vector<Entity*>& getNonStaticBodies();
	
	float getSceneX() const;
	float getSceneY() const;
	float getSceneWidth() const;
	float getSceneHeight() const;

	friend std::istream& operator>>(std::istream& is, Scene& scene);
	friend std::ostream& operator<<(std::ostream& os, Scene& scene);
private:
	
	//Camera mainCamera;
	std::string nameOFLevel = "";
	std::vector<std::pair<Rectangle, unsigned int>> gatesOut;
	std::vector<std::pair<unsigned int, std::pair<Point2f, Point2f > >> gatesIn;
	std::vector<Entity*> nonStaticBodies;
	std::vector<Enemy*> enemies;
	std::vector<Entity*> collisionBodies;
	std::vector<Entity*> splittedCollisionBodies[4][4];
	ParallaxLayers layersOfScene;
	int tileSizeX, tileSizeY;
	float sceneX = 0, 
		sceneWidth = 0,
		sceneHeight = 0,
		sceneY = 0;
	unsigned int sId;
};

