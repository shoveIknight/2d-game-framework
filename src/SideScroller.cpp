#include "SideScroller.h"

//Return wheter given entity object collide with other collision bodies
//in world
bool SideScroller::collisionCheck(Entity& current_player, bool secondIteration)
{
	bool tmp = false,
		break_state = false,
		change_back = false,
		wallOnLeft = false,
		wallOnRight = false,
		wall_on_top = false,
		wall_on_bottom = false,
		pushing = false,
		collided_with_same_object = false;
	std::vector<Entity*> tmpEntities;
	std::vector<float> tmpDistanceDiff;
	tmpEntities = currentScene->bodiesAtArea(*current_player.getHitboxRect());
	//auto lastElement = currentScene->getCollisionBodiesEnd();

	//auto firstElement = currentScene->getCollisionBodiesBegin();
	auto lastElement = tmpEntities.end();
	auto firstElement = tmpEntities.begin();
	std::vector<Entity*> entityToMove;
	Entity* entityOnBottom = nullptr;
	std::pair<std::pair<float, int>, std::pair<float, int>> return_pair;

	//for some special occasions we call collisionCheck again 
	//sometimes we set a velocity just before the collisionCheck call
	//in those cases we do not want it to be resetted
	if (!secondIteration)
	{
		current_player.normalize();
	}

	int length = current_player.getLength();
	int width = current_player.getWidth();
	EntityType cEntityType = EntityType::None;
	float x_velocity = current_player.getVelocityX() * timeStep;
	float y_velocity = current_player.getVelocityY() * timeStep;
	float current_x_position = current_player.mostLeftPoint();
	float current_y_position = current_player.highestPoint();
	float next_x_position = current_x_position + x_velocity;
	float next_y_position = current_y_position + y_velocity;
	EntityType currentPlayerType = current_player.get_type();
	CollisionShape* currentEntityHitbox = current_player.getHitboxRect();
	CollisionShape nextEntityHitbox = *currentEntityHitbox; //delete after
	Point2f ret = { 0 , 0 };

	nextEntityHitbox.x += x_velocity;
	nextEntityHitbox.y += y_velocity;

	for (auto iter = firstElement; iter != lastElement; iter++)
	{
		bool cant_collide_from_right = false,
			cant_collide_from_left = false,
			cant_collide_with_y = false,
			cant_collide_from_top = false,
			cant_collide_from_bottom = false,
			already_collided = false;
		CollisionShape worldHitbox = *(*iter)->getHitboxRect(); //delete after
		EntityType collidedEntityType = (*iter)->get_type();
		//we dont want to compare object with itself
		//until we introduce unique entity IDs
		//we check that like this
		if (currentEntityHitbox->x == worldHitbox.x &&
			currentEntityHitbox->y == worldHitbox.y &&
			currentEntityHitbox->h == worldHitbox.h &&
			currentEntityHitbox->w == worldHitbox.w)
		{
			continue;

		}
		//if the x pos of the iter object 
		//is lower than our current object's x pos
		//than we pass that object when checking
		//collision from right and vice versa for left
		if (  (nextEntityHitbox.getShape() == Rect && x_velocity >= 0 && 0.3f < current_x_position + width - worldHitbox.x)
			|| (nextEntityHitbox.getShape() == Circ && x_velocity >= 0 && 0.3f < current_x_position + currentEntityHitbox->getRadius() - worldHitbox.x)     )
		{

			cant_collide_from_right = true;

		}
		if (nextEntityHitbox.getShape() == Rect && x_velocity <= 0 && worldHitbox.x + worldHitbox.w - current_x_position > 0.3f
			|| nextEntityHitbox.getShape() == Circ && x_velocity <= 0 && worldHitbox.x + worldHitbox.w - (current_x_position + currentEntityHitbox->getRadius()) > 0.3f)
		{

			cant_collide_from_left = true;

		}
		if (x_velocity == 0)
		{

			//if object stationary 
			//we give it a temp velocity to check left and right collsions	

			if (!cant_collide_from_right)
			{
				CollisionShape tmpHitbox = nextEntityHitbox;
				tmpHitbox.x += 0.1;

				//if the object on right or left is pushable
				//than we do not consider it as a wall unless  
				//there is a wall on the right or left of that pushable object

				if (_collisionChecker(&worldHitbox, &tmpHitbox, x_velocity, y_velocity, ret) &&
					(current_player.getPushConstant() <= (*iter)->getPushConstant() || (*iter)->isWallOnRight()))
				{

					wallOnRight = true;
					cEntityType = collidedEntityType;

				}
			}
			if (!cant_collide_from_left)
			{
				CollisionShape tmpHitbox = nextEntityHitbox;
				tmpHitbox.x -= 0.1;
				if (_collisionChecker(&worldHitbox, &tmpHitbox, x_velocity, y_velocity, ret) &&
					(current_player.getPushConstant() <= (*iter)->getPushConstant() || (*iter)->isWallOnLeft()))
				{
					wallOnLeft = true;
					cEntityType = collidedEntityType;
				}
			}
		}
		//if the y pos of the iter object 
		//is lower than our current object's y pos
		//than we pass that object when checking
		//collision from top and vice versa for bottom
		if (   (nextEntityHitbox.getShape() == Rect && y_velocity >= 0 && current_y_position - worldHitbox.y > 0.3f)
			|| (nextEntityHitbox.getShape() == Circ && (y_velocity > 0 && current_y_position + nextEntityHitbox.getRadius() - worldHitbox.y > 0.3f
				|| (cant_collide_from_right && cant_collide_from_left && y_velocity >= 0 && current_y_position - worldHitbox.y > 0.3f) )   ) )
		{
			cant_collide_from_bottom = true;
		}
		if (  (nextEntityHitbox.getShape() == Rect &&  y_velocity <= 0 && 0.3f < worldHitbox.y + worldHitbox.h - current_y_position)
			|| (nextEntityHitbox.getShape() == Circ && y_velocity <= 0 && (0.3f < worldHitbox.y + worldHitbox.h - (current_y_position + currentEntityHitbox->getRadius() )
				|| (cant_collide_from_right && cant_collide_from_left&& y_velocity <= 0 && 0.3f < worldHitbox.y + worldHitbox.h - current_y_position)      )    )  )
		{
			cant_collide_from_top = true;
		}
		//after checking with previous pos of world entity wheter it is collideable
		// from 4 sides
		//after that we add its speed so we can check
		//if it would collide after that
		worldHitbox.x += (*iter)->getVelocityX() * timeStep;
		worldHitbox.y += (*iter)->getVelocityY() * timeStep;

		if (abs(y_velocity) < 0.05) // was always true
		{
			//if object stationary 
			//we give it a temp velocity to check top and bottom collsions

			if (!cant_collide_from_top)
			{
				//if the object on top or bottom is pushable
				//than we do not consider it as a wall unless  
				//there is a wall on the top or bottom of that pushable object
				CollisionShape tmpHitbox = nextEntityHitbox;
				tmpHitbox.y -= 0.1;
				if (_collisionChecker(&worldHitbox, &tmpHitbox, x_velocity, y_velocity, ret) &&
					(current_player.getPushConstant() < (*iter)->getPushConstant() || (*iter)->isOnCeiling()))
				{
					wall_on_top = true;
				}
			}
			if (!cant_collide_from_bottom)
			{
				CollisionShape tmpHitbox = nextEntityHitbox;
				tmpHitbox.y += 0.1 * timeStep;
				if (_collisionChecker(&worldHitbox, &tmpHitbox, x_velocity, y_velocity, ret) &&
					(current_player.getPushConstant() < (*iter)->getPushConstant() || (*iter)->isOnFloor()))
				{
					wall_on_bottom = true;
					entityOnBottom = *iter;
				}
			}
		}
		if (cant_collide_from_right && cant_collide_from_left && cant_collide_from_top && cant_collide_from_bottom)
		{
			//we skip that world object if our current entity cannot collide in any shape or form
			continue;
		}
		if (_collisionChecker(&worldHitbox, currentEntityHitbox, x_velocity, y_velocity, ret)) // basic rectangle collision check
		{
			if (currentEntityHitbox->getShape() == Circ)
			{
				if (!cant_collide_from_right)
				{
					if (ret.x > 0 && (return_pair.first.second == 0 || abs(return_pair.first.first) > abs(ret.x)))
					{
						return_pair.first.first = ret.x;
						return_pair.first.second = 1;
					}
				}
				if (!cant_collide_from_left)
				{
					if (ret.x < 0 && (return_pair.first.second == 0 || abs(return_pair.first.first) > abs(ret.x)))
					{
						return_pair.first.first = ret.x;
						return_pair.first.second = -1;
					}
				}

				if (!cant_collide_from_bottom)
				{
					if (ret.y > 0 && (return_pair.second.second == 0 || abs(return_pair.second.first) > abs(ret.y)))
					{
						return_pair.second.first = ret.y;
						return_pair.second.second = 1;
					}
				}
				if (!cant_collide_from_top)
				{
					if (ret.y < 0 && (return_pair.second.second == 0 || abs(return_pair.second.first) > abs(ret.y)))
					{
						return_pair.second.first = ret.y;
						return_pair.second.second = -1;
					}
				}

			}
			else if (currentEntityHitbox->getShape() == Rect)
			{
				//if the world object can be collided in x axis
				if (!cant_collide_from_right || !cant_collide_from_left)
				{
					if (x_velocity > 0 && !cant_collide_from_right)
					{
						//we calculate the distance between the will-be-collided world object 
						//and our current entity
						float tmp_float = worldHitbox.x - (current_x_position + (float)width);
						// then we check if there exist any other collision
						//if so we need to decide which is the first entity our current entity will collide
						if ((return_pair.first).first == 0 || abs(tmp_float) < abs((return_pair.first).first))
						{
							already_collided = true;
							(return_pair.first).first = tmp_float;
							(return_pair.first).second = 1;
							//check if current entity can push the collided object
							//if if that's the case instead of stopping current entity we push the other
							if (current_player.getPushConstant() > (*iter)->getPushConstant())
							{
								entityToMove.push_back(*iter);
								pushing = true;
							}
							else
							{
								pushing = false;
								entityToMove.push_back(*iter);
							}

						}
					}
					else if (x_velocity < 0 && !cant_collide_from_left)
					{
						float tmp_float = worldHitbox.x + worldHitbox.w - current_x_position;
						if ((return_pair.first).first == 0 || abs(tmp_float) < abs((return_pair.first).first))
						{
							already_collided = true;
							(return_pair.first).first = tmp_float;
							(return_pair.first).second = -1;
							if (current_player.getPushConstant() > (*iter)->getPushConstant())
							{
								entityToMove.push_back(*iter);
								pushing = true;
							}
							else
							{
								pushing = false;
							}

						}
					}
				}

				if (!cant_collide_from_bottom || !cant_collide_from_top)
				{
					if (y_velocity > 0 && !cant_collide_from_bottom)
					{

						float tmp_float = worldHitbox.y - current_y_position - length;
						tmpDistanceDiff.push_back(tmp_float);
						if ((return_pair.second).first == 0 || abs(tmp_float) <= abs((return_pair.second).first))
						{
							if (already_collided == true)
							{
								collided_with_same_object = true;
							}
							else if (already_collided == false)
							{
								collided_with_same_object = false;
							}
							if (current_player.getPushConstant() > (*iter)->getPushConstant() || secondIteration)
							{
								entityToMove.push_back(*iter);
								pushing = true;
							}
							else
							{
								pushing = false;
							}
							entityOnBottom = *iter;
							(return_pair.second).first = tmp_float;
							(return_pair.second).second = 1;
						}
					}
					else if (y_velocity < 0 && !cant_collide_from_top)
					{
						float tmp_float = worldHitbox.y + worldHitbox.h - current_y_position;
						tmpDistanceDiff.push_back(tmp_float);
						if ((return_pair.second).first == 0 || abs(tmp_float) <= abs((return_pair.second).first))
						{
							if (current_player.getPushConstant() > (*iter)->getPushConstant() || secondIteration)
							{
								entityToMove.push_back(*iter);
								pushing = true;
							}
							else
							{
								pushing = false;
							}
							(return_pair.second).first = tmp_float;
							(return_pair.second).second = -1;

						}
					}
				}
			}

		}
	}

	if (secondIteration && wall_on_top)
	{
		//__debugbreak();
	}


	if (wallOnRight)
	{
		current_player.rightIsWall();
		//set the wallRide according to the type of the collided object
		if (cEntityType != EntityType::Static && cEntityType != EntityType::Dynamic)
		{
			current_player.setRideableWallFalse();
		}
		else
		{
			current_player.setRideableWallTrue();
			if (cEntityType == EntityType::Enemy)
			{
				__debugbreak();
			}
		}
	}
	else
	{
		current_player.rightIsNotWall();
	}
	if (wallOnLeft)
	{
		current_player.leftIsWall();
		if (cEntityType != EntityType::Static && cEntityType != EntityType::Static)
		{
			current_player.setRideableWallFalse();
		}
		else
		{
			if (cEntityType == EntityType::Enemy)
			{
				__debugbreak();
			}
			current_player.setRideableWallTrue();
		}
	}
	else
	{
		current_player.leftIsNotWall();
	}

	if (y_velocity == 0 || current_player.isEntityGotPushedY())
	{
		if (current_player.isEntityGotPushedY() && y_velocity < 0)
		{
			current_player.itIsOnFloor();
		}
		if (wall_on_top)
		{
			current_player.setOnCeiling();
		}
		else
		{
			current_player.setOnNotCeiling();
		}
		if (wall_on_bottom)
		{
			//if the object on bottom is moving
			//the object on top should move with that object
			if (entityOnBottom != nullptr && (entityOnBottom->getVelocityX() != 0 || entityOnBottom->getVelocityY() != 0) && secondIteration == false)
			{
				current_player.setVelocityX(entityOnBottom->getVelocityX() * entityOnBottom->getFrictionCoef() + current_player.getVelocityX());
				collisionCheck(current_player, true);
			}
			current_player.itIsOnFloor();

		}
		else
		{
			current_player.startsToFall();
		}
	}

	if (nextEntityHitbox.getShape() == Circ && (return_pair.second.second != 0 || return_pair.first.second != 0))
	{
		current_player.setVelocityY(return_pair.second.first / timeStep);
		current_player.setVelocityX(return_pair.first.first / timeStep);
		if (return_pair.second.second == -1)
		{
			current_player.collidedFromTop();
		}
		else if (return_pair.second.second == 1)
		{
			current_player.itIsOnFloor();
		}
		if (return_pair.first.second == 1)
		{
			current_player.rightIsWall();
		}
		else if (return_pair.first.second == -1)
		{
			current_player.leftIsWall();
		}
		if (return_pair.first.second || return_pair.second.second)
		{
			return true;
		}
		return false;
	}





	if (return_pair.second.second != 0 && return_pair.first.second != 0 &&
		abs(x_velocity) != 0 && abs(y_velocity) != 0
		&& abs(return_pair.first.first) / abs(x_velocity) > abs(return_pair.second.first) / abs(y_velocity))
	{
		current_player.setVelocityY(return_pair.second.first / timeStep);
		if (return_pair.second.second == 1)
		{
			current_player.itIsOnFloor();
		}
		else
		{
			current_player.collidedFromTop();
		}
		if (collided_with_same_object == false)
		{
			current_player.setVelocityX(return_pair.first.first / timeStep);
			if (return_pair.first.second == 1)
			{
				current_player.rightIsWall();
			}
			else if (return_pair.first.second == -1)
			{
				current_player.leftIsWall();
			}
		}



	}
	else if (return_pair.second.second != 0 && return_pair.first.second != 0
		&& abs(x_velocity) != 0 && abs(y_velocity) != 0
		&& abs(return_pair.first.first) / abs(x_velocity) < abs(return_pair.second.first) / abs(y_velocity))
	{
		current_player.setVelocityX(return_pair.first.first / timeStep);
		if (return_pair.first.second == 1)
		{
			current_player.rightIsWall();
		}
		else if (return_pair.first.second == -1)
		{
			current_player.leftIsWall();
		}

		if (collided_with_same_object == false)
		{
			current_player.setVelocityY(return_pair.second.first / timeStep);
			if (return_pair.second.second == 1)
			{
				current_player.itIsOnFloor();
			}
			else
			{
				current_player.collidedFromTop();
			}
		}


	}
	else if (return_pair.first.second == 0 && return_pair.second.second != 0)
	{
		if (pushing)
		{
			auto entityIter = entityToMove.begin();
			auto distanceIter = tmpDistanceDiff.begin();
			for (; entityIter != entityToMove.end(), distanceIter != tmpDistanceDiff.end(); entityIter++, distanceIter++)
			{
				(*entityIter)->getPushedY(current_player.getVelocityY() - (*distanceIter) / timeStep);
				bool tmpCheck = collisionCheck(*(*entityIter), true);
				if ((y_velocity > 0 && (*entityIter)->isOnFloor()) || (y_velocity < 0 && (*entityIter)->isOnCeiling()))
				{
					current_player.setVelocityY(return_pair.second.first / timeStep);
					if (return_pair.second.second == 1)
					{
						current_player.itIsOnFloor();
					}
					else
					{
						current_player.collidedFromTop();
					}
				}
				if (return_pair.first.second == -1)
				{
					(*entityIter)->itIsOnFloor();
				}
				else if (return_pair.first.second == 1)
				{
					(*entityIter)->collidedFromTop();
				}
			}


		}
		else
		{
			current_player.setVelocityY(return_pair.second.first / timeStep);
			if (return_pair.second.second == 1)
			{
				if (entityOnBottom != nullptr && (entityOnBottom->getVelocityX() != 0 || entityOnBottom->getVelocityY() != 0) && secondIteration == false)
				{
					current_player.setVelocityX(entityOnBottom->getVelocityX() * entityOnBottom->getFrictionCoef() + current_player.getVelocityX());
					collisionCheck(current_player, true);
				}

				current_player.itIsOnFloor();
			}
			else if (return_pair.first.second == -1)
			{
				current_player.collidedFromTop();
			}
		}


	}
	else if (return_pair.second.second == 0 && return_pair.first.second != 0)
	{
		if (pushing)
		{
			for (auto iter = entityToMove.begin(); iter != entityToMove.end(); iter++)
			{
				(*iter)->getPushedX(current_player.getVelocityX() - return_pair.first.first / timeStep);
				bool tmpCheck = collisionCheck(*(*iter), true);
				if ((x_velocity > 0 && (*iter)->isWallOnRight()) || (x_velocity < 0 && (*iter)->isWallOnLeft()))
				{
					current_player.setVelocityX(return_pair.first.first / timeStep);
					if (return_pair.first.second == 1)
					{
						current_player.rightIsWall();
					}
					else if (return_pair.first.second == 1)
					{
						current_player.leftIsWall();
					}
				}
				if (return_pair.first.second == -1)
				{
					(*iter)->rightIsWall();
				}
				else if (return_pair.first.second == 1)
				{
					(*iter)->leftIsWall();
				}
			}

		}
		else
		{
			current_player.setVelocityX(return_pair.first.first / timeStep);
			if (return_pair.first.second == 1)
			{
				current_player.rightIsWall();
			}
			else if (return_pair.first.second == -1)
			{
				current_player.leftIsWall();
			}
		}


	}
	if (return_pair.first.second || return_pair.second.second)
	{
		return true;
	}
	return false;
}


bool SideScroller::_collisionChecker(CollisionShape *worldHitbox, CollisionShape *entityHitbox, float& xVelocity, float& yVelocity, Point2f &retVelocity)
{
	if (entityHitbox->getShape() == 0 && worldHitbox->getShape() == 0)
	{
		return RectCollision(worldHitbox, entityHitbox, xVelocity, yVelocity);
	}
	else if (entityHitbox->getShape() == 1 && worldHitbox->getShape() == 0)
	{
		return _rectCircCollision(worldHitbox, entityHitbox, xVelocity, yVelocity, retVelocity);
	}
}

bool SideScroller::RectCollision(CollisionShape* worldRect, CollisionShape* hitboxRect, float xVelocity, float yVelocity)
{
	Rectangle aabbRect = hitboxRect;
	if (yVelocity < 0)
	{
		aabbRect.y += yVelocity;
		aabbRect.h -= yVelocity;
	}
	else if (yVelocity > 0)
	{
		aabbRect.h += yVelocity;
	}


	if (xVelocity < 0)
	{
		aabbRect.x += xVelocity;
		aabbRect.w -= xVelocity;
	}
	else if (xVelocity > 0)
	{
		aabbRect.w += xVelocity;
	}


	if (_basicRectCollision(worldRect, aabbRect) == false)
	{
		return false;
	}
	else if (xVelocity == 0 || yVelocity == 0)
	{
		return true;
	}

	std::cout << "xV: " << xVelocity << " yV: " << yVelocity;
	if ((xVelocity > 0 && yVelocity < 0) || (xVelocity < 0 && yVelocity > 0))
	{
		
		if ( (worldRect->y + worldRect->h ) - ((yVelocity / xVelocity) * (worldRect->x + worldRect->w - hitboxRect->x) + hitboxRect->y) >= 0.1f
			&&  0.1f <= ( (yVelocity / xVelocity) * (worldRect->x - (hitboxRect->x + hitboxRect->w) ) + ( hitboxRect->y + hitboxRect->h ) ) - (worldRect->y) )
		{
			std::cout << "sadasdasd" << yVelocity << std::endl;
			return true;
		}
		else
		{
			return false;
		}

	}	

	else if (xVelocity > 0 && yVelocity > 0 || xVelocity < 0 && yVelocity < 0)
	{
		if ( (worldRect->y + worldRect->h ) >= (yVelocity / xVelocity) * (worldRect->x - (hitboxRect->x + hitboxRect->w) ) + hitboxRect->y
			&& (worldRect->y) <= (yVelocity / xVelocity) * ( (worldRect->x + worldRect->w) - (hitboxRect->x)) + (hitboxRect->y + hitboxRect->h))
		{
			return true;
		}
		else
		{
			return false;
		}

	}
	return false;
}

bool SideScroller::_rectCircCollision(CollisionShape* worldRect, CollisionShape* hitboxCircle, float& xVelocity, float& yVelocity, Point2f& retVelocity)
{
	float radius = hitboxCircle->getRadius();
	Rectangle aabbRect(hitboxCircle->x, hitboxCircle->y, 2 * radius, 2 * radius);
	if (yVelocity < 0)
	{
		aabbRect.y += yVelocity;
		aabbRect.h -= yVelocity;
	}
	else if (yVelocity > 0)
	{
		aabbRect.h += yVelocity;
	}
	if (xVelocity < 0)
	{
		aabbRect.x += xVelocity;
		aabbRect.w -= xVelocity;
	}
	else if (xVelocity > 0)
	{
		aabbRect.w += xVelocity;
	}
	if (_basicRectCollision(worldRect, aabbRect) == false)
	{
		return false;
	}
	float centerX = hitboxCircle->x + hitboxCircle->getRadius();
	float centerY = hitboxCircle->y + hitboxCircle->getRadius();
	Point2f closestPoint = Math::closestPointToRect({ centerX, centerY }, worldRect);

	if (abs(xVelocity)  < 0.01 || abs(yVelocity) < 0.01)
	{
		if (closestPoint.x == centerX && abs(yVelocity) > 0.01)
		{
			if (closestPoint.y > centerY)
			{
				retVelocity.y = closestPoint.y - (centerY + radius);
				return true;
			}
			else
			{
				retVelocity.y = closestPoint.y - (centerY - radius);
				return true;
			}

		}
		else if (closestPoint.y == centerY && abs(xVelocity) > 0.01)
		{
			if (closestPoint.x > centerX)
			{
				retVelocity.x = closestPoint.x - (centerX + radius);
				return true;
			}
			else
			{
				retVelocity.x = closestPoint.x - (centerX - radius);
				return true;
			}
			
		}
		if (abs(xVelocity) > 0.01)
		{
			if (xVelocity > 0 && centerX > worldRect->x)
			{
				Point2f contactPoint = { centerX + Math::getPointOnCircle(hitboxCircle, closestPoint.y - centerY), closestPoint.y };
				retVelocity.x = closestPoint.x - contactPoint.x;
				return true;

			}
			else if (xVelocity < 0 && centerX < worldRect->x)
			{
				Point2f contactPoint = { centerX - Math::getPointOnCircle(hitboxCircle, closestPoint.y - centerY), closestPoint.y };
				retVelocity.x = closestPoint.x - contactPoint.x;
				return true;

			}
		}
		else if (abs(yVelocity) > 0.01)
		{
			if (yVelocity > 0)
			{
				Point2f contactPoint = { closestPoint.x, centerY + Math::getPointOnCircle(hitboxCircle, closestPoint.x - centerX)};
				retVelocity.y = closestPoint.y - contactPoint.y;
				return true;

			}
			else if (yVelocity < 0)
			{
				Point2f contactPoint = { centerX - Math::getPointOnCircle(hitboxCircle, closestPoint.y - centerY), closestPoint.y };
				retVelocity.y = closestPoint.y - contactPoint.y;
				return true;
			}
		}

	}
	else
	{
		float mVelocity = -yVelocity / xVelocity;
		if (   (closestPoint.x == centerX &&   (abs( (worldRect->w + worldRect->x - closestPoint.x ) / xVelocity ) > abs(abs(closestPoint.y - centerY) - radius) / yVelocity)   && abs(yVelocity) > abs(abs(closestPoint.y - centerY) - radius))     
			|| (abs(mVelocity * (abs(closestPoint.x - centerX ) - radius) ) >= abs((centerY - closestPoint.y)) && abs(xVelocity) > abs(abs(closestPoint.x - centerX) - radius) )  )
		{

			retVelocity.y = closestPoint.y - (centerY);
			if (retVelocity.y < 0)
			{
				retVelocity.y += radius;
			}
			else if (retVelocity.y > 0)
			{
				retVelocity.y -= radius;
			}
			retVelocity.x = retVelocity.y / -mVelocity;
			return true;
		}

		else if ( abs(centerY - closestPoint.y) - radius >= abs( mVelocity * (closestPoint.x - centerX)  )  )
		{
			retVelocity.x = closestPoint.x - centerX;
			if (retVelocity.x > 0)
			{
				retVelocity.x -= radius;
			}
			else if (retVelocity.x < 0)
			{
				retVelocity.x += radius;
			}
			retVelocity.y = retVelocity.x * -mVelocity;
			return true;





		}

		else
		{
			Point2f contactPoint = Math::lineCircleIntersect(xVelocity, yVelocity, closestPoint, hitboxCircle);
			retVelocity.x = closestPoint.x - contactPoint.x;
			retVelocity.y = closestPoint.y - contactPoint.y;
			return true;
		}
	}


}

bool SideScroller::_basicRectCollision(Rectangle rect1, Rectangle rect2) const
{
	if (rect1.x < rect2.x + rect2.w &&
		rect1.x + rect1.w > rect2.x &&
		rect1.y < rect2.y + rect2.h &&
		rect1.h + rect1.y > rect2.y)
	{
		return true;
	}
	return false;
}

//Checks whether given rectangle collide with any other collidable objects in world or not
bool SideScroller::collision_helper(Rectangle hitboxRect, float v_x, float v_y)
{
	hitboxRect.x += v_x;
	hitboxRect.y += v_y;

	for (auto iter = currentScene->getCollisionBodiesBegin(); iter != currentScene->getCollisionBodiesEnd(); iter++)
	{
		Rectangle tmp_rect = (*iter)->getHitboxRect();
		if (tmp_rect.x < hitboxRect.x + hitboxRect.w &&
			tmp_rect.x + tmp_rect.w > hitboxRect.x &&
			tmp_rect.y < hitboxRect.y +hitboxRect.h &&
			tmp_rect.y + tmp_rect.h > hitboxRect.y)
		{
			return true;
		}

	}
	return false;
}



void SideScroller::collisionThread(std::vector<Entity*> worldBodies, Entity& current_player , bool& wallOnRight, bool& wallOnLeft, bool& wall_on_top, bool& wall_on_bottom, std::pair<std::pair<float, int>, std::pair<float, int>>& return_pair)
{
	
	int length = current_player.getLength();
	int width = current_player.getWidth();
	float x_velocity = current_player.getVelocityX(), y_velocity = current_player.getVelocityY();
	float current_x_position = current_player.mostLeftPoint(),
		current_y_position = current_player.highestPoint(),
		next_x_position = current_x_position + x_velocity,
		next_y_position = current_y_position + y_velocity;
	Rectangle currentEntityRect = current_player.getHitboxRect();
	Rectangle nextEntityRect = currentEntityRect;
	for (auto iter = worldBodies.begin(); iter != worldBodies.end(); ++iter)
	{
		Rectangle	worldRect = (*iter)->getHitboxRect();
		bool cant_collide_from_right = false,
			cant_collide_from_left = false,
			cant_collide_with_y = false,
			cant_collide_from_top = false,
			cant_collide_from_bottom = false;
		if (currentEntityRect.x == worldRect.x &&
			currentEntityRect.y == worldRect.y &&
			currentEntityRect.h == worldRect.h &&
			currentEntityRect.w == worldRect.w)
		{
			return;
		}

		if (x_velocity >= 0 && worldRect.x < current_x_position + width)
		{
			cant_collide_from_right = true;
			//continue;
		}
		if (x_velocity <= 0 && worldRect.x + worldRect.w > current_x_position)
		{
			cant_collide_from_left = true;
			//continue;
		}
		if (x_velocity == 0)
		{
			if (!cant_collide_from_right)
			{
				Rectangle tmpRect = nextEntityRect;
				tmpRect.x += 0.1 * (timeStep / 10);
				//if (RectCollision(worldRect, tmpRect))
				{
					wallOnRight = true;
				}
			}
			if (!cant_collide_from_left)
			{
				Rectangle tmpRect = nextEntityRect;
				tmpRect.x -= 0.1 * (timeStep / 10);
				//if (RectCollision(worldRect, tmpRect))
				{
					wallOnLeft = true;
				}
			}
		}
		if (y_velocity > 0 && current_y_position + length > worldRect.y)
		{
			cant_collide_from_bottom = true;
			//continue;
		}
		if (y_velocity < 0 && current_y_position < worldRect.y + worldRect.h)
		{
			cant_collide_from_top = true;
		}
		if (y_velocity == 0)
		{
			if (!cant_collide_from_top)
			{
				Rectangle tmpRect = nextEntityRect;
				tmpRect.y -= 0.1 * (timeStep / 10);
				//if (RectCollision(worldRect, tmpRect))
				{
					wall_on_top = true;
				}
			}
			if (!cant_collide_from_bottom)
			{
				Rectangle tmpRect = nextEntityRect;
				tmpRect.y += 1 * (timeStep / 10);
				//if (RectCollision(worldRect, tmpRect))
				{
					wall_on_bottom = true;
				}
			}
		}
		if (cant_collide_from_right && cant_collide_from_left && cant_collide_from_top && cant_collide_from_bottom)
		{
			return;
		}

		//if (RectCollision(worldRect, nextEntityRect))
		{
			if (!cant_collide_from_right || !cant_collide_from_left)
			{
				if (x_velocity > 0 && !cant_collide_from_right)
				{
					float tmp_float = worldRect.x - (current_x_position + (float)width);
					if ((return_pair.first).first == 0 || abs(tmp_float) < abs((return_pair.first).first))
					{
						(return_pair.first).first = tmp_float;
						(return_pair.first).second = 1;
					}
				}
				else if (x_velocity < 0 && !cant_collide_from_left)
				{
					float tmp_float = worldRect.x + worldRect.w - current_x_position;
					if ((return_pair.first).first == 0 || abs(tmp_float) < abs((return_pair.first).first))
					{
						(return_pair.first).first = tmp_float;
						(return_pair.first).second = -1;
					}
				}
			}

			if (!cant_collide_from_bottom || !cant_collide_from_top)
			{
				if (y_velocity > 0 && !cant_collide_from_bottom)
				{
					float tmp_float = worldRect.y - current_y_position - length;
					if ((return_pair.second).first == 0 || abs(tmp_float) < abs((return_pair.second).first))
					{
						(return_pair.second).first = tmp_float;
						(return_pair.second).second = 1;
					}
				}
				else if (y_velocity < 0 && !cant_collide_from_top)
				{
					float tmp_float = worldRect.y + worldRect.h - current_y_position;
					if ((return_pair.second).first == 0 || abs(tmp_float) < abs((return_pair.second).first))
					{
						(return_pair.second).first = tmp_float;
						(return_pair.second).second = -1;
					}
				}
			}
		}
	}
}


void SideScroller::Init()
{
	previousFrameTime = getTime();
	player1->Init(3500, 50, 30,100);
	player1->setType(EntityType::Player);
	newGame.Init();
	loadLevel();
	for (int i = 3600; i < 3600; i += 50)
	{
		//Enemy* new_enemy = new Enemy;
		//new_enemy->tempCircleTest(i+400, 50, 24, newGame.getRenderer());
		//currentScene->addEnemy(new_enemy);
		Enemy* new_enemy = new Enemy;
		new_enemy->Init(i+600, 50, newGame.getRenderer());
		currentScene->addEnemy(new_enemy);
	}
	player1->loadAnimations(newGame.getRenderer());
	currentScene->addCollisionBody(player1);
	currentScene->setNewFocusedEntity(player1);
}


void SideScroller::draw()
{
	CollisionShape background = Rectangle( -1000, -1000, 5000, 5000 );
	newGame.setDrawColor(0, 100, 0, 0xFF);
	newGame.drawRect(background);
	for (auto iter = currentScene->getCollisionBodiesBegin(); iter != currentScene->getCollisionBodiesEnd(); iter++) 
	{
		if (((*iter)->mostLeftPoint() > currentScene->getCamX() + newGame.getScreenWidth() + 15 || (*iter)->mostRightPoint() < currentScene->getCamX() - 15) && !currentScene->isEditorActive() )
		{
			continue;
		}
		if(!(*iter)->isEffectedByGravity())
		{
			Rectangle dstRect = (*iter)->getHitboxRect();
			dstRect.x -= currentScene->getCamX();
			dstRect.y -= currentScene->getCamY();
			if (currentScene->isEditorActive())
			{
				dstRect = currentScene->scaleRect(dstRect);
			}


			newGame.setDrawColor(0, 128, 128, 0xFF);
			newGame.drawRect(dstRect);
		}

	}
	draw_entity(*player1);
	draw_enemies();
	if (currentScene->isEditorActive() && currentScene->isFirstPointRecorded())
	{
		int secondX, secondY;
		SDL_GetMouseState(&secondX, &secondY);
		secondX += currentScene->getCamX();
		secondY += currentScene->getCamY();
		int firstX = (currentScene->getRecordedPointX() - currentScene->getCamX()) / currentScene->getSceneScale() + currentScene->getCamX();
		int firstY = (currentScene->getRecordedPointY() - currentScene->getCamY()) / currentScene->getSceneScale() + currentScene->getCamY();
		int width= 0, height = 0, x = 0, y = 0;
		if (firstX > secondX)
		{
			width = firstX - secondX;
			x = secondX;
		}
		else
		{
			width = secondX - firstX;
			x = firstX;
		}
		if (firstY > secondY)
		{
			height = firstY - secondY;
			y = secondY;
		}
		else
		{
			height = secondY - firstY;
			y = firstY;
		}
		x -= currentScene->getCamX();																		   
		y -= currentScene->getCamY();

		Rectangle dstRect(x, y, width, height);

		newGame.setDrawColor(0, 128, 128, 120);
		newGame.drawRect(dstRect);
	}
	if (currentScene->isEditorActive() && currentScene->isEntitySelected())
	{
		Entity* selectedEntity = currentScene->getSelectedEntity();
		Rectangle selectedRect = selectedEntity->getHitboxRect();
		selectedRect.h += 1;
		selectedRect.w += 1;
		selectedRect.x -= currentScene->getCamX();
		selectedRect.y -= currentScene->getCamY();
		selectedRect = currentScene->scaleRect(selectedRect);
		SDL_FRect tmpRect = { selectedRect.x, selectedRect.y, selectedRect.w, selectedRect.h };
		newGame.setDrawColor(0xff, 0, 0, 0xff);
		//newGame.drawRect(selectedRect);
		SDL_RenderDrawRectF(newGame.getRenderer(), &tmpRect);
	}
	float width = 0;
	if (currentScene->isEditorActive())
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				Rectangle selectedRect = { i*currentScene->getSceneWidth()/4  , -400 + i * currentScene->getSceneHeight() / 4
					,(j+1)* currentScene->getSceneWidth() / 4  ,(j + 1)* currentScene->getSceneHeight() / 4 };
				selectedRect.x -= currentScene->getCamX();
				selectedRect.y -= currentScene->getCamY();
				selectedRect = currentScene->scaleRect(selectedRect);
				SDL_SetRenderDrawColor(newGame.getRenderer(), 0xff, 0, 0, 0xff);
				SDL_FRect tmpRect = { selectedRect.x, selectedRect.y, selectedRect.w, selectedRect.h };
				SDL_RenderDrawRectF(newGame.getRenderer(), &tmpRect);
			}
		}
	}

}

void SideScroller::clear()
{
	newGame.clear();
}

void SideScroller::delay(int n)
{
	newGame.delay(n);
}

void SideScroller::Quit()
{
	saveLevel();
	newGame.quit();
}

void SideScroller::draw_enemies()
{
	for (auto iter = currentScene->getEnemiesBegin(); iter != currentScene->getEnemiesEnd(); iter++)
	{
		
		if (((*iter)->getEntityPointer()->mostLeftPoint() > currentScene->getCamX() + newGame.getScreenWidth() + 15 || (*iter)->getEntityPointer()->mostRightPoint() < currentScene->getCamX() - 15) && !currentScene->isEditorActive())
		{
			(*iter)->getEntity().advanceAnimation(timeStep);
			continue;
		}
		else
		{
			draw_entity((*iter)->getEntity());
		}
	}
		
	
}

void SideScroller::draw_entity(Entity& current_entity)
{
	Rectangle dstRect = current_entity.getHitboxRect(); //add or substract from x and y to adjust the orientation of hitbox and texture															   //substracting from x makes texture to go left with respect to hitbox and vice versa for adding
	dstRect.x -= currentScene->getCamX();																	   //substracting from y makes texture to go upward with respect to hitbox and vice versa for adding
	dstRect.y -= currentScene->getCamY();

	if (current_entity.get_texture() != NULL)
	{
		float heightGap = current_entity.getFrameHeight() - dstRect.h;
		dstRect.w = current_entity.getFrameWidth();
		dstRect.h = current_entity.getFrameHeight();
		if (current_entity.get_facing() == "right")
		{
			dstRect.x -= current_entity.getFrameGap(); 
			dstRect.y -= heightGap;
			if (currentScene->isEditorActive())
			{
				dstRect = currentScene->scaleRect(dstRect);
			}
			SDL_FRect tmpRect = { dstRect.x, dstRect.y, dstRect.w, dstRect.h };
			SDL_RenderCopyExF(newGame.getRenderer(), current_entity.get_texture(), NULL, &tmpRect, 0, NULL, SDL_FLIP_NONE);
		}
		else if (current_entity.get_facing() == "left")
		{
			dstRect.x -= current_entity.getFrameGap();
			dstRect.y -= heightGap;
			if (currentScene->isEditorActive())
			{
				dstRect = currentScene->scaleRect(dstRect);
			}
			SDL_FRect tmpRect = { dstRect.x, dstRect.y, dstRect.w, dstRect.h };
			SDL_RenderCopyExF(newGame.getRenderer(), current_entity.get_texture(), NULL, &tmpRect, 0, NULL, SDL_FLIP_HORIZONTAL);
		}
		current_entity.advanceAnimation(timeStep);
	}	
	else
	{
		if (currentScene->isEditorActive())
		{
			dstRect = currentScene->scaleRect(dstRect);
		}
		SDL_SetRenderDrawColor(newGame.getRenderer(), 0xff, 0, 0, 0xff);
		SDL_FRect tmpRect = { dstRect.x, dstRect.y, dstRect.w, dstRect.h };
		SDL_RenderFillRectF(newGame.getRenderer(), &tmpRect);
	}
	if (draw_hitboxes)
	{
		CollisionShape dstRect = *current_entity.getHitboxRect();
		dstRect.x -= currentScene->getCamX();
		dstRect.y -= currentScene->getCamY();
		if (currentScene->isEditorActive())
		{
			dstRect = currentScene->scaleRect(dstRect);
		}
		SDL_SetRenderDrawColor(newGame.getRenderer(), 0, 0, 0xff, 120);
		SDL_FRect tmpRect = { dstRect.x, dstRect.y, dstRect.w, dstRect.h };

		SDL_RenderFillRectF(newGame.getRenderer(), &tmpRect);
		
		SDL_SetRenderDrawBlendMode(newGame.getRenderer(), SDL_BLENDMODE_BLEND);
	}

}

void SideScroller::move_enemies()
{
	for (auto iter = currentScene->getEnemiesBegin(); iter != currentScene->getEnemiesEnd(); iter++)
	{
		Entity tmp = (*iter)->getEntity();
		tmp.normalize();
		collisionCheck((*iter)->getEntity());
		(*iter)->move(timeStep);
		tmp.normalize();
	}
}

void SideScroller::drawPauseMenu()
{
	SDL_FRect menu = { newGame.getScreenWidth() / 2 - newGame.getScreenWidth() * 0.1 , newGame.getScreenHeight() / 2 - newGame.getScreenHeight() * 0.1, newGame.getScreenWidth() * 0.2, newGame.getScreenHeight() * 0.2  };
	SDL_SetRenderDrawColor(newGame.getRenderer(), 0xff, 0, 0, 0xff);
	SDL_RenderFillRectF(newGame.getRenderer(), &menu);

}

void SideScroller::update_enemies()
{
	std::vector<Enemy*> tmpEnemyVector;
	for (auto iter = currentScene->getEnemiesBegin(); iter != currentScene->getEnemiesEnd(); ++iter)
	{
		(*iter)->updateEnemy(timeStep, *player1->getHitboxRect());
		collisionCheck((*iter)->getEntity());
		if ((*iter)->shouldBeDeleted())
		{
			tmpEnemyVector.push_back(*iter);
		}
	}
	for (auto iter = tmpEnemyVector.begin(); iter != tmpEnemyVector.end(); ++iter)
	{
		currentScene->deleteEnemy(*iter);
	}
}

void SideScroller::move_entities()
{
	for (auto iter = currentScene->getNonStaticBodies().begin(); iter != currentScene->getNonStaticBodies().end(); ++iter)
	{
		if ( (*iter)->get_type() == EntityType::Dynamic || (*iter)->get_type() == EntityType::Enemy)
		{
			(*iter)->normalize();
			collisionCheck(*(*iter));
		}
	}
	for (auto iter = currentScene->getNonStaticBodies().begin(); iter != currentScene->getNonStaticBodies().end(); ++iter)
	{
		if ((*iter)->get_type() == EntityType::Dynamic || (*iter)->get_type() == EntityType::Enemy)
		{
			(*iter)->move(timeStep);
			(*iter)->normalize();
		}
	}
}

void SideScroller::setJumpConditionFalse()
{
	player1->setJumpConditionFalse();
}

void SideScroller::move()
{

	player1->normalize();
	collisionCheck(*player1);
	//update_enemies();
	move_entities();
	currentScene->updateCamera(newGame.getScreenWidth(), newGame.getScreenHeight() , timeStep);
	player1->move(timeStep);
}

void SideScroller::update()
{
	
	timeStep = std::chrono::duration<float, std::milli>(getTime() - previousFrameTime).count();
	previousFrameTime = getTime();
	numberOfFrames++;
	avgFps += (int)(1000 / timeStep);
	if (currentScene->isEntitySelected())
	{
		//std::cout << player1->getHitboxRect().y - currentScene->getSelectedEntity()->getHitboxRect().y << 
			//" posY: " << player1->getHitboxRect().y;
	}
	 //<< "avg : " << int(avgFps / numberOfFrames) << " fps : " << (int)(1000 / timeStep) << std::endl;
	if (gameState == "running")
	{

		
		move();
		if (currentScene->checkSceneChange(player1->getHitboxRect()))
		{
			unsigned int nextSceneId= currentScene->checkSceneChange(player1->getHitboxRect());
			//saveLevel();
			//loadLevel(name);
			unsigned int prevSceneID = currentScene->getSceneId();
			currentScene = scenes[nextSceneId];
			Rectangle newPlayerRect  = currentScene->setTheScene(prevSceneID, player1->getHitboxRect());
			player1->setNewPosition({ newPlayerRect.x, newPlayerRect.y });
		}
		gravity();
		clear();
		draw();


	}
	else if (gameState == "paused")
	{
		drawPauseMenu();
	}
	auto tmpTime = getTime();
	newGame.updateRender();
	//std::cout <<	"avg : " << int(avgFps / numberOfFrames) << " isOnFloor: ";
	if(player1->isOnFloor())
	std::cout<< player1->isOnFloor() << "yVelocity: " << player1->getVelocityY() << std::endl;
		//<< " drawing takes " << std::chrono::duration<float, std::milli>(getTime() - tmpTime).count() / timeStep << " ms" << std::endl;
	delay((1000 / 20) );
}
void SideScroller::resetXVelocity()
{
	player1->resetXVelocity();
}

void SideScroller::goRight()
{
	player1->moveRight(timeStep);
}

void SideScroller::goLeft()
{
	player1->moveLeft(timeStep);
}

void SideScroller::gravity()
{
	player1->applyGravity(timeStep);
	for (auto iter = currentScene->getCollisionBodiesBegin(); iter != currentScene->getCollisionBodiesEnd(); ++iter)
	{
		//if((*(*iter)).get_type() != "static" && (*(*iter)).get_type() != "dynamic")
		(*(*iter)).applyGravity(timeStep);
	}
}

void SideScroller::jump()
{
	player1->doJump(timeStep);
}


void SideScroller::set_draw_hitboxes_true()
{
	if (draw_hitboxes)
		draw_hitboxes = false;
	else
		draw_hitboxes = true;
}


void SideScroller::editorModeToggle()
{
	if (currentScene->isEditorActive())
	{
		currentScene->deactivate();
	}
	else
	{
		currentScene->activate();
	}
	
}

void SideScroller::increaseScale()
{
	currentScene->increaseScale();
}

void SideScroller::decreaseScale()
{
	currentScene->decreaseScale();
}

void SideScroller::resetScale()
{
	currentScene->resetScale();
}

void SideScroller::attack()
{
	if (player1->isAbleToAttack())
	{
		Rectangle attackBox = player1->attack();
		if (player1->get_facing() == "left")
		{
			hitRegister(attackBox, "right");
		}
		else if (player1->get_facing() == "right")
		{
			hitRegister(attackBox, "left");
		}
	}

}

bool SideScroller::hitRegister(Rectangle hitboxRect, std::string direction)
{
	for (auto iter = currentScene->getEnemiesBegin(); iter != currentScene->getEnemiesEnd(); ++iter)
	{
		CollisionShape enemyRect = *(*iter)->getHitboxRect();
		if (enemyRect.x < hitboxRect.x + hitboxRect.w &&
			enemyRect.x + enemyRect.w > hitboxRect.x &&
			enemyRect.y < hitboxRect.y + hitboxRect.h &&
			enemyRect.y + enemyRect.h > hitboxRect.y)
		{
			(*iter)->gotHit(100, direction, timeStep);
			if ((*iter)->isDied())
			{
				currentScene->deleteEntity((*iter)->getEntityPointer());
			}
			return true;
		}
	}
	return false;
}


std::chrono::steady_clock::time_point SideScroller::getTime()
{
	return std::chrono::high_resolution_clock::now();
}

void SideScroller::newStaticEntity(int x, int y)
{
	if (!currentScene->isEditorActive())
	{
		return;
	}
	//temporary circle object creation
	/*else
	{
		Enemy* new_enemy = new Enemy;
		new_enemy->Init(x, y, newGame.getRenderer());
		currentScene->addEnemy(new_enemy);
		//Enemy* new_enemy = new Enemy;
		//new_enemy->tempCircleTest(x, y, 50, newGame.getRenderer());
		//currentScene->addEnemy(new_enemy);
		return;
	}*/




	if (currentScene->isFirstPointRecorded())
	{
		currentScene->newStaticEntity( { x * currentScene->getSceneScale() + currentScene->getCamX() , y * currentScene->getSceneScale() + currentScene->getCamY() } );
	}
	else
	{
		if (currentScene->isPointerOnCollisionBody(x * currentScene->getSceneScale() + currentScene->getCamX(), y * currentScene->getSceneScale() + currentScene->getCamY(),
			currentScene->getCollisionBodiesBegin(), currentScene->getCollisionBodiesEnd()))
		{
			return;
		}
		currentScene->recordPoint(x * currentScene->getSceneScale() + currentScene->getCamX(), y * currentScene->getSceneScale() + currentScene->getCamY());
	}
}


void SideScroller::deleteSelectedEntity()
{
	currentScene->deleteSelected();
}

void SideScroller::pauseOrUnPause()
{
	if (gameState == "paused")
	{
		unPauseGame();
	}
	else if ( gameState == "running" )
	{
		pauseGame();
	}
}

void SideScroller::slideEntity(int x, int y)
{
	if(currentScene->isEditorActive())
	currentScene->slideSelectedEntity(x * currentScene->getSceneScale() + currentScene->getCamX(), y * currentScene->getSceneScale() + currentScene->getCamY());
}

void SideScroller::setEntityDynamic()
{	
	currentScene->setSelectedEntityDynamic();
	currentScene->addToDynamicBodies(currentScene->getSelectedEntity());
}

void SideScroller::pauseGame()
{
	gameState = "paused";
}

void SideScroller::unPauseGame()
{
	gameState = "running";
}



void SideScroller::saveLevel()
{
	for (auto iter = scenes.begin(); iter != scenes.end(); iter++)
	{
		std::ofstream levelFile;
		std::string currentLevel = (*iter)->getName() + ".txt";
		levelFile.open(currentLevel);
		levelFile << *(*iter);
		levelFile.close();
	}

}

void SideScroller::loadLevel()
{
	std::ifstream levelFile, levelsFile;
	levelsFile.open("levels.txt");
	std::string levelName;

	levelsFile >> levelName;

	while (levelName != "EOF")
	{
		levelFile.open(levelName + ".txt");
		Scene* tmp_scene = new Scene;
		levelFile >> *tmp_scene;
		tmp_scene->setName(levelName);
		
		tmp_scene->splitCollisionBodies();
		//*tmp_scene = scene;
		scenes.push_back(tmp_scene);
		levelsFile >> levelName;
		levelFile.close();
	}
	currentScene = scenes.front();
	levelsFile.close();
}