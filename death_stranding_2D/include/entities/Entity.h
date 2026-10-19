// Reference: "Game Programming Patterns"

#pragma once

#include "core/GameBase.h"

class World;
class Layer;

class Entity
{
public:
    Entity(float x, float y, int width, int height)
        : x_(x), y_(y), width_(width), height_(height) {}
    virtual ~Entity() = default;

    virtual void update(World &world, Layer &layer) = 0;
    virtual void render(World &world, Layer &layer) = 0;

    inline GEB::Position center() const
    {
        return GEB::Position{
            x_ + width_ / 2,
            y_ + height_ / 2,
        };
    }
    inline float x() const { return x_; }
    inline float y() const { return y_; }
    inline int width() const { return width_; }
    inline int height() const { return height_; }

    inline void setColor(unsigned int r, unsigned int g, unsigned int b)
    {
        color_.r = r;
        color_.g = g;
        color_.b = b;
    }

protected:
    GEB::Color color_{0, 0, 0};

    float x_;
    float y_;

    int width_;
    int height_;
};

class Character : public Entity
{
public:
    Character(float x, float y, int width, int height)
        : Entity(x, y, width, height) {}
    virtual ~Character() = default;

protected:
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