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
	 * @brief Adds new rectangular static object to the scene represented by first recorded point and two.
	 * 
	 */
	void newStaticEntity(Point2f secondPoint);
	void newCircleEntity(int secondX, int secondY);
	void addCollisionBody(Entity*);
	void addEnemy(Enemy*);
	void deleteSelected();
	void deleteEntity(Entity*);
	void deleteEnemy(Enemy*);
	void addGate(std::pair<Rectangle, unsigned int>);
	void splitCollisionBodies();
	void addToDynamicBodies(Entity* newDynamicEntity);
	std::pair<Point2f, Point2f > findGateIn(unsigned int);
	Rectangle setTheScene(unsigned int, Rectangle);
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

