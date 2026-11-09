#pragma once

#include "GamesEngineeringBase.h"
#include <cmath>
#include "core/Types.h"

namespace GamesEngineeringBase::Math
{

    template <typename T>
    inline T abs(const T &value)
    {
        return value >= 0 ? value : -value;
    }

    inline Vec2 normalize(const Vec2 &v)
    {
        Vec2 result;
        // Scale down first to avoid very large numbers when squaring.
        float scale = std::max(abs(v.x), abs(v.y));
        if (scale == 0.0f)
            return Vec2{0.0f, 0.0f};

        float x = v.x / scale;
        float y = v.y / scale;
        float squared = x * x + y * y;

        float length = std::sqrt(squared);

        result.x = x / length;
        result.y = y / length;
        return result;
    }

    inline float dotProduct(const Vec2 &a, const Vec2 &b)
    {
        return a.x * b.x + a.y * b.y;
    }

    // clockwise
    inline Vec2 perpendicular(const Vec2 &v)
    {
        return Vec2{-v.y, v.x};
    }

    // counter-clockwise
    inline Vec2 perpendicularCCW(const Vec2 &v)
    {
        return Vec2{v.y, -v.x};
    }

    inline int ceilToInt(float value)
    {
        const int truncated = static_cast<int>(value);
        return value > truncated ? truncated + 1 : truncated;
    }

    inline int floorToInt(float value)
    {
        const int truncated = static_cast<int>(value);
        return value < truncated ? truncated - 1 : truncated;
    }

    const float Pi = 3.14159265358979323846f;

    inline float toRadians(float degrees)
    {
        return degrees * (Pi / 180.0f);
    }

    inline float toDegrees(float radians)
    {
        return radians * (180.0f / Pi);
    }

}
