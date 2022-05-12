#include "SceneEditor.h"

float SceneEditor::getSceneScale() const
{
    return sceneScale;
}

void SceneEditor::increaseScale()
{
    sceneScale += 0.05;
}

void SceneEditor::decreaseScale()
{
    sceneScale -= 0.05;
}

void SceneEditor::activate()
{
    isActive = true;
}

void SceneEditor::deactivate()
{
    isActive = false;
}

void SceneEditor::resetScale()
{
    sceneScale = 1;
}

void SceneEditor::setSelectedEntityFalse()
{
    entitySelected = false;
    selectedEntity = nullptr;
}

void SceneEditor::slideSelectedEntity(int x, int y)
{
    if (isEntitySelected())
    {
        CollisionShape tmp_rect = *selectedEntity->getHitboxRect();
        tmp_rect.x += x - oldMousePosX;
        tmp_rect.y += y - oldMousePosY;
        selectedEntity->setNewPosition({ tmp_rect.x, tmp_rect.y });
        oldMousePosX = x;
        oldMousePosY = y;
    }
}

void SceneEditor::setSelectedEntityDynamic()
{
    if (isEntitySelected())
    {
        selectedEntity->setType(EntityType::Dynamic);
    }
}

bool SceneEditor::isEditorActive() const
{
    return isActive;
}

bool SceneEditor::isFirstPointRecorded() const
{
    return firstPointRecorded;
}

int SceneEditor::getTopLeftX() const
{
    return firstX;
}

int SceneEditor::getTopLeftY() const
{
    return firstY;
}

void SceneEditor::setFirstPoint(int x, int y)
{
    firstX = x;
    firstY = y;
    firstPointRecorded = true;
}

Rectangle SceneEditor::scaleRect(Rectangle rect)
{
    rect.x = (rect.x) / sceneScale;
    rect.y = (rect.y) / sceneScale;
    rect.w /= sceneScale;
    rect.h /= sceneScale;
    return rect;
}

void SceneEditor::resetFirstPoint()
{
    firstX = 0;
    firstY = 0;
    firstPointRecorded = false;
}

bool SceneEditor::isPointerOnCollisionBody(int x, int y, std::vector<Entity*>::iterator bodiesBegin, std::vector<Entity*>::iterator bodiesEnd)
{
    for (; bodiesBegin != bodiesEnd; bodiesBegin++)
    {
        if ((*bodiesBegin)->get_type() != EntityType::Static)
        {
            continue;
        }
        CollisionShape tmpHitBox = *(*bodiesBegin)->getHitboxRect();
        if (x > tmpHitBox.x &&
            x < tmpHitBox.x + tmpHitBox.w &&
            y > tmpHitBox.y &&
            y < tmpHitBox.y + tmpHitBox.h)
        {
            setSelectedEntity(*bodiesBegin);
            oldMousePosX = x;
            oldMousePosY = y;
            slideBody = true;
            return true;
        }
    }
    return false;
}

bool SceneEditor::isEntitySelected() const
{
    return entitySelected;
}


Entity* SceneEditor::getSelectedEntity() const
{
    return selectedEntity;
}

void SceneEditor::setSelectedEntity(Entity* entity)
{
    selectedEntity = entity;
    entitySelected = true;
}