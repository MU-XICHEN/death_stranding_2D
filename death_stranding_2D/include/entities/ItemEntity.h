#pragma once

#include "entities/Entity.h"

// ItemEntity is used to anything which should consider collision.
class ItemEntity : public Entity
{
public:
    ItemEntity(float x, float y, int width, int height, float speed = PIXEL_PER_SEC)
        : Entity(x, y, width, height), speed_(speed) {}
    virtual ~ItemEntity() = default;

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

    void calculateBoundaryTop(GEB::Boundary &boundary, const ItemEntity *barrierItem);
    void calculateBoundaryBottom(GEB::Boundary &boundary, const ItemEntity *barrierItem);
    void calculateBoundaryLeft(GEB::Boundary &boundary, const ItemEntity *barrierItem);
    void calculateBoundaryRight(GEB::Boundary &boundary, const ItemEntity *barrierItem);
    void calculateBoundaryImpl(GEB::Boundary &boundary, const ItemEntity *barrierItem, GEB::Direction direction);
    GEB::Boundary calculateBoundary(World &world, Layer &layer, GEB::Direction diretion);

    void moveUp(float distance, float topBoundary);
    void moveDown(float distance, float bottomBoundary);
    void moveLeft(float distance, float leftBoundary);
    void moveRight(float distance, float rightBoundary);
    void moveUpLeft(float distance, float topBoundary, float leftBoundary);
    void moveUpRight(float distance, float topBoundary, float rightBoundary);
    void moveDownLeft(float distance, float bottomBoundary, float leftBoundary);
    void moveDownRight(float distance, float bottomBoundary, float rightBoundary);

protected:
    GEB::Color color_{0, 0, 0};
    float speed_;
};
