#include <iostream>

#include "entities/ItemEntity.h"
#include "world/World.h"
#include "world/Map.h"
#include "layers/Layer.h"
#include "stages/Play.h"

void ItemEntity::update(World &world, Layer &layer) {}

void ItemEntity::render(World &world, Layer &layer)
{
    const Viewport &viewport = world.gameInfo().viewport();

    float transX = x_ - viewport.x();
    float transY = y_ - viewport.y();

    for (int i = 0; i < width_; i++)
    {
        for (int j = 0; j < height_; j++)
        {
            float targetX = transX + i;
            float targetY = transY + j;

            if (targetX < 0 || targetY < 0)
                continue;

            world.canvas().draw(static_cast<int>(targetX),
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
void ItemEntity::moveUpLeft(float distance, float topBoundary, float leftBoundary)
{
    moveUp(distance, topBoundary);
    moveLeft(distance, leftBoundary);
}
void ItemEntity::moveUpRight(float distance, float topBoundary, float rightBoundary)
{
    moveUp(distance, topBoundary);
    moveRight(distance, rightBoundary);
}
void ItemEntity::moveDownLeft(float distance, float bottomBoundary, float leftBoundary)
{
    moveDown(distance, bottomBoundary);
    moveLeft(distance, leftBoundary);
}
void ItemEntity::moveDownRight(float distance, float bottomBoundary, float rightBoundary)
{
    moveDown(distance, bottomBoundary);
    moveRight(distance, rightBoundary);
}

void ItemEntity::calculateBoundaryTop(GEB::Boundary &boundary, const ItemEntity *barrierItem)
{
    const float barrierRight = barrierItem->x() + barrierItem->width();
    const float barrierBottom = barrierItem->y() + barrierItem->height();
    // half-open intervals [min, max)
    if (barrierRight <= x_ || barrierItem->x() >= x_ + width_)
        return;

    if (barrierBottom <= y_)
        boundary.top = GEB::Math::max(boundary.top, barrierBottom);
}

void ItemEntity::calculateBoundaryBottom(GEB::Boundary &boundary, const ItemEntity *barrierItem)
{
    const float barrierRight = barrierItem->x() + barrierItem->width();

    if (barrierRight <= x_ || barrierItem->x() >= x_ + width_)
        return;

    if (barrierItem->y() >= y_ + height_)
        boundary.bottom = GEB::Math::min(boundary.bottom, barrierItem->y());
}

void ItemEntity::calculateBoundaryLeft(GEB::Boundary &boundary, const ItemEntity *barrierItem)
{
    const float barrierRight = barrierItem->x() + barrierItem->width();
    const float barrierBottom = barrierItem->y() + barrierItem->height();
    if (barrierBottom <= y_ || barrierItem->y() >= y_ + height_)
        return;

    if (barrierRight <= x_)
        boundary.left = GEB::Math::max(boundary.left, barrierRight);
}

void ItemEntity::calculateBoundaryRight(GEB::Boundary &boundary, const ItemEntity *barrierItem)
{
    const float barrierBottom = barrierItem->y() + barrierItem->height();
    if (barrierBottom <= y_ || barrierItem->y() >= y_ + height_)
        return;

    if (barrierItem->x() >= x_ + width_)
        boundary.right = GEB::Math::min(boundary.right, barrierItem->x());
}

void ItemEntity::calculateBoundaryImpl(GEB::Boundary &boundary, const ItemEntity *barrierItem, GEB::Direction direction)
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
    case GEB::Direction::DownLeft:
        calculateBoundaryBottom(boundary, barrierItem);
        calculateBoundaryLeft(boundary, barrierItem);
        break;
    case GEB::Direction::DownRight:
        calculateBoundaryBottom(boundary, barrierItem);
        calculateBoundaryRight(boundary, barrierItem);
        break;
    case GEB::Direction::UpLeft:
        calculateBoundaryTop(boundary, barrierItem);
        calculateBoundaryLeft(boundary, barrierItem);
        break;
    case GEB::Direction::UpRight:
        calculateBoundaryTop(boundary, barrierItem);
        calculateBoundaryRight(boundary, barrierItem);
        break;
    case GEB::Direction::None:
        break;
    default:
        break;
    }
}

GEB::Boundary ItemEntity::calculateBoundaryFocusHero(World &world, Layer &layer, GEB::Direction diretion)
{
    const Map &map = world.gameInfo().map();
    const HeroInfo &heroInfo = world.gameInfo().heroInfo();

    GEB::Boundary boundary{
        .top = static_cast<float>(map.topBoundary()),
        .bottom = static_cast<float>(map.bottomBoundary()),
        .left = static_cast<float>(map.leftBoundary()),
        .right = static_cast<float>(map.rightBoundary())};

    ItemEntity tempItem(heroInfo.x,
                        heroInfo.y,
                        heroInfo.width, heroInfo.height);

    calculateBoundaryImpl(boundary, &tempItem, diretion);

    return boundary;
}

GEB::Boundary ItemEntity::calculateBoundary(World &world, Layer &layer, GEB::Direction diretion)
{
    const Map &map = world.gameInfo().map();

    GEB::Boundary boundary{
        .top = static_cast<float>(map.topBoundary()),
        .bottom = static_cast<float>(map.bottomBoundary()),
        .left = static_cast<float>(map.leftBoundary()),
        .right = static_cast<float>(map.rightBoundary())};

    if (PlayStage *playStage = dynamic_cast<PlayStage *>(&(layer.currentStage())))
    {
        const auto &list = playStage->itemEntityList();
        int size = list.size();
        for (int i = 0; i < size; i++)
        {
            if (!list[i] || list[i] == this)
                continue;
            calculateBoundaryImpl(boundary, list[i], diretion);
        }
    }

    return boundary;
}
