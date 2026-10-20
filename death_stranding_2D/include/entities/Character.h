#pragma once

#include "Entity.h"

class Character : public Entity
{
public:
    Character(float x, float y, int width, int height, float speed = PIXEL_PER_SEC)
        : Entity(x, y, width, height), speed_(speed) {}
    virtual ~Character() = default;

protected:
    float speed_;

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
    void moveUpLeft(float distance, float boundaryX, float boundaryY)
    {
        moveUp(distance, boundaryY);
        moveLeft(distance, boundaryX);
    }
    void moveUpRight(float distance, float boundaryX, float boundaryY)
    {
        moveUp(distance, boundaryY);
        moveRight(distance, boundaryX);
    }
    void moveDownLeft(float distance, float boundaryX, float boundaryY)
    {
        moveDown(distance, boundaryY);
        moveLeft(distance, boundaryX);
    }
    void moveDownRight(float distance, float boundaryX, float boundaryY)
    {
        moveDown(distance, boundaryY);
        moveRight(distance, boundaryX);
    }
};