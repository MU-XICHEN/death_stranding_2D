#pragma once
#include "core/GameBase.h"
#include "entities/Entity.h"
#include "structure/Array.h"

struct MapInfo
{
    float x;
    float y;
    int width;
    int height;
    int borderSize;
};

class Map : public Entity
{
public:
    explicit Map() : Entity(0, 0, 0, 0) {}

    explicit Map(float x,
                 float y,
                 int width,
                 int height,
                 int borderSize,
                 GEB::Color borderColor)
        : Entity(x, y, width, height),
          mapInfo_{x, y, width, height, borderSize},
          borderSize_(borderSize),
          borderColor_(borderColor) {}

    inline int topBoundary() const { return borderSize_; }
    inline int bottomBoundary() const { return height_ - borderSize_; }

    inline int leftBoundary() const { return borderSize_; }
    inline int rightBoundary() const { return width_ - borderSize_; }

    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;

private:
    GEB::Color borderColor_ = GEB::Color(255, 255, 255);
    int borderSize_ = MAP_BORDER_SIZE;

    MapInfo mapInfo_;
};