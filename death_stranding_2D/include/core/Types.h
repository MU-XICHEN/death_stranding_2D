#pragma once

namespace GamesEngineeringBase
{

    enum TextSizeType
    {
        TEXT_SIZE_SMALL = 0,
        TEXT_SIZE_MEDIUM = 1,
        TEXT_SIZE_LARGE = 2,
    };

    enum TextSizeW
    {
        TEXT_SIZE_W_SMALL = 9,
        TEXT_SIZE_W_MEDIUM = 12,
        TEXT_SIZE_W_LARGE = 18
    };

    enum TextSizeH
    {
        TEXT_SIZE_H_SMALL = 18,
        TEXT_SIZE_H_MEDIUM = 24,
        TEXT_SIZE_H_LARGE = 36
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

    struct Matrix2
    {
        Vec2 first;
        Vec2 second;

        Vec2 operator*(const Vec2 &other) const
        {
            return Vec2{first.x * other.x + second.x * other.y, first.y * other.x + second.y * other.y};
        }

        Matrix2 operator+(const Matrix2 &other) const
        {
            return Matrix2{first + other.first, second + other.second};
        }

        Matrix2 operator-(const Matrix2 &other) const
        {
            return Matrix2{first - other.first, second - other.second};
        }
        Matrix2 operator*(float scalar) const
        {
            return Matrix2{first * scalar, second * scalar};
        }
        Matrix2 operator/(float scalar) const
        {
            return Matrix2{first / scalar, second / scalar};
        }
    };


}
