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
            x_ + width_ / 2.0f,
            y_ + height_ / 2.0f,
        };
    }
    inline void setPos(float x, float y) { x_ = x; y_ = y; }
    inline float x() const { return x_; }
    inline float y() const { return y_; }
    
    inline int width() const { return width_; }
    inline int height() const { return height_; }

protected:
    float x_;
    float y_;

    int width_;
    int height_;
};
