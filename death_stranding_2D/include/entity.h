// Reference: "Game Programming Patterns"
#pragma once
#include "Types.h"

class World;

class Entity
{
public:
    Entity(int x, int y, int width, int height)
        : x_(x), y_(y), width_(width), height_(height) {}
    virtual ~Entity() = default;

    virtual void update(World &world) = 0;
    virtual void render(World &world) = 0;

    inline Position center() const
    {
        return Position{
            x_ + width_ / 2,
            y_ + height_ / 2,
        };
    }
    inline int x() const { return x_; }
    inline int y() const { return y_; }
    inline int width() const { return width_; }
    inline int height() const { return height_; }

    inline void setColor(unsigned int r, unsigned int g, unsigned int b)
    {
        color_.r = r;
        color_.g = g;
        color_.b = b;
    }

protected:
    Color color_{0, 0, 0};

    int x_;
    int y_;

    int width_;
    int height_;
};
