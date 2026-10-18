#pragma once

#include "Entity.h"

class Hero : public Entity
{
public:
    using Entity::Entity;

    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;

private:
    void moveUp(float distance, float boundary)
    {
        float resPos = y_ - distance;
        y_ = GEB::Math::max(resPos, boundary);
    }
    void moveDown(float distance, float boundary)
    {
        float resPos = y_ + distance;
        y_ = GEB::Math::min(resPos, boundary);
    }
    void moveLeft(float distance, float boundary)
    {
        float resPos = x_ - distance;
        x_ = GEB::Math::max(resPos, boundary);
    }
    void moveRight(float distance, float boundary)
    {
        float resPos = x_ + distance;
        x_ = GEB::Math::min(resPos, boundary);
    }

    void handleEvents(World &world);
};