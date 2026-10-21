#pragma once
#include "core/GameBase.h"
#include "entities/Entity.h"

class Map : public Entity
{
public:
    Map(
        float x, float y, float width, float height) : Entity(x, y, width, height),
                                                       borderSize_(0),
                                                       borderColor_(0, 0, 0) {}

    inline int topBoundary() const { return borderSize_; }
    inline int bottomBoundary() const { return height_ - borderSize_; }

    inline int leftBoundary() const { return borderSize_; }
    inline int rightBoundary() const { return width_ - borderSize_; }

private:
    GEB::Color borderColor_;
    int borderSize_;
};