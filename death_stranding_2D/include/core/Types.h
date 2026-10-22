#pragma once

namespace GamesEngineeringBase
{
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

    struct DirectionVec
    {
        float dx = 0;
        float dy = 0;
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
}
