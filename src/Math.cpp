#include "Math.h"



float Math::getPointOnCircle(const Circle& circle, float x)
{
    return sqrt( pow(circle.getRadius(),2) - pow(x,2)  );
}


Point2f Math::closestPointToRect(Point2f point, Rectangle rect)
{
    Point2f retPoint;
    if ( point.x <= rect.x )
    {
        retPoint.x = rect.x;
    }
    else if (point.x >= rect.x + rect.w)
    {
        retPoint.x =  rect.x + rect.w;
    }
    else
    {
        retPoint.x = point.x;
    }

    if (point.y <= rect.y)
    {
        retPoint.y = rect.y;
    }
    else if (point.y >= rect.y + rect.h)
    {
        retPoint.y = rect.y + rect.h;
    }
    else
    {
        retPoint.y = point.y;
    }

    if (retPoint == point)
    {
        if (point.x - rect.x <= rect.w / 2)
        {
            retPoint.x = rect.x;
        }
        else if (point.x - rect.x > rect.w / 2)
        {
            retPoint.x = rect.x + rect.w;
        }
        if (point.y - rect.y <= rect.h / 2)
        {
            retPoint.y = rect.y;
        }

        if (fabsf(point.x - retPoint.x) <= fabsf(point.y - retPoint.y))
        {
            retPoint.y = point.y;
        }
        else if (fabsf(point.x - retPoint.x) > fabsf(point.y - retPoint.y))
        {
            retPoint.x = point.x;
        }
    }

    return retPoint;
}


Point2f Math::lineCircleIntersect(float mX, float mY, Point2f point, Circle circ)
{
    Point2f orthoProjection = projectPointOntoLine({ circ.centerX, circ.centerY }, -mY/mX, point);

    float delta = sqrt(circ.radius * circ.radius - ((orthoProjection.x - circ.centerX) * (orthoProjection.x - circ.centerX) + (orthoProjection.y - circ.centerY) * (orthoProjection.y - circ.centerY)));

    if (mX > 0 && mY > 0)
    {
        float m = mY / mX;
        float x = sqrt((delta * delta) / (m * m + 1));
        float y = x * m;
        return { orthoProjection.x + x, orthoProjection.y + y };
    }
    else if (mX < 0 && mY > 0)
    {
        float m = mY / mX;
        float x = sqrt((delta * delta) / (m * m + 1));
        float y = x * m;
        return { orthoProjection.x - x, orthoProjection.y + y };
    }
    else if (mX < 0 && mY > 0)
    {
        float m = mY / mX;
        float x = sqrt((delta * delta) / (m * m + 1));
        float y = x * m;
        return { orthoProjection.x - x, orthoProjection.y + y };
    }
    else if (mX > 0 && mY < 0)
    {
        float m = mY / mX;
        float x = sqrt((delta * delta) / (m * m + 1));
        float y = x * m;
        return { orthoProjection.x + x, orthoProjection.y - y };
    }
    else if (mX < 0 && mY < 0)
    {
        float m = mY / mX;
        float x = sqrt((delta * delta) / (m * m + 1));
        float y = x * m;
        return { orthoProjection.x - x, orthoProjection.y - y };
    }
}

Point2f Math::projectPointOntoLine(Point2f project, float m, Point2f point)
{
    Point2f orthoProjection;
    point.x -= project.x;
    point.y -= project.y;


    orthoProjection.x = (m / ((m * m) + 1)) * (m * point.x - point.y);
    orthoProjection.y = orthoProjection.x * (-1 / m);
    return { orthoProjection.x + project.x, orthoProjection.y + project.y };
}
