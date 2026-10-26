#include <iostream>

#include "entities/ItemEntity.h"
#include "world/World.h"
#include "map/Map.h"
#include "layers/Layer.h"
#include "stages/Play.h"
#include "entities/AABB.h"



void ItemEntity::update(World &world, Layer &layer) {}

void ItemEntity::render(World &world, Layer &layer)
{
    const Viewport &viewport = world.gameInfo().viewport();
    auto &canvas = world.canvas();
    const int visibleWidth = viewport.width();
    const int visibleHeight = viewport.height();

    float xRelativeToViewport = x_ - viewport.x();
    float yRelativeToViewport = y_ - viewport.y();

    if (xRelativeToViewport >= visibleWidth || yRelativeToViewport >= visibleHeight ||
        xRelativeToViewport + width_ <= 0 || yRelativeToViewport + height_ <= 0)
        return;

    for (int i = 0; i < width_; i++)
    {
        for (int j = 0; j < height_; j++)
        {
            float targetX = xRelativeToViewport + i;
            float targetY = yRelativeToViewport + j;

            if (targetX < 0 || targetY < 0 ||
                targetX >= visibleWidth || targetY >= visibleHeight)
                continue;

            canvas.draw(static_cast<int>(targetX),
                        static_cast<int>(targetY),
                        color_.r,
                        color_.g,
                        color_.b);
        }
    }
}

// Movement functions
void ItemEntity::moveUp(float distance, float topBoundary)
{
    float resPos = y_ - distance;
    y_ = GEB::Math::min(y_, GEB::Math::max(resPos, topBoundary));
}
void ItemEntity::moveDown(float distance, float bottomBoundary)
{
    float resPos = y_ + distance;
    y_ = GEB::Math::max(y_, GEB::Math::min(resPos, bottomBoundary - height_));
}
void ItemEntity::moveLeft(float distance, float leftBoundary)
{
    float resPos = x_ - distance;
    x_ = GEB::Math::min(x_, GEB::Math::max(resPos, leftBoundary));
}
void ItemEntity::moveRight(float distance, float rightBoundary)
{
    float resPos = x_ + distance;
    x_ = GEB::Math::max(x_, GEB::Math::min(resPos, rightBoundary - width_));
}

void ItemEntity::calculateBoundaryTop(GEB::Boundary &boundary, const Entity *barrierItem)
{
    const float barrierRight = barrierItem->x() + barrierItem->width();
    const float barrierBottom = barrierItem->y() + barrierItem->height();
    // half-open intervals [min, max)
    if (barrierRight <= x_ || barrierItem->x() >= x_ + width_)
        return;

    if (barrierBottom <= y_)
        boundary.top = GEB::Math::max(boundary.top, barrierBottom);
}

void ItemEntity::calculateBoundaryBottom(GEB::Boundary &boundary, const Entity *barrierItem)
{
    const float barrierRight = barrierItem->x() + barrierItem->width();

    if (barrierRight <= x_ || barrierItem->x() >= x_ + width_)
        return;

    if (barrierItem->y() >= y_ + height_)
        boundary.bottom = GEB::Math::min(boundary.bottom, barrierItem->y());
}

void ItemEntity::calculateBoundaryLeft(GEB::Boundary &boundary, const Entity *barrierItem)
{
    const float barrierRight = barrierItem->x() + barrierItem->width();
    const float barrierBottom = barrierItem->y() + barrierItem->height();
    if (barrierBottom <= y_ || barrierItem->y() >= y_ + height_)
        return;

    if (barrierRight <= x_)
        boundary.left = GEB::Math::max(boundary.left, barrierRight);
}

void ItemEntity::calculateBoundaryRight(GEB::Boundary &boundary, const Entity *barrierItem)
{
    const float barrierBottom = barrierItem->y() + barrierItem->height();
    if (barrierBottom <= y_ || barrierItem->y() >= y_ + height_)
        return;

    if (barrierItem->x() >= x_ + width_)
        boundary.right = GEB::Math::min(boundary.right, barrierItem->x());
}

void ItemEntity::calculateBoundaryImpl(GEB::Boundary &boundary, const Entity *barrierItem, GEB::Direction direction)
{
    switch (direction)
    {
    case GEB::Direction::Up:
        calculateBoundaryTop(boundary, barrierItem);
        break;
    case GEB::Direction::Down:
        calculateBoundaryBottom(boundary, barrierItem);
        break;
    case GEB::Direction::Left:
        calculateBoundaryLeft(boundary, barrierItem);
        break;
    case GEB::Direction::Right:
        calculateBoundaryRight(boundary, barrierItem);
        break;
    case GEB::Direction::None:
        break;
    default:
        break;
    }
}

GEB::Boundary ItemEntity::calculateBoundary(const MapInfo &map, GEB::Direction diretion, Entity **candidateItemHead, int size)
{
    GEB::Boundary boundary{
        .top = map.topBoundary(),
        .bottom = map.bottomBoundary(),
        .left = map.leftBoundary(),
        .right = map.rightBoundary()};

    for (int i = 0; i < size; i++)
    {
        if (!(*(candidateItemHead + i)) || *(candidateItemHead + i) == this)
            continue;
        calculateBoundaryImpl(boundary, *(candidateItemHead + i), diretion);
    }

    return boundary;
}

void ItemEntity::moveWithCollision(
    float distance,
    GEB::Direction direction,
    const MapInfo &map,
    Entity **candidateItemHead,
    int size)
{
    // calculate boundaries horizontally and vertically, respectively, in case of diagonal movement
    switch (direction)
    {
    case GEB::Direction::Up:
    case GEB::Direction::UpLeft:
    case GEB::Direction::UpRight:
        moveUp(distance, calculateBoundary(map, GEB::Direction::Up, candidateItemHead, size).top);
        break;
    case GEB::Direction::Down:
    case GEB::Direction::DownLeft:
    case GEB::Direction::DownRight:
        moveDown(distance, calculateBoundary(map, GEB::Direction::Down, candidateItemHead, size).bottom);
        break;
    default:
        break;
    }

    switch (direction)
    {
    case GEB::Direction::Left:
    case GEB::Direction::UpLeft:
    case GEB::Direction::DownLeft:
        moveLeft(distance, calculateBoundary(map, GEB::Direction::Left, candidateItemHead, size).left);
        break;
    case GEB::Direction::Right:
    case GEB::Direction::UpRight:
    case GEB::Direction::DownRight:
        moveRight(distance, calculateBoundary(map, GEB::Direction::Right, candidateItemHead, size).right);
        break;
    default:
        break;
    }
}
