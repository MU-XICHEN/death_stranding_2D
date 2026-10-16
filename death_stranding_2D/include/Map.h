#pragma once
#include "Types.h"

struct Map
{
public:
    int width;
    int height;
    int borderSize;
    Color borderColor;

    inline int topBoundary() const { return borderSize; }
    inline int bottomBoundary() const { return height - borderSize; }

    inline int leftBoundary() const { return borderSize; }
    inline int rightBoundary() const { return width - borderSize; }
};