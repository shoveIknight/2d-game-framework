#pragma once
#include <fstream>
#include <iostream>
#include <SDL.h>
#include <vector>
#include "Entity.h"

class SceneEditor
{
public:
	float getSceneScale() const;
	bool isEditorActive() const;
	bool isFirstPointRecorded() const;
	int getTopLeftX() const;
	int getTopLeftY() const;

	void setFirstPoint(int, int);
	Rectangle scaleRect(Rectangle);
	void resetFirstPoint();
	bool isPointerOnCollisionBody(int, int, std::vector<Entity*>::iterator, std::vector<Entity*>::iterator);
	bool isEntitySelected() const;
	Entity* getSelectedEntity() const;
	void setSelectedEntity(Entity*);
	void increaseScale();
	void decreaseScale();
	void activate();
	void deactivate();
	void resetScale();
	void setSelectedEntityFalse();
	void slideSelectedEntity(int x, int y);
	void setSelectedEntityDynamic();

private:
	float sceneScale = 1.0,
		oldMousePosX,
		oldMousePosY;
	bool isActive = false,
		entitySelected = false,
		firstPointRecorded = false,
		slideBody = false;
	int firstX, firstY;
	Entity* selectedEntity;
};

