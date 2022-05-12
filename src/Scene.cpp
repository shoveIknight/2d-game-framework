#include "Scene.h"


void Scene::newStaticEntity(Point2f secondPoint)
{
	Entity* new_entity = new Entity;
	int width, height, x, y;
	if (getTopLeftX() > secondPoint.x)
	{
		width = getTopLeftX() - secondPoint.x;
		x = secondPoint.x;
	}
	else
	{
		width = secondPoint.x - getTopLeftX();
		x = getTopLeftX();
	}
	if (getTopLeftY() > secondPoint.y)
	{
		height = getTopLeftY() - secondPoint.y;
		y = secondPoint.y;
	}
	else
	{
		height = secondPoint.y - getTopLeftY();
		y = getTopLeftY();
	}
	new_entity->Init(x, y, width, height, 1);
	new_entity->setType(EntityType::Static);
	addCollisionBody(new_entity);
	resetFirstPoint();
}


void Scene::newCircleEntity(int secondX, int secondY)
{
	Entity* new_entity = new Entity;
	int width, height, x, y;
	if (getTopLeftX() > secondX)
	{
		width = getTopLeftX() - secondX;
		x = secondX;
	}
	else
	{
		width = secondX - getTopLeftX();
		x = getTopLeftX();
	}
	if (getTopLeftY() > secondY)
	{
		height = getTopLeftY() - secondY;
		y = secondY;
	}
	else
	{
		height = secondY - getTopLeftY();
		y = getTopLeftY();
	}
	new_entity->Init(x, y, width, height, 1);
	new_entity->setType(EntityType::Static);
	addCollisionBody(new_entity);
	resetFirstPoint();
}

void Scene::addCollisionBody(Entity* newBody)
{
	collisionBodies.push_back(newBody);
	if (newBody->get_type() == EntityType::Static && isEditorActive())
	{
		Rectangle tmp_rect = *newBody->getHitboxRect();
		int i = int(tmp_rect.x / tileSizeX);
		int j = int((tmp_rect.x + tmp_rect.w) / tileSizeX);
		int k = int(tmp_rect.y / tileSizeY);
		int m = int((tmp_rect.y + tmp_rect.h) / tileSizeY);
		if (j == 4)
			j = 3;
		if (m == 4)
			m = 3;
		while (i < j + 1)
		{
			int tmp_k = k;
			while (tmp_k < m + 1)
			{
				std::cout << i << tmp_k << std::endl;
				splittedCollisionBodies[i][tmp_k].push_back(newBody);
				tmp_k++;
			}
			i++;
		}
	}
	else if (newBody->get_type() != EntityType::Static)
	{
		nonStaticBodies.push_back(newBody);
	}

	if (newBody->highestPoint() < sceneY && newBody->get_type() == EntityType::Static)
	{
		sceneY = newBody->highestPoint();
	}
	if (newBody->mostLeftPoint() < sceneX && newBody->get_type() == EntityType::Static)
	{
		sceneX = newBody->mostLeftPoint();
	}
	if (newBody->mostRightPoint() - sceneX > sceneWidth && newBody->get_type() == EntityType::Static)
	{
		sceneWidth = newBody->mostRightPoint() - sceneX;
		setMaxCamPosX(newBody->mostRightPoint() - getCamWidth());
	}
	if (newBody->lowestPoint() - sceneY > sceneHeight && newBody->get_type() == EntityType::Static)
	{
		sceneHeight = newBody->mostRightPoint() - sceneY;
		setMaxCamPosY(sceneHeight);
	}
}
unsigned int Scene::checkSceneChange(const Rectangle& hitboxRect)
{
	for (auto iter = gatesOut.begin(); iter != gatesOut.end(); iter++)
	{
		Rectangle tmp_rect = (*iter).first;
		if (tmp_rect.x < hitboxRect.x + hitboxRect.getWidht() &&
			tmp_rect.x + tmp_rect.w > hitboxRect.x &&
			tmp_rect.y < hitboxRect.y + hitboxRect.h &&
			tmp_rect.y + tmp_rect.h > hitboxRect.y)
		{
			return (*iter).second;
		}
	}
	return 0;

}
unsigned int Scene::getSceneId()
{
	return sId;
}
std::string Scene::getName()
{
	return nameOFLevel;
}
void Scene::setName(std::string name)
{
	nameOFLevel = name;
}
void Scene::addEnemy(Enemy* newEnemy)
{
	enemies.push_back(newEnemy);
	addCollisionBody(newEnemy->getEntityPointer());
}

void Scene::addGate(std::pair<Rectangle, unsigned int> newGate)
{
	gatesOut.push_back(newGate);
}

void Scene::splitCollisionBodies()
{
	tileSizeX = int(sceneWidth / 4);
	tileSizeY = int(sceneHeight / 4);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			splittedCollisionBodies[i][j].clear();
		}
	}


	for (auto iter = collisionBodies.begin(); iter != collisionBodies.end(); iter++)
	{
		if ((*iter)->get_type() != EntityType::Static)
			continue;
		Rectangle tmp_rect = *(*iter)->getHitboxRect();
		int i = int(tmp_rect.x / tileSizeX);
		int j = int((tmp_rect.x + tmp_rect.w )/ tileSizeX);
		int k = int(tmp_rect.y / tileSizeY);
		int m = int((tmp_rect.y + tmp_rect.h) / tileSizeY);
		if (j == 4)
			j = 3;
		if (m == 4)
			m = 3;
		while (i < j + 1)
		{
			int tmp_k = k;
			while (tmp_k < m +1)
			{
				std::cout << i << tmp_k << std::endl;
				splittedCollisionBodies[i][tmp_k].push_back(*iter);
				tmp_k++;
			}
			i++;
		}

	}

}

void Scene::addToDynamicBodies(Entity* newDynamicEntity)
{
	nonStaticBodies.push_back(newDynamicEntity);
	std::sort(nonStaticBodies.begin(), nonStaticBodies.end(),
		[](Entity* const& lhs, Entity* const& rhs)
		{
			return lhs->getPushConstant() < rhs->getPushConstant();
		}
	);
}

std::pair<Point2f, Point2f> Scene::findGateIn(unsigned int prevSceneID)
{
	for (auto iter = gatesIn.begin(); iter != gatesIn.end(); iter++)
	{
		if (iter->first == prevSceneID)
		{
			return iter->second;
		}
	}
	return std::pair<Point2f, Point2f>();
}

Rectangle Scene::setTheScene(unsigned int prevSceneID, Rectangle playerRect)
{
	std::pair<Point2f, Point2f > positions = findGateIn(prevSceneID);
	setNewCamPosition(positions.first.x, positions.first.y);
	playerRect.x = positions.second.x;
	playerRect.y = positions.second.y;
	return playerRect;
}

void Scene::deleteSelected()
{
	deleteEntity(getSelectedEntity());
	setSelectedEntityFalse();
}

void Scene::deleteEntity(Entity* entityToBeDeleted)
{
	for (auto iter = collisionBodies.begin(); iter != collisionBodies.end(); iter++)
	{
		if (*iter == entityToBeDeleted)
		{
			collisionBodies.erase(iter);
			break;
		}
	}
	splitCollisionBodies();
}

void Scene::deleteEnemy(Enemy* enemyToBeDeleted)
{
	for (auto iter = enemies.begin(); iter != enemies.end(); iter++)
	{
		if (*iter == enemyToBeDeleted)
		{
			enemies.erase(iter);
			break;
		}
	}
}

Node* Scene::getRoot()
{
	return nullptr;
}

int Scene::getSize()
{
	return collisionBodies.size();
}

std::vector<Entity*>::iterator Scene::getCollisionBodiesBegin()
{
	return collisionBodies.begin();
}

std::vector<Entity*>::iterator Scene::getCollisionBodiesEnd() 
{
	return collisionBodies.end();
}

std::vector<Entity*> Scene::bodiesAtArea(Rectangle tmp_rect)
{
	int i = int(tmp_rect.x / tileSizeX);
	int j = int((tmp_rect.x + tmp_rect.w) / tileSizeX);
	int k = int(tmp_rect.y / tileSizeY);
	int m = int((tmp_rect.y + tmp_rect.h) / tileSizeY);

	if (m == 4)
		m = 3;
	if (j == 4)
		j = 3;
	std::vector<Entity*> tmpBodies;
	while (i < j + 1)
	{
		int tmp_k = k;
		while (tmp_k < m + 1)
		{
			tmpBodies.insert(tmpBodies.end(), splittedCollisionBodies[i][tmp_k].begin(), splittedCollisionBodies[i][tmp_k].end());
			tmp_k++;
		}
		i++;
	}
	tmpBodies.insert(tmpBodies.end(), nonStaticBodies.begin(), nonStaticBodies.end());
	return tmpBodies;
}

std::vector<Enemy*>::iterator Scene::getEnemiesBegin()
{
	return enemies.begin();
}

std::vector<Enemy*>::iterator Scene::getEnemiesEnd()
{
	return enemies.end();
}

std::vector<std::pair<Rectangle, unsigned int>>::iterator Scene::gatesOutBegin()
{
	return gatesOut.begin();
}

std::vector<std::pair<Rectangle, unsigned int>>::iterator Scene::gatesOutEnd()
{
	return gatesOut.end();
}

std::vector<std::pair<unsigned int, std::pair<Point2f, Point2f>>>::iterator Scene::gatesInBegin()
{
	return gatesIn.begin();
}

std::vector<std::pair<unsigned int, std::pair<Point2f, Point2f>>>::iterator Scene::gatesInEnd()
{
	return gatesIn.end();
}

std::vector<Entity*> &Scene::getNonStaticBodies()
{
	return nonStaticBodies;
}

float Scene::getSceneX() const
{
	return sceneX;
}

float Scene::getSceneY() const
{
	return sceneY;
}

float Scene::getSceneWidth() const
{
	return sceneWidth;
}

float Scene::getSceneHeight() const
{
	return sceneHeight;
}



std::istream& operator>>(std::istream& is, Scene& scene)
{
	float x, y, width, length;
	is >> x >> y >> width >> length;
	while (!(x == 0 && y == 0 && width == 0 && length == 0))
	{
		Entity* new_entity = new Entity;
		new_entity->Init(x, y, width, length , 1);
		new_entity->setType(EntityType::Static);
		scene.addCollisionBody(new_entity);
		is >> x >> y >> width >> length;
	}
	Rectangle rect = { 6800 + 30, 664, 30, 30 };
	std::pair<Rectangle, unsigned int> pair;
	pair.first = rect;
	pair.second = 1;
	scene.addGate(pair);
	return is;
}

std::ostream& operator<<(std::ostream& os, Scene& scene)
{
	auto iter_begin = scene.getCollisionBodiesBegin();
	auto iter_end = scene.getCollisionBodiesEnd();
	for (auto iter = iter_begin; iter != iter_end; ++iter)
	{
		if ((*iter)->get_type() != EntityType::Static)
		{
			continue;
		}
		Rectangle tmp_rect = *(*iter)->getHitboxRect();
		os << tmp_rect.x << " " << tmp_rect.y << " " << tmp_rect.w << " " << tmp_rect.h << "\n";
	}
	
	os << "0" << " " << "0" << " " << "0" << " " << "0" << "\n";

	auto iter_begin1 = scene.gatesOutBegin();
	auto iter_end1 = scene.gatesOutEnd();
	for (auto iter = iter_begin1; iter != iter_end1; ++iter)
	{
		Rectangle tmp_rect = iter->first;
		os << tmp_rect.x << " " << tmp_rect.y << " " << tmp_rect.w << " " << tmp_rect.h << " " << iter->second << "\n";
	}

	os << "0" << " " << "0" << " " << "0" << " " << "0" << "\n";

	auto iter_begin2 = scene.gatesInBegin();
	auto iter_end2 = scene.gatesInEnd();
	for (auto iter = iter_begin2; iter != iter_end2; ++iter)
	{
		os << iter->first << " " << iter->second.first.x << " " << iter->second.first.y << " " << iter->second.second.x << " " << iter->second.second.y << "\n";
	}

	os << "0" << " " << "0" << " " << "0" << " " << "0" << "\n";

	return os;
}