#pragma once

#include "core/Types.h"

namespace GamesEngineeringBase::Math
{

    template <typename T>
    inline T min(const T &lhs, const T &rhs)
    {
        return lhs <= rhs ? lhs : rhs;
    }

    template <typename T>
    inline T max(const T &lhs, const T &rhs)
    {
        return lhs >= rhs ? lhs : rhs;
    }

    template <typename T>
    inline T abs(const T &value)
    {
        return value >= 0 ? value : -value;
    }

    // Implemented based on Newton–Raphson method
    inline Vec2 normalize(const Vec2 &v)
    {
        Vec2 result;
        float scale = max(abs(v.x), abs(v.y));
        if (scale == 0.0f)
            return Vec2{0.0f, 0.0f};

        float x = v.x / scale;
        float y = v.y / scale;
        float squared = x * x + y * y;

        float length = 2.0f; // Initial guess because result range is [1, 2]
        const float epsilon = 1e-6f;
        const int maxIterations = 8;

        for (int i = 0; i < maxIterations; ++i)
        {
            float next = 0.5f * (length + squared / length);

            float difference = next - length;
            if (difference < 0.0f)
                difference = -difference;

            length = next;

            if (difference <= epsilon * length)
                break;
        }

        result.x = x / length;
        result.y = y / length;
        return result;
    }

    inline float dotProduct(const Vec2 &a, const Vec2 &b)
    {
        return a.x * b.x + a.y * b.y;
    }

}
