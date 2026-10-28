#pragma once

#include <cmath>
#include "core/GameBase.h"

namespace GamesEngineeringBase::Utils
{
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

    static bool SAT(const GEB::Vec2 *rectA, const GEB::Vec2 *rectB)
    {
        const int size = 4;
        GEB::Pair<float, float> projectionAY;
        GEB::Pair<float, float> projectionAX;
        GEB::Pair<float, float> projectionBY;
        GEB::Pair<float, float> projectionBX;

        projectionAX.first = projectionAX.second = rectA[0].x;
        projectionAY.first = projectionAY.second = rectA[0].y;

        projectionBX.first = projectionBX.second = rectB[0].x;
        projectionBY.first = projectionBY.second = rectB[0].y;

        for (int i = 0; i < size; ++i)
        {
            projectionAX.first = GEB::Math::min(projectionAX.first, rectA[i].x);
            projectionAX.second = GEB::Math::max(projectionAX.second, rectA[i].x);

            projectionAY.first = GEB::Math::min(projectionAY.first, rectA[i].y);
            projectionAY.second = GEB::Math::max(projectionAY.second, rectA[i].y);
        }
        for (int i = 0; i < size; ++i)
        {
            projectionBX.first = GEB::Math::min(projectionBX.first, rectB[i].x);
            projectionBX.second = GEB::Math::max(projectionBX.second, rectB[i].x);

            projectionBY.first = GEB::Math::min(projectionBY.first, rectB[i].y);
            projectionBY.second = GEB::Math::max(projectionBY.second, rectB[i].y);
        }

        if (projectionAX.second < projectionBX.first || projectionBX.second < projectionAX.first)
            return false;
        if (projectionAY.second < projectionBY.first || projectionBY.second < projectionAY.first)
            return false;

        return true;
    }
}