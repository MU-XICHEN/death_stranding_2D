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
};

class Map : public Entity
{
public:
    explicit Map() : Entity(0, 0, 0, 0) {}

    explicit Map(float x,
                 float y,
                 int width,
                 int height,
                 GEB::Color borderColor)
        : Entity(x, y, width, height),
          mapInfo_{x, y, width, height},
          borderColor_(borderColor) {}

    inline int topBoundary() const { return 0; }
    inline int bottomBoundary() const { return height_; }

    inline int leftBoundary() const { return 0; }
    inline int rightBoundary() const { return width_; }

    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;

private:
    GEB::Color borderColor_ = GEB::Color(255, 255, 255);

    MapInfo mapInfo_;
};