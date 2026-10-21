#pragma once

#include "entities/Entity.h"
#include "world/Map.h"

class Character : public Entity
{
public:
    Character(float x, float y, int width, int height, float speed = PIXEL_PER_SEC)
        : Entity(x, y, width, height), speed_(speed) {}
    virtual ~Character() = default;

    // update needs to be implemented by derived classes
    virtual void update(World &world, Layer &layer) = 0;

    // render is implemented here and can be used by derived classes
    virtual void render(World &world, Layer &layer) override;

    void setColor(unsigned int r, unsigned int g, unsigned int b)
    {
        color_.r = r;
        color_.g = g;
        color_.b = b;
    }

    void moveUp(float distance, const Map &map);
    void moveDown(float distance, const Map &map);
    void moveLeft(float distance, const Map &map);
    void moveRight(float distance, const Map &map);
    void moveUpLeft(float distance, const Map &map);
    void moveUpRight(float distance, const Map &map);
    void moveDownLeft(float distance, const Map &map);
    void moveDownRight(float distance, const Map &map);

protected:
    GEB::Color color_{0, 0, 0};
    float speed_;
};