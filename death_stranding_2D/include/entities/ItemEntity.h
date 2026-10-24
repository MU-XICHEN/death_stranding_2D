#pragma once

#include "entities/Entity.h"

class Map;

// ItemEntity is used to anything which should consider collision.
class ItemEntity : public Entity
{
public:
    ItemEntity(float x, float y, int width, int height, float speed = PIXEL_PER_SEC)
        : Entity(x, y, width, height), speed_(speed) {}
    virtual ~ItemEntity() = default;

    // update method needs to be implemented by derived classes
    virtual void update(World &world, Layer &layer);

    // render mothod is implemented here and can be used by derived classes
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
    GEB::Boundary calculateBoundary(const Map &map, GEB::Direction diretion, ItemEntity **candidateItemHead, int size);

    void moveUp(float distance, float topBoundary);
    void moveDown(float distance, float bottomBoundary);
    void moveLeft(float distance, float leftBoundary);
    void moveRight(float distance, float rightBoundary);

    void moveWithCollision(
        float distance,
        GEB::Direction direction,
        const Map &map,
        ItemEntity **candidateItemHead,
        int size);

protected:
    GEB::Color color_{0, 0, 0};
    float speed_;
};
