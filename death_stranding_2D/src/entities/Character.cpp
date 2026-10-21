#include "entities/Character.h"
#include "entities/Environment.h"
#include "world/World.h"
#include "world/Map.h"
#include "layers/Layer.h"

void Character::render(World &world, Layer &layer)
{
    const Environment &environment = world.gameInfo().environment();

    float transX = x_ - environment.x();
    float transY = y_ - environment.y();

    for (int i = 0; i < width_; i++)
    {
        for (int j = 0; j < height_; j++)
        {
            float targetX = transX + i;
            float targetY = transY + j;

            if (targetX < 0 || targetY < 0)
            {
                continue;
            }

            world.canvas().draw(static_cast<int>(targetX),
                                static_cast<int>(targetY),
                                color_.r,
                                color_.g,
                                color_.b);
        }
    }
}

// Movement functions
void Character::moveUp(float distance, const Map &map)
{
    float resPos = y_ - distance;
    y_ = GEB::Math::max(resPos, static_cast<float>(map.topBoundary()));
}
void Character::moveDown(float distance, const Map &map)
{
    float resPos = y_ + distance;
    y_ = GEB::Math::min(resPos, static_cast<float>(map.bottomBoundary() - height_));
}
void Character::moveLeft(float distance, const Map &map)
{
    float resPos = x_ - distance;
    x_ = GEB::Math::max(resPos, static_cast<float>(map.leftBoundary()));
}
void Character::moveRight(float distance, const Map &map)
{
    float resPos = x_ + distance;
    x_ = GEB::Math::min(resPos, static_cast<float>(map.rightBoundary() - width_));
}
void Character::moveUpLeft(float distance, const Map &map)
{
    moveUp(distance, map);
    moveLeft(distance, map);
}
void Character::moveUpRight(float distance, const Map &map)
{
    moveUp(distance, map);
    moveRight(distance, map);
}
void Character::moveDownLeft(float distance, const Map &map)
{
    moveDown(distance, map);
    moveLeft(distance, map);
}
void Character::moveDownRight(float distance, const Map &map)
{
    moveDown(distance, map);
    moveRight(distance, map);
}