#pragma once

#include <cmath>
#include "core/GameBase.h"

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

    // get projection of a vertex onto a unit axis
    static float project(const GEB::Vec2 vertex,
                         const GEB::Vec2 &unitAxis)
    {
        return vertex.x * unitAxis.x + vertex.y * unitAxis.y;
    }

    // get projection interval of a rectangle onto a unit axis
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

            interval.first = GEB::Math::min(interval.first, value);
            interval.second = GEB::Math::max(interval.second, value);
        }

        return interval;
    }

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
};