#pragma once
#include <algorithm>
#include <chrono>
#include <fstream>
#include <list>
#include <iostream>
#include <thread>
#include <vector>
#include <utility>

#include "Camera.h"
#include "Enemy.h"
#include "Entity.h"
#include "Renderer.h"
#include "Scene.h"
#include "SceneEditor.h"
#include "Animation.h"
#include "Tree.h"

class SideScroller
{
public:
	void Init();
	void Quit();
	void resetXVelocity();
	void goRight();
	void goLeft();
	void jump();
	void update();
	void set_draw_hitboxes_true();
	void setJumpConditionFalse();
	void editorModeToggle();
	void increaseScale();
	void decreaseScale();
	void resetScale();
	void attack();
	void newStaticEntity(int x, int y);
	void saveLevel();
	void loadLevel();
	void deleteSelectedEntity();
	void pauseOrUnPause();
	void slideEntity(int x, int y);
	void setEntityDynamic();

private:
	void collisionThread(std::vector<Entity*>, Entity&, bool&, bool&, bool&, bool&, std::pair<std::pair<float, int>, std::pair<float, int>>& );
	void pauseGame();
	void unPauseGame();
	void drawPauseMenu();
	void update_enemies();
	void move_entities();
	void move_enemies();
	void draw_enemies();
	void gravity();
	void move();
	void draw_entity(Entity&);
	void draw();
	void clear();
	void delay(int);
	std::chrono::steady_clock::time_point getTime();
	bool hitRegister(Rectangle, std::string);
	bool RectCollision(CollisionShape*, CollisionShape*, float xVelocity, float yVelocity);

	bool _collisionChecker(CollisionShape *entity, CollisionShape *worldObject, float& xVelocity, float& yVelocity, Point2f& retVelocity);
	bool _basicRectCollision(Rectangle rect1, Rectangle rect2) const;
	bool _rectCircCollision(CollisionShape* worldRect, CollisionShape* hitboxCircle, float& xVelocity, float& yVelocity, Point2f& retVelocity);
	bool collisionCheck(Entity& current_player, bool secondIteration = false);
	bool collision_helper(Rectangle, float, float);

private:
	int avgFps, numberOfFrames;
	Tree deneme;
	float timeStep;
	std::string gameState = "running";
	std::chrono::steady_clock::time_point previousFrameTime;
	Scene* currentScene = new Scene;
	Scene scene;
	std::vector<Scene*> scenes;
	std::pair<float, float> center;
	std::vector<Entity*> collisionBodies;
	float last_center = 9999;
	Renderer newGame;
	Entity* player1 = new Entity;
	std::vector<Enemy*> enemies;
	Camera mainCamera;
	bool draw_hitboxes = false;
};

