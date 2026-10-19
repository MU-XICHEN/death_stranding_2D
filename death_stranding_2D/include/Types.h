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
        Down,
        Up,
        Left,
        Right,
        DownLeft,
        DownRight,
        UpLeft,
        UpRight
    };
}
