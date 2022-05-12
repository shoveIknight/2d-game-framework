#pragma once
#include <math.h>
#include <cmath>

enum ShapeType
{
	Rect,
	Circ,
};

struct Point2f
{
	float x, y;
};

class Rectangle;

class CollisionShape
{
public:
	CollisionShape();
	CollisionShape(float x, float y, float w, float h, ShapeType shape) : x(x), y(y), h(h), w(w), shape(shape) {};
	ShapeType getShape() const { return shape; };
	virtual float getRadius() const { return 0; };
	virtual float getWidth() const { return w; };
	virtual float getLength() const { return h; };
	float x, y, w, h;
	ShapeType shape;
private:


};


class Circle : public CollisionShape
{
public:

	Circle(float x, float y, float radius) : CollisionShape(x, y, 2 * radius, 2 * radius, Circ), radius(radius)
	{
		centerX = x + radius;
		centerY = y + radius;
	};
	Circle(const CollisionShape& shape) : CollisionShape(shape.x, shape.y, 2 * shape.getRadius(), 2 * shape.getRadius(), Circ), radius(shape.getRadius())
	{
		centerX = x + radius;
		centerY = y + radius;
	};
	Circle(CollisionShape* shape) : CollisionShape(shape->x, shape->y, 2 * shape->getRadius(), 2 * shape->getRadius(), Circ), radius(shape->getRadius())
	{
		centerX = x + radius;
		centerY = y + radius;
	};
	inline float getRadius() const { return radius; };

	float radius, centerX, centerY;
};



class Rectangle : public CollisionShape
{

public:
	Rectangle() :CollisionShape(0, 0, 0, 0, Rect){ };
	Rectangle(float x, float y, float w, float h) : CollisionShape(x, y, w, h,  Rect){};
	inline float getWidht() const { return w; };
	Rectangle(const CollisionShape &shape) : CollisionShape(shape.x, shape.y, shape.w, shape.h, Rect) {};
	Rectangle(CollisionShape *shape) : CollisionShape(shape->x, shape->y, shape->w, shape->h, Rect) {};
};


namespace Math
{
	/**
	 * @brief Find the closest point on rect to the point.
	 * 
	 * @return Returns the closest point to point on the rect as Point2f 
	 */
	Point2f closestPointToRect(Point2f point, Rectangle rect);
	inline bool operator==(const Point2f& lhs, const Point2f& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; };
	inline Point2f operator-(const Point2f& lhs, const Point2f& rhs) { return { lhs.x - rhs.x, lhs.y - rhs.y }; };
	/**
	 * @brief Finds the corresponding coordinate for given x value on given circle.
	 * 
	 * @param circle The circle you want to find corresponding value.
	 * @param x The value you want to find corresnpondance.
	 * @return Corresponding coordinate on circle as float 
	 */
	float getPointOnCircle(const Circle& circle, float x);

	Point2f lineCircleIntersect(float mX, float mY, Point2f point, Circle circ);

	/**
	 * @brief Find projection of a point on a given line.
	 * 
	 * @param project The point you want to project on the line.
	 * @param m Slope of the line
	 * @param point A point on the line.
	 * @return The projected point on the line as Point2f. 
	 */
	Point2f projectPointOntoLine(Point2f project, float m, Point2f point);
}
