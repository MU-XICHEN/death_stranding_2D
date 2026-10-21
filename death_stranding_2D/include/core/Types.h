#pragma once

namespace GamesEngineeringBase
{
    struct Color
    {
        int r;
        int g;
        int b;
    };

    struct Position
    {
        float x;
        float y;
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
        float dx;
        float dy;
    };

    struct Boundary
    {
        float top;
        float bottom;
        float left;
        float right;
    };
}
