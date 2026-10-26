#pragma once

namespace GamesEngineeringBase
{

    enum TextSize
    {
        TEXT_SIZE_SMALL = 0,
        TEXT_SIZE_MEDIUM = 1,
        TEXT_SIZE_LARGE = 2,
    };
    
    struct Color
    {
        int r = 0;
        int g = 0;
        int b = 0;
    };

    struct Position
    {
        float x = 0;
        float y = 0;
    };

    enum Direction
    {
        None,
        Down,
        Up,
        Left,
        Right,
        DownLeft,
        DownRight,
        UpLeft,
        UpRight
    };

    template <typename T, typename U>
    struct Pair
    {
        T first = 0;
        U second = 0;
    };

    struct Boundary
    {
        float top = 0;
        float bottom = 0;
        float left = 0;
        float right = 0;
    };

    struct Vec2
    {
        float x = 0;
        float y = 0;

        Vec2 operator+(const Vec2 &other) const
        {
            return Vec2{x + other.x, y + other.y};
        }

        Vec2 operator-(const Vec2 &other) const
        {
            return Vec2{x - other.x, y - other.y};
        }

        Vec2 operator*(float scalar) const
        {
            return Vec2{x * scalar, y * scalar};
        }

        Vec2 operator/(float scalar) const
        {
            return Vec2{x / scalar, y / scalar};
        }
    };

}
