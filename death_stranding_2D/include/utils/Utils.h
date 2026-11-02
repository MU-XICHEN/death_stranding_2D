#pragma once

#include <cmath>
#include "core/GameBase.h"
#include "GamesEngineeringBase.h"
#include "entities/Entity.h"
#include "entities/AABB.h"

namespace GamesEngineeringBase::Utils
{
    static GEB::Vec2 rotateCounterClockwise(GEB::Vec2 point, GEB::Vec2 origin, float degrees)
    {
        float radians = GEB::Math::toRadians(degrees);
        float sinRadians = std::sin(radians);
        float cosRadians = std::cos(radians);

        point.x -= origin.x;
        point.y -= origin.y;

        float xnew = point.x * cosRadians + point.y * sinRadians;
        float ynew = -point.x * sinRadians + point.y * cosRadians;

        point.x = xnew + origin.x;
        point.y = ynew + origin.y;
        return point;
    }

    static GEB::Vec2 rotateClockwise(GEB::Vec2 point, GEB::Vec2 origin, float degrees)
    {
        float radians = GEB::Math::toRadians(degrees);
        float sinRadians = std::sin(radians);
        float cosRadians = std::cos(radians);

        point.x -= origin.x;
        point.y -= origin.y;

        float xnew = point.x * cosRadians - point.y * sinRadians;
        float ynew = point.x * sinRadians + point.y * cosRadians;

        point.x = xnew + origin.x;
        point.y = ynew + origin.y;
        return point;
    }

    static GEB::Vec2 transDirToUnitVec2(GEB::Direction direction)
    {
        switch (direction)
        {
        case GEB::Direction::Up:
            return GEB::Math::normalize(GEB::Vec2{0.0f, -1.0f});
        case GEB::Direction::Down:
            return GEB::Math::normalize(GEB::Vec2{0.0f, 1.0f});
        case GEB::Direction::Left:
            return GEB::Math::normalize(GEB::Vec2{-1.0f, 0.0f});
        case GEB::Direction::Right:
            return GEB::Math::normalize(GEB::Vec2{1.0f, 0.0f});
        case GEB::Direction::DownLeft:
            return GEB::Math::normalize(GEB::Vec2{-1.0f, 1.0f});
        case GEB::Direction::DownRight:
            return GEB::Math::normalize(GEB::Vec2{1.0f, 1.0f});
        case GEB::Direction::UpLeft:
            return GEB::Math::normalize(GEB::Vec2{-1.0f, -1.0f});
        case GEB::Direction::UpRight:
            return GEB::Math::normalize(GEB::Vec2{1.0f, -1.0f});
        default:
            return GEB::Vec2{0.0f, 0.0f};
        }
    }

    static GEB::Direction translateEventsToDirection(const GamesEngineeringBase::Window &canvas, GEB::Direction prevDirection)
    {
        if (canvas.keyPressed('W') && canvas.keyPressed('A'))
            return GEB::Direction::UpLeft;
        else if (canvas.keyPressed('W') && canvas.keyPressed('D'))
            return GEB::Direction::UpRight;
        else if (canvas.keyPressed('S') && canvas.keyPressed('A'))
            return GEB::Direction::DownLeft;
        else if (canvas.keyPressed('S') && canvas.keyPressed('D'))
            return GEB::Direction::DownRight;
        else if (canvas.keyPressed('W'))
            return GEB::Direction::Up;
        else if (canvas.keyPressed('S'))
            return GEB::Direction::Down;
        else if (canvas.keyPressed('A'))
            return GEB::Direction::Left;
        else if (canvas.keyPressed('D'))
            return GEB::Direction::Right;

        // up\down\left\right keys
        else if (canvas.keyPressed(VK_UP) && canvas.keyPressed(VK_LEFT))
            return GEB::Direction::UpLeft;
        else if (canvas.keyPressed(VK_UP) && canvas.keyPressed(VK_RIGHT))
            return GEB::Direction::UpRight;
        else if (canvas.keyPressed(VK_DOWN) && canvas.keyPressed(VK_LEFT))
            return GEB::Direction::DownLeft;
        else if (canvas.keyPressed(VK_DOWN) && canvas.keyPressed(VK_RIGHT))
            return GEB::Direction::DownRight;
        else if (canvas.keyPressed(VK_UP))
            return GEB::Direction::Up;
        else if (canvas.keyPressed(VK_DOWN))
            return GEB::Direction::Down;
        else if (canvas.keyPressed(VK_LEFT))
            return GEB::Direction::Left;
        else if (canvas.keyPressed(VK_RIGHT))
            return GEB::Direction::Right;

        return prevDirection;
    }

    static GEB::Vec2 translateEventsToDirectionVec2(const GamesEngineeringBase::Window &canvas)
    {
        // wsad
        if (canvas.keyPressed('W') && canvas.keyPressed('A'))
            return GEB::Vec2{-1, -1};
        else if (canvas.keyPressed('W') && canvas.keyPressed('D'))
            return GEB::Vec2{1, -1};
        else if (canvas.keyPressed('S') && canvas.keyPressed('A'))
            return GEB::Vec2{-1, 1};
        else if (canvas.keyPressed('S') && canvas.keyPressed('D'))
            return GEB::Vec2{1, 1};
        else if (canvas.keyPressed('W'))
            return GEB::Vec2{0, -1};
        else if (canvas.keyPressed('S'))
            return GEB::Vec2{0, 1};
        else if (canvas.keyPressed('A'))
            return GEB::Vec2{-1, 0};
        else if (canvas.keyPressed('D'))
            return GEB::Vec2{1, 0};

        // up\down\left\right keys
        else if (canvas.keyPressed(VK_UP) && canvas.keyPressed(VK_LEFT))
            return GEB::Vec2{-1, -1};
        else if (canvas.keyPressed(VK_UP) && canvas.keyPressed(VK_RIGHT))
            return GEB::Vec2{1, -1};
        else if (canvas.keyPressed(VK_DOWN) && canvas.keyPressed(VK_LEFT))
            return GEB::Vec2{-1, 1};
        else if (canvas.keyPressed(VK_DOWN) && canvas.keyPressed(VK_RIGHT))
            return GEB::Vec2{1, 1};
        else if (canvas.keyPressed(VK_UP))
            return GEB::Vec2{0, -1};
        else if (canvas.keyPressed(VK_DOWN))
            return GEB::Vec2{0, 1};
        else if (canvas.keyPressed(VK_LEFT))
            return GEB::Vec2{-1, 0};
        else if (canvas.keyPressed(VK_RIGHT))
            return GEB::Vec2{1, 0};

        return GEB::Vec2{0, 0};
    }

    // get projection of a vertex onto a unit axis
    static float project(const GEB::Vec2 vertex,
                         const GEB::Vec2 &unitAxis)
    {
        return vertex.x * unitAxis.x + vertex.y * unitAxis.y;
    }

    // get projection interval of a rectangle onto a unit axis
    // make unitAxis is unit vector, which make result from project(dot product) is right.
    static GEB::Pair<float, float> projectRect(const GEB::Vec2 *vertices,
                                               const GEB::Vec2 &unitAxis)
    {
        GEB::Pair<float, float> interval;

        float first = project(vertices[0], unitAxis);

        interval.first = first;  // min
        interval.second = first; // max

        for (int i = 1; i < 4; ++i)
        {
            float value = project(vertices[i], unitAxis);

            interval.first = std::min(interval.first, value);
            interval.second = std::max(interval.second, value);
        }

        return interval;
    }

    static GEB::Pair<float, float> projectLine(
        const GEB::Vec2 &ori,
        const GEB::Vec2 &des,
        const GEB::Vec2 &unitAxis)
    {
        GEB::Pair<float, float> interval;

        float first = project(ori, unitAxis);

        interval.first = first;  // min
        interval.second = first; // max

        float value = project(des, unitAxis);

        interval.first = std::min(interval.first, value);
        interval.second = std::max(interval.second, value);

        return interval;
    }

    /**
     * Make SAT rectangle for SAT collision detection
     * After returning, users can calculate the axies of the rectangle using:
     * rectA[1] - rectA[0],
     * rectA[2] - rectA[0],
     */
    static void adaptForSAT(
        GEB::Vec2 (&rect)[4])
    {
        const GEB::Vec2 points[4] = {rect[0], rect[1], rect[2], rect[3]};

        int diagonalIndex = 1;
        float maxDistanceSquared = 0.0f;

        for (int i = 1; i < 4; ++i)
        {
            const GEB::Vec2 offset = points[i] - points[0];
            const float distanceSquared =
                offset.x * offset.x + offset.y * offset.y;

            if (distanceSquared > maxDistanceSquared)
            {
                maxDistanceSquared = distanceSquared;
                diagonalIndex = i;
            }
        }

        rect[0] = points[0];
        rect[3] = points[diagonalIndex];

        int index = 1;
        for (int i = 1; i < 4; ++i)
        {
            if (i != diagonalIndex)
                rect[index++] = points[i];
        }
    }

    // detect whehter rectA and rectB is collided
    static bool SAT(const GEB::Vec2 *rectA, const GEB::Vec2 *rectB)
    {
        // Get two edges from each rectangle to form the axes for SAT
        const GEB::Vec2 edges[4] = {
            rectA[1] - rectA[0],
            rectA[2] - rectA[0],
            rectB[1] - rectB[0],
            rectB[2] - rectB[0]};

        GEB::Vec2 axes[4];

        for (int i = 0; i < 4; ++i)
        {
            if (edges[i].x == 0.0f && edges[i].y == 0.0f)
                return false;

            // use perpendicular vector as the axis for SAT
            // as if there is a gap between the rectangles along this axis, their projections on normal will not overlap.
            axes[i] = GEB::Vec2{-edges[i].y, edges[i].x};
        }

        for (const GEB::Vec2 &axis : axes)
        {
            // caculate the projection
            const auto projectionA = projectRect(rectA, axis);
            const auto projectionB = projectRect(rectB, axis);

            const float minA = projectionA.first;
            const float maxA = projectionA.second;
            const float minB = projectionB.first;
            const float maxB = projectionB.second;

            if (maxA < minB || maxB < minA)
                return false;
        }

        return true;
    }

    // detect whether a line(origin to destination) and box(rect) is collided.
    static bool SATLineBox(const GEB::Vec2 &origin,
                           const GEB::Vec2 &destination,
                           const GEB::Vec2 *rect)
    {
        const int edgeCount = 3;
        const GEB::Vec2 edges[edgeCount] = {
            destination - origin,
            rect[1] - rect[0],
            rect[2] - rect[0]};

        GEB::Vec2 axes[edgeCount];

        for (int i = 0; i < edgeCount; ++i)
        {
            if (edges[i].x == 0.0f && edges[i].y == 0.0f)
                return false;

            // use perpendicular vector as the axis for SAT
            // as if there is a gap between the rectangles along this axis, their projections on normal will not overlap.
            axes[i] = GEB::Vec2{-edges[i].y, edges[i].x};
        }

        for (const GEB::Vec2 &axis : axes)
        {
            // caculate the projection
            const auto projectionA = projectLine(origin, destination, axis);
            const auto projectionB = projectRect(rect, axis);

            const float minA = projectionA.first;
            const float maxA = projectionA.second;
            const float minB = projectionB.first;
            const float maxB = projectionB.second;

            if (maxA < minB || maxB < minA)
                return false;
        }
        return true;
    }

    static void getRectByPoint(const GEB::Vec2 &originPoint,
                               const GEB::Direction &direction,
                               int width,
                               int height,
                               GEB::Vec2 hitBoxPoints[4])
    {
        GEB::Vec2 originPoint1 = originPoint;

        const auto unitDirVec2 = GEB::Utils::transDirToUnitVec2(direction);
        const GEB::Vec2 startDirVec2 = GEB::Math::perpendicularCCW(unitDirVec2);

        const auto hitBoxWidth = static_cast<float>(width);
        const auto hitBoxHeight = static_cast<float>(height);

        GEB::Vec2 originPoint2 = originPoint1 + (startDirVec2 * hitBoxWidth);

        const auto perpVec2 = unitDirVec2;

        // get hit box points
        hitBoxPoints[0] = originPoint1 + (perpVec2 * (hitBoxHeight / 2));
        hitBoxPoints[1] = originPoint1 - (perpVec2 * (hitBoxHeight / 2));
        hitBoxPoints[2] = originPoint2 + (perpVec2 * (hitBoxHeight / 2));
        hitBoxPoints[3] = originPoint2 - (perpVec2 * (hitBoxHeight / 2));
    }

    static void getRectByEntity(const Entity *entity, GEB::Vec2 points[4])
    {
        GEB::Vec2 rectB[4] = {
            GEB::Vec2{entity->x(), entity->y()},
            GEB::Vec2{entity->x(), entity->y()} + GEB::Vec2(static_cast<float>(entity->width()), 0),
            GEB::Vec2{entity->x(), entity->y()} + GEB::Vec2(0, static_cast<float>(entity->height())),
            GEB::Vec2{entity->x(), entity->y()} + GEB::Vec2(static_cast<float>(entity->width()), static_cast<float>(entity->height())),
        };

        memset(points, 0, sizeof(GEB::Vec2) * 4);
        memcpy(points, rectB, sizeof(GEB::Vec2) * 4);
    }

    static GEB::Vec2 getDestination(GEB::Vec2 startPos,
                                    const GEB::Direction &direction,
                                    float distance)
    {
        GEB::Vec2 unitDirVec2 = GEB::Utils::transDirToUnitVec2(direction);
        return startPos + (unitDirVec2 * distance);
    }

    // ips holds up to two line/AABB boundary intersections, ordered along dir.
    // Return the number of valid points.
    static int getIntersectionPoints(
        GEB::Vec2 point,
        GEB::Vec2 dir,
        const AABB &aabb,
        GEB::Vec2 *ips)
    {
        if (!ips || (dir.x == 0.0f && dir.y == 0.0f))
            return 0;
        /**
         * Calculate the line equation using the point and direction vector.
         * For example, point -> (x, y) and dir -> (a, b), we can get offset by 'ay = bx + offset'.
         */
        float offset = dir.x * point.y - dir.y * point.x;
        auto yFunc = [&](float x)
        {
            return (dir.y * x + offset) / dir.x;
        };

        auto xFunc = [&](float y)
        {
            return (dir.x * y - offset) / dir.y;
        };

        float left = aabb.x();
        float right = left + aabb.width();
        float top = aabb.y();
        float bottom = top + aabb.height();
        int count = 0;

        auto addPoint = [&](float x, float y)
        {
            if (x < left || x > right || y < top || y > bottom)
                return;
            for (int i = 0; i < count; ++i)
            {
                if (ips[i].x == x && ips[i].y == y)
                    return;
            }

            if (count < 2)
                ips[count++] = {x, y};
        };

        if (dir.x != 0.0f)
        {
            addPoint(left, yFunc(left));
            addPoint(right, yFunc(right));
        }
        if (dir.y != 0.0f)
        {
            addPoint(xFunc(top), top);
            addPoint(xFunc(bottom), bottom);
        }

        // zero or one
        if (count != 2)
            return count;

        // two points, make sure the first is nearest.
        // make sure that the first is nearest.
        GEB::Vec2 firstToSecond = ips[1] - ips[0];
        bool needsSwap = GEB::Math::dotProduct(firstToSecond, dir) < 0.0f;
        if (needsSwap)
        {
            GEB::Vec2 temp = ips[0];
            ips[0] = ips[1];
            ips[1] = temp;
        }
        return count;
    }

}
